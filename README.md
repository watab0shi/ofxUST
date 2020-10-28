# ofxUST
Addon for using HOKUYO AUTOMATIC range sensor [UST Series](http://www.hokuyo-aut.co.jp/search/single.php?serial=16) on openFrameworks.

![](https://github.com/antimodular/ofxUST/blob/dev_antimodular/run.gif?raw=true)


# Install Library
Download latest URG library from [here](https://sourceforge.net/projects/urgnetwork/files/urg_library/).

Copy the library to the addon's lib folder.
![](https://github.com/antimodular/ofxUST/blob/dev_antimodular/readme_media/run.gif?raw=true)

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
IP Address 192.168.0.15
Subnet Mask 255.255.255.0
Default Gateway 192.168.0.1

# Devices
- Hokuyo UST UST-10LX

# Compiling
- After using the project generator i had to remove all urg_library-1.2.5 files from the Header Search Path and the Compile Sources tabs.
![](https://github.com/antimodular/ofxUST/blob/dev_antimodular/readme_media/buildPhase_compileSources.png?raw=true)
![](https://github.com/antimodular/ofxUST/blob/dev_antimodular/readme_media/headerSearchPaths.png?raw=true)
