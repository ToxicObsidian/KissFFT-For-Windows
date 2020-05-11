# KissFFT-For-Windows
一个Windows版本的Kiss FFT.

Kiss FFT是一个由Mark Borgerding开发的FFT库.

* 文档(Documents):
    + zh-cn
    + [en-us](README-en.md)

## 什么是KF4Win?
* __KF4Win是能够让您轻松部署在Windows下的Kiss FFT库__
    + KF4Win为Kiss FFT库添加了许多面向对象的高级API，可以让您更加快速地上手Kiss FFT
    + KF4Win为Kiss FFT库增加了许多API文档，可以让您更容易了解这些API的运作方式
    + KF4Win提供了一系列的针对于新手的教程代码和数据，能够让您直接上手这一款FFT库
    + KF4Win提供了可以在Windows平台部署的脚本，您可以利用 __nmake命令__ 来编译KF4Win

## KF4Win能做一些什么?
* __KF4Win是一个FFT(Fast Fourier Transform)库__
    + KF4Win支持100%所有的Kiss FFT库的API，您大可以直接使用这些函数，例如kiss_fft()
    + KF4Win提供了一些C++类，能让您不再关注内存管理和数据处理，您给它数据，它给您结果，就这么简单
    + KF4Win可以被编译为动态库(.dll)和静态库(.lib)，可以自由地被利用于各种库或应用程序上

## 我该如何使用KF4Win?
* __很抱歉，KF4Win暂时并没有被完成，但您同样可以利用git将代码克隆至本地进行编译__
    + 使用 __nmake__ 进行编译:
    `nmake /f Makefile.msc`

## 我该如何清理KF4Win的目录?
* __使用命令：__ `nmake /f Makefile.msc clean` __即可__

## 新手上路
* __对于新手们来说，特别是没有使用过FFT工具的人来说，利用动态链接库可以快速上手KF4Win__
    1. 下载并解压源码包
        + 使用 __git__ : `git clone https://github.com/ToxicObsidian/KissFFT-For-Windows.git`
        + 使用 __HTTPS__ : [下载源码](https://github.com/ToxicObsidian/KissFFT-For-Windows/archive/master.zip)
    2. 启动Microsoft Visual Studio本机工具命令提示
    3. 进入带有源码的文件夹
    4. 使用 __nmake__ 编译 : `nmake /f Makefile.msc`
    5. 将 __include__ 文件夹下的文件拷贝到您的工程目录下
    6. 将 __lib__ 文件夹下的KF4Win.dll和KF4Win.lib拷贝至您的工程目录下
    7. 在工程中添加 `#include "kf4win.h"` 以使用KF4Win
    8. 链接工程时链接KF4Win.lib
        + 使用 Visual Studio GUI
            + 在工程中添加 `#pragma comment(lib, "KF4Win.lib")` ，并将KF4Win.lib拷贝至Visual Studio的库目录
            + 或者
            + 在工程属性中选择 __VC++目录__ ， 在库目录中添加KF4Win.lib所在的目录
        + 使用 Visual Studio 命令行工具
            + 链接时使用 `link ... KF4Win.lib ...` 即可
    9. 验证结果

## 更多文档
* __更多文档请读DOCUMENT.md:__ [Document](DOCUMENT.md)

Copyright © 2019-2020 by Toxic_Obsidian. All rights reserved.
