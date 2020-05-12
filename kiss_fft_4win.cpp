/**
 *				This is the source code of KF4Win
 *	If you want to modify it, please notice that kf4win_fft::Feed()
 *		is the most complicated function. Due to some historical reasons...
 *	It's not quite easy for everybody to understand the logic, including the author.
 *	There's a dragon in front. Good luck.
 *													-- Toxic_Obsidian
 *
 *	The source code of KF4Win is licensed under MIT License.
 *	For more details, see ./LICENSE
 */
#include "kiss_fft_4win.h"
#include "Utils.h"
#ifdef _DEBUG
#include <iostream>
#endif

#ifndef KFCopyMemory
#ifndef _CRT_SECURE_NO_WARNINGS
#define KFCopyMemory(des, src, size) memcpy_s((des), (size), (src), (size))
#else
#define KFCopyMemory(des, src, size) memcpy((des), (src), (size))
#endif
#endif


kf4win_fft::kf4win_fft() {
	this->bInited = false;
	this->nMaxBufSize = 0;
	this->KFConfig = NULL;
	this->pInBuffer = this->pOutBuffer = NULL;

}
kf4win_fft::~kf4win_fft() {
	if (this->bInited)
		this->Release();
}


int kf4win_fft::Init(unsigned long nMaxBufferSize) {
	if (nMaxBufferSize <= 3)
		return -1;

	this->nMaxBufSize = nMaxBufferSize % 2 == 0 ? nMaxBufferSize : nMaxBufferSize - 1;
	SafeFree(this->pInBuffer);
	SafeFree(this->pOutBuffer);
	this->npHead = 0;
	this->npTail = 0;
	this->nInCount = 0;
	this->nOutCount = 0;
	this->nContextCount = 0;

	this->bInited = true;
	return 0;
}


int kf4win_fft::CreateContext(unsigned long nBufferCount) {
	if (!this->bInited)
		return -1;
	if (nBufferCount * 2 * UNIT_SIZE > this->nMaxBufSize)
		return -2;

	if (this->pInBuffer || this->pOutBuffer)
		return -3;

	if (!SafeMalloc(this->pInBuffer, nBufferCount * 2 * UNIT_SIZE) || !SafeMalloc(this->pOutBuffer, nBufferCount * UNIT_SIZE)) {
		SafeFree(this->pInBuffer);
		SafeFree(this->pOutBuffer);
		return -4;
	}
	this->nContextCount = nBufferCount;
	// Must do this process, or the data will become trash
	memset(this->pInBuffer, 0, nBufferCount * 2 * UNIT_SIZE);
	memset(this->pOutBuffer, 0, nBufferCount * UNIT_SIZE);

	// Current Size
	this->nInCount = 0;
	this->nOutCount = 0;
	this->npHead = 0;
	this->npTail = 0;

	return 0;
}

int kf4win_fft::Feed(void *pData, unsigned long nCount, int nDataType) {
	if (!this->bInited)
		return -1;
	if (!this->pInBuffer || !this->pOutBuffer)
		return -2;

	// INT32
	if (nDataType == KF4W_TYPE::T_INT32) {
		int *pOriSignal = (int*)pData;
		for (unsigned long __i = 0; __i < nCount; __i++) {
			if (this->npTail - this->npHead + 1 < this->nContextCount && this->npTail >= this->npHead) {
				if (this->npTail == this->npHead && this->npHead == 0) {
					this->pInBuffer->r = *pOriSignal * 1.0f;
					this->nInCount++;
				}
				else {
					this->npTail++;
					(this->pInBuffer + this->npTail)->r = (float)*(pOriSignal + __i);
					this->nInCount++;
				}
			}
			else {
				this->npTail++;
				if (this->npTail >= this->nContextCount * 2) this->npTail = 0;
				this->npHead++;
				if (this->npHead >= this->nContextCount * 2) this->npHead = 0;
				(this->pInBuffer + this->npTail)->r = *(pOriSignal + __i) * 1.0f;
			}
		}
	}
	// FLOAT
	else if (nDataType == KF4W_TYPE::T_FLOAT) {
		float *pOriSignal = (float*)pData;
		for (unsigned long __i = 0; __i < nCount; __i++) {
			if (this->npTail - this->npHead + 1 < this->nContextCount && this->npTail >= this->npHead) {
				if (this->npTail == this->npHead && this->npHead == 0 && this->nInCount == 0) {
					this->pInBuffer->r = *pOriSignal;
					this->nInCount++;
				}
				else {
					this->npTail++;
					(this->pInBuffer + this->npTail)->r = *(pOriSignal + __i);
					this->nInCount++;
				}
			}
			else {
				this->npTail++;
				if (this->npTail >= this->nContextCount * 2) this->npTail = 0;
				this->npHead++;
				if (this->npHead >= this->nContextCount * 2) this->npHead = 0;
				(this->pInBuffer + this->npTail)->r = *(pOriSignal + __i);
			}
		}
	}
	// Original Complex
	else if (nDataType == KF4W_TYPE::T_KFCOMPLEX) {
		// Do a little trick, they won't know that the memory didn't work like a circle at all.
		if (nCount >= this->nContextCount) {
			unsigned long __bias = nCount - this->nContextCount;
			this->npHead = 0;
			this->npTail = this->nContextCount - 1;
			KFCopyMemory(this->pInBuffer, (KFComplex*)pData + __bias, this->nContextCount * UNIT_SIZE);
			this->nInCount = this->nContextCount;
		}
		// Cannot do the tricks
		// But remember, at this situation, nCount will be less than nContextCount
		else {
			// Initial state
			if (this->npHead == this->npTail && this->npTail == 0 && this->nInCount == 0) {
				KFCopyMemory(this->pInBuffer, pData, nCount * UNIT_SIZE);
				this->npTail = nCount - 1;
				this->nInCount = nCount;
			}
			// Not initial state, but npHead < npTail
			else if (this->npHead < this->npTail) {
				// nInCount < nContextCount, npTail will not go back to 0
				if (this->nInCount < this->nContextCount) {
					KFCopyMemory(this->pInBuffer + this->npTail + 1, pData, nCount * UNIT_SIZE);
					this->npTail += nCount;
					this->npHead += nCount + this->nInCount > this->nContextCount ? nCount + this->nInCount - this->nContextCount : 0;
					this->nInCount = this->nInCount + nCount > this->nContextCount ? this->nContextCount : this->nInCount + nCount;
				}
				// else, the npTail might go back to 0
				else {
					unsigned long __incr = this->npTail + nCount;
					// Back to 0
					// In this situation, npHead won't back to 0
					if (__incr >= this->nContextCount * 2) {
						unsigned long __partlen;
						__partlen = this->nContextCount * 2 - this->npTail;
						KFCopyMemory(this->pInBuffer + this->npTail + 1, pData, __partlen * UNIT_SIZE);
						KFCopyMemory(this->pInBuffer, (KFComplex*)pData + __partlen, (nCount - __partlen) * UNIT_SIZE);
						this->npHead += nCount;
						this->npTail = __incr - this->nContextCount * 2;
					}
					// Not back to 0
					else {
						KFCopyMemory(this->pInBuffer + this->npTail + 1, pData, nCount * UNIT_SIZE);
						this->npTail += nCount;
						this->npHead += nCount;
					}

				}
			}
			// npHead > npTail
			else if (this->npHead > this->npTail) {
				KFCopyMemory(this->pInBuffer + this->npTail + 1, pData, nCount * UNIT_SIZE);
				this->npTail += nCount;
				this->npHead += nCount;
				if (this->npHead > this->nContextCount * 2) this->npHead -= this->nContextCount * 2;
			}
		}
	}
	// Not supported
	else
		return -3;

	// Fix bug 2. (Fixed)
	/**
	std::cout << "Total feeded " << this->nInCount << std::endl;
	std::cout << "npHead = " << this->npHead << " npTail = " << this->npTail << std::endl;
	*/
	/**
	for (unsigned long i = this->npHead; i <= this->npTail; i++) {
		std::cout << "Fix Bug 2. i = " << i << " Mag = " << (this->pInBuffer + i)->r << std::endl;
	}
	*/

	return 0;
}


