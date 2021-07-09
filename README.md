# ofxUST
Addon for using HOKUYO AUTOMATIC range sensor [UST Series](http://www.hokuyo-aut.co.jp/search/single.php?serial=16) on openFrameworks.

# Install Library
Download latest URG library from [here](https://sourceforge.net/projects/urgnetwork/files/urg_library/).

For Mac  
Enter the following commands to terminal.  
`$ cd /path/to/download/urg_library`  
`$ make`  
`$ make install`  

## macOS Big Sur

If you failed `make` with this error,  
`urg_utils.c:218:5: error: implicit declaration of function 'usleep' is invalid in C99 [-Werror,-Wimplicit-function-declaration]`  

Add `#include <unistd.h>` line to `urg_utils.c`.

```
  #include <math.h>
+ #include <unistd.h>
```

### Building Project
If you get these warnings with Release build,  
```
ld: warning: ignoring file /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX11.3.sdk/System/Library/Frameworks//QTKit.framework/QTKit.tbd, missing required architecture arm64 in file /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX11.3.sdk/System/Library/Frameworks//QTKit.framework/QTKit.tbd
ld: warning: ignoring file ../../../addons/ofxUST/libs/urg/lib/osx/liburg_cpp.so, building for macOS-arm64 but attempting to link with file built for macOS-x86_64
ld: warning: ignoring file ../../../addons/ofxUST/libs/urg/lib/osx/liburg_cpp.so, building for macOS-arm64 but attempting to link with file built for macOS-x86_64
```

Set `Build Settings` below. (`PROJECT` & `TARGET`)  
`Build Settings > Architectures > Build Active Architecture Only > Release > "Yes"`

# Connection
Connect the sensor to the power supply.  
Connect sensor's LAN cable to your PC.  
Set the IP address according to the IP address of the device. (ex: `192.168.0.100`)  

# Caution
Due to the specifications of the sensor, ofApp will fixed to 40FPS.  
Please use with `ofThread` if necessary.  
