#include "ofxUST.h"
#include "ofAppRunner.h"
#include "ofLog.h"


// ofxUST
//----------------------------------------
ofxUST::ofxUST()
: direction( DIRECTION_DOWN )
, bMirror( false )
, time( 0.0 )
, lastCheckTime( 0.0 )
, checkInterval( 1.0 )
{
}

// open
//----------------------------------------
void ofxUST::open(std::string _deviceIp)
{
  bConnected = urg.open( _deviceIp.c_str(), port, Urg_driver::Ethernet );
  
  if( bConnected )
  {
    ofLog() << "[ofxUST::open] succeeded" << std::endl
            << "  info" << std::endl
            << "    product_type     : " << urg.product_type() << std::endl
            << "    firmware_version : " << urg.firmware_version() << std::endl
            << "    serial_id        : " << urg.serial_id() << std::endl
            << "    status           : " << urg.status() << std::endl
            << "    state            : " << urg.state() << std::endl
            << "    min_distance     : " << urg.min_distance() << std::endl
            << "    max_distance     : " << urg.max_distance() << std::endl
            << "    min_step         : " << urg.min_step() << std::endl
            << "    max_step         : " << urg.max_step();
  }
  else
  {
    ofLog() << "[ofxUST::open] failed";
  }
}

// setDirection
//----------------------------------------
void ofxUST::setDirection( ofxUST::Direction _dir )
{
  direction = _dir;
}

// setMirror
//----------------------------------------
void ofxUST::setMirror( bool _b )
{
  bMirror = _b;
}

// getDirection
//----------------------------------------
ofxUST::Direction ofxUST::getDirection()
{
  return direction;
}

// getMinDistance
//----------------------------------------
int ofxUST::getMinDistance()
{
  return urg.min_distance();
}

// getMaxDistance
//----------------------------------------
int ofxUST::getMaxDistance()
{
  return urg.max_distance();
}

// getMinStep
//----------------------------------------
int ofxUST::getMinStep()
{
  return urg.min_step();
}

// getMaxStep
//----------------------------------------
int ofxUST::getMaxStep()
{
  return urg.max_step();
}

// isConnected
//----------------------------------------
bool ofxUST::isConnected()
{
  return bConnected;
}

// setScanningParameterBySteps
//----------------------------------------
void ofxUST::setScanningParameterBySteps( int _minStep, int _maxStep, int _skipStep )
{
  minStep = _minStep;
  maxStep = _maxStep;
  skip    = _skipStep;
  
  if( bConnected )
  {
    minAngle = urg.step2deg( minStep );
    maxAngle = urg.step2deg( maxStep );
    urg.set_scanning_parameter( minStep, maxStep, skip );
  }
}

// setScanningParameterByAngles
//----------------------------------------
void ofxUST::setScanningParameterByAngles( float _minAngle, float _maxAngle, int _skipStep )
{
  minAngle = _minAngle;
  maxAngle = _maxAngle;
  skip     = _skipStep;
  
  if( bConnected )
  {
    minStep = urg.deg2step( minAngle );
    maxStep = urg.deg2step( maxAngle );
    urg.set_scanning_parameter( minStep, maxStep, skip );
  }
}

// startMeasurement
//----------------------------------------
void ofxUST::startMeasurement()
{
  if( bConnected )
  {
    urg.start_measurement( Urg_driver::Distance, Urg_driver::Infinity_times, 0 );
  }
}

// stopMeasurement
//----------------------------------------
void ofxUST::stopMeasurement()
{
  if( bConnected )
  {
    urg.stop_measurement();
  }
}

// update
//----------------------------------------
void ofxUST::update()
{
  time += ofGetLastFrameTime();

  if( !bConnected )
  {
    if( time > lastCheckTime + checkInterval )
    {
      // try re-open
      open();
      if( bConnected )
      {
        setScanningParameterBySteps( minStep, maxStep, skip );
        startMeasurement();
      }
      lastCheckTime = time;
    }
    return;
  }
  
  if( !urg.get_distance( data ) )
  {
    // error
    ofLog() << "[ofxUST::update][Urg_driver::get_distance()] " << urg.what();
    
    close();
    bConnected = false;
    return;
  }
  
  long min_distance = urg.min_distance();
  long max_distance = urg.max_distance();
  size_t nData      = data.size();
  
  coordinates.clear();
  
  for( size_t i = 0; i < nData; i += skip )
  {
    int idx = ( bMirror ) ? ( nData - 1 ) - i : i;
    
    long l  = data.at( i );
    
    if( ( l < min_distance ) || ( l > max_distance ) ) continue;
    
    double radian = urg.index2rad( idx ) + ( ( int )direction * HALF_PI );
    long   x      = ( long )( cos( radian ) * l );
    long   y      = ( long )( sin( radian ) * l );
    
    coordinates.push_back( ofVec2f( x, y ) );
  }
}

// close
//----------------------------------------
void ofxUST::close()
{
  if( bConnected )
  {
    urg.close();
  }
}
