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
* __C类型的接口__
    + 创建一个KF4Win句柄: `HKF4WIN hkf = KF_Alloc();`
    + 销毁一个KF4Win句柄: `KF_Release(hkf); hkf == NULL;`
    + `HKF4WIN KF_Alloc()`
        + __用途__: 创建一个KF4Win句柄
        + __参数__
            + 无
        + __返回值__
            + 成功: 一个KF4Win句柄
            + 失败: NULL
    + `int KF_AllocEx(HKF4WIN* handle, unsigned long nMaxBufferSize)`
        + __用途__: 创建一个KF4Win句柄
        + __参数__
            + handle 一个HKF4WIN指针, 成功时被设置为非0, 失败时被设置为0
            + nMaxBufferSize 指定了最大可用的内存大小, 但KF4Win不会去申请这么大的内存
        + __返回值__
            + 成功: KF_OK(0)
            + 失败: 非0, 见下方KFCAPI返回值
    + `int KF_CreateContext(const HKF4WIN handle, unsigned long nContextMaxCnt)`
        + __用途__: 创建一个上下文
        + __参数__
            + handle 一个KF4Win句柄
            + nContextMaxCnt 指定了上下文的大小, 单位是UNIT_SIZE
        + __返回值__
            + 成功: KF_OK(0)
            + 失败: 非0, 见下方KFCAPI返回值
    + `int KF_Feed(const HKF4WIN handle, void* pData, unsigned long nCount, int kf4win_type)`
        + __用途__: 向上下文填充数据
        + __参数__
            + handle 一个KF4Win句柄
            + pData 一个指向采样数据的指针
            + nCount 采样数据大小, 单位是UNIT_SIZE
            + kf4win_type 采样数据格式, 详情见下方的KF4W_TYPE解释
        + __返回值__
            + 成功: KF_OK(0)
            + 失败: 非0, 见下方KFCAPI返回值
    + `int KF_DoProcess(const HKF4WIN handle)`
        + 等同于 `KF_DoProcessEx(handle, 1);`
    + `int KF_DoProcessEx(const HKF4WIN handle, unsigned long stride)`
        + __用途__: 对上下文中的数据进行FFT处理
        + __参数__
            + handle 一个KF4Win句柄
            + stride 指定了每一步的大小，若为非1的数，则每stride个点取一个点参与FFT
        + __返回值__
            + 成功: KF_OK(0)
            + 失败: 非0, 见下方KFCAPI返回值
    + `int KF_CleanContext(const HKF4WIN handle)`
        + __用途__: 清除上下文中的数据
        + __参数__
            + handle 一个KF4Win句柄
        + __返回值__
            + 成功: KF_OK(0)
            + 失败: 非0, 见下方KFCAPI返回值
    + ```int KF_GetResult(
            const HKF4WIN handle,
            KFComplex* pResult,
            unsigned long uSizeInUnit,
            unsigned long* uGetInUnit,
            bool absproc
        )```
        + __用途__: 从上下文中获取上一次FFT的结果, 如果pResult设置为NULL, 则将uGetInUnit设置为结果大小
        + __参数__
            + handle 一个KF4Win句柄
            + pResult 一个指向一个KFComplex数组的指针
            + uSizeInUnit 数组大小, 单位是UNIT_SIZE
            + uGetInUnit 结果大小, 单位是UNIT_SIZE
            + absproc 如果设置为true, 则对结果做取模处理, 结果的实数部分为模, 虚数部分为0
        + __返回值__
            + 成功: KF_OK(0)
            + 失败: 非0, 见下方KFCAPI返回值
    + `void KF_ReleaseContext(const HKF4WIN handle)`
        + __用途__: 释放上下文的内存
        + __参数__
            + handle 一个KF4Win句柄
        + __返回值__
            + 无
    + `void KF_Release(HKF4WIN* handle)`
        + __用途__: 销毁一个KF4Win句柄, 并将handle设置为NULL
        + __参数__
            + handle 一个HKF4WIN指针
        + __返回值__
            + 无
* __KF4W_TYPE解释__
    + KF4Win计划实现8种支持的数据格式, 分别在KF4W_TYPE枚举中定义
        | KF4W_TYPE 类型         |  对应数据类型   |
        | :-------------------: | :----------------:|
        | KF4W_TYPE::T_INT8     |   8位整数        |
        | KF4W_TYPE::T_INT16    |   16位整数       |
        | KF4W_TYPE::T_INT24    |   24位整数       |
        | KF4W_TYPE::T_INT32    |   32位整数       |
        | KF4W_TYPE::T_FLOAT    |   32位浮点数      |
        | KF4W_TYPE::T_DOUBLE   |   64位浮点数      |
        | KF4W_TYPE::T_KFCOMPLEX |   KFComplex类型 |
        | KF4W_TYPE::T_INT64    |   64位整数       |
    + 目前, KF4Win只支持其中三种, 即T_INT32, T_FLOAT 和 T_KFCOMPLEX
* __KFCAPI返回值__
    + KF4Win的CAPI中, 除了0以外的任何值都代表失败
        |   宏定义    |  值  |    意义        |
        | :---------- | :--: | ------------: |
        | KF_OK       |  0   | 成功           |
        | KFE_NOTINIT |  -1  | 未初始化       |
        | KFE_ZEROCTX |  -2  | 未创建上下文   |
        | KFE_OUTMEM  |  -3  | 内存不足       |
        | KFE_LESSBUF |  -4  | 缓冲区过小     |
        | KFE_CALL2RD |  -5  | 方法调用了两次  |
        | KFE_INVPARA |  -6  | 非法参数       |
