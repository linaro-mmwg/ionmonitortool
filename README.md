# ionmonitortool

The purpose of this tool is to help report many buffers are being used in various "secure video" pipeline use cases.  

There are usually 3 secure heaps (decrypt, decoded, display) which are unmapped ion heaps, so they are unavailable for other general system activities.  
This tool will help visualize how the secured memory is managed, depending of the type of video streams.  
It will allow to detect memory issues as memory leaks, and also to optimize the size of these heaps.  

## How it works

The tool is a userspace binary based on debug info given by the ION driver.  
Some patches are needed in the ION driver to enable debug information.  

### ION driver

Basically, the patches add debugfs infrastructure to ION driver.   
So, it will create a debug files in /sys/kernel/debug/ion/ for each supported existing heaps.  
The debug function that reports information about the heap is triggered by open system call on the debugfs file.  
For example:
```
 cat /sys/kernel/debug/ion/<heap-name>
```

The debug file will give you many information about the heap, as :  
- Heap size
- Free space
- Allocated space
- Largest allocated spac 
- Largest free space
- Heap fragmentation

The heap fragmentation in given by:
```
(free_space - largest_free_space)/free_space
```

At the moment, all ion heaps are supported by the tool but system heaps.  

The debug info can be disabled and enabled dynamically:
```
// disable the tool on the heap <heap-name>
 echo 0 > /sys/kernel/debug/ion/<heap-name>
// enable it back
 echo 1 > /sys/kernel/debug/ion/<heap-name>
```

### Ion-monitor-tool

The tool is designed to be run on Android 9 with DRM enabled, and with a Kernel 4.14 patched with ION driver modifications.  
Please, refer to section Environment for more details.  

The tool will automatically drive Exoplayer, to run video playback (atm Widevine clear and secure content).  
It will perform one measure before playing a video playback and one after the playback.  
Then, the measures will be compared to check if some memory hasn't been released.  
In one side, if the difference is positive, a warning will be recorded.  
In the other side, if the difference is negative, it means that the memory has been tardily released, so a warning will be deleted.  
These steps will be repeated the number of time the user wants, so it is possible to check for memory leaks after many video playbacks.  
As each stream is considered as a test case, the tool will report a pass/fail state for each test case.  

The tool also set a test plan based on some parameters. See section "Run ion-monitor-tool".  

## Run ion-monitor-tool

When you run the tool, you will need to tell what test plan you want to run. (See section test plans to have a detailed list)  
To test clear content : -c  
To test secure content : -s  
To test UHD (4k) content : -u  
Note that UHD content needs to be paired at least one of the other options.  

```
// run only clear content
./ion-monitor-tool -c
// run clear and secure content
./ion-monitor-tool -c -s
```

You can choose what heap to monitor. Remember you need to pass a supported heap type to the tool.  
By default, the heap /sys/kernel/debug/ion/display is selected.  
```
// monitor optee heap and play secure content
./ion-monitor-tool -f /sys/kernel/debug/ion/optee -s
```

You may also want to set the number of playback repeat.  
By default, the tool will repeat the playback and the measures 5 times.  
```
// monitor default heap and play secure content 10 times for each test case
./ion-monitor-tool -s -r 10
```

Naturally, the tool has a help menu, that can be displayed with -h option.  
```
./ion-monitor-tool -h
```

## Test plans

Today, 4 different test plans are supported:  

Clear, which embeds :
```
	WIDEVINE_DASH_H264_CLEAR_SD
	WIDEVINE_DASH_H264_CLEAR_HD
	WIDEVINE_DASH_H264_CLEAR_SD_HD
	WIDEVINE_DASH_H265_CLEAR_SD
	WIDEVINE_DASH_H265_CLEAR_HD
	WIDEVINE_DASH_H265_CLEAR_SD_HD
```

Secure, which embeds :
```
	WIDEVINE_DASH_H264_SECURE_CENC_SD
	WIDEVINE_DASH_H264_SECURE_CENC_HD
	WIDEVINE_DASH_H264_SECURE_CENC_SD_HD
	WIDEVINE_DASH_H264_SECURE_CBC1_SD
	WIDEVINE_DASH_H264_SECURE_CBC1_HD
	WIDEVINE_DASH_H264_SECURE_CBC1_SD_HD
	WIDEVINE_DASH_H264_SECURE_CBCS_SD
	WIDEVINE_DASH_H264_SECURE_CBCS_HD
	WIDEVINE_DASH_H264_SECURE_CBCS_SD_HD
	WIDEVINE_DASH_H265_SECURE_CENC_SD
	WIDEVINE_DASH_H265_SECURE_CENC_HD
	WIDEVINE_DASH_H265_SECURE_CENC_SD_HD
```

Clear UHD, which embeds :
```
	WIDEVINE_DASH_H264_CLEAR_UHD
	WIDEVINE_DASH_H265_CLEAR_UHD
```

Secure UHD, which embeds :
```
	WIDEVINE_DASH_H264_SECURE_CENC_UHD
	WIDEVINE_DASH_H264_SECURE_CBC1_UHD
	WIDEVINE_DASH_H264_SECURE_CBCS_UHD
	WIDEVINE_DASH_H265_SECURE_CENC_UHD
```

## Read the results

It is recommended to save all the tool's output to a logfile so you can parse it at the end of the test.  
Direct type of logs are printed by the tool :  

INFO : gives information about the test, like values measured.  
WARNING : only used when leak behavior has been detected.  
RESULT : gives test case result, this is what you want to parse if you want to get results easily.  

## Log files

The tool will also dump heap informations in seperated log files, including timestamps.  
The files are created in the same directory the tool is in.  
This feature is useful for developers who wants to trace graphs after the test, you only have to import it in Excel for example.  

## Impact on performances

The debug info is made to be isolated enough from buffer allocation time so it won't interfere on playback performances.  
For that, the information to display are updated only when the tool is used.  

## Environment

The tool has been designed and tested in Android 9 environment, using kernel 4.14.  
It should be easily portable to other kernel version, but at the moment only 4.14 is supported.  

The patches are available on this repository :  
https://github.com/axelnxp/linux-imx/tree/imx_4.14.98_2.0.0_ga  

The following commit gives all patches needed to enable debug info in the ION driver :  
https://github.com/axelnxp/linux-imx/commit/f85d8b4c8cb94dd8adb020f15d4b1ae7a16ff16b  

Exoplayer needs to be installed in the Android image, and an internet connection is required.  
It has been tested on NXP's imx8m.  

## Maintainers

* Axel Le Bourhis <mailto:axel.lebourhis@linaro.org>  
