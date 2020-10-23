# ofxUST
Addon for using HOKUYO AUTOMATIC range sensor [UST Series](http://www.hokuyo-aut.co.jp/search/single.php?serial=16) on openFrameworks.

# Install Library
Download latest URG library from [here](https://sourceforge.net/projects/urgnetwork/files/urg_library/).

Copy the library to the addon's lib folder.
![](https://user-images.githubusercontent.com/1054816/97043938-41724f00-1541-11eb-865e-ecbc206bf3c9.png)

For Mac  
Enter the following commands to terminal.  
`$ cd /path/to/download/urg_library`  
`$ make`  
`$ make install`  

# Connection
Connect the sensor to the power supply.  
Connect sensor's LAN cable to your PC.  

Set the IP address according to the IP address of the device.  
I my case i set my computer to  
IP Address 192.168.1.15
Subnet Mask 255.255.255.0
Default Gateway 192.168.1.1

# Devices
- Hokuyo UST UST-10LX

