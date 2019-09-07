#include "ofApp.h"

// setup
//----------------------------------------
void ofApp::setup()
{
  bMirror = false;
  scale   = 0.15;
  
  ust.open();
  
  if( ust.isConnected() )
  {
    ust.setDirection( ofxUST::DIRECTION_DOWN );
    ust.setMirror( bMirror );
    ust.setScanningParameterByAngles( -135, 135, 1 );
    ust.startMeasurement();
  }
  else
  {
    ofLog() << "Connection failed!";
  }

  ofSetFrameRate( 40 );
  
  ofBackground( 0 );
}

// update
//----------------------------------------
void ofApp::update()
{
  ust.update();
}

// draw
//----------------------------------------
void ofApp::draw()
{
  ofPushMatrix();
  {
    ofTranslate( ofGetWidth() / 2, ofGetHeight() / 2 );
    ofScale( scale, scale );
    
    ofSetColor( 120 );
    glBegin( GL_LINES );
    for( int i = 0; i < ust.coordinates.size(); ++i )
    {
      glVertex2f( 0, 0 );
      glVertex2f( ust.coordinates.at( i ).x, ust.coordinates.at( i ).y );
    }
    glEnd();
  }
  ofPopMatrix();
  
  ofSetColor( 255 );
  ofDrawBitmapString( "Direction [arrow] : " + ofToString( ( int )ust.getDirection() ), 20, 40 );
  ofDrawBitmapString( "Mirror    [m]     : " + ofToString( bMirror ),                   20, 60 );
  ofDrawBitmapString( "FPS               : " + ofToString( ofGetFrameRate(), 1 ),       20, 80 );
}

void ofApp::mouseScrolled( int _x, int _y, float _scrollX, float _scrollY )
{
  scale = ofClamp( scale - _scrollY * 0.01, 0.01, 1.0 );
}

// keyPressed
//----------------------------------------
void ofApp::keyPressed( int _key )
{
  if( _key == 'm' )
  {
    bMirror = !bMirror;
    ust.setMirror( bMirror );
  }
  
  if( _key == OF_KEY_RIGHT ) ust.setDirection( ofxUST::DIRECTION_RIGHT );
  if( _key == OF_KEY_DOWN )  ust.setDirection( ofxUST::DIRECTION_DOWN );
  if( _key == OF_KEY_LEFT )  ust.setDirection( ofxUST::DIRECTION_LEFT );
  if( _key == OF_KEY_UP )    ust.setDirection( ofxUST::DIRECTION_UP );
}
