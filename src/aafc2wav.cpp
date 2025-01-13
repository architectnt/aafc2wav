#include <fileutils.h>
#include <libaafcfunc.h>

#pragma pack(push, 1) // why
typedef struct {
    char ChunkID[4];
    unsigned int ChunkSize;
    char Format[4];
    char Subchunk1ID[4];
    unsigned int Subchunk1Size;
    unsigned short AudioFormat;
    unsigned short NumChannels;
    unsigned int SampleRate;
    unsigned int ByteRate;
    unsigned short BlockAlign;
    unsigned short BitsPerSample;
    char Subchunk2ID[4];
    unsigned int Subchunk2Size;
} WavHeader;
#pragma pack(pop)

static char* concat_path_wav(const char* filename) {
    if (filename == NULL || *filename == '\0')
        return NULL;

    size_t len = (strlen(filename) + strlen(".wav/")) + 1;
    char* result = (char*)malloc(len);
    if (result)
        snprintf(result, len, "%s.wav", filename);
    return result;
}

AAFCOUTPUT export_wav(const short* data, size_t data_length, int freq, unsigned char channels) {
    int bps = 16; // Assuming 16 bit PCM audio
    int byps = bps / 8;

    WavHeader h = {};
    memcpy(h.ChunkID, "RIFF", 4);
    memcpy(h.Format, "WAVE", 4);
    memcpy(h.Subchunk1ID, "fmt ", 4);
    h.Subchunk1Size = 16; // PCM
    h.AudioFormat = 1;    // PCM
    h.NumChannels = channels;
    h.SampleRate = freq;
    h.ByteRate = freq * channels * byps;
    h.BlockAlign = channels * byps;
    h.BitsPerSample = bps;
    memcpy(h.Subchunk2ID, "data", 4);
    h.Subchunk2Size = data_length * byps;
    h.ChunkSize = 36 + h.Subchunk2Size;

    size_t size = sizeof(WavHeader) + h.Subchunk2Size;

    unsigned char* rst = (unsigned char*)malloc(size);
    if (!rst) {
        return {};
    }

    memcpy(rst, &h, sizeof(WavHeader));
    unsigned char* ptr = rst + sizeof(WavHeader);
    for (size_t i = 0; i < data_length; i++) {
        *(short*)(ptr + i * 2) = data[i];
    }

    AAFCOUTPUT outp = {size, rst};
    return outp;
}

int main(int argc, char* argv[]) {
    if (argc == 1 || argc == 2) {
        printf("usage\n\naafc2wav <inputpath> <output>\n(theres no need to put the extension ['.wav'] within the output path.)\n\n");
        return -1;
    }

    AAFCDECOUTPUT f = LoadAAFC(ReadFile(argv[1]).data);
    if (!f.data) return 1;
    const char* fn = filename_without_extension(argv[2]);

    AAFCOUTPUT wavout = export_wav((short*)FloatToInt(f.data, f.header.samplelength, 16), f.header.samplelength, f.header.freq, f.header.channels);
    if (!wavout.data) {
        printf("could not export to wav\n");
        return 2;
    }

    FILE* fout = fopen(concat_path_wav(argv[2]), "wb");
    fwrite(wavout.data, 1, wavout.size, fout);
    printf("completed!\n\nPATH: %s\n\n", fn);
}