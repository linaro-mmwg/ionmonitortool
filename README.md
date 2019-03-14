ionmonitortool
==============

The purpose of this tool is to help report many buffers are being used in various "secure video" pipeline use cases.

There are usually 3 secure heaps (decrypt, decoded, display) which are unmapped ion heaps, so they are unavailable for other general system activities.
This tool will help visualize how the secured memory is managed, depending of the type of video streams.
It will allow to detect memory issues as memory leaks, and also to optimize the size of these heaps.


How it works
------------

The tool is integrated to kernel space, in ION driver.
Basically, it's based on debugfs infrastructure. So, it will create a debug files in /sys/kernel/debug/ion/ for each allocated heaps.
The debug function that reports information about the heap is triggered by open system call on the debugfs file.
For example:
```
# cat /sys/kernel/debug/ion/<heap-name>
```

The tool will give you many information about the heap, as :
- Heap size
- Free space
- Allocated space
- Largest allocated space
- Largest free space
- Heap fragmentation

The heap fragmentation in given by:
```
(free_space - largest_free_space)/free_space
```

At the moment, only unmapped heaps are supported by the tool.

The tool can be disable and enabled dynamically:
```
// disable the tool on the heap <heap-name>
# echo 0 > /sys/kernel/debug/ion/<heap-name>
// enable it back
# echo 1 > /sys/kernel/debug/ion/<heap-name>
```

Impact on performances
----------------------

The tool is made to be isolated enough from buffer allocation time so it won't interfere on playback performances.
For that, the information to display are updated only when the tool is used.

Environment
-----------

The tool has been designed and tested in Android environment, using kernel 4.14.
It should be easily portable to other kernel version, but at the moment only 4.14 is supported.

Maintainers
-----------

* Axel Le Bourhis <mailto:axel.lebourhis@linaro.org>



