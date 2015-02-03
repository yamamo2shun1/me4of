#include "ofApp.h"

#include "ext.h"
#include "ext_obex.h"

//--------------------------------------------------------------
void ofApp::setup() {
  post("setup...");

  ofBackground(bgColorRed, bgColorGreen, bgColorBlue, bgColorAlpha);
  ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::exit() {

}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
  ofBackground(bgColorRed, bgColorGreen, bgColorBlue, bgColorAlpha);
  
  ofSetColor(255, 255, 255, 255);
  ofDrawBitmapString(ofToString((int)ofGetFrameRate()), 10, 20);
  ofDrawBitmapString(ofToString((int)ofGetWidth()), 10, 30);
  ofDrawBitmapString(ofToString((int)ofGetHeight()), 10, 40);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) { 

}
