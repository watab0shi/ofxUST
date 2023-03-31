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

- After using the project generator i had to remove all urg_library-1.2.5 or urg_library-1.2.7 files from the Header Search Path and the Compile Sources tabs.
![](https://github.com/antimodular/ofxUST/blob/dev_antimodular/readme_media/buildPhase_compileSources.png?raw=true)
![](https://github.com/antimodular/ofxUST/blob/dev_antimodular/readme_media/headerSearchPaths.png?raw=true)

- For macOS 12.6.3 and of_v20230326_osx_release i also had to replace `std::auto_ptr<pImpl> pimpl;` with `std::unique_ptr<pImpl> pimpl;` as it was suggested here https://github.com/Open-Transactions/Moneychanger/issues/308#issuecomment-395919991
- I also copied `/Applications/of_v20230326_osx_release/addons/ofxUST/libs/urg_library-1.2.7/src/liburg_cpp.so` and `/liburg_cpp.a` to `/Applications/of_v20230326_osx_release/addons/ofxUST/libs/urg/lib/osx`
- Also copied everything in folder `/Applications/of_v20230326_osx_release/addons/ofxUST/libs/urg_library-1.2.7/include/cpp` to `/Applications/of_v20230326_osx_release/addons/ofxUST/libs/urg/include`
