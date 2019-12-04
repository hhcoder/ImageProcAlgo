// Practice: Morphology operation: erode, then dilate

// Commandline usage:
//   a.out ../source_images/erode_dilate.y ../processed_result/erode_dilate.y 640 480

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

void Erode(const unsigned char* src, unsigned char* dst, const size_t in_width, const size_t in_height)
{
    for(int j=1; j<in_height-1; j++)
    {
        for(int i=1; i<in_width-1; i++)
        {
            unsigned char max = 0;
            for(int t=0; t<3; t++)
            {
                for(int s=-1; s<=1; s++)
                {
                    size_t y = j+s;
                    size_t x = i+t;

                    if(max<src[x+y*in_width])
                        max = src[x+y*in_width];
                }
            }

            dst[i+j*in_width] = max;
        }
    }
}

void Dilate(const unsigned char* src, unsigned char* dst, const size_t in_width, const size_t in_height)
{
    for(int j=1; j<in_height-1; j++)
    {
        for(int i=1; i<in_width-1; i++)
        {
            unsigned char min = 255;
            for(int t=0; t<3; t++)
            {
                for(int s=-1; s<=1; s++)
                {
                    size_t y = j+s;
                    size_t x = i+t;

                    if(min>src[x+y*in_width])
                        min = src[x+y*in_width];
                }
            }

            dst[i+j*in_width] = min;
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

    Dilate(src, dst, in_width, in_height);
    Dilate(dst, src, in_width, in_height);
    Dilate(src, dst, in_width, in_height);
    Dilate(dst, src, in_width, in_height);
    Dilate(src, dst, in_width, in_height);
    Dilate(dst, src, in_width, in_height);
    Dilate(src, dst, in_width, in_height);

    Erode(src, dst, in_width, in_height);
    Erode(dst, src, in_width, in_height);
    Erode(src, dst, in_width, in_height);
    Erode(dst, src, in_width, in_height);
    Erode(src, dst, in_width, in_height);
    Erode(dst, src, in_width, in_height);
    Erode(src, dst, in_width, in_height);

    if(false==Write(dst, in_size, out_fname))
    {
        printf("Error in writing: %s!\n", out_fname);
        return -1;
    }

    free(src);
    free(dst);

}
