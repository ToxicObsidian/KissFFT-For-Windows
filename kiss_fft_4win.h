/**
 *		KF4Win - Kiss FFT For Windows
 *	This is the most used header file of KF4Win.
 *	If you want to use KF4Win, just use `#include "kiss_fft_4win.h"` or `#include "kf4win.h"`.
 *
 *	This project(aka. KF4Win) is licensed under MIT License.
 *	For more details, see ./LICENSE
 *
 *	This Project is written in C++, but provides C-style APIs,
 *		which are the functions with `KF_` prefix.
 *
 *	Copyright (c) 2019 - 2020, Toxic_Obsidian.
 */
#pragma once
#ifndef T_KF4WIN_INC__
#define T_KF4WIN_INC__

#include "_kiss_fft_guts.h"

#define T_KISS_FFT_EXPORT
#ifdef  T_KISS_FFT_EXPORT
#define T_KF4WIN_EXPORT __declspec(dllexport)
#else
#define T_KF4WIN_EXPORT __declspec(dllimport)
#endif	//T_KISS_FFT_EXPORT

typedef kiss_fft_cpx KFComplex;

#ifndef T_FUNCARG_IO_FLAG_DEFINED
#define T_FUNCARG_IO_FLAG_DEFINED
#define _In_Array_
#define _In_Const_Array_
#define _In_Reference_Array_
#define _In_CR_Array_
#define _In_Object_
#define _In_Variable_
#define _In_Const_Object_
#define _In_Const_Variable_
#define _In_Reference_Object_
#define _In_Reference_Variable_
#define _In_CR_Object_
#define _In_CR_Variable_
#define _Out_Array_
#define _Out_Variable_
#define _Out_Object_
#define _Out_None_
#define _Out_Reference_Variable_
#define _Out_Reference_Object_
#endif	//T_FUNCARG_IO_FLAG

#ifndef T_CLASS_DECORATION_DEFINED
#define T_CLASS_DECORATION_DEFINED
#define T_BLOCK_MEMBER private
#endif

#ifndef T_UNIT_DEFINED
#define T_UNIT_DEFINED
#define seconds
#define minutes seconds * 60
#define hours seconds * 3600
#define hertz
#if defined(T_HERTZ_ABBR_DEFINED) || defined(Hz)
#undef Hz
#endif
#define Hz hertz
#define kHz Hz * 1000
#define MHz kHz * 1000
#endif

enum KF4W_TYPE : int {
	T_INT8 = 1,
	T_INT16 = 2,
	T_INT24 = 3,
	T_INT32 = 4,
	T_FLOAT = 5,
	T_DOUBLE = 6,
	T_KFCOMPLEX = 7,
	T_INT64 = 8
};


#define KFUNIT_SIZE sizeof(KFComplex)
#define UNIT_SIZE KFUNIT_SIZE

#ifdef __cplusplus
class T_KF4WIN_EXPORT kf4win_fft {
public:
	kf4win_fft();
	~kf4win_fft();

	/* Make sure that nMaxBuffer is a very large number, or the context won't able to be created.*/
	/* Always returns 0, or the nMaxBufferSize is too less */
	_Out_Variable_	int		Init(
		_In_Const_Variable_			unsigned long nMaxBufferSize = (unsigned long)(-1));

	/* Create a context that allows you to feed data in.*/
	/* The context behaves like a circle, you can feed more than *nBufferSize* data, but */
	/*		Kiss FFT will always process the data whose count is less than or equal to *nBufferSize* */
	_Out_Variable_	int		CreateContext(
		_In_Const_Variable_			unsigned long nBufferCount);

	/* Duplicate data and put them into the queue */
	/* You can call more than once Feed() and feed data whose count is more than *nBufferSize* */
	/* The data exceeded *nBufferSize* will be dropped */
	_Out_Variable_	int		Feed(
		_In_Array_					void*	pData,
		_In_Const_Variable_			unsigned long nCount,
		_In_Const_Variable_			int		nDataType);

	/* Clean the data in the buffer */
	_Out_None_		void	CleanContext();

	/* Do FFT */
	/* If stride != 1, Kiss FFT will process the signal every *stride* samples.*/
	_Out_Variable_	int		DoProcess(
		_In_Variable_				unsigned long stride = 1);

