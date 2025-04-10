#include <fileutils.h>
#include <libaafcfunc.h>

unsigned char* create_wav_header(unsigned long dtsize, unsigned long samplerate, unsigned short chn, unsigned char bps) {
    unsigned char* dt = (unsigned char*)malloc(44);
    unsigned char* ptr = dt;
    const unsigned short bytesps = bps / 8;

    memcpy(ptr, "RIFF", 4); ptr += 4;
    *(unsigned int*)ptr = 36 + dtsize; ptr += 4;
    memcpy(ptr, "WAVE", 4); ptr += 4;
    memcpy(ptr, "fmt ", 4); ptr += 4;
    *(unsigned int*)ptr = 16; ptr += 4;
    *(unsigned short*)ptr = 1; ptr += 2;
    *(unsigned short*)ptr = chn; ptr += 2;
    *(unsigned int*)ptr = samplerate; ptr += 4;
    *(unsigned int*)ptr = samplerate * chn * bytesps; ptr += 4;
    *(unsigned short*)ptr = chn * bytesps; ptr += 2;
    *(unsigned short*)ptr = bps; ptr += 2;
    memcpy(ptr, "data", 4); ptr += 4;
    *(unsigned int*)ptr = dtsize; ptr += 4;

    return dt;
}

AAFCOUTPUT export_wav(const short* data, size_t data_length, int freq, unsigned char channels) {
    unsigned char* h = create_wav_header(data_length * 2, freq, channels, 16);
    size_t size = 44 + (data_length * 2);

    unsigned char* rst = (unsigned char*)malloc(size);
    if (!rst) {
        free(h);
        return {};
    }

    memcpy(rst, h, 44);
    free(h);
    memcpy(rst + 44, data, data_length * 2);

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

    AAFCOUTPUT wavout = export_wav((short*)FloatToInt(f.data, f.header.samplelength, 16), f.header.samplelength, f.header.freq, f.header.channels);
    if (!wavout.data) {
        printf("could not export to wav\n");
        return 2;
    }

    const char* outpath = concat_path(argv[2]);
    FILE* fout = fopen(outpath, "wb");
    if (fout == NULL) {
        free(wavout.data);
        printf("shucksers, couldn't open file.\n");
        return 4;
    }
    fwrite(wavout.data, 1, wavout.size, fout);
    free(wavout.data);

    printf("completed!\n\nPATH: %s\n\n", outpath);
    return 0;
}