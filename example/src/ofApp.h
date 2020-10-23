#pragma once

#include "ofMain.h"
#include "ofxUST.h"

class ofApp : public ofBaseApp
{
public:
  void setup();
  void update();
  void draw();
  void mouseScrolled( int _x, int _y, float _scrollX, float _scrollY );
  void keyPressed( int _key );
  
  ofxUST ust;
  bool   bMirror;
  float  scale;
};
