//
//  ofxUST.cpp
//  URGSensor_Test
//
//  Created by M-wataru-kani on 2017/01/27.
//
//

#include "ofxUST.h"


// ofxUST
//------------------------------------------------------------
ofxUST::ofxUST()
{
  cout << "[ofxUST] " << "Urg_driver::open()" << endl;
  
  if( urg.open( "192.168.0.10", 10940, Urg_driver::Ethernet ) )
  {
    bConnected = true;
    
    cout << "[ofxUST] " << "  succeeded" << endl;
    cout << "[ofxUST] " << "info" << endl;
    cout << "[ofxUST] " << "  minDistance : " << urg.min_distance() << endl;
    cout << "[ofxUST] " << "  maxDistance : " << urg.max_distance() << endl;
    cout << "[ofxUST] " << "  minStep     : " << urg.min_step() << endl;
    cout << "[ofxUST] " << "  maxStep     : " << urg.max_step() << endl;
    
    direction = DIRECTION_RIGHT;
    bMirror   = false;
  }
  else
  {
    bConnected = false;
    cout << "[ofxUST] " << "  failed" << endl;
  }
}


// ~ofxUST
//------------------------------------------------------------
ofxUST::~ofxUST()
{
}


// setDirection
//------------------------------------------------------------
void ofxUST::setDirection( ofxUST::Direction _dir )
{
  direction = _dir;
}


// setMirror
//------------------------------------------------------------
void ofxUST::setMirror( bool _b )
{
  bMirror = _b;
}


// getDirection
//------------------------------------------------------------
ofxUST::Direction ofxUST::getDirection()
{
  return direction;
}


// getMinDistance
//------------------------------------------------------------
int ofxUST::getMinDistance()
{
  return urg.min_distance();
}


// getMaxDistance
//------------------------------------------------------------
int ofxUST::getMaxDistance()
{
  return urg.max_distance();
}


// getMinStep
//------------------------------------------------------------
int ofxUST::getMinStep()
{
  return urg.min_step();
}


// getMaxStep
//------------------------------------------------------------
int ofxUST::getMaxStep()
{
  return urg.max_step();
}


// isConnected
//------------------------------------------------------------
bool ofxUST::isConnected()
{
  return bConnected;
}


// setScanningParameterBySteps
//------------------------------------------------------------
void ofxUST::setScanningParameterBySteps( int _minStep, int _maxStep, int _skipStep )
{
  skip = _skipStep;
  if( bConnected ) urg.set_scanning_parameter( _minStep, _maxStep, _skipStep );
}


// setScanningParameterByAngles
//------------------------------------------------------------
void ofxUST::setScanningParameterByAngles( float _minAngle, float _maxAngle, int _skipStep )
{
  skip = _skipStep;
  if( bConnected ) urg.set_scanning_parameter( urg.deg2step( _minAngle ), urg.deg2step( _maxAngle ), _skipStep );
}


// startMeasurement
//------------------------------------------------------------
void ofxUST::startMeasurement()
{
  if( bConnected ) urg.start_measurement( Urg_driver::Distance, Urg_driver::Infinity_times, 0 );
}


// stopMeasurement
//------------------------------------------------------------
void ofxUST::stopMeasurement()
{
  if( bConnected ) urg.stop_measurement();
}


// update
//------------------------------------------------------------
void ofxUST::update()
{
  if( !bConnected ) return;
  
  long time_stamp = 0;
  
  if( !urg.get_distance( data, &time_stamp ) )
  {
    cout << "[ofxUST] " << "Urg_driver::get_distance() : " << urg.what() << endl;
    return;
  }
  
  long min_distance = urg.min_distance();
  long max_distance = urg.max_distance();
  size_t nData      = data.size();
  
  coordinates.clear();
  
  for( size_t i = 0; i < nData; ++i )
  {
    int idx = ( bMirror ) ? ( nData - 1 ) - i : i;
    if( skip > 0 ) idx *= skip;
    
    long l  = data.at( i );
    
    if( ( l < min_distance ) || ( l > max_distance ) ) continue;
    
    double radian = urg.index2rad( idx ) + ( ( int )direction * HALF_PI );
    long x        = static_cast< long >( cos( radian ) * l );
    long y        = static_cast< long >( sin( radian ) * l );
    
    coordinates.push_back( ofVec2f( x, y ) );
  }
}


// close
//------------------------------------------------------------
void ofxUST::close()
{
  if( bConnected ) urg.close();
}
