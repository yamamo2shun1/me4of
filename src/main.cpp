#include "ofMain.h"
#include "ofApp.h"

#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"

////////////////////////// object struct
typedef struct _me4of 
{
  //t_object ob;// the object itself (must be first)
  t_pxobject ob;// the object itself (t_pxobject in MSP instead of t_object)
  double offset;
} t_me4of;

///////////////////////// function prototypes
//// standard set
void *me4of_new(t_symbol *s, long argc, t_atom *argv);
void me4of_free(t_me4of *x);
void me4of_assist(t_me4of *x, void *b, long m, long a, char *s);
void me4of_init(t_me4of *x, t_symbol *s, long argc, t_atom *argv);
void me4of_bgcolor(t_me4of *x, t_symbol *s, long argc, t_atom *argv);
void me4of_dsp(t_me4of *x, t_signal **sp, short *count);
void me4of_dsp64(t_me4of *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
t_int *me4of_perform(t_int *w);
void me4of_perform64(t_me4of *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

//////////////////////// global class pointer variable
t_class *me4of_class;
ofApp *myapp;

DWORD WINAPI ThreadProc(LPVOID lpParam) {
  while(TRUE) {
    if(myapp->bInitOfWindow) {
      post("init oF...");
      ofSetupOpenGL(myapp->windowWidth, myapp->windowHeight, OF_WINDOW);
      ofRunApp(myapp);
      myapp->bInitOfWindow = false;
    }
    post("looping...");
    Sleep(1000);
  }
}

//========================================================================
int C74_EXPORT main(void) {
  myapp = new ofApp();
  myapp->bInitOfWindow = false;

  DWORD dwThreadId;
  HANDLE hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, &dwThreadId);
  
  // object initialization, OLD STYLE
  // setup((t_messlist **)&simplemax_class, (method)simplemax_new, (method)simplemax_free, (short)sizeof(t_simplemax), 
  //		0L, A_GIMME, 0);
  // addmess((method)simplemax_assist,			"assist",		A_CANT, 0);  

  // object initialization, NEW STYLE
  t_class *c;

  c = class_new("me4of~", (method)me4of_new, (method)dsp_free, (long)sizeof(t_me4of), 0L, A_GIMME, 0);
	
  /* you CAN'T call this from the patcher */
  class_addmethod(c, (method)me4of_assist,  "assist",  A_CANT,  0);
  class_addmethod(c, (method)me4of_init,    "init",    A_GIMME, 0);
  class_addmethod(c, (method)me4of_bgcolor, "bgcolor", A_GIMME, 0);

  class_addmethod(c, (method)me4of_dsp,     "dsp",     A_CANT,  0);// Old 32-bit MSP dsp chain compilation for Max 5 and earlier
  class_addmethod(c, (method)me4of_dsp64,   "dsp64",   A_CANT,  0);// New 64-bit MSP dsp chain compilation for Max 6
	
  class_dspinit(c);
  
  class_register(CLASS_BOX, c); /* CLASS_NOBOX */
  me4of_class = c;

  post("I am the me4of~ object");

  return 0;
}

void *me4of_new(t_symbol *s, long argc, t_atom *argv)
{
  t_me4of *x = (t_me4of *)object_alloc(me4of_class);

  if (x) {
    dsp_setup((t_pxobject *)x, 1);// MSP inlets: arg is # of inlets and is REQUIRED! 
                                  // use 0 if you don't need inlets
    outlet_new(x, "signal");      // signal outlet (note "signal" rather than NULL)
    x->offset = 0.0;
  }
  return (x);

#if 0
  t_me4of *x = NULL;
  long i;
    
  // object instantiation, NEW STYLE
  if(x = (t_me4of *)object_alloc(me4of_class)) {
    object_post((t_object *)x, "a new %s object was instantiated: %p", s->s_name, x);
    object_post((t_object *)x, "it has %ld arguments", argc);
        
    for (i = 0; i < argc; i++) {
      if ((argv + i)->a_type == A_LONG) {
        object_post((t_object *)x, "arg %ld: long (%ld)", i, atom_getlong(argv+i));
      } else if ((argv + i)->a_type == A_FLOAT) {
        object_post((t_object *)x, "arg %ld: float (%f)", i, atom_getfloat(argv+i));
      } else if ((argv + i)->a_type == A_SYM) {
        object_post((t_object *)x, "arg %ld: symbol (%s)", i, atom_getsym(argv+i)->s_name);
      } else {
        object_error((t_object *)x, "forbidden argument");
      }
    }
  }
  return (x);
#endif
}

void me4of_free(t_me4of *x)
{
  ;
}

void me4of_assist(t_me4of *x, void *b, long m, long a, char *s) {
  if (m == ASSIST_INLET) { // inlet
    sprintf(s, "I am inlet %ld", a);
  } 
  else {	// outlet
    sprintf(s, "I am outlet %ld", a); 			
  }
}

void me4of_init(t_me4of *x, t_symbol *s, long argc, t_atom *argv) {
  //post("init");

  myapp->windowWidth = (argc) ? atom_getlong(argv) : 1024;
  myapp->windowHeight = (argc) ? atom_getlong(argv + 1) : 768;
  
  if(!myapp->bInitOfWindow) {
    myapp->bInitOfWindow = true;
  }
}

void me4of_bgcolor(t_me4of *x, t_symbol *s, long argc, t_atom *argv) {
  //post("bgcolor");
  
  myapp->bgColorRed   = (argc) ? atom_getlong(argv) : 0;
  myapp->bgColorGreen = (argc) ? atom_getlong(argv + 1) : 0;
  myapp->bgColorBlue  = (argc) ? atom_getlong(argv + 2) : 0;
  myapp->bgColorAlpha = (argc) ? atom_getlong(argv + 3) : 0;
}

//***********************************************************************************************

// this function is called when the DAC is enabled, and "registers" a function for the signal chain in Max 5 and earlier.
// In this case we register the 32-bit, "simplemsp_perform" method.
void me4of_dsp(t_me4of *x, t_signal **sp, short *count)
{
  post("my sample rate is: %f", sp[0]->s_sr);
	
  // dsp_add
  // 1: (t_perfroutine p) perform method
  // 2: (long argc) number of args to your perform method
  // 3...: argc additional arguments, all must be sizeof(pointer) or long
  // these can be whatever, so you might want to include your object pointer in there
  // so that you have access to the info, if you need it.
  dsp_add(me4of_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}


// this is the Max 6 version of the dsp method -- it registers a function for the signal chain in Max 6,
// which operates on 64-bit audio signals.
void me4of_dsp64(t_me4of *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
  post("my sample rate is: %f", samplerate);
	
  // instead of calling dsp_add(), we send the "dsp_add64" message to the object representing the dsp chain
  // the arguments passed are:
  // 1: the dsp64 object passed-in by the calling function
  // 2: the symbol of the "dsp_add64" message we are sending
  // 3: a pointer to your object
  // 4: a pointer to your 64-bit perform method
  // 5: flags to alter how the signal chain handles your object -- just pass 0
  // 6: a generic pointer that you can use to pass any additional data to your perform method
	
  object_method(dsp64, gensym("dsp_add64"), x, me4of_perform64, 0, NULL);
}


//***********************************************************************************************

// this is the 32-bit perform method for Max 5 and earlier
t_int *me4of_perform(t_int *w) {
  // DO NOT CALL post IN HERE, but you can call defer_low (not defer)
	
  // args are in a vector, sized as specified in simplemsp_dsp method
  // w[0] contains &simplemsp_perform, so we start at w[1]
  t_me4of *x = (t_me4of *)(w[1]);
  t_float *inL = (t_float *)(w[2]);
  t_float *outL = (t_float *)(w[3]);
  int n = (int)w[4];
	
  // this perform method simply copies the input to the output, offsetting the value
  while (n--)
    *outL++ = *inL++ + x->offset;
		
  // you have to return the NEXT pointer in the array OR MAX WILL CRASH
  return w + 5;
}


// this is 64-bit perform method for Max 6
void me4of_perform64(t_me4of *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam) {
  t_double *inL = ins[0];   // we get audio for each inlet of the object from the **ins argument
  t_double *outL = outs[0]; // we get audio for each outlet of the object from the **outs argument
  int n = sampleframes;
	
  // this perform method simply copies the input to the output, offsetting the value
  while (n--)
    *outL++ = *inL++ + x->offset;
}
