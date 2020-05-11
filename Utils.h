/**
 *	This file provides several safe templates
 *	Using these codes can make it easy to code.
 *
 *	These templates are easy to read.
 *	If you want to use these in your project, use `#include "Utils.h"`
 *
 *	This header file is a part of KF4Win
 */
#pragma once
#ifndef T_UTILS_INC__
#define T_UTILS_INC__
#include <cstdlib>

template<typename T>
bool SafeNewObj(T*& t) {
	try {
		t = new T();
	}
	catch (...) {
		t = NULL;
		return false;
	}
	return true;
}

template<typename T>
bool SafeNewArray(T*& t, size_t s) {
	try {
		t = new T[s];
	}
	catch (...) {
		t = NULL;
		return false;
	}
	return true;
}

template<typename T>
bool SafeMalloc(T*& t, size_t unitcount) {
	t = (T*)malloc(sizeof(T) * unitcount);
	if (!t)
		return false;
	return true;
}

template<typename T>
void SafeDeleteArray(T*& t) {
	if (t) {
		delete[] t;
		t = NULL;
	}
}

template<typename T>
void SafeDeleteObj(T*& t) {
	if (t) {
		delete t;
		t = NULL;
	}
}

template<typename T>
void SafeFree(T*& t) {
	if (t) {
		free(t);
		t = NULL;
	}
}

// Do not use this method if the object is newed.
template<typename T>
void SafeRelease(T*& t) {
	if (t) {
		t->Release();
		t = NULL;
	}
}

#endif // T_UTILS_INC__