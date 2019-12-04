#include "Halide.h"

#include <iostream>

#include "halide_image_io.h"

using namespace Halide::Tools;

using namespace Halide;

void MipiUnpack(
        const void* in_src, 
        void* out_dst, 
        const int width, 
        const int height) 
{
    const uint8_t* proc_src = static_cast<const uint8_t*>(in_src);

    uint16_t* proc_dst = static_cast<uint16_t*>(out_dst);

    for (int i = 0; i < width * height; i += 4)
    {
        uint16_t val0 = proc_src[0];
        uint16_t val1 = proc_src[1];
        uint16_t val2 = proc_src[2];
        uint16_t val3 = proc_src[3];
        uint16_t val4 = proc_src[4];

        proc_dst[0] = (val0 << 2) | (val4 & 3);
        proc_dst[1] = (val1 << 2) | ((val4 >> 2) & 3);
        proc_dst[2] = (val2 << 2) | ((val4 >> 4) & 3);
        proc_dst[3] = (val3 << 2) | ((val4 >> 6) & 3);

        proc_src += 5;
        proc_dst += 4;
    }
}

void* Read(const char* in_fname, const size_t sz)
{
    void* src = malloc(sz);
    if(NULL==src)
    {
        printf("Error in allocating buffer: %d\n", __LINE__);
        return NULL;
    }
    FILE* fp = fopen(in_fname, "rb");
    if(NULL==fp)
    {
        printf("Error opening data file: %s\n", in_fname);
        return NULL;
    }
    if( sz != fread(src, 1, sz, fp) )
    {
        printf("Error reading data file: %s\n", in_fname);
        return NULL;
    }
    fclose(fp);

    return src;
}

int Write(const char* out_fname, const void* buf, const size_t sz)
{
    FILE* fp = fopen(out_fname, "wb");
    if( sz != fwrite(buf, 1, sz, fp) )
    {
        printf("Error writing file: %s\n", out_fname);
        return 0;
    }
    fclose(fp);
    return 1;
}

int main(int argc, char **argv) 
{
    const char* in_fname = argv[1];
    const int raw_width = atoi(argv[2]);
    const int raw_height = atoi(argv[3]);
    const char* out_fname = argv[4];
    const size_t raw_size = raw_width*raw_height;

    const size_t mipi_bytes = raw_size*5/4;
    void* mipi_src = Read(in_fname, mipi_bytes);
    if(NULL==mipi_src)
        return -1;

    const size_t raw_bytes = sizeof(uint16_t)*raw_size;
    void* bayer_raw = malloc(raw_bytes);
    if(NULL==bayer_raw)
    {
        printf("Error in allocating bayer raw\n");
        return -1;
    }

    MipiUnpack(
        mipi_src, 
        bayer_raw, 
        raw_width, 
        raw_height);

    Write("unpacked.raw", bayer_raw, raw_bytes);

    free(bayer_raw);
    free(mipi_src);
}

