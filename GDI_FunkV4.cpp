#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE,
    LPSTR,
    int
) {
    const int sampleRate = 8000;
    const int seconds = 500; // 8 minutes
    const int bufferSize = sampleRate * seconds;

    static unsigned char buffer[bufferSize];

    // Generate bytebeat-style music
    for (int t = 0; t < bufferSize; t++) {
        // FIRST PART (intro)
        if (t < sampleRate * 8) {
            buffer[t] = (unsigned char)(
                (t * (t >> 5 | t >> 8)) ^ (t >> 3)
            );
        }
        // REST OF TRACK
        else {
            buffer[t] = (unsigned char)(
                (t * 3 & t >> 10) | (t >> 4)
            );
        }
    }

    WAVEFORMATEX wf = {};
    wf.wFormatTag = WAVE_FORMAT_PCM;
    wf.nChannels = 1;
    wf.nSamplesPerSec = sampleRate;
    wf.wBitsPerSample = 8;
    wf.nBlockAlign = 1;
    wf.nAvgBytesPerSec = sampleRate;

    HWAVEOUT hWaveOut = nullptr;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wf, 0, 0, CALLBACK_NULL);

    WAVEHDR header = {};
    header.lpData = (LPSTR)buffer;
    header.dwBufferLength = bufferSize;

    waveOutPrepareHeader(hWaveOut, &header, sizeof(header));
    waveOutWrite(hWaveOut, &header, sizeof(header));

    // Keep program alive while sound plays
    Sleep(seconds * 1000);

    waveOutUnprepareHeader(hWaveOut, &header, sizeof(header));
    waveOutClose(hWaveOut);

    return 0;
}