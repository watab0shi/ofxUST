#pragma once

#include "ofMain.h"
#include "ofxUST.h"

class ofApp : public ofBaseApp
{
public:
  void setup();
  void update();
  void draw();

  void keyPressed( int key );
  
  ofxUST ust;
  bool   bMirror;
};
