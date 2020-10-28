//
//  lidarHandler.h
//  example_tracking2
//
//  Created by Stephan Schulz on 2020-10-24.
//

#ifndef lidarHandler_h
#define lidarHandler_h

#include "ofxUST.h"
#include "onePerson.h"

class lidarHandler{
private:
    
public:
    
    ofxUST ust;
    
    vector<ofPolyline> pLines;
    
    vector<ofVec2f> points;
    vector<ofColor> colors;
    
    ofPolyline bgLine;
    ofPolyline bgLine2;
    
    ofxPanel gui_lidar;
    ofParameter<float> cloudScale;
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
    
    //---tracker
    ofxCv::RectTrackerFollower<onePerson> tracker;
    vector<onePerson> followers;
    vector<cv::Rect> boundingRects;
    
    ofParameterGroup parameters_track;
    
    ofParameter<int> tracker_persistence;
    int old_tracker_persistence;
    ofParameter<int> tracker_maxDistance;
    int old_tracker_maxDistance;
    ofParameter<int> minAge;
    ofParameter<int> maxSize,minSize;
    ofParameter<float> dyingTime;
    ofParameter<int> foundFacesSize;
    ofParameter<float> smoothingRate;
    
    ofParameter<float> bgScaler;
    float old_bgScaler;
    
    //---smoothing filter
    ofParameter<bool> bUseBiQuadFilter;
    bool old_bUseBiQuadFilter;
    
