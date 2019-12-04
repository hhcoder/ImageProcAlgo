//Practice: de-interleave a YUV (NV21) format image

// Command line usage:
// a.out ../source_images/deinterleave_480x864.yuv 480 864 d.y d.uv


#include <stdlib.h>
#include <stdio.h>

#include <stddef.h>
#include <stdint.h>

class Plane
{
public:
    enum ErrCode
    {
        FAIL,
        SUCCESS
    };

    static bool IsFail(ErrCode err)
    {
        return err!=SUCCESS;
    }

public:
    Plane(const uint32_t in_width, const uint32_t in_height, const uint32_t in_offset, uint8_t* in_p)
        : width(in_width), height(in_height), offset(in_offset), p(in_p)
    {
    }

    ~Plane()
    {
    }

    ErrCode Write(const char* ofname)
    {
        FILE* fp = fopen(ofname, "wb");
        if(NULL==fp)
        {
            printf("Error in opening: %s", ofname);
            return FAIL;
        }

        if(offset!=1)
        {
            size_t byte_size = width*height;
            uint8_t* p_tmp = (uint8_t*)malloc(sizeof(uint8_t)*byte_size);
            if(NULL==p_tmp)
            {
                printf("Error in allocation");
                fclose(fp);
                return FAIL;
            }
            uint8_t* src = p;
            for(size_t i=0; i<byte_size; i++)
            {
                p_tmp[i] = *src;
                src+=offset;
            }
            if(byte_size!=fwrite(p_tmp, sizeof(uint8_t), byte_size, fp))
            {
                printf("Error in writing: %s\n", ofname);
                fclose(fp);
                return FAIL;
            }
            fclose(fp);
        }
        else
        {
            size_t byte_size = width*height;
            fwrite(p, sizeof(uint8_t), byte_size, fp);
            fclose(fp);
        }

        return SUCCESS;
    }

private:
    uint32_t width;
    uint32_t height;
    uint32_t offset;
    uint8_t* p;
};

class ImageYuYv
{
public:
    ImageYuYv(const uint32_t in_width, const uint32_t in_height, uint8_t* in_buf)
    {
        uint8_t* py = in_buf;
        uint8_t* puv = py+1;
        y = new Plane(in_width, in_height, 2, py);
        uv = new Plane(in_width, in_height, 2, puv);
    }

    bool Write(const char* y_name, const char* uv_name)
    {
        if( Plane::IsFail(y->Write(y_name)) )
            return false;
        if( Plane::IsFail(uv->Write(uv_name)) )
            return false;

        return true;
    }
private:
    Plane* y;
    Plane* uv;
};

int main(int argc, char* argv[])
{
    const char* in_fname = argv[1];
    const size_t in_width = atoi(argv[2]);
    const size_t in_height = atoi(argv[3]);
    const char* out_y_fname = argv[4];
    const char* out_uv_fname = argv[5];

    const size_t in_bytes = in_width*in_height*2;
    uint8_t* in_buf = (uint8_t*)malloc(sizeof(uint8_t)*in_bytes);

    FILE* fp = fopen(in_fname, "rb");
    fread(in_buf, sizeof(uint8_t), in_bytes, fp);
    fclose(fp);

    if(NULL==in_buf)
        return -1;

    ImageYuYv img(in_width, in_height, in_buf);

    img.Write(out_y_fname, out_uv_fname);

    free(in_buf);

    return 0;
}
