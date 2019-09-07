#include "ofxUST.h"


// ofxUST
//----------------------------------------
ofxUST::ofxUST()
: direction( DIRECTION_DOWN )
, bMirror( false )
{
}

// open
//----------------------------------------
bool ofxUST::open()
{
  bConnected = urg.open( deviceIp.c_str(), port, Urg_driver::Ethernet );
  
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
    urg.set_scanning_parameter( _minStep, _maxStep, _skipStep );
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
    urg.set_scanning_parameter( urg.deg2step( _minAngle ), urg.deg2step( _maxAngle ), _skipStep );
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
  if( !bConnected ) return;
  
  long time_stamp = 0;
  
  // error
  if( !urg.get_distance( data, &time_stamp ) )
  {
    ofLog() << "[ofxUST::update][Urg_driver::get_distance()] " << urg.what();
    
    close();
    
    // try re-open
    bool bOpen = open();
    if( bOpen )
    {
      setScanningParameterByAngles( minAngle, maxAngle, skip );
      startMeasurement();
    }
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