    ofParameter<float> bqFilter_fc;
    float old_bqFilter_fc;
    void setup(){
        
//        bMirror = false;
//        scale   = 0.15;
        
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
         gui_lidar.add(cloudY.set("cloudY",0,-600,600)); 
//         gui_lidar.add(minPolylineSize.set("minPolylineSize",5,1,100)); 
        gui_lidar.add(maxPointDistance.set("maxPointDistance",100,1,1000)); 
      
        gui_lidar.add(bgScaler.set("bgScaler",0.97,0.5,1)); 
        
        parameters_track.setName("tracking");
        parameters_track.add(tracker_persistence.set("tracker_persistence",15,0,200));
        parameters_track.add(tracker_maxDistance.set("tracker_maxDistance",500,0,500));
        parameters_track.add(dyingTime.set("dyingTime",0.2,0,2));
        parameters_track.add(minAge.set("minAge",15,0,50));
        parameters_track.add(minSize.set("minSize",40,1,150));
        parameters_track.add(maxSize.set("maxSize",350,151,800));
        parameters_track.add(foundFacesSize.set("foundFacesAmount",0,0,40));
        parameters_track.add(smoothingRate.set("smoothingRate",0.1,0,1)); //0.1
        
        parameters_track.add(bUseBiQuadFilter.set("useBiQuadFilter",false));
        parameters_track.add(bqFilter_fc.set("bqFilter_fc",0.035,0,0.3));
        
        
        gui_lidar.add(parameters_track);
        gui_lidar.loadFromFile("GUIs/gui_lidar.xml");
        
        tracker.setPersistence(tracker_persistence);
        // an object can move up to 50 pixels per frame
        tracker.setMaximumDistance(tracker_maxDistance);
        
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
        
        bgLine.addVertex(0,0);
        bgLine.addVertex(500,0);
        bgLine.addVertex(500,500);
        bgLine.addVertex(0,500);
        bgLine.close();
        
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
    
            if(bgLine.inside(ust.coordinates.at(i).x, ust.coordinates.at(i).y)){
                points.push_back(ust.coordinates.at(i));
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
        
        //--update tracker
        
        boundingRects.clear();
        ofRectangle boundRect;
        for(int i=0; i<pLines.size(); i++){
//            if(pLines[i].getVertices().size() > minPolylineSize){
//                boundingRects.push_back(toCv(pLines[i].getBoundingBox()));
//            }
            boundRect = pLines[i].getBoundingBox();
            if(boundRect.getWidth() >= minSize && boundRect.getWidth() <= maxSize){
//            if(pLines[i].getVertices().size() > minPolylineSize){
                boundingRects.push_back(toCv(pLines[i].getBoundingBox()));
            }
            
            
        }
        
        foundFacesSize = boundingRects.size();
        tracker.track(boundingRects);
        
        ofRectangle videoRect(0,0,1000,1000);
        //!!! it's important to update these variables before passing it on to followers array
        int cnt = 0;
        for(auto & v:tracker.getFollowers()){
            //                ofLog()<<"auto dyingTime "<<dyingTime;
            v.setSmoothingRate(smoothingRate);
            v.setDyingTime(dyingTime);
            v.setMaxRect(videoRect);
            v.setRectScaler(1);
            v.setRectYoffset(0);
    
            v.bqFilter_fc = bqFilter_fc; 
            v.bUseBiQuadFilter = bUseBiQuadFilter;
            cnt++;
            
            v.updateRects();
        }
        //                followers.clear();
        followers = tracker.getFollowers();
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
        if(old_tracker_persistence != tracker_persistence){
            old_tracker_persistence = tracker_persistence;
            tracker.setPersistence(tracker_persistence);
        }
        
        if(old_tracker_maxDistance != tracker_maxDistance){
            old_tracker_maxDistance = tracker_maxDistance;
            tracker.setMaximumDistance(tracker_maxDistance);
        }
        
        if(old_bUseBiQuadFilter !=  bUseBiQuadFilter){
            old_bUseBiQuadFilter = bUseBiQuadFilter;
            for(auto & v:tracker.getFollowers()){
                v.bUseBiQuadFilter = bUseBiQuadFilter;
            }
        }
        
        if(old_bqFilter_fc != bqFilter_fc){
            old_bqFilter_fc = bqFilter_fc;
            for(auto & v:tracker.getFollowers()){
                v.bqFilter_fc = bqFilter_fc;
            }
            
        }

        if(old_bgScaler != bgScaler){
            old_bgScaler = bgScaler;
            getBackground();
        }
    }
    void draw(){
//        points.clear();
        ofPushMatrix();
        ofSetLineWidth(1);
        ofTranslate( ofGetWidth() / 2, ofGetHeight() / 2 + cloudY );
        ofScale( cloudScale, cloudScale );
        
        for( int i = 0; i < ust.coordinates.size(); ++i ){
            ofSetColor( 127,100 );
            ofDrawLine( 0, 0 , ust.coordinates.at( i ).x, ust.coordinates.at( i ).y );
        }
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
        bgLine.draw();
        bgLine2.draw();
        //    
        int pLine_cnt = 0;
        ofNoFill();
        ofSetColor(255,255);
        ofRectangle boundRect;
        for(int i=0; i<pLines.size(); i++){
            
            boundRect = pLines[i].getBoundingBox();
            if(boundRect.getWidth() >= minSize && boundRect.getWidth() <= maxSize){
//            if(pLines[i].getVertices().size() > minPolylineSize){
//                ofSetColor(colors[pLine_cnt]);
                pLines[i].draw();
//                ofLog()<<i<<" size "<<pLines[i].getVertices().size();
//                ofDrawRectangle(pLines[i].getBoundingBox());
//                pLine_cnt++;
            }
            
        }
        
        int cnt = 0;
        for(auto & v:tracker.getFollowers()){
            
            int temp_age = tracker.getAge(v.getLabel());
            if(temp_age < minAge) continue;
            
            int temp_l = v.getLabel();
            v.draw("age:"+ofToString(tracker.getAge(temp_l)));
            
            
//            string msg = ofToString(temp_l) + " :" + ofToString(tracker.getAge(temp_l));
//            ofDrawBitmapString(msg, v.rect.x,v.rect.y-20);
            
            
            ofSetColor(255);
            if(v.getDead() == false){
                ofDrawBitmapString(ofToString(temp_l), 10, 50+(15*cnt));
                cnt ++;
            } 
        }
        
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
        ofDrawBitmapString( "boundingRects amt : " + ofToString(  boundingRects.size() ),       20, temp_y+=15 );
        ofPopMatrix();
    }
    void getBackground(){
        bgLine.clear();
        bgLine.addVertex(0,0);
        for( int i = 0; i < ust.coordinates.size(); ++i ){
            bgLine.addVertex(ust.coordinates.at(i).x, ust.coordinates.at(i).y);
        }
        bgLine.close();
        bgLine2 = bgLine;
        bgLine.scale(bgScaler,bgScaler);
        
    }
};


#endif /* lidarHandler_h */
