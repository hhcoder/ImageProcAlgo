// Practice:
//    Median filter on 8-bit gray scale image

// Commandline usage:
//    TBD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class ImageY
{
public:
    ImageY(const size_t in_width, const size_t in_height)
        : width(in_width), height(in_height), size_in_bytes(width*height), ptr(NULL)
    {
        Allocate();
    }

    ~ImageY()
    {
        Free();
    }

    inline size_t Width() const { return width; }
    inline size_t Height() const { return height; }

    inline unsigned char* GetPtr(const size_t in_x, const size_t in_y) 
    { 
        return ptr+in_y*Width()+in_x; 
    }

private:
    bool Allocate()
    {
        if(NULL==ptr)
            ptr = (unsigned char*)malloc(size_in_bytes);
        if(NULL==ptr)
        {
            printf("Error in allocation!\n");
            return false;
        }
        return true;
    }

    void Free()
    {
        if(NULL!=ptr)
            free(ptr);
    }

private:
    size_t width;
    size_t height;
    size_t size_in_bytes;
    unsigned char* ptr;
};

class SimpleMath
{
    static bool IsEven(const size_t in_val){ return ((in_val%2)==0); }
};

class Cache1D
{
public:
    Cache1D(const size_t in_width)
        : width(in_width), data(NULL)
    {
    }

    bool Allocate()
    {
        data = (unsigned char*)malloc(sizeof(unsigned char)*width);
        if(NULL==data)
            return false;
        return true;
    }

    void Free()
    {
        if(NULL!=data)
            free(data);
        data = NULL;
    }

    inline unsigned char& Get(const size_t in_idx) const
    {
        return data[in_idx];
    }

    void Copy(const unsigned char* in_src)
    {
        memcpy(data, in_src, sizeof(unsigned char)*width);
    }

private:
    size_t width;
    unsigned char* data;
};

class Cache2D
{
public:
    Cache2D(const size_t in_width, const size_t in_height)
        : width(in_width), height(in_height), data(NULL), offset(0)
    {
        Allocate();
    }

    ~Cache2D()
    {
        Free();
    }

private:
    void Free()
    {
        if(NULL!=data)
        {
            for(unsigned int i=0; i<height; i++)
                delete data[i];

            delete[] data;

            data = NULL;
        }
    }

    bool Allocate()
    {
        Free();

        data = new Cache1D*[height];
        if(NULL==data)
            return false;

        for(unsigned int i=0; i<height; i++)
        {
            data[i] = new Cache1D(width);
            if(false==data[i]->Allocate())
                return false;
        }

        return true;
    }

public:
    void SetTo(const unsigned char* in_src, const size_t in_stride)
    {
        Allocate();

        const unsigned char* psrc = in_src; 

        for(unsigned int i=0; i<height; i++)
        {
            data[i]->Copy(psrc);
            psrc+=in_stride;
        }

        offset = 0;
    }
    
    void PushLine(const unsigned char* in_line_start)
    {
        data[offset]->Copy(in_line_start);
        offset = offset+1%height;
    }

    unsigned char& Get(const size_t in_x, const size_t in_y) const
    {
        size_t y = in_y + offset % height;
        return data[y]->Get(in_x);
    }

private:
    size_t width;
    size_t height;
    Cache1D** data;

    size_t offset;
};

class FilterOp
{
public:
    FilterOp(const size_t in_kernel_width, const size_t in_kernel_height)
    {
        img = in_img;
        cache = NULL; 
        curr_ptr = NULL;

        processed_width = 0;
        processed_height = 0;

        kernel_width = in_kernel_width; 
        half_kernel_width = floor(kernel_width/2);
        
        kernel_height = in_kernel_height;
        half_kernel_height = floor(kernel_height/2);
    }

    ~FilterOp()
    {
        if(NULL!=cache)
            delete cache;
    }

    bool Start(ImageY* in_img)
    {
        if( SimpleMath::IsEven(in_kernel_width) || 
            SimpleMath::IsEven(in_kernel_height) )
        {
            return false;
        }

        if(NULL!=cache)
            delete cache;

        cache = new Cache2D(in_kernel_width, in_kernel_height)

        curr_ptr = img->GetPtr(half_kernel_width, half_kernel_width);
        cache->SetTo(curr_ptr);
    }

    bool Process()
    {
        Start(in_img);
        while(IsNotEnd())
        {
            ProcessImpl(cache->Data(), cache->Width(), cache->Height());
            Next();
        }
    }

    void Next() 
    { 
        count++;
        if(count >= img->Height()-kernel_height)
        cache->PushLine(
    }

    bool IsNotEnd() 
    {
    }

    unsigned char Calculate()
    {
        unsigned char* src = p - half_kernel_width - half_kernel_width*img->Width();
        for(unsigned int i=0; i<kernel_width; i++)
        {
        }
    }

    virtual unsigned char CalculateImpl() = 0;

private:
    ImageY* img;

    unsigned char* curr_ptr;
    size_t count;

    size_t kernel_width;
    size_t half_kernel_width;
    size_t kernel_height;
    size_t half_kernel_height;


    Cache2D* cache;
};

int main(void)
{
    return 0;
}
