//Practice: Bayer mipi decode -> G channel interpolation -> Display

// Compile:
//  g++ bayer_g_interp.cpp

// Commandline usage:
//  ./a.out ../_source_images/bayer_g_interp_4032x3024.raw ../_processed_results/bayer_g_interp_4032x3024.raw 4032 3024
//  python ../py_show_image/show_image.py -i ../_processed_results/bayer_g_interp_4032x3024.raw -w 4032 -h 3024

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <stdint.h>
#include <iostream>

class DimInfo
{
public:
    DimInfo(const int32_t in_w, const int32_t in_h)
        : width(in_w), height(in_h)
    {
    }

    size_t Len() const { return width*height; }

    int32_t width;
    int32_t height;
};

template <typename TypeSrc, typename TypeDst> class ProcUnit
{
public:
    ProcUnit(const std::string& in_name, const DimInfo& in_dim)
        : name(in_name), dim(in_dim)
    {
        dst = new TypeDst(dim);
    }

    virtual ~ProcUnit()
    {
        delete dst;
    }

    TypeDst* Process(const TypeSrc* src)
    {
        ProcessImpl(src, dst);
        return dst;
    }

protected:
    virtual void ProcessImpl(const TypeSrc* in_src, TypeDst* in_dst) = 0;

private:
    std::string name;
    DimInfo dim;
    TypeDst* dst;
};

class MipiBin
{
public:
    MipiBin(const DimInfo& in_dim) 
        : len(in_dim.Len()*5/4)
    {
        buf = malloc(len);
    }

    ~MipiBin()
    {
        free(buf);
    }

    void Read(const std::string& in_fname)
    {
        FILE* fp = fopen(in_fname.c_str(), "rb");

        if(NULL==fp)
        {
            std::cout << "Error opening: " << in_fname << "when reading." << std::endl;
            return;
        }

        size_t read_len = fread(buf, sizeof(unsigned char), len, fp);
        fclose(fp);
    }

    void* buf;
    size_t len;
};

template <typename T>
class Pixel 
{
public:
    Pixel(T* in_v)
        : v(in_v)
    {}

    Pixel& operator=(const T& in_v) { *v = in_v; return *this; }

private:
    T* v;
};

template <typename T>
class Buf2D 
{
public:
    Buf2D(const DimInfo& in_dim)
        : dim(in_dim)
    {
        buf = (uint16_t*)malloc(sizeof(T)*in_dim.Len());
    }

    ~Buf2D()
    {
        free(buf);
    }

    int32_t Width() const { return dim.width; }
    int32_t Height() const { return dim.height; }

    Pixel<T> operator()(const int32_t x, const int32_t y) { return Pixel<T>(buf + y*dim.width + x); }

    void Write(const std::string& out_fname)
    {
        FILE* fp = fopen(out_fname.c_str(), "wb");

        if(NULL==fp)
        {
            std::cout << "Error opening: " << out_fname << "when writing." << std::endl;
            return;
        }

        size_t write_len = fwrite(buf, sizeof(T), dim.Len(), fp);
        fclose(fp);
    }

    DimInfo dim;
    T* buf;
};

class ReadFileIn : public ProcUnit<std::string, MipiBin>
{
public:
    ReadFileIn(const DimInfo& in_dim)
        : ProcUnit(
                std::string("Read file in"), 
                DimInfo(in_dim))
    {
    }

    virtual void ProcessImpl(const std::string* in_src, MipiBin* in_dst) 
    {
        in_dst->Read(*in_src);
    }

};


class BayerBuf : public Buf2D<uint16_t>
{
public:
    BayerBuf(const DimInfo& in_dim)
        : Buf2D<uint16_t>(in_dim)
    {
    }
};

class MipiUnpack : public ProcUnit<MipiBin, BayerBuf>
{
public:
    MipiUnpack(const DimInfo& in_dim)
        : ProcUnit(
                std::string("Mipi Unpack"),
                DimInfo(in_dim))
    {
    }

    virtual void ProcessImpl(const MipiBin* in_src, BayerBuf* in_dst) 
    {
        const unsigned char* proc_src = static_cast<unsigned char*>(in_src->buf);

        unsigned short* proc_dst = in_dst->buf;

        int32_t width = 4032;
        int32_t height = 3024;

        for (int i = 0; i < width * height; i += 4)
        {
            unsigned short val0 = 0;
            val0 += proc_src[0];
            unsigned short val1 = 0;
            val1 += proc_src[1];
            unsigned short val2 = 0;
            val2 += proc_src[2];
            unsigned short val3 = 0;
            val3 += proc_src[3];
            unsigned short val4 = 0;
            val4 += proc_src[4];

            proc_dst[0] = (val0 << 2) | (val4 & 3);
            proc_dst[1] = (val1 << 2) | ((val4 >> 2) & 3);
            proc_dst[2] = (val2 << 2) | ((val4 >> 4) & 3);
            proc_dst[3] = (val3 << 2) | ((val4 >> 6) & 3);

            proc_src += 5;
            proc_dst += 4;
        }
    }
};

int main(int argc, char* argv[])
{
    const char* in_fname = argv[1]; 
    const char* out_fname = argv[2];
    const int32_t in_width = atoi(argv[3]);
    const int32_t in_height = atoi(argv[4]);

    DimInfo in_dim(in_width, in_height);

    ReadFileIn read_in(in_dim);

    std::string in_fstring(in_fname);
    MipiBin* mipi_bin = read_in.Process(&in_fstring);

    MipiUnpack mipi_unpack(in_dim);

    BayerBuf* bayer_buf = mipi_unpack.Process(mipi_bin);

    bayer_buf->Write(std::string(out_fname));
    
    return 0;
}
