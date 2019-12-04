// Practice: Simplified wavelet based noise reduction implementation

// Commandline usage: 
//  a.out ../source_images/wavelet_denoise.yuv 640 480 ../processed_results/wavelet_denoise.yuv

#include "wavelet_denoise.h"

int main(int argc, char* argv[])
{
    const char* in_fname = argv[1];
    const size_t in_width = atoi(argv[2]);
    const size_t in_height = atoi(argv[3]);
    const char* out_fname = argv[4];

    Image* img = new Image(in_width, in_height);

    img->Read(in_fname);

    WaveletPyramid w(img);

    w.Decompose();

    img->Write(out_fname);

    return 0;
}