void kf4win_fft::CleanContext() {
	if(!this->bInited)
		return;
	if(!this->pInBuffer || !this->pOutBuffer)
		return;
	memset(this->pInBuffer, 0, this->nContextCount * 2);
	memset(this->pOutBuffer, 0, this->nContextCount * 2);
	this->npHead = 0;
	this->npTail = 0;
	this->nInCount = 0;
	this->nOutCount = 0;
}


int kf4win_fft::DoProcess(unsigned long stride) {
	if (!this->bInited)
		return -1;
	if (!this->pInBuffer || !this->pOutBuffer)
		return -2;
	// Must be free()d manually
	this->KFConfig = kiss_fft_alloc(this->nInCount, 0, NULL, NULL);

	if (this->npHead > this->npTail) {
		unsigned long nPart1Length = this->nContextCount * 2 - this->npHead;
		KFCopyMemory(this->pOutBuffer, this->pInBuffer + this->npHead, nPart1Length * UNIT_SIZE);
		KFCopyMemory(this->pOutBuffer + nPart1Length, this->pInBuffer, (this->npTail + 1) * UNIT_SIZE);
	}
	else {
		KFCopyMemory(this->pOutBuffer, this->pInBuffer + this->npHead, this->nInCount * UNIT_SIZE);
	}

	// Fix bug 3. (Fixed)
	/**
	for (unsigned long i = 0; i < this->nInCount; i++) {
		std::cout << "Fix bug 3. i = " << i << " Mag = " << (this->pOutBuffer + i)->r << std::endl;
	}
	*/

	// Use the same buffer, but we know that kiss_fft will create a temporary buffer XD.
	kiss_fft_stride(this->KFConfig, this->pOutBuffer, this->pOutBuffer, stride);
	this->nOutCount = this->nInCount;

	free(this->KFConfig);
	return 0;
}

int kf4win_fft::GetResult(KFComplex* pResult, unsigned long& nResCount, bool bDoABSProcess) {
	if (!this->bInited)
		return -1;
	if (!this->pOutBuffer)
		return -2;

	if (pResult == NULL) {
		nResCount = this->nOutCount;
		return 0;
	}
	if (nResCount < this->nOutCount)
		return -3;

	KFCopyMemory(pResult, this->pOutBuffer, this->nOutCount * UNIT_SIZE);

	if(bDoABSProcess)
		for (unsigned long __i = 0; __i < this->nOutCount; __i++) {
			(pResult + __i)->r = sqrt((pResult + __i)->r * (pResult + __i)->r + (pResult + __i)->i * (pResult + __i)->i);
			(pResult + __i)->i = 0.0f;
		}

	return 0;
}

void kf4win_fft::ReleaseContext() {
	this->npHead = this->npTail = 0;
	this->nInCount = this->nOutCount = 0;
	SafeFree(this->pInBuffer);
	SafeFree(this->pOutBuffer);
	this->nContextCount = 0;
}


void kf4win_fft::Release() {
	this->ReleaseContext();
	this->nMaxBufSize = 0;
	this->bInited = false;
}
