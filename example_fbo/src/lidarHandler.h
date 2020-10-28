//
//  lidarHandler.h
//  example_tracking2
//
//  Created by Stephan Schulz on 2020-10-24.
//

#ifndef lidarHandler_h
#define lidarHandler_h

#include "ofxUST.h"

class lidarHandler{
private:
    
public:
    
    ofxUST ust;
    
    int fboWidth, fboHeight;
    ofFbo lidarFBO;
    ofImage lidarImage;
    ofPixels lidarPixels;
    
    ofParameter<float> circleRadius;
    ofParameter<bool> bShowLines;
    
    vector<ofPolyline> pLines;
    
    vector<ofVec2f> points;
    vector<ofColor> colors;
    
    ofPolyline bgLine;
    ofPolyline bgLine2;
    
    ofxPanel gui_lidar;
    ofParameter<float> cloudScale;
    ofParameter<int> cloudX;
    ofParameter<int> cloudY;
    ofParameter<bool> bMirror;
    bool old_bMirror;
    ofParameter<int> direction;
    int old_direction;
    
    ofParameter<int> minPolylineSize;
    ofParameter<int> maxPointDistance;
    
    int minAngle, maxAngle;
    //    ofParameter<int> minAngle;
    //    int old_minAngle;
    //    ofParameter<int> maxAngle;
    //    int old_maxAngle;
    
    ofParameter<int> angleRange;
    int old_angleRange;
    
    ofParameter<int> angleSkipSteps;
    int old_angleSkipSteps;


    
    ofParameter<float> bgScaler;
    float old_bgScaler;
    

    void setup(int w, int h){
        
        //        bMirror = false;
        //        scale   = 0.15;
        fboWidth = w;
        fboHeight = h;
        
        gui_lidar.setup();
        gui_lidar.setName("LIDAR");
        gui_lidar.setPosition(10,100);
        gui_lidar.setHeaderBackgroundColor(ofColor(255,0,0));
        gui_lidar.add(angleRange.set("angleRange",90,1,270)); 
        //         gui_lidar.add(maxAngle.set("maxAngle",135,-135,135)); 
        //        gui_lidar.add(minAngle.set("minAngle",-540,-540,540)); 
        //        gui_lidar.add(maxAngle.set("maxAngle",540,-540,540)); 
        gui_lidar.add(angleSkipSteps.set("angleSkipSteps",1,1,10)); 
        
        gui_lidar.add(bMirror.set("mirror",true));
        gui_lidar.add(direction.set("direction",0,0,3));
        gui_lidar.add(cloudScale.set("cloudScale",0.15,0.01,0.2)); 
        gui_lidar.add(cloudX.set("cloudX",0,-1280,1280)); 
        gui_lidar.add(cloudY.set("cloudY",0,-1280,1280)); 
        //         gui_lidar.add(minPolylineSize.set("minPolylineSize",5,1,100)); 
        gui_lidar.add(maxPointDistance.set("maxPointDistance",100,1,1000)); 
        
        gui_lidar.add(bgScaler.set("bgScaler",0.97,0,1));
        gui_lidar.add(bShowLines.set("showLines",false));
        gui_lidar.add(circleRadius.set("circleRadius",3,0.1,50));
        
        gui_lidar.loadFromFile("GUIs/gui_lidar.xml");

        
        minAngle = -angleRange/2;
        maxAngle = angleRange/2;
        
        ust.open();
        
        if( ust.isConnected() )
        {
            ust.setDirection( ofxUST::DIRECTION_DOWN );
            ust.setMirror( bMirror );
            //        ust.setScanningParameterByAngles( -135, 135, 1 );
            //setScanningParameterByAngles( float _minAngle, float _maxAngle, int _skipStep )
            //        ust.setScanningParameterByAngles( -135, 135, 1 );
            ust.setScanningParameterByAngles( minAngle, maxAngle, angleSkipSteps );
            ust.startMeasurement();
        }
        else
        {
            ofLog() << "Connection failed!";
        }
        
        old_angleRange = angleRange; 
        //        old_minAngle = minAngle;
        //        old_maxAngle = maxAngle;
        old_angleSkipSteps = angleSkipSteps;
        
        colors.resize(1078);
        for(int i=0; i<colors.size(); i++){
            colors[i] = ofColor(ofRandom(20,255),ofRandom(20,255),ofRandom(20,255));
        }
        
        //        bgLine.addVertex(0,0);
        //        bgLine.addVertex(2000,0);
        //        bgLine.addVertex(2000,500);
        //        bgLine.addVertex(0,500);
        //        bgLine.close();
        
        lidarFBO.allocate(fboWidth,fboHeight,GL_RGBA);
        lidarFBO.begin();
        ofClear(255,255,255, 0);
        lidarFBO.end();
        //      ust.update();
        //    allLines.resize(ust.coordinates.size());
        //    for(int i=0; i<allLines.size();i++ ){
        //        allLines[i].setup(i);
        //    }
        
    }
    void update(){
        checkGui();
        ust.update();
        
        points.clear();
        for( int i = 0; i < ust.coordinates.size(); ++i ){
            
            if(bgScaler == 0){
                points.push_back(ust.coordinates.at(i));
            } else{ 
                if(bgLine.inside(ust.coordinates.at(i).x, ust.coordinates.at(i).y)){
                    points.push_back(ust.coordinates.at(i));
                }
            }
        }
        
        pLines.clear();
        pLines.resize(1);
        float temp_dist = 0;
        //group intersections
        if(points.size() > 1){
            for(int i=0; i<points.size()-1; i++){
                temp_dist = points[i].distance(points[i+1]);
                
                if(temp_dist <= maxPointDistance){
                    pLines.back().addVertex(points[i].x, points[i].y);
                } else{
                    //                ofLog()<<"temp_dist "<<temp_dist;
                    pLines.emplace_back();
                }
            }
        }
        
      
    }
    
    
    
    ofPixels getImage(){
        
        if(lidarFBO.isAllocated()){
            lidarFBO.begin();
            ofClear(255,255,255, 0);
            ofPushMatrix();
            ofFill();
            ofSetColor(50,255);
            ofDrawRectangle(0,0,fboWidth, fboHeight);
            ofTranslate( fboWidth / 2 + cloudX, fboHeight/ 2 + cloudY );
            ofScale( cloudScale, cloudScale );
            
            
            if(bShowLines){
                ofSetColor( 127,100 );
                glBegin( GL_LINES );
                for( int i = 0; i < ust.coordinates.size(); ++i )
                {
                    glVertex2f( 0, 0 );
                    glVertex2f( ust.coordinates.at( i ).x, ust.coordinates.at( i ).y );
                }
                glEnd();
            }
            
            ofSetLineWidth(3);
            if(bgScaler > 0){
                ofSetColor(ofColor::grey);
                bgLine2.draw();
                ofSetColor(ofColor::darkGrey);
                bgLine.draw();
            }
            
            ofFill();
            for( int i = 0; i < points.size(); ++i ){
                ofSetColor( 255,255 );
                ofDrawCircle(points[i].x, points[i].y,circleRadius);
            }
            
            ofSetLineWidth(2);
            ofSetColor( 255,255 );
            for(int i=0; i<pLines.size(); i++){
                pLines[i].draw();
            }
            
            ofPopMatrix();
            lidarFBO.end();
            
            lidarFBO.readToPixels(lidarPixels);
        }else{
            ofLog()<<"lidar fbo not allocated";
        }
        
        return lidarPixels;
    }
    void drawFbo(){
        
        lidarFBO.draw(0,0);
    }
    
