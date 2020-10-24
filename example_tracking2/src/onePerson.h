//
//  onePerson.h
//  faceTracker
//
//  Created by Stephan Schulz on 2020-06-17.
//

#ifndef onePerson_h
#define onePerson_h


using namespace ofxCv;
using namespace cv;

class onePerson : public ofxCv::RectFollower {
protected:
    ofColor color;
    
    
    float startedDying;
    ofPolyline historyLine;
    
public:
    float dyingTime = 0.1;
    
    ofVec2f curPos, smoothPos;
    ofRectangle rect,smoothedRect;
    float smoothingRate;

    
    ofRectangle scaled_roiRect;
    float rectScaler = 1;
    ofRectangle maxRect;
    
    ofxBiquadFilter2f bqFilter_pos; 
    ofxBiquadFilter4f bqFilter_rect;
    
    bool bUseBiQuadFilter;
    float bqFilter_fc;
    float old_bqFilter_fc;
    
    
    float rectYoffset = 0;
    onePerson()
    :startedDying(0) {
    }
    
    void setDyingTime(float _time){
        dyingTime = _time;
    }
    void setSmoothingRate(float _rate){
        smoothingRate = _rate;
        //        ofLog()<<label<<" smoothingRate "<<smoothingRate<<" dyingTime "<<dyingTime;
        
    }
    void setRectScaler(float _scale){
        rectScaler = _scale;
    }
    void setRectYoffset(float _value){
        rectYoffset = _value;
    }
    void setMaxRect(ofRectangle & _rect){
        maxRect = _rect;
    }
    void setup(const cv::Rect& track) {
        
        color.setHsb(ofRandom(0, 255), 255, 255);
        rect = toOf(track);
        
//        ofLog()<<label<<" setup track ";
        curPos = rect.getCenter();
        //    curPos = toOf(track).getCenter();
        smoothPos = curPos;
        smoothingRate = 0.4;
    }
    
    
    void update(const cv::Rect& track) {
        //    void update(const onePose & track) {
        rect = toOf(track);
        //        rect = track.getFaceRect();
        curPos = rect.getCenter();
    }
    void updateRects(){
        
        
        if(bUseBiQuadFilter == true){
            if(old_bqFilter_fc != bqFilter_fc){
                old_bqFilter_fc = bqFilter_fc;
                ofLog()<<"onePerson reset filter";
                  bqFilter_pos.setFc(bqFilter_fc);
                bqFilter_rect.setFc(bqFilter_fc);
            }
            
             bqFilter_pos.update(curPos);
            bqFilter_rect.update(ofVec4f(rect.getTopLeft().x,rect.getTopLeft().y,rect.getBottomRight().x,rect.getBottomRight().y));
           
            smoothPos = bqFilter_pos.value();
            ofVec4f temp_vec4 = bqFilter_rect.value();
            
            smoothedRect.set(ofVec2f(temp_vec4[0],temp_vec4[1]),ofVec2f(temp_vec4[2],temp_vec4[3]));

        } else{
            
            double one_minus_alpha = 1.0 - smoothingRate;
            
            //    curPos = toOf(track).getCenter();
            smoothPos.interpolate(curPos, smoothingRate);
            
            //        new_BPM_duration = int(bpm_alpha * BPM_duration) + int((1 - bpm_alpha) * temp_bpmDuration); //running average
            //        ofLog()<<label<<" smoothingRate "<<smoothingRate<<" dyingTime "<<dyingTime;
            //        smoothedRect.x = ofLerp(smoothedRect.x, rect.x, smoothingRate);
            //        smoothedRect.y = ofLerp(smoothedRect.y, rect.y, smoothingRate);
            //        smoothedRect.width = ofLerp(smoothedRect.width, rect.width, smoothingRate);
            //        smoothedRect.height = ofLerp(smoothedRect.height, rect.height, smoothingRate);
            
            //        x = x*(1-p) + pnt.x*p;
            //        y = y*(1-p) + pnt.y*p;
            
            //        glm::vec3 temp_tl = rect.getTopLeft();
            //        smoothedRect.getTopLeft().interpolate(ofVec2f(temp_tl),smoothingRate);
            //        glm::vec3 temp_br = rect.getBottomRight();
            //        smoothedRect.getBottomRight().interpolate(temp_br,smoothingRate);
            //        
            //        temp_tl = rect.getTopLeft();
            //        smoothedBodyRect.getTopLeft().interpolate(temp_tl,smoothingRate);
            //        temp_br = rect.getBottomRight();
            //        smoothedBodyRect.getBottomRight().interpolate(temp_br,smoothingRate);
            
            
            glm::vec3 temp_tl =  smoothedRect.getTopLeft()*one_minus_alpha +  rect.getTopLeft() * smoothingRate; //smoothingRate * smoothedRect.getTopLeft() + one_minus_alpha * rect.getTopLeft();
            glm::vec3 temp_br =  smoothedRect.getBottomRight()*one_minus_alpha +  rect.getBottomRight() * smoothingRate; //smoothingRate * smoothedRect.getBottomRight() + one_minus_alpha * rect.getBottomRight();
            smoothedRect.set(temp_tl,temp_br);

            
            
            //        if(startedDying) {
            //            float temp_s = ofMap(ofGetElapsedTimef() - startedDying, 0, dyingTime, 1, 0, true);
            ////            ofLog()<<"dying temp_s "<<temp_s<<" rect before "<<smoothedRect;
            //            smoothedRect.scaleWidth(temp_s);
            ////            ofLog()<<"dying temp_s "<<temp_s<<" rect after "<<smoothedRect;
            //            smoothedBodyRect.scaleWidth(temp_s);
            //        }
        }
        historyLine.addVertex(ofPoint(smoothPos));
        
//        scaled_roiRect = smoothedRect;
//        scaled_roiRect.scaleFromCenter(rectScaler);
//        scaled_roiRect.y += rectYoffset;
//        scaled_roiRect = constrainRect(scaled_roiRect, maxRect);
    }
    
