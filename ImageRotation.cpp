// Practice:
//   Rotate image to its 180 degree 
//   Notice it is an in-place operation

// Commandline usage:
//  a.out ../source_images/rotate_180_426x240.y ../processed_result_426x240.y 426 240

#include <stdio.h>
#include <stdlib.h>

class ImageType
{
public:
    ImageType(const size_t in_width, const size_t in_height)
        : width(in_width), height(in_height), length(width*height), p(NULL)
    {
        p = (unsigned char*)malloc(length);
    }

    ~ImageType()
    {
        if(NULL!=p)
            return;
    }

    bool Read(const char* ifname)
    {
        FILE* fp = fopen(ifname, "rb");
        if(NULL==fp)
        {
            printf("Error opening: %s\n", ifname);
            return false;
        }
        if(length!=fread(p, sizeof(unsigned char), length, fp))
        {
            printf("Error reading: %s\n", ifname);
            fclose(fp);
            return false;
        }
        fclose(fp);
        return true;
    }

    bool Write(const char* ofname)
    {
        FILE* fp = fopen(ofname, "wb");
        if(NULL==fp)
        {
            printf("Error opening: %s\n", ofname);
            return false;
        }
        if(length!=fwrite(p, sizeof(unsigned char), length, fp))
        {
            printf("Error reading: %s\n", ofname);
            fclose(fp);
            return false;
        }
        fclose(fp);
        return true;
    }

    void Rotate()
    {
        RotateImage(p, length);
    }

private:
    static void RotateImage(unsigned char* p, const size_t length)
    {
        size_t mid_way = length/2;
        unsigned char* p_start = p;
        unsigned char* p_end = p+(length-1);

        for(size_t i=0; i<mid_way; i++)
        {
            unsigned char t = *p_start;
            *p_start = *p_end;
            *p_end = t;
            p_start++;
            p_end--;
        }
    }

private:
    size_t width;
    size_t height;
    size_t length;
    unsigned char* p;
};

int main(int argc, char* argv[])
{
    const char* in_fname = argv[1];
    const char* out_fname = argv[2];
    const size_t in_width = atoi(argv[3]);
    const size_t in_height = atoi(argv[4]);

    ImageType im(in_width, in_height);

    if(true!=im.Read(in_fname))
        return -1;

    im.Rotate();

    if(true!=im.Write(out_fname))
        return -1;

    return 0;
}

// Commandline usage:
//   a.out ../source_image/rotate_90_426x240.y ../processed_results/rotate_90_426x240.y 426 240

#include <stdio.h>
#include <stdlib.h>

void RotateImageClockWise(const unsigned char* src, unsigned char* dst, const unsigned int in_width, const unsigned int in_height)
{
    const unsigned char* psrc = src;
    unsigned char* pdst = dst;

    unsigned int i;
    unsigned int j;

    for(j=0; j<in_height; j++)
    {
        pdst = dst+in_height-j;
        for(i=0; i<in_width; i++)
        {
            *pdst = *psrc;
            psrc++;
            pdst+=in_height;
        }
    }
}

void RotateImageCounterColockWise(const unsigned char* src, unsigned char* dst, const unsigned int in_width, const unsigned int in_height)
{
    const unsigned char* psrc = src;
    unsigned char* pdst = dst;

    unsigned int i;
    unsigned int j;

    for(j=0; j<in_height; j++)
    {
        pdst = dst+in_width*in_height-j+1;
        for(i=0; i<in_width; i++)
        {
            *pdst = *psrc;
            psrc++;
            pdst-=in_height;
        }
    }
}

bool ReadFromFile(unsigned char* src, const char* in_fname, const size_t in_size)
{
    FILE* fp = fopen(in_fname, "rb");
    if(NULL==fp)
        return false;
    if(in_size!=fread(src, sizeof(unsigned char), in_size, fp))
    {
        fclose(fp);
        return false;
    }
    fclose(fp);
    return true;
}

bool WriteToFile(unsigned char* src, const char* out_fname, const size_t out_size)
{
    FILE* fp = fopen(out_fname, "wb");
    if(NULL==fp)
        return false;
    if(out_size!=fwrite(src, sizeof(unsigned char), out_size, fp))
    {
        fclose(fp);
        return false;
    }
    fclose(fp);
    return true;
}

int main(int argc, char* argv[])
{
    const char* in_fname = argv[1];
    const unsigned int in_width = atoi(argv[2]);
    const unsigned int in_height = atoi(argv[3]);
    const char* out_fname = argv[4];

    const size_t img_size = in_width*in_height;

    unsigned char* img_src = (unsigned char*)malloc(img_size);
    unsigned char* img_dst = (unsigned char*)malloc(img_size);

    if(false==ReadFromFile(img_src, in_fname, img_size))
    {
        printf("Error in reading file: %s\n", in_fname);
        return -1;
    }

    // RotateImageCounterColockWise(img_src, img_dst, in_width, in_height);
    RotateImageClockWise(img_src, img_dst, in_width, in_height);

    if(false==WriteToFile(img_dst, out_fname, img_size))
    {
        printf("Error in reading file: %s\n", out_fname);
        return -1;
    }

    free(img_src);
    free(img_dst);

    return 0;
}

// Practice: Rotate image with any angle user specified

// Commandline usage:
//  a.out ../source_image/rotate_any_angle_426x240.y ../processed_results/rotate_any_angle_426x240.y 426 240 0.05

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
        // printf("Error in reading: %s!\n", in_fname);
        // return false;
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

void Rotate(const unsigned char* src, unsigned char* dst, const size_t in_width, const size_t in_height, const float angle)
{
    for(size_t j=0; j<in_height; j++)
    {
        for(size_t i=0; i<in_width; i++)
        {
            float dst_i = i*cos(angle) - j*sin(angle);
            float dst_j = i*sin(angle) + j*cos(angle);

            if(dst_i>in_width-1||dst_j>in_height-1)
                continue;
            if(dst_i<1||dst_j<1)
                continue;

            float hor_left = dst_i - floor(dst_i);
            float hor_right = 1.0f - hor_left;
            float ver_top = dst_j - floor(dst_j);
            float ver_bott = 1.0f - ver_top;

            float p00 = hor_left*ver_top;
            float p10 = hor_right*ver_top;
            float p01 = hor_left*ver_bott;
            float p11 = hor_right*ver_bott;

            float p = src[i + j*in_width] * p00 +
                      src[(i+1) + j*in_width] * p10 +
                      src[i + (j+1)*in_width] * p01 +
                      src[(i+1) + (j+1)*in_width] * p11;

            unsigned char v;
            if(p>255.0f)
                v = 255;
            else if(p<0.0f)
                v = 0;
            else
                v = (unsigned char)p;

            unsigned int dst_idx = (unsigned int)dst_i+(unsigned int)dst_j*in_width;
            dst[dst_idx] = v;
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
    const float in_angle = atof(argv[5]);

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

    Rotate(src, dst, in_width, in_height, in_angle);

    if(false==Write(dst, in_size, out_fname))
    {
        printf("Error in writing: %s!\n", out_fname);
        return -1;
    }

    free(src);
    free(dst);

}

