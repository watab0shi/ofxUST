//
//  oneLine.h
//  example_tracking
//
//  Created by Stephan Schulz on 2020-10-23.
//

#ifndef oneLine_h
#define oneLine_h
class oneLine{
    
public:
    int index;
    float age;
    ofVec2f bg_pos;
    float bg_distance;
    ofVec2f position;
    float distance;  
    float threshold_dist = 30;
    
    Hysteresis hysteresis;
    FadeTimer fadeTimer;
    bool bDetection;
    
    float smoothingRate_invt;
    float smoothingRate;
    
    void setup(int _idx){
        index = _idx;
        // wait to turn on, wait longer to turn off
        hysteresis.setDelay(0.1, 0);
        // fade in fast, fade out slow
        fadeTimer.setLength(1, 1);
        
        smoothingRate = 0.5;
        smoothingRate_invt = 1.0 - smoothingRate;
    }
    
    void update(ofVec2f _pos){
        
        float prev_distance = _pos.distance(position);
        
        if(prev_distance > 300) ofLog()<<"prev_dist big "<<prev_distance<<" pos "<<_pos;
        
         position = (smoothingRate * position) + (smoothingRate_invt * _pos); //running average
//        position = _pos + (_pos + position)/2.0;
        
        distance = ofVec2f(0,0).distance(position);
//        if(bg_pos.distance(position) > threshold_dist){
        float distDiff = ABS(bg_distance - distance);
        if(distDiff > threshold_dist){
            bDetection = true;
//            ofLog()<<index<<" distDiff "<<distDiff<<" position "<<position;
        }else{
            bDetection = false;
        }
        hysteresis.update(bDetection);
        
        if(hysteresis.wasTriggered()) {
//            ofLog() << "triggered";
        }
        if(hysteresis.wasUntriggered()) {
//            ofLog() << "untriggered";
        }
        
//        fadeTimer.update(hysteresis);
        
    }
    
    void draw(){
        
        float alpha = bDetection ? 100 : 255;
        if(hysteresis.get() == true) ofSetColor(255,255,0,alpha);
        else ofSetColor(127,alpha);
        ofDrawLine(0,0,position.x,position.y);
        
        ofSetColor(50,alpha);
        ofDrawLine(position , bg_pos);
        
        ofFill();
        ofSetColor( 0,255,255 );
        ofDrawCircle(bg_pos, 6);
    }
    void drawInfo(){
        stringstream msg;
        msg << "detected: " << (bDetection ? "on" : "off") << endl;
        msg << "hysteresis: " << (hysteresis.get() ? "on" : "off") << endl;
        msg << "fadeTimer: " << fadeTimer.get() << endl;
        ofDrawBitmapString(msg.str(), 10, 20);
    }
    
    void setBackground(ofVec2f _pos){
        bg_pos = _pos;
        bg_distance = ofVec2f(0,0).distance(_pos);
    }
};

#endif /* oneLine_h */