    float getDyingFactor(){
        if(startedDying) {
            return ofMap(ofGetElapsedTimef() - startedDying, 0, dyingTime, 1, 0, true);
        }else{
            return 1;
        }
    }
    void draw(string _otherStr) {
        ofPushStyle();
        float size = 16;
        ofSetColor(255);
        if(startedDying) {
            //            ofLog()<<"startedDying "<<startedDying;
            ofSetColor(ofColor::red);
            size = ofMap(ofGetElapsedTimef() - startedDying, 0, dyingTime, size, 0, true);
        }
        ofNoFill();
//        ofSetColor(color);
                ofSetColor(127,127);
        ofDrawCircle(curPos, size);
        ofDrawRectangle(rect);
        
//        historyLine.draw();
        

        ofNoFill();

         ofSetColor(color);
        ofDrawRectangle(smoothedRect);
//        ofDrawCircle(smoothPos, size);

        
        //draw text info with box
        ofPushMatrix();
        ofTranslate(rect.getLeft(),rect.getBottom()+40);
        float add_y = 0; //(label % 10) * 15;
        string str = "id:"+ofToString(label)+","+_otherStr;
        ofBitmapFont bf;
        ofRectangle bf_rect = bf.getBoundingBox(str, 0,0);  
        ofFill();
        ofSetColor(0,255);
        ofDrawRectangle(bf_rect.x,bf_rect.y,-bf_rect.getWidth(),bf_rect.getHeight());
        ofSetColor(255,255);
        
//        ofDrawBitmapString(str, curPos.x,curPos.y + add_y);
        ofDrawBitmapString(str, 0,0);        
        ofPopMatrix();

        
        //scaled and constrained roi
//        ofSetColor(0);
//        ofDrawRectangle(scaled_roiRect);
        
        ofPopStyle();
    }
    
    void kill() {
        float curTime = ofGetElapsedTimef();
        //     ofLog()<<"dyingTime "<<dyingTime;
        if(startedDying == 0) {
            startedDying = curTime;
        } else if(curTime - startedDying > dyingTime) {
            dead = true;
        }
    }
    
    ofRectangle constrainRect( ofRectangle & rect0,  ofRectangle & rect1){
        
        //                if(rect0.getLeft() < rect1.getLeft()) rect0.x = rect1.x;
        //                if(rect0.getTop() < rect1.getTop()) rect0.y = rect1.y;  
        
        //        ofLog()<<"constrainRect rect0 "<<rect0<<" rect1 "<<rect1;
        
        ofRectangle intersect_rect = rect1.getIntersection(rect0);
        
        float left = intersect_rect.getLeft()  - rect0.getLeft();
        float right = intersect_rect.getRight() - rect0.getRight();
        float top = intersect_rect.getTop() - rect0.getTop();
        float bottom = intersect_rect.getBottom() - rect0.getBottom();
        
        ofRectangle corrected_rect = rect0;
        corrected_rect.translateX(left);
        corrected_rect.translateX(right);
        
        corrected_rect.translateY(top);
        corrected_rect.translateY(bottom);
        
        return corrected_rect;
    }
    
};
#endif /* onePerson_h */
