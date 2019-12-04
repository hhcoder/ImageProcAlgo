// Practice:
//    Cubic interpolation for image upscaling


// Commandline usage:
//    a.out ../source_images/cubic_interpolation_640x480.y 640 480 1024 960 ../processed_results/cubic_interpolation_
// Notice the executable will output 2 images, one is linear interpolation result, the other is cubic scaler


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <sys/time.h>

enum ReturnType
{
    SUCCESS,
    FAIL
};

class TimeLogger
{
public:
    TimeLogger(const char* iname)
        : exe_name(iname)
    {
        gettimeofday(&td1x, NULL);
    }

    int GetMs()
    {
        gettimeofday(&td2x, NULL);
        long tdiff = 1000000* (td2x.tv_sec - td1x.tv_sec) + td2x.tv_usec - td1x.tv_usec;

        return (int)(tdiff/1000);
    }

    ~TimeLogger()
    {
        int ms = GetMs();

        if(NULL!=exe_name)
            printf("%20s |->processing time:[%d ms] \n", exe_name, ms);
        else
            printf("                    |->processing time:[%d ms] \n", ms);
    }
    
private:
    const char* exe_name;
        
private:
    struct timeval td1x;
    struct timeval td2x;
};

class Image
{
public:
    Image(const size_t in_w, const size_t in_h)
        : w(in_w), h(in_h), buf(NULL)
    {
    }

    ~Image()
    {
        if(NULL==buf)
            free(buf);
    }

    ReturnType Allocate()
    {
        if(NULL==buf)
        {
            buf = (uint8_t*)malloc(GetSizeInBytes());
        }

        if(NULL==buf)
            return FAIL;
        return SUCCESS;
    }

    ReturnType Read(const char* ifname)
    {
        if(NULL==buf)
            return FAIL;
        FILE* fp = fopen(ifname, "rb");
        if(NULL==fp)
        {
            printf("Error opening %s\n", ifname);
            return FAIL;
        }

        size_t s = GetSizeInBytes();
        if(s!=fread(buf, sizeof(uint8_t), s, fp))
        {
            printf("Error reading %s\n", ifname);
            fclose(fp);
            return FAIL;
        }

        fclose(fp);
        return SUCCESS;
    }

    ReturnType Write(const char* ofname)
    {
        if(NULL==buf)
            return FAIL;
        FILE* fp = fopen(ofname, "wb");
        if(NULL==fp)
        {
            printf("Error opening %s\n", ofname);
            return FAIL;
        }

        size_t s = GetSizeInBytes();
        if(s!=fwrite(buf, sizeof(uint8_t), s, fp))
        {
            printf("Error reading %s\n", ofname);
            fclose(fp);
            return FAIL;
        }

        fclose(fp);
        return SUCCESS;
    }

    void SetToCheckerBoard()
    {
        const int checker_board_h = 100;
        const int checker_board_w = checker_board_h; 
        const uint8_t white = 0xFF;
        const uint8_t black = 0x00;
        for(int j=0; j<h; j++)
         {
             int idx_j = j/checker_board_w;

             for(int i=0; i<w; i++)
             {
                int idx_i = i/checker_board_h;
                if(
                    idx_i%2==0&&idx_j%2==0 ||
                    idx_i%2==1&&idx_j%2==1   
                    )
                {
                    SetPixel(i, j, black);
                }
                else
                {
                    SetPixel(i, j, white);
                }
             }
         }
    }

public:
    inline size_t GetWidth() const
    {
        return w;
    }

    inline size_t GetHeight() const
    {
        return h;
    }

    inline uint8_t GetPixel(const int x, const int y) const
    {
        size_t ix;
        size_t iy;

        if(x>w)
            ix = w-1;
        else if(x<0)
            ix = 0;
        else
            ix = x;

        if(y>h)
            iy = h-1;
        else if (y<0)
            iy = 0;
        else
            iy = y;

        return *(buf + iy*w + ix);
    }

    inline void SetPixel(const int x, const int y, const uint8_t v)
    {
        *(buf + y*w + x) = v;
    }

private:
    inline size_t GetSizeInBytes() const
    {
        return w*h;
    }

private:
    int w;
    int h;
    uint8_t* buf;
};

