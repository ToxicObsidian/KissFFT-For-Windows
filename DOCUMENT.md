# API文档
Kiss FFT的API非常简单，但即便如此，KF4Win还是提供了更加简单的API编程接口。

如果您是对于FFT已经很了解了，或者您有自己的内存管理方法，那么直接使用Kiss FFT的API可能会有更高的效率，
但如果您是新手，或苦于编写很复杂的内存管理方法，那么KF4Win提供的API接口会让你更加容易上手。

## KF4Win提供的API
* __C++类型的接口__
    + Class `KF4Win`
        + 创建一个KF4Win对象: `KF4Win *kf = new KF4Win();`
        + 销毁一个KF4Win对象: `delete kf;`
        + `int KF4Win::Init(unsigned long nMaxBufferSize)`
            + __用途__: 初始化KF4Win对象, 未被初始化的KF4Win对象无法使用其它成员方法
            + __参数__
                + nMaxBufferSize 指定了最大可用的内存大小, 但KF4Win不会去申请这么大的内存，可缺省
            + __返回值__
                + 0  - OK
                + -1 - nMaxBufferSize过小(<=3)
        + `int KF4Win::CreateContext(unsigned long nBufferCount)`
            + __用途__: 从堆中申请一块内存作为FFT的上下文
            + __参数__
                + nBufferCount 指定了这块内存的大小, 单位是UNIT_SIZE, 对于初始的Kiss FFT来说, UNIT_SIZE的大小为8字节
            + __返回值__
                + 0  - OK
                + -1 - 对象尚未被初始化
                + -2 - 指定的大小超出对象规定的最大可用内存大小
                + -3 - 上下文已经被初始化
                + -4 - 堆中的剩余内存不足
        + `int KF4Win::Feed(void* pData, unsigned long nCount, int nDataType)`
            + __用途__: 将采样数据提交给KF4Win对象, 多次Feed会使上下文的采样数据数量始终保持在nBufferCount, 如果数据规模超出nBufferCount, 则KF4Win会丢弃最开始Feed的数据
            + __参数__
                + pData 是指向您的采样数据的指针
                + nCount 的大小指定了pData中有多少采样点, 单位是UNIT_SIZE
                + nDataType 需要您提供一个枚举类KF4W_TYPE的值, 具体请看下方的KF4W_TYPE解释
            + __返回值__
                + 0  - OK
                + -1 - 对象尚未被初始化
                + -2 - 上下文未被初始化
                + -3 - 不支持的数据类型, 详情见下方的KF4W_TYPE解释
        + `int KF4Win::DoProcess(unsigned long stride)`
            + __用途__: 处理KF4Win上下文中的数据
            + __参数__
                + stride 指定了每一步的大小，若为非1的数，则每stride个点取一个点参与FFT，可缺省
            + __返回值__
                + 0  - OK
                + -1 - 对象尚未被初始化
                + -2 - 上下文未被初始化
        + `void KF4Win::CleanContext()`
            + __用途__: 清除上下文中已有的数据
            + __参数__
                + 无
            + __返回值__
                + 无
        + `int KF4Win::GetResult(KFComplex* pResult, unsigned long& nResCount, bool bDoABSProcess)`
            + __用途__: 获得FFT结果, 如果pResult == NULL, nResCount会被设置为FFT结果的个数
            + __参数__
                + pResult 一个指向一个KFComplex数组的指针
                + nResCount 传入时为pResult的大小, 单位为UNIT_SIZE, 传出时为输出结果大小, 单位为UNIT_SIZE
                + bDoABSProcess 如果设置为true, 则对结果做取模处理, 结果的实数部分为模, 虚数部分为0
            + __返回值__
                + 0  - OK
                + -1 - 对象尚未被初始化
                + -2 - 上下文未被初始化
                + -3 - 结果缓冲区的大小小于要返回的结果的大小
        + `void KF4Win::ReleaseContext()`
            + __用途__: 释放上下文的内存
            + __参数__
                + 无
            + __返回值__
                + 无
        + `void KF4Win::Release()`
            + __用途__: 释放整个KF4Win对象
            + __参数__
                + 无
            + __返回值__
                + 无
