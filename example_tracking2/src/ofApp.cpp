#include "ofApp.h"

// setup
//192.168.0.15
//255.255.255.0
//192.168.0.1

//----------------------------------------
void ofApp::setup()
{
    ofSetFrameRate( 40 );
    ofBackground( 0 );
    
    lidar_object.setup();
    
    
    
}

void ofApp::exit(){
    
}
// update
//----------------------------------------
void ofApp::update()
{
   
      lidar_object.update();

}


// draw
//----------------------------------------
void ofApp::draw()
{
   lidar_object.draw();
    lidar_object.drawInfo(ofGetWidth() - 250, 10);
    lidar_object.gui_lidar.draw();
}


// keyPressed
//----------------------------------------
void ofApp::keyPressed( int key )
{
   
    
     if(key == 'b') lidar_object.getBackground();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
//    if(key == 'g'){
//        bShowGui = !bShowGui;
//        //        video_object.uvcController.bShowGUI = bShowGui;
//        if(bShowGui == false){
//            saveGui();
//            ofHideCursor();
//        }else{
//            ofShowCursor();
//        }
//    }
    
//    if(key == 'f') bFullscreen = !bFullscreen;
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
