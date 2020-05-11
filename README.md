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
* __很抱歉，KF4Win暂时并没有被写完，但您同样可以利用git将代码克隆至本地进行编译__
    + 使用 __nmake__ 进行编译:
    `nmake /f Makefile.msc`

## 我该如何清理KF4Win的目录?
* __使用命令：__ `nmake /f Makefile.msc clean` __即可__

Copyright © 2019-2020 by Toxic_Obsidian. All rights reserved.