class Interpolator
{
public:
    static ReturnType BilinearDownScale(const Image* src, Image* dst)
    {
        TimeLogger log(__func__);

        float x_ratio = (float)src->GetWidth()/(float)dst->GetWidth();
        float y_ratio = (float)src->GetHeight()/(float)dst->GetHeight();

        for(size_t j=0; j<dst->GetHeight(); j++)
        {
            float y = (float)j*y_ratio;

            for(size_t i=0; i<dst->GetWidth(); i++)
            {
                float x = (float)i*x_ratio;

                uint8_t v = PixelBiLinearInterp(src, x, y);

                dst->SetPixel(i, j, v);
            }
        }

        return SUCCESS;
    }

    static ReturnType BiCubicDownScale(const Image* src, Image* dst)
    {
        TimeLogger log(__func__);

        float x_ratio = (float)src->GetWidth()/(float)dst->GetWidth();
        float y_ratio = (float)src->GetHeight()/(float)dst->GetHeight();

        for(size_t j=0; j<dst->GetHeight(); j++)
        {
            float y = (float)j*y_ratio;

            for(size_t i=0; i<dst->GetWidth(); i++)
            {
                float x = (float)i*x_ratio;

                uint8_t v = PixelBiCubitInterp(src, x, y);

                dst->SetPixel(i, j, v);
            }
        }
        return SUCCESS;
    }

private:
    static inline uint8_t Clamp(float v)
    {
        if(v>255.0f)
            return 255;
        if(v<0.0f)
            return 0;
        return (uint8_t)v;
    }

    static uint8_t PixelBiLinearInterp(const Image* src, const float x, const float y)
    {
        int y0 = floor(y);
        int y1 = y0+1;
        int x0 = floor(x);
        int x1 = x0+1;

        const uint8_t p00 = src->GetPixel(x0, y0);
        const uint8_t p01 = src->GetPixel(x0, y1);
        const uint8_t p10 = src->GetPixel(x1, y0);
        const uint8_t p11 = src->GetPixel(x1, y1);

        float alpha = x-x0;
        float beta = y-y0;

        float p0 = alpha*p10 + (1.0f-alpha)*p00;
        float p1 = alpha*p11 + (1.0f-alpha)*p01;

        float p = beta*p1 + (1.0f-beta)*p0;

        return Clamp(p);
    }

    static uint8_t PixelBiCubitInterp(const Image* src, const float x, const float y)
    {
        int x0 = floor(x)-1;
        int y0 = floor(y)-1;

        uint8_t hor[4];
        uint8_t ver[4];

        for(size_t j=0; j<4; j++)
        {
            for(size_t i=0; i<4; i++)
            {
                hor[i] = src->GetPixel(x0+i, y0+j);
            }

            ver[j] = PixelCubicInterp(hor[0], hor[1], hor[2], hor[3], x-x0+1);
        }

        float p = PixelCubicInterp(ver[0], ver[1], ver[2], ver[3], y-y0+1);

        return Clamp(p);
    }

    static float PixelCubicInterp(
        const uint8_t p0, const uint8_t p1, const uint8_t p2, const uint8_t p3,
        const float x)
    {
        // x is between [0 1]
        // notice here the x0, x1, x2, x3 
        //     relative to p0, p1, p2, p3
        //              is -1,  0,  1,  2

        float a = (-1.0/2.0)*(float)p0 + 3.0/2.0*(float)p1 - 3.0/2.0*(float)p2 + 1.0/2.0*(float)p3;
        float b = (float)p0 - 5.0/2.0*(float)p1 + 2.0*(float)p2 - 1.0/2.0*(float)p3;
        float c = (-1.0/2.0)*(float)p0 + 1.0/2.0*(float)p2;
        float d = (float)p1;
        float x_square = x * x;
        float x_cubic = x_square*x;

        float y = a * x_cubic + b * x_square + c * x + d;

        return y;
    }
};

int main(int argc, char* argv[])
{
    const char* ifname = argv[1]; 
    const int in_width = atoi(argv[2]);
    const int in_height = atoi(argv[3]);

    const int out_width = atoi(argv[4]); 
    const int out_height = atoi(argv[5]);

    Image* src = new Image(in_width, in_height);
    src->Allocate();
    src->Read(ifname);
    // src->SetToCheckerBoard();

    Image* dst = new Image(out_width, out_height);
    dst->Allocate();

    Interpolator::BilinearDownScale(src, dst);

    char ofname[128];

    sprintf(ofname, "_linear_%3dx%3d.y", out_width, out_height);
    dst->Write(ofname);

    Interpolator::BiCubicDownScale(src, dst);
    sprintf(ofname, "_cubic_%3dx%3d.y", out_width, out_height);
    dst->Write(ofname);

    delete src;
    return 0;
}
