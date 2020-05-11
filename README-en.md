# KissFFT-For-Windows
A Windows Version of Kiss FFT.

Kiss FFT is an FFT library developed by Mark Borgerding.

* Documents(文档):
    + [zh-cn](README.md)
    + en-us

## What is KF4Win?
* __KF4Win is a library which is based on Kiss FFT, allows you to easily deploy on Windows__
    + KF4Win has lots of High-Level APIs, you can use them while start using Kiss FFT
    + KF4Win has a more complete document, you can aquire the usage of those APIs
    + KF4Win provides Startup codes and data, you can freely use them to start using this library
    + KF4Win provides scripts that can compile this library on Windows，you can use __nmake__ to compile KF4Win

## What can KF4Win do?
* __KF4Win is an FFT(Fast Fourier Transform) library__
    + KF4Win supports 100% all of the Kiss FFT APIs, you can directly use those APIs, e.g. kiss_fft()
    + KF4Win provides several C++ classes, which allows you to not focus on memory managing or data processing, but the result. Feed it data and it will return the result, simple
    + KF4Win can be compiled to a dynamic library(.dll) or a static library(.lib), and can be freely used on any libraries or applications

## How can I use KF4Win?
* __It is sorry to say that, KF4Win is not finished yet. But you can use git to clone it to local and compile it__
    + use __nmake__ to compile:
    `nmake /f Makefile.msc`

## How can I clean the directory of KF4Win?
* __use:__ `nmake /f Makefile.msc clean`

## For starters
* __For starters, using the dynamic library link of KF4Win is a good choice.__
    1. Download and extract the source code.
        + using __git__ : `git clone https://github.com/ToxicObsidian/KissFFT-For-Windows.git`
        + using __HTTPS__ : [src code](https://github.com/ToxicObsidian/KissFFT-For-Windows/archive/master.zip)
    2. Start the prompt of tools for Microsoft Visual Studio.
    3. Change directory to the source code folder.
    4. Use __nmake__ to compile : `nmake /f Makefile.msc`.
    5. Copy files in the __include__ folder to your project folder.
    6. Copy KF4Win.dll & KF4Win.lib in the __lib__ folder to your project folder.
    7. Add `#include "kf4win.h"` to your code.
    8. Link KF4Win.lib
        + Using Visual Studio GUI
            + Add `#pragma comment(lib, "KF4Win.lib")` to your code. Copy KF4Win.lib to your Visual Studio lib directory.
            + Or
            + __Options > VC++ Directories__, add the folder which includes KF4Win.lib to the Library Directories.
        + Using prompt of tools for Visual Studio
            + Use `link ... KF4Win.lib ...`
    9. Check the result.

## More Documents
* __Read DOCUMENT.md:__ [Document](DOCUMENT.md)

Copyright © 2019-2020 by Toxic_Obsidian. All rights reserved.