	/* Duplicate the result to the buffer */
	/* If *bDoABSProcess* equals true, every KFComplex object in pResult array will be processed like: */
	/*		(pResult + i)->r = abs(*(pResult + i)); */
	/*		(pResult + i)->i = 0; */
	/* If pResult == NULL, nResCount will set to the value of this->nOutCount */
	/* If nResCount < this->nCount, GetResult() will return -3 */
	/* Make sure the space is larger than this->nOutCount*sizeof(KFComplex) */
	_Out_Variable_	int		GetResult(
		_Out_Array_					KFComplex* pResult,
		_Out_Reference_Variable_	unsigned long& nResCount,
		_In_Variable_				bool bDoABSProcess);

	/* Release the buffer */
	_Out_None_		void	ReleaseContext();

	/* Reset all of the configurations */
	_Out_None_		void	Release();

/* Do not use these functions */
T_BLOCK_MEMBER:
	kf4win_fft(const kf4win_fft&);
	kf4win_fft operator = (const kf4win_fft&);

private:
	bool			bInited;
	unsigned long	nMaxBufSize;
	unsigned long	nContextCount;
	unsigned long	nInCount;
	unsigned long	nOutCount;

	kiss_fft_cfg	KFConfig;
	// Data Storage range: [npHead, npTail] not [npHead, npTail)
	unsigned long	npHead;
	unsigned long	npTail;
	KFComplex*		pInBuffer;
	KFComplex*		pOutBuffer;

};
typedef kf4win_fft KF4Win;
#endif	//__cplusplus

/**
 *	C-style APIs
 */

typedef void *PKF4WIN, *LPKF4WIN;
typedef LPKF4WIN HKF4WIN;

#ifndef T_KF4WIN_CSTYLE_VALUES
#define T_KF4WIN_CSTYLE_VALUES
#define KFRV(x) int((x))
#define KF_OK		KFRV(0)  // OK
#define KFE_NOTINIT KFRV(-1) // Not initialized
#define KFE_ZEROCTX KFRV(-2) // Context not created
#define KFE_OUTMEM	KFRV(-3) // Out of memory
#define KFE_LESSBUF KFRV(-4) // Buffer size is too small
#define KFE_CALL2RD KFRV(-5) // Method called twice before the release method is called
#define KFE_INVPARA KFRV(-6) // Invalid parameter
#endif

extern "C" {
	// Use KF_AllocEx() for more detail.
	HKF4WIN T_KF4WIN_EXPORT KF_Alloc();

	// Specify max buffer size, for KF_Alloc, nMaxBufferSize = (unsigned long)(-1)
	int		T_KF4WIN_EXPORT KF_AllocEx(HKF4WIN* handle, unsigned long nMaxBufferSize);

	// Create a context
	// nContextMaxCnt indicates the size of the context, if nContextMaxCnt = 44100, the size of the output buffer will be 0~44100
	int		T_KF4WIN_EXPORT KF_CreateContext(const HKF4WIN handle, unsigned long nContextMaxCnt);

	// Feed data
	// Only support KF4W_TYPE::T_INT16, KF4W_TYPE::T_FLOAT, KF4W_TYPE::T_KFCOMPLEX now.
	int		T_KF4WIN_EXPORT KF_Feed(const HKF4WIN handle, void* pData, unsigned long nCount, int kf4win_type);

	// Do process
	// If stride = 1, KF4Win will process every sample. If stride = 2, KF4Win will process every 2rd sample, etc.
	int		T_KF4WIN_EXPORT KF_DoProcess(const HKF4WIN handle);
	int		T_KF4WIN_EXPORT KF_DoProcessEx(const HKF4WIN handle, unsigned long stride);

	// Clean the context
	int 	T_KF4WIN_EXPORT KF_CleanContext(const HKF4WIN handle);

	// Get Data
	// uSizeInUnit indicates the buffer(pResult) size
	// uGetInUnit returns the output buffer size.
	// If absproc == true, the result will become the absolute value.
	// If pResult == NULL, uGetInUnit will return the current output buffer size.
	int		T_KF4WIN_EXPORT KF_GetResult(const HKF4WIN handle, KFComplex* pResult, unsigned long uSizeInUnit, unsigned long* uGetInUnit, bool absproc);

	// Clear data
	void	T_KF4WIN_EXPORT KF_ReleaseContext(const HKF4WIN handle);

	// Release
	void	T_KF4WIN_EXPORT KF_Release(HKF4WIN handle);
}
#endif	//T_KF4WIN_INC__
