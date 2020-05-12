#include "kiss_fft_4win.h"
#include "Utils.h"

HKF4WIN KF_Alloc() {
	KF4Win* kf;
	if (!SafeNewObj(kf))
		return NULL;
	return kf;
}

int KF_AllocEx(HKF4WIN* handle, unsigned long nMaxBufferSize) {
	handle = NULL;
	KF4Win* kf;
	if (!SafeNewObj(kf))
		return KFE_OUTMEM;
	if (0 != kf->Init(nMaxBufferSize))
		return KFE_LESSBUF;
	handle = (HKF4WIN*)kf;
	return KF_OK;
}

int KF_CreateContext(const HKF4WIN handle, unsigned long nContextMaxCnt) {
	if (handle == NULL)
		return KFE_NOTINIT;
	KF4Win* kf = (KF4Win*)handle;
	int r = kf->CreateContext(nContextMaxCnt);
	if (r == -1)
		return KFE_NOTINIT;
	else if (r == -2)
		return KFE_INVPARA;
	else if (r == -3)
		return KFE_CALL2RD;
	else if (r == -4)
		return KFE_OUTMEM;
	return KF_OK;
}

int KF_Feed(const HKF4WIN handle, void* pData, unsigned long nCount, int kf4win_type) {
	if (handle == NULL)
		return KFE_NOTINIT;
	KF4Win* kf = (KF4Win*)handle;
	int r = kf->Feed(pData, nCount, kf4win_type);
	if (r == -1)
		return KFE_NOTINIT;
	else if (r == -2)
		return KFE_ZEROCTX;
	else if (r == -3)
		return KFE_INVPARA;
	return KF_OK;
}

int KF_DoProcess(const HKF4WIN handle) {
	return KF_DoProcessEx(handle, 1);
}

int KF_DoProcessEx(const HKF4WIN handle, unsigned long stride) {
	if (handle == NULL)
		return KFE_NOTINIT;
	KF4Win* kf = (KF4Win*)handle;
	int r = kf->DoProcess(stride);
	if (r == -1)
		return KFE_NOTINIT;
	else if (r == -2)
		return KFE_ZEROCTX;
	return KF_OK;
}

int KF_CleanContext(const HKF4WIN handle){
	if (handle == NULL)
		return KFE_NOTINIT;
	KF4Win* kf = (KF4Win*)handle;
	kf->CleanContext();
	return KF_OK;
}

int KF_GetResult(const HKF4WIN handle, KFComplex* pResult, unsigned long uSizeInUnit, unsigned long* uGetInUnit, bool absproc) {
	if (handle == NULL)
		return KFE_NOTINIT;
	KF4Win* kf = (KF4Win*)handle;
	unsigned long u = uSizeInUnit;
	int r = kf->GetResult(pResult, u, absproc);
	*uGetInUnit = 0;
	if (r == -1)
		return KFE_NOTINIT;
	else if (r == -2)
		return KFE_ZEROCTX;
	else if (r == -3)
		return KFE_OUTMEM;
	*uGetInUnit = u;
	return KF_OK;
}

void KF_ReleaseContext(const HKF4WIN handle) {
	if (handle == NULL)
		return;
	KF4Win* kf = (KF4Win*)handle;
	kf->ReleaseContext();
}

void KF_Release(HKF4WIN handle) {
	KF4Win* kf = (KF4Win*)handle;
	// Call kf4win_fft::Release() automatically
	SafeDeleteObj(kf);
	handle = NULL;
}
