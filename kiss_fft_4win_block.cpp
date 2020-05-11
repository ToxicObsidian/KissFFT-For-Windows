#include "kiss_fft_4win.h"


kf4win_fft::kf4win_fft(const kf4win_fft& f) {
	// Do nothing, just block this function.
}

kf4win_fft kf4win_fft::operator = (const kf4win_fft& f) {
	// A normal return value, cannot be called.
	return *this;
}