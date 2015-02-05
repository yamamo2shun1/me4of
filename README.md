# me4of
Max External Object included OpenFrameworks code and to control OpenFrameworks directly

## A. Demo
* http://youtu.be/m4ovWBJz-gg

## B. Quickstart

  1. Download the of_[version]_vs_release.zip.  
     http://openframeworks.cc/download/  
  2. Download the MaxSDK-6.1.4.zip.  
     https://cycling74.com/downloads/sdk/  
  3. Extract the downloaded files to any place you like.  
  4. Copy `MaxSDK-6.1.4\c74support` folder to `of_[version]_vs_release\libs`.  
  5. Clone the me4of~ repository into the `of_v0.8.4_vs_release\apps\myapps`.  
    ```
    $ git clone https://github.com/tkrworks/me4of me4of~
    ```
  6. Copy all dll files in `of_v0.8.4_vs_release\apps\myapps\me4of~\bin` to the following folders(both or only existing folder).

    ```
    C:\Program Files\Cycling '74\Max 7\resources\support
    C:\Program Files (x86)\Cycling '74\Max 7\resources\support
    ```
  7. Build the project. After than, Add the full path of `of_v0.8.4_vs_release\apps\myapps\me4of~\bin` to Max's File Preferences. Or copy .mxe to the existing path in File Preferences.  

## C. Make project by yourself

  1. same process as quickstart 1-4.  
  2. Make new project by ProjectGenerator.exe.  
  3. Build this project and copy all `bin\*.dll` files to the following folders.

    ```
    [If you use Max7]  
    C:\Program Files\Cycling '74\Max 7\resources\support  
    C:\Program Files (x86)\Cycling '74\Max 7\resources\support  
    
    [If you use Max6]
    C:\Program Files\Cycling '74\Max 6.1\support
    C:\Program Files (x86)\Cycling '74\Max 6.1\support
    ```
  4. Modify some configurations in your project properties. The changes are as follows.  
  5. Modify ofApp.h, ofApp.cpp and Main.cpp by reference to me4of~'s code.  
  6. Build again and check whether .mxe is found or not in your bin folder. If you find the .mxe, your build is succeeded. Then, add the full path of `apps\myapps\[your project name]\bin` to Max's File Preferences or copy .mxe to the existing path in File Preferences.  

## The Changes in VS Project Properties  
* Configuration Properties  
  - General  
    -> Project Defaults  
      Configuration Type：`Dynamic Library(.dll)`  
      Character Set：`Use Multi-Byte Character Set`  
    -> General  
      Target Extension：`.mxe`  

  - C/C++  
    -> General  
      Additional Include Directories： 
      ```
      ..\..\..\libs\c74support\max-includes  
      ..\..\..\libs\c74support\msp-includes  
      ..\..\..\libs\c74support\jit-includes  
      ```
    -> Preprocessor  
      Preprocessor Definitions：`WIN_VERSION;WIN32;_DEBUG;_WINDOWS;_USRDLL;WIN_EXT_VERSION;` 
  - Linker  
    -> General  
      Output File：`$(OutDir)$(ProjectName).mxe`  
      Additional Library Directories：

      ```
      ..\..\..\libs\c74support\max-includes  
      ..\..\..\libs\c74support\msp-includes  
      ..\..\..\libs\c74support\jit-includes  
      ```
    -> Input  
      Additional Dependencies：
      
      ```
      MaxAPI.lib  
      MaxAudio.lib  
      Jitlib.lib  
      ```
