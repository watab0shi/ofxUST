#include "ofApp.h"

// setup
//----------------------------------------
void ofApp::setup()
{
     ofSetFrameRate( 40 );
    ofBackground( 0 );
    
    bMirror = false;
    scale   = 0.15;
    
    ust.open();
    
    if( ust.isConnected() )
    {
        ust.setDirection( ofxUST::DIRECTION_DOWN );
        ust.setMirror( bMirror );
        //        ust.setScanningParameterByAngles( -135, 135, 1 );
        //setScanningParameterByAngles( float _minAngle, float _maxAngle, int _skipStep )
//        ust.setScanningParameterByAngles( -135, 135, 1 );
         ust.setScanningParameterByAngles( -135, 35, 1 );
        ust.startMeasurement();
    }
    else
    {
        ofLog() << "Connection failed!";
    }
   
    
      ust.update();
    allLines.resize(ust.coordinates.size());
    for(int i=0; i<allLines.size();i++ ){
        allLines[i].setup(i);
    }
    
   
    
    
}

// update
//----------------------------------------
void ofApp::update()
{
    ust.update();
    
    if(ust.coordinates.size() > allLines.size()){
        allLines.resize(ust.coordinates.size());
        for(int i=0; i<allLines.size();i++ ){
            allLines[i].setup(i);
        }
         ofLog()<<"ust.coordinates.size() "<<ust.coordinates.size();
    }
    
    if(ofGetElapsedTimef() < 4){
        getBackground();
    }
    for(int i = 0; i < ust.coordinates.size(); ++i ){
        allLines[i].update(ust.coordinates.at(i));
    }
}

void ofApp::getBackground(){
    for( int i = 0; i < ust.coordinates.size(); ++i )
    {
        allLines[i].setBackground(ust.coordinates.at( i ));
    }
}
// draw
//----------------------------------------
void ofApp::draw()
{
    ofPushMatrix();
    {
        ofTranslate( ofGetWidth() / 2, ofGetHeight() / 2 );
        ofScale( scale, scale );
        
        for(auto & aLine : allLines ){
            aLine.draw();
        }
        
//        //        glBegin( GL_LINES );
//        for( int i = 0; i < ust.coordinates.size(); ++i )
//        {
//            ofSetColor( 127 );
//            ofDrawLine( 0, 0 , ust.coordinates.at( i ).x, ust.coordinates.at( i ).y );
//            
//            ofSetColor( 50 );
//            ofDrawLine(ust.coordinates.at( i ).x, ust.coordinates.at( i ).y , allLines[i].bg_pos.x, allLines[i].bg_pos.y);
//            
//            //            float dist = ofVec2f(0,0).distance(ust.coordinates.at( i ));
//            //            if(allLines[i].distance != dist){
//            //                allLines[i].distance = dist;
//            //                allLines[i].age = 0;
//            //                 ofSetColor( 255,0,0 );
//            //                ofDrawCircle(ust.coordinates.at( i ), 4);
//            //            } else{
//            //                allLines[i].age++;
//            //            }
//            
//            
//            
//        }
        //        glEnd();
    }

    
    ofPopMatrix();
    
    ofSetColor( 255 );
    int temp_y = 0;
    ofDrawBitmapString( "Direction [arrow] : " + ofToString( ( int )ust.getDirection() ), 20, temp_y+=15 );
    ofDrawBitmapString( "Mirror    [m]     : " + ofToString( bMirror ),                   20, temp_y+=15 );
    ofDrawBitmapString( "FPS               : " + ofToString( ofGetFrameRate(), 1 ),       20, temp_y+=15 );
    ofDrawBitmapString( "line amt          : " + ofToString(  ust.coordinates.size() ),       20, temp_y+=15 );
    
}

void ofApp::mouseScrolled( int _x, int _y, float _scrollX, float _scrollY )
{
    scale = ofClamp( scale - _scrollY * 0.01, 0.01, 1.0 );
}

// keyPressed
//----------------------------------------
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
    
    if(key == 'b') getBackground();
}

