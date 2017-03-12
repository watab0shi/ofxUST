//
//  ofxUST.h
//  URGSensor_Test
//
//  Created by M-wataru-kani on 2017/01/27.
//
//

#ifndef __URGSensor_Test__ofxUST__
#define __URGSensor_Test__ofxUST__

#include "ofAppRunner.h"
#include "ofVec2f.h"
#include "Urg_driver.h"

using namespace std;
using namespace qrk;


// ofUST
//--------------------------------------------------------------------------------
class ofxUST
{
public:
  ofxUST();
  ~ofxUST();
  
  enum Direction
  {
    DIRECTION_RIGHT = 0,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_UP
  };
  
  void setDirection( Direction _dir );
  void setMirror( bool _b );
  
  void setScanningParameterBySteps( int _minStep, int _maxStep, int _skipStep );
  void setScanningParameterByAngles( float _minAngle, float _maxAngle, int _skipStep );
  
  Direction getDirection();
  
  int  getMinDistance();
  int  getMaxDistance();
  
  int  getMinStep();
  int  getMaxStep();
  
  bool isConnected();
  
  void startMeasurement();
  void stopMeasurement();
  
  void update();
  
  void close();
  
  vector< long >    data;
  vector< ofVec2f > coordinates;
  
private:
  
  Urg_driver urg;
  bool       bConnected;
  Direction  direction;
  int        skip;
  bool       bMirror;
};

#endif /* defined(__URGSensor_Test__ofxUST__) */
