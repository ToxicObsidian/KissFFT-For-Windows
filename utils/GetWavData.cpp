#include <iostream>
#include <Windows.h>
//#include <xaudio2.h>
#include <cstdio>
#include <cstring>

using namespace std;

// Definition from mmreg.h & mmeapi.h, just use the value.
// We don't care about more formats.
#ifndef WAVE_FORMAT_PCM
#define WAVE_FORMAT_PCM         0x0001
#endif
#ifndef WAVE_FORMAT_IEEE_FLOAT
#define WAVE_FORMAT_IEEE_FLOAT  0x0003
#endif
// A format structure from xaudio2.h, just use this structure, not functions.
#ifndef _WAVEFORMATEX_
typedef struct tWAVEFORMATEX{
    WORD        wFormatTag;         /* format type */
    WORD        nChannels;          /* number of channels (i.e. mono, stereo...) */
    DWORD       nSamplesPerSec;     /* sample rate */
    DWORD       nAvgBytesPerSec;    /* for buffer estimation */
    WORD        nBlockAlign;        /* block size of data */
    WORD        wBitsPerSample;     /* number of bits per sample of mono data */
    WORD        cbSize;             /* the count in bytes of the size of */
                                    /* extra information (after cbSize) */
} WAVEFORMATEX;
#endif
// A format structure from xaudio2.h, just use this structure, not functions.
#ifndef _WAVEFORMATEXTENSIBLE_
typedef struct {
    WAVEFORMATEX    Format;
    union {
        WORD wValidBitsPerSample;       /* bits of precision  */
        WORD wSamplesPerBlock;          /* valid if wBitsPerSample==0 */
        WORD wReserved;                 /* If neither applies, set to zero. */
    } Samples;
    DWORD           dwChannelMask;      /* which channels are */
                                        /* present in stream  */
    GUID            SubFormat;
} WAVEFORMATEXTENSIBLE;
#endif

// Integer definition
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT  ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);
HRESULT RemoveFileNameSuffix(char* szName);
void    ReverseMemory(void* mem, unsigned int len);    // Currently not used.


