// Practice: implement histogram stretching on an image

// Command line:
//  a.out ../source_images/linearization_640x480.y ../processed_results/linearization_result.y 640 480

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

bool Read(unsigned char* in_buf, const size_t in_size, const char* in_fname)
{
    FILE* fp = fopen(in_fname, "rb");
    if(NULL==fp)
    {
        printf("Error opening :%s\n", in_fname);
        return false;
    }
    if(in_size!=fread(in_buf, sizeof(unsigned char), in_size, fp))
    {
        printf("Error in reading: %s!\n", in_fname);
        return false;
    }

    fclose(fp);
    return true;
}

bool Write(unsigned char* out_buf, const size_t out_size, const char* out_fname)
{
    FILE* fp = fopen(out_fname, "wb");
    if(NULL==fp)
    {
        printf("Error opening :%s\n", out_fname);
        return false;
    }
    if(out_size!=fwrite(out_buf, sizeof(unsigned char), out_size, fp))
    {
        return false;
    }
    fclose(fp);
    return true;
}

void GetHist(unsigned int in_hist[256], const unsigned char* in_buf, const size_t in_size)
{
    memset(in_hist, 0, sizeof(unsigned int)*256);

    const unsigned char* p = in_buf;
    size_t i=0;
    while(i!=in_size)
    {
        in_hist[*p]++;
        p++;
        i++;
    }

    for(int i=0; i<256; i++)
    {
        printf("%d\n", in_hist[i]);
    }
}

int GetLowValue(unsigned int src_hist[256], const size_t total_pixels, const float ratio)
{
    unsigned int sum = 0;
    for(int i=0; i<256; i++)
    {
        sum += src_hist[i];
        float r = (float)sum/(float)total_pixels;
        if(r>=ratio)
            return i;
    }
    return 255;
}

int GetHighValue(unsigned int src_hist[256], const size_t total_pixels, const float ratio)
{
    unsigned int sum = 0;
    for(int i=255; i>=0; i--)
    {
        sum += src_hist[i];
        float r = (float)sum/(float)total_pixels;
        if(r>ratio)
            return i;
    }
    return 0;
}

void Linearize(const unsigned char* src, unsigned char* dst, const size_t in_width, const size_t in_height)
{
    unsigned int src_hist[256];
    GetHist(src_hist, src, in_width*in_height);

    int low_val = GetLowValue(src_hist, in_width*in_height, 0.1f);
    int high_val = GetHighValue(src_hist, in_width*in_height, 0.1f);

    float ratio = (255.0f)/((float)high_val - (float)low_val);

    printf("\n ratio = %f\n", ratio);

    for(size_t j=0; j<in_height; j++)
    {
        for(size_t i=0; i<in_width; i++)
        {
            int v = (src[i+j*in_width] - low_val)*ratio;
            if(v>255)
                v = 255;
            if(v<0)
                v = 0;
            dst[i+j*in_width] = (unsigned char)v;
        }
    }
}

int main(int argc, char* argv[])
{
    const char* in_fname = argv[1];
    const char* out_fname = argv[2];
    const size_t in_width = atoi(argv[3]);
    const size_t in_height = atoi(argv[4]);
    const size_t in_size = in_width*in_height;

    unsigned char* src = (unsigned char*)malloc(sizeof(unsigned char)*in_size);
    if(NULL==src)
    {
        printf("Error in allocation!\n");
        return -1;
    }

    if(false==Read(src, in_size, in_fname))
    {
        return -1;
    }

    unsigned char* dst = (unsigned char*)malloc(sizeof(unsigned char)*in_size);
    if(NULL==dst)
    {
        printf("Error in allocation!\n");
        return -1;
    }

    Linearize(src, dst, in_width, in_height);

    if(false==Write(dst, in_size, out_fname))
    {
        printf("Error in writing: %s!\n", out_fname);
        return -1;
    }

    free(src);
    free(dst);

}