    void draw(){
        //        points.clear();
        ofPushMatrix();
        ofSetLineWidth(1);
        ofTranslate( ofGetWidth() / 2 + cloudX, ofGetHeight() / 2 + cloudY );
        ofScale( cloudScale, cloudScale );
        
        ofSetColor( 127,100 );
        glBegin( GL_LINES );
        for( int i = 0; i < ust.coordinates.size(); ++i )
        {
            glVertex2f( 0, 0 );
            glVertex2f( ust.coordinates.at( i ).x, ust.coordinates.at( i ).y );
        }
        glEnd();
        //        
        //        
        //        pLines.clear();
        //        pLines.resize(1);
        //        float temp_dist = 0;
        //        if(points.size() > 1){
        //            for(int i=0; i<points.size()-1; i++){
        //                temp_dist = points[i].distance(points[i+1]);
        //                
        //                if(temp_dist < 100){
        //                    pLines.back().addVertex(points[i].x, points[i].y);
        //                } else{
        //                    //                ofLog()<<"temp_dist "<<temp_dist;
        //                    pLines.emplace_back();
        //                }
        //            }
        //        }
        ofSetColor(255);
        ofFill();
        //    for(int i=0; i<points.size(); i++){
        //        ofDrawCircle(points[i], 10);
        //    }
        
        ofSetLineWidth(3);
        ofSetColor(ofColor::grey);
        bgLine2.draw();
        ofSetColor(ofColor::darkGrey);
        bgLine.draw();
        
       
        
        ofPopMatrix();
        
        
    }
    
    void drawInfo(int x, int y){
        ofPushMatrix();
        ofTranslate(x,y);
        ofSetColor( 255 );
        int temp_y = 0;
        ofDrawBitmapString( "Direction [arrow] : " + ofToString( ( int )ust.getDirection() ), 20, temp_y+=15 );
        ofDrawBitmapString( "Mirror    [m]     : " + ofToString( bMirror ),                   20, temp_y+=15 );
        ofDrawBitmapString( "FPS               : " + ofToString( ofGetFrameRate(), 1 ),       20, temp_y+=15 );
        ofDrawBitmapString( "line amt          : " + ofToString(  ust.coordinates.size() ),       20, temp_y+=15 );
        ofPopMatrix();
    }
    
    void checkGui(){
        if(old_bMirror != bMirror){
            old_bMirror = bMirror;
            ust.setMirror( bMirror );
        }
        
        if(old_direction != direction){
            old_direction = direction;
            if(direction == 0) ust.setDirection( ofxUST::DIRECTION_RIGHT );
            if(direction == 1) ust.setDirection( ofxUST::DIRECTION_DOWN );
            if(direction == 2) ust.setDirection( ofxUST::DIRECTION_LEFT );
            if(direction == 3) ust.setDirection( ofxUST::DIRECTION_UP );
        }
        
        
        //        if(old_minAngle != minAngle || old_maxAngle != maxAngle || old_angleSkipSteps != angleSkipSteps){
        if(old_angleRange != angleRange || old_angleSkipSteps != angleSkipSteps){
            //            old_minAngle = minAngle;
            //            old_maxAngle = maxAngle;
            old_angleRange = angleRange;
            minAngle = -angleRange / 2;
            maxAngle = angleRange / 2;
            old_angleSkipSteps = angleSkipSteps;
            ust.setScanningParameterByAngles( minAngle, maxAngle, angleSkipSteps );
            ust.startMeasurement();
        }
        
        if(old_bgScaler != bgScaler){
            old_bgScaler = bgScaler;
            getBackground();
        }
    }
    
    void getBackground(){
        bgLine.clear();
        bgLine.addVertex(0,0);
        bgLine2.clear();
        bgLine2.addVertex(0,0);
        for( int i = 0; i < ust.coordinates.size(); ++i ){
            bgLine.addVertex(ust.coordinates.at(i).x, ust.coordinates.at(i).y);
            bgLine2.addVertex(ust.coordinates.at(i).x, ust.coordinates.at(i).y);
        }
        bgLine.close();
        bgLine2.close();
        bgLine.scale(bgScaler,bgScaler);
        
    }
};


#endif /* lidarHandler_h */