// Entry Point
int main(int argc, char* argv[]){
    bool bRVFlag = false;
    if(argc < 2){
        cout << "Usage:" << endl;
        cout << "\tGetWavData %file% [output_file]" << endl;
        return 0;
    }
    else if(argc == 3)
        if(strcmp(argv[1], "-r")==0 || strcmp(argv[2], "-r")==0)
            bRVFlag = true;
    HRESULT hr;
    char* audioFile = argv[1];
    HANDLE hAudioFile = CreateFile(audioFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hAudioFile == INVALID_HANDLE_VALUE){
        cout << "Cannot open the file: " << audioFile << endl;
        return 0;
    }



    // Read RIFF chunk data.
    DWORD dwChunkSize, dwChunkOffset;
    hr = FindChunk(hAudioFile, fourccRIFF, dwChunkSize, dwChunkOffset);
    if(FAILED(hr)){
        cout << "Cannot find a RIFF chunk" << endl;
        return 0;
    }
    BYTE* dataBuffer;
    dataBuffer = (BYTE*)malloc(dwChunkSize);
    hr = ReadChunkData(hAudioFile, dataBuffer, dwChunkSize, dwChunkOffset);
    if(FAILED(hr)){
        cout << "Cannot read RIFF chunk data" << endl;
        free(dataBuffer);
        return 0;
    }
    if( fourccWAVE != *((int*)dataBuffer) ){
        cout << audioFile << " is not an audio file" << endl;
        free(dataBuffer);
        return 0;
    }
    free(dataBuffer);



    // Read FMT chunk data
    hr = FindChunk(hAudioFile, fourccFMT, dwChunkSize, dwChunkOffset);
    if(FAILED(hr)){
        cout << "Cannot find a FMT chunk" << endl;
        return 0;
    }
    WAVEFORMATEXTENSIBLE wfx;
    hr = ReadChunkData(hAudioFile, &wfx, dwChunkSize, dwChunkOffset);
    if(FAILED(hr)){
        cout << "Cannot read FMT chunk data" << endl;
        return 0;
    }
    if(wfx.Format.wFormatTag != WAVE_FORMAT_PCM && wfx.Format.wFormatTag != WAVE_FORMAT_IEEE_FLOAT){
        cout << "Cannot analyze this wave format(not an integer, nor a float): " << wfx.Format.wFormatTag << endl;
        return 0;
    }



    // CreateFiles
    char* szInfoFileName = (char*)malloc(256);
    char* szDataFileName = (char*)malloc(256);
    ZeroMemory(szInfoFileName, 256);
    ZeroMemory(szDataFileName, 256);
    hr = RemoveFileNameSuffix(audioFile);
    if( FAILED(hr) ){
        cout << "The file: " << audioFile << " is not a file with a suffix, please check the name" << endl;
    }
    sprintf(szInfoFileName, "%s_format_info.txt", audioFile);
    sprintf(szDataFileName, "%s_data.txt", audioFile);
    HANDLE hInfoFile = CreateFile(szInfoFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hInfoFile == INVALID_HANDLE_VALUE){
        cout << "Cannot create file: " << szInfoFileName << endl;
        free(szInfoFileName);
        free(szDataFileName);
        return 0;
    }
    HANDLE hDataFile = CreateFile(szDataFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hDataFile == INVALID_HANDLE_VALUE){
        cout << "Cannot create file: " << szDataFileName << endl;
        free(szInfoFileName);
        free(szDataFileName);
        return 0;
    }
    free(szInfoFileName);
    free(szDataFileName);



    // Write info into info file.
    char* szAudioInfo = (char*)malloc(4096);
    ZeroMemory(szAudioInfo, 4096);
    DWORD dwWritten;
    sprintf(szAudioInfo, "====File: %s.wav====\nFormatTag = %hd\nChannels = %d\nSamples Per Second = %d\nAverage Bytes Per Second = %d\nBlock Align = %d\nBits Per Sample = %hd\n",
        audioFile, wfx.Format.wFormatTag, wfx.Format.nChannels, wfx.Format.nSamplesPerSec,
        wfx.Format.nAvgBytesPerSec, wfx.Format.nBlockAlign, wfx.Format.wBitsPerSample);
    WriteFile(hInfoFile, szAudioInfo, strlen(szAudioInfo), &dwWritten, NULL);
    free(szAudioInfo);



    // Write audio data as float or integer into Data File.
    hr = FindChunk(hAudioFile, fourccDATA, dwChunkSize, dwChunkOffset);
    if(FAILED(hr)){
        cout << "Cannot find a DATA chunk" << endl;
        return 0;
    }
    dataBuffer = (BYTE*)malloc(dwChunkSize);
    hr = ReadChunkData(hAudioFile, dataBuffer, dwChunkSize, dwChunkOffset);
    if(FAILED(hr)){
        cout << "Cannot read a DATA chunk" << endl;
        free(dataBuffer);
        return 0;
    }
    if(wfx.Format.wFormatTag == WAVE_FORMAT_PCM || wfx.Format.wFormatTag == WAVE_FORMAT_IEEE_FLOAT){
        // IEEE_FLOAT
        char  szSample[18];
        if( wfx.Format.wBitsPerSample == 32 && wfx.Format.wFormatTag == WAVE_FORMAT_IEEE_FLOAT ){
            float fSample;
            for(unsigned long cnt = 0; cnt < (int)((float)(dwChunkSize / wfx.Format.wBitsPerSample) * 8); cnt++){
                ZeroMemory(szSample, 18);
                CopyMemory(&fSample, dataBuffer + cnt * 4, 4);
                if(bRVFlag) ReverseMemory(&fSample, 4);
                sprintf(szSample, "%.5f\n", fSample);
                WriteFile(hDataFile, szSample, strlen(szSample), &dwWritten, NULL);
            }
        }
        // int32_t
        else if( wfx.Format.wBitsPerSample == 32 ){
            int32_t nSample;
            for(unsigned long cnt = 0; cnt < (int)((float)(dwChunkSize / wfx.Format.wBitsPerSample) * 8); cnt++){
                ZeroMemory(szSample, 18);
                CopyMemory(&nSample, dataBuffer + cnt * 4, 4);
                if(bRVFlag) ReverseMemory(&nSample, 4);
                sprintf(szSample, "%d\n", nSample);
                WriteFile(hDataFile, szSample, strlen(szSample), &dwWritten, NULL);
            }
        }
        // int24_t
        // Unable to find a way to read a 24bit integer.
        // Finding solutions
        // int16_t
        else if( wfx.Format.wBitsPerSample == 16 ){
            int16_t nSample;
            for(unsigned long cnt = 0; cnt < (int)((float)(dwChunkSize / wfx.Format.wBitsPerSample) * 8); cnt++){
                ZeroMemory(szSample, 18);
                CopyMemory(&nSample, dataBuffer + cnt * 2, 2);
                if(bRVFlag) ReverseMemory(&nSample, 2);
                sprintf(szSample, "%hd\n", nSample);
                WriteFile(hDataFile, szSample, strlen(szSample), &dwWritten, NULL);
            }
        }
        // int8_t
        else if( wfx.Format.wBitsPerSample == 8 ){
            int8_t nSample;
            for(unsigned long cnt = 0; cnt < (int)((float)(dwChunkSize / wfx.Format.wBitsPerSample) * 8); cnt++){
                ZeroMemory(szSample, 18);
                CopyMemory(&nSample, dataBuffer + cnt, 1);
                if(bRVFlag) ReverseMemory(&nSample, 1);
                sprintf(szSample, "%hhd\n", nSample);
                WriteFile(hDataFile, szSample, strlen(szSample), &dwWritten, NULL);
            }
        }
    }
    free(dataBuffer);


    CloseHandle(hAudioFile);
    CloseHandle(hInfoFile);
    CloseHandle(hDataFile);
    return 0;
}

