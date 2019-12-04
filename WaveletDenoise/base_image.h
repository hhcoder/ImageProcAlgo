#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

enum ReturnType
{
    SUCCESS = 0x00,
    FAIL
};

class ByteBuffer
{
public:
    ByteBuffer(const size_t in_width, const size_t in_height)
        : w(in_width), h(in_height), buf(NULL)
    {
        buf = (uint8_t*)malloc(GetSize());
        if(NULL==buf)
        {
            printf("Error in allocating buffer size %ld\n", GetSize());
        }
    }

    ~ByteBuffer()
    {
        if(NULL!=buf)
            free(buf);
    }

    ReturnType Read(const char* ifname)
    {
        FILE* fp = fopen(ifname, "rb");

        if(NULL==fp)
        {
            printf("Error opening %s\n", ifname);
            return FAIL;
        }

        if(GetSize() != fread(GetBuf(), sizeof(uint8_t), GetSize(), fp))
        {
            printf("Error writing %s\n", ifname);
            fclose(fp);
            return FAIL;
        }

        fclose(fp);
        return SUCCESS;
    }

    ReturnType Write(const char* ofname)
    {
        char fname[128];

        sprintf(fname, "%s_%dx%d.yuv", ofname, (int)GetWidth(), (int)GetHeight());

        FILE* fp = fopen(fname, "wb");

        if(NULL==fp)
        {
            printf("Error opening %s (%s)\n", fname, ofname);
            return FAIL;
        }

        if(GetSize() != fwrite(GetBuf(), sizeof(uint8_t), GetSize(), fp))
        {
            printf("Error writing %s (%s)\n", fname, ofname);
            fclose(fp);
            return FAIL;
        }

        fclose(fp);
        return SUCCESS;
    }

    static uint8_t Clamp(const int32_t v) 
    {
        if(v>255)
            return 0xFF;
        if(v<0)
            return 0;
    }
    
public:
    size_t GetWidth() const { return w; }
    size_t GetHeight() const { return h; }
    size_t GetSize() const { return w*h; }
    uint8_t* GetBuf(){ return buf; }

private:
    size_t w;
    size_t h;
    uint8_t* buf;
};

class Image
{
private:
    void Clear()
    {
        memset(buf, 0xFF, GetSizeInByte());
    }

    void Allocate()
    {
        if(NULL==buf)
        {
            buf = (int32_t*)malloc(GetSizeInByte());
            if(NULL==buf)
            {
                printf("Error in allocating buffer size %ld\n", w*h);
            }
            Clear();
        }
    }

    void Free()
    {
        if(NULL!=buf)
            free(buf);
    }

public:
    Image(const size_t in_width, const size_t in_height)
        : buf(NULL), w(in_width), h(in_height), s(in_width)
    {
        Allocate();
    }

    ~Image()
    {
        Free();
    }

    ReturnType CopyFrom(const Image* src)
    {
        if(GetWidth()!=src->GetWidth() || GetHeight()!=src->GetHeight())
            return FAIL;

        memcpy(buf, src->GetBuf(), GetSizeInByte());

        return SUCCESS;
    }

    ReturnType Read(const char* ifname)
    {
        ByteBuffer bbf(GetWidth(), GetHeight());
        
        if( bbf.Read(ifname) != SUCCESS)
        {
            return FAIL;
        }

        uint8_t* p = bbf.GetBuf();

        for(size_t i=0; i<bbf.GetSize(); i++)
        {
            SetPixel(i, (int32_t)*p);
            p++;
        }

        return SUCCESS;
    }

    ReturnType Write(const char* ofname)
    {
        ByteBuffer bbf(GetWidth(), GetHeight());

        uint8_t* p = bbf.GetBuf();

        for(size_t i=0; i<bbf.GetSize(); i++)
        {
            *p = ByteBuffer::Clamp(GetPixel(i));
            p++;
        }

        return bbf.Write(ofname);
    }

public:
    inline int32_t GetWidth() const { return w;  }

    inline int32_t GetStride() const { return s; }

    inline int32_t GetHeight() const { return h; }

    inline int32_t* GetPtr(int32_t x, int32_t y) const 
    { 
        return (buf + y*w + x); 
    }

    inline int32_t* GetPtr(int32_t x, int32_t y) 
    {      
        return (buf+y*w+x);
    }

    inline int32_t GetPixel(const int32_t x, const int32_t y) const
    {
        return *(buf+y*w+x);
    }

    inline int32_t GetPixel(const size_t offset)
    {
        return *(buf+offset);
    }

    inline void SetPixel(const int32_t x, const int32_t y, const int32_t v)
    {
        if(x>w) printf("x>w!\n");
        if(x<0) printf("x<0!\n");
        if(y>h) printf("y>h!\n");
        if(y<0) printf("x<0!\n");
        *(buf+y*w+x) = v;
    }

    inline void SetPixel(const size_t offset, const int32_t v)
    {
        *(buf+offset) = v;
    }
    
    inline size_t GetSizeInByte()
    {
        return w*h*sizeof(int32_t);
    }

    inline int32_t* GetBuf() const
    {
        return buf;
    }

private:
    int32_t* buf;
    size_t w;
    size_t h;
    size_t s;
};

