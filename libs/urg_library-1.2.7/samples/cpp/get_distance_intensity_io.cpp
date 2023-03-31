/*!
  \example get_distance_io.cpp Obtains distance, intensity and IO(input/output) data

  $Id$
*/

#include "Urg_driver.h"
#include "Connection_information.h"
#include "math_utilities.h"
#include <iostream>
#include <sstream> 
#include <bitset>

using namespace qrk;
using namespace std;


namespace
{
    void print_data(const Urg_driver& urg,
                    const vector<long>& data,
		            const vector<unsigned short>& intensity,
		            const vector<long>& io,
		            long time_stamp)
    {
		cout << "timestamp: " << time_stamp << " [msec]" << endl;

	// Display IO information
		stringstream ss;
		ss << bitset<18>(io[0]);
		string input = ss.str();
		stringstream ss2;
		ss2 << bitset<18>(io[1]);
		string output = ss2.str();
		cout << "input    : " << input  << " (" << io[0] << ")" << endl;
		cout << "output   : " << output << " (" << io[1] << ")" << endl;

    // Shows only the front step
        int front_index = urg.step2index(0);
        cout << "distance : " << data[front_index] << " [mm]" << endl;
		cout << "intensity: " << data[front_index] << " [1]"  << endl;

        cout << endl;
    }
}


int main(int argc, char *argv[])
{
    Connection_information information(argc, argv);

    // Connects to the sensor
    Urg_driver urg;
    if (!urg.open(information.device_or_ip_name(),
                  information.baudrate_or_port_number(),
                  information.connection_type())) {
        cout << "Urg_driver::open(): "
             << information.device_or_ip_name() << ": " << urg.what() << endl;
        return 1;
    }

    // Gets measurement data
#if 1
    // Case where the measurement range (start/end steps) is defined
    urg.set_scanning_parameter(urg.deg2step(-90), urg.deg2step(+90), 0);
#endif
    enum { Capture_times = 10 };
    urg.start_measurement(Urg_driver::Distance_intensity_io, Urg_driver::Infinity_times, 0);
    for (int i = 0; i < Capture_times; ++i) {
        vector<long> data;
		vector<unsigned short> intensity;
		vector<long> io;
        long time_stamp = 0;

        if (!urg.get_distance_intensity_io(data, intensity, io, &time_stamp)) {
            cout << "Urg_driver::get_distance_intensity_io(): " << urg.what() << endl;
            return 1;
        }
        print_data(urg, data, intensity, io, time_stamp);
    }

#if defined(URG_MSC)
    getchar();
#endif
    return 0;
}