/**
 *  Functions below are not essential,
 *      but they do help to read a .wav file.
 *  Just know the wave file is a RIFF format file and
 *      functions below can help you to read RIFF file data.
 *  The code is copied from the url below:
 *      https://docs.microsoft.com/en-us/windows/win32/xaudio2/how-to--load-audio-data-files-in-xaudio2
 *  A little modification has been done.
**/

HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition) {
	HRESULT hr = S_OK;

	// Invalid HANDLE
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN)) {
		return HRESULT_FROM_WIN32(GetLastError());
	}

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK) {
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL)) {
			hr = HRESULT_FROM_WIN32(GetLastError());
		}
		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL)) {
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType) {
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL)) {
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT)) {
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == fourcc) {
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize) return S_FALSE;
	}

	return S_OK;
}


HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset) {
	HRESULT hr = S_OK;

	// Invalid HANDLE
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN)) {
		return HRESULT_FROM_WIN32(GetLastError());
	}

	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL)) {
		hr = HRESULT_FROM_WIN32(GetLastError());
	}
	return hr;
}


HRESULT RemoveFileNameSuffix(char* szName){
    for(int i = (int)strlen(szName) - 1; i >= 0; i--){
        if(*(szName + i) == '.'){
            *(szName + i) = '\0';
            return S_OK;
        }
    }
    return E_FAIL;
}


void ReverseMemory(void* mem, unsigned int len) {
    void* cpy = malloc(len);
    for(unsigned int i = 0; i < len; i++){
        *((BYTE*)((BYTE*)mem + i)) = *((BYTE*)((BYTE*)cpy - i + 1));
    }
    CopyMemory(mem, cpy, len);
    free(cpy);
}
