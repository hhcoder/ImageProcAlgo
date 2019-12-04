// Practice: Sobel filter 
//
// Commandline Usage:
//   a.out ../source_images/sobel.y ../processed_results/sobel.y 426 240

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

class Image
{
public:
    Image(const char* ifname, const uint32_t iwidth, const uint32_t iheight)
        : width(iwidth), height(iheight), size(width*height), buf(NULL)
    {
        buf = (uint8_t*)malloc(sizeof(uint8_t)*width*height);
        if(NULL!=ifname)
        {
            FILE* fp = fopen(ifname, "rb");
            fread(buf, sizeof(uint8_t), size, fp);
            fclose(fp);
        }
    }

    void Write(const char* ofname)
    {
        FILE* fp = fopen(ofname, "wb");
        fwrite(buf, sizeof(uint8_t), size, fp);
        fclose(fp);
    }

    inline uint32_t GetWidth() const { return width; }
    inline uint32_t GetHeight() const { return height; }
    inline uint32_t GetSize() const { return size; }
    inline uint8_t* GetBuf() { return buf; }

private:
    uint32_t width;
    uint32_t height;
    uint32_t size;
    uint8_t* buf;

};

class TmpBuf
{
public:
    TmpBuf(Image* in_src, Image* in_dst)
    {
        move_step = 0;
        offset = in_src->GetWidth();

        uint8_t* src_buf = in_src->GetBuf();

        start = src_buf;
        end = src_buf + in_src->GetSize();

        const uint8_t width = offset;
        ptrs[0] = start;
        ptrs[1] = ptrs[0]+1;
        ptrs[2] = ptrs[1]+1;
        ptrs[3] = ptrs[0]+width;
        ptrs[4] = ptrs[3]+1;
        ptrs[5] = ptrs[4]+1;
        ptrs[6] = ptrs[3]+width;
        ptrs[7] = ptrs[6]+1;
        ptrs[8] = ptrs[7]+1;

        dst = in_dst->GetBuf() + offset + 1;
    }

    bool IsEnd()
    {
        return (ptrs[8] == end);
    }

    void GoNextPixel()
    {
        if(move_step == offset-2)
        {
            for(unsigned int i=0; i<9; i++)
                ptrs[i]+=3;
            dst+=3;
            move_step = 0;
        }
        else
        {
            for(unsigned int i=0; i<9; i++)
                ptrs[i]++;
            dst++;
            move_step++;
        }
    }

    uint8_t operator[](const unsigned int i)
    {
        return *ptrs[i];
    }

    void Assign(const uint8_t in_val)
    {
        *dst = in_val;
    }

private:
    uint8_t* start;
    uint8_t* end;
    uint8_t* ptrs[9];
    uint8_t* dst;
    uint32_t move_step;
    uint32_t offset;
};

class Lut
{
public:
    Lut(const float in_val)
        : val(in_val)
    {
        for(int i=0; i<255; i++)
            tbl[i] = (int)(i*val);
    }

    int Multiply(const uint8_t in_val)
    {
        return tbl[in_val];
    }

private:
    float val;
    int tbl[255];
};

class Kernel
{
public:
    Kernel(const float in_val[9])
    {
        for(unsigned int i=0; i<9; i++)
        {
            val[i] = new Lut(in_val[i]);
        }
    }

    int Multiply(TmpBuf& tbuf)
    {
        int sum = 0;
        for(unsigned int i=0; i<9; i++)
        {
            sum+=val[i]->Multiply(tbuf[i]);
        }
        return sum/9;
    }

    void Convolute(Image* src, Image* dst)
    {
        TmpBuf tbuf(src, dst);
        while(!tbuf.IsEnd())
        {
            int sum = Multiply(tbuf);

            uint8_t val = Clip(sum);

            tbuf.Assign(val);

            tbuf.GoNextPixel();
        }
    }

private:
    uint8_t Clip(int val)
    {
        if(val>255)
            return 255;
        if(val<0)
            return 0;
        return (uint8_t)val;
    }

private:
    Lut* val[9];
};

int main(int argc, char* argv[])
{
    const char* src_name = argv[1];
    const int in_width = atoi(argv[2]);
    const int in_height = atoi(argv[3]);

    Image src(src_name, in_width, in_height);
    Image dst(NULL, in_width, in_height);

    float in_kernel[9] = {1.0f, 0.0f, -1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f, -1.0f};
    Kernel sobel(in_kernel);

    sobel.Convolute(&src, &dst);

    dst.Write("dst.y");

    return 0;
}
