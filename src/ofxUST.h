#pragma once

#include "ofThread.h"
#include "ofVec2f.h"
#include "Urg_driver.h"

using namespace qrk;


// ofUST
//----------------------------------------
class ofxUST : public ofThread
{
public:
  ofxUST();
  ~ofxUST();
  
  enum Direction
  {
    DIRECTION_RIGHT = 0,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_UP,
    DIRECTION_SIZE
  };

  void open(std::string _deviceIp = "192.168.0.10");
  
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

  void start();
  void stop();
  
  void update();
  
  void close();

  void threadedFunction();
  
  std::vector< long >    data;
  std::vector< long >    dataBuffer;
  std::vector< ofVec2f > coordinates;
  
private:
  const int  port = 10940;
  std::string deviceIp;
  
  Urg_driver urg;
  bool       bConnected;
  
  int        minStep, maxStep;
  float      minAngle, maxAngle;
  int        skip;
  
  Direction  direction;
  bool       bMirror;

  float time;
  float checkInterval;
  float lastCheckTime;

  bool isFrameNew;
};
