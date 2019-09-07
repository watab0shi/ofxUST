#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
  bMirror = false;
  
  ust.setDirection( ofxUST::DIRECTION_DOWN );
  ust.setMirror( bMirror );
  ust.setScanningParameterByAngles( -135, 135, 1 );
  
  if( ust.isConnected() ) ust.startMeasurement();
  else                    ofLog() << "Connection failed!";

  ofSetFrameRate( 40 );
  
  ofBackground( 0 );
}

//--------------------------------------------------------------
void ofApp::update()
{
  ust.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
  static float s = 0.15;
  ofPushMatrix();
  {
    ofTranslate( ofGetWidth() / 2, ofGetHeight() / 2 );
    ofScale( s, s );
    
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
}

//--------------------------------------------------------------
void ofApp::keyPressed( int key )
{
  if( key == 'm' )
  {
    bMirror = !bMirror;
    ust.setMirror( bMirror );
  }
  
  if( key == OF_KEY_RIGHT ) ust.setDirection( ofxUST::DIRECTION_RIGHT );
  if( key == OF_KEY_DOWN )  ust.setDirection( ofxUST::DIRECTION_DOWN );
  if( key == OF_KEY_LEFT )  ust.setDirection( ofxUST::DIRECTION_LEFT );
  if( key == OF_KEY_UP )    ust.setDirection( ofxUST::DIRECTION_UP );
}
