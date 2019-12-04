#include <stdio.h>
#include <stdlib.h>

class Image
{
public:
    Image(const size_t in_width, const size_t in_height)
        : width(in_width), height(in_height), buf(NULL)
    {
    }

    ~Image()
    {
        Free();
    }

    enum ErrCode
    {
        SUCCESS,
        FAIL
    };

    static bool IsSuccess(const ErrCode in_code) { return in_code==SUCCESS; }
    static bool IsFail(const ErrCode in_code) { return in_code==FAIL; }

public:
    inline size_t GetWidth() const { return width; }
    inline size_t GetHeight() const { return height; }
    inline size_t GetSizeInByt() const { return width*height; }

public:
    static Image* CreateFromFile(const char* in_fname, const size_t in_width, const size_t in_height)
    {
        Image* new_img = new Image(in_width, in_height);
        if( IsFail(new_img->Allocate()) )
            return NULL;

        if( IsFail(new_img->ReadFromFile(in_fname)) )
            return NULL;

        return new_img;
    }

    ErrCode WriteToFile(const char* out_fname)
    {
        if(NULL==buf)
        {
            printf("Image is not initialized\n");
            return FAIL;
        }

        FILE* fp = fopen(out_fname, "wb");
        if(NULL==fp)
        {
            printf("Error in opening %s to write\n", out_fname);
            return FAIL;
        }
        const size_t bytes_to_write = GetSizeInByte();
        if( bytes_to_write!=fwrite(buf, sizeof(uint8_t), bytes_to_write, fp) )
        {
            printf("Error in writing %s\n", out_fname);
            return FAIL;
        }
        fclose(fp);
        return SUCCESS;
    }

private:
    ErrCode Allocate()
    {
        buf = (uint8_t*)malloc(sizeof(uint8_t)*GetSizeInByte());
        if(NULL==buf)
            return FAIL;
        return SUCCESS;
    }

    void Free()
    {
        if(NULL!=buf)
        {
            free(buf);
            buf = NULL;
        }
    }

    ErrCode ReadFromFile(const char* in_fname)
    {
        if(NULL==buf)
        {
            printf("Image is not initialized\n");
            return FAIL;
        }

        FILE* fp = fopen(in_fname, "rb");
        if(NULL==fp)
        {
            printf("Error in opening %s to read\n", in_fname);
            return FAIL;
        }
        const size_t bytes_to_read = GetSizeInByte();
        if( bytes_to_read!=fread(buf, sizeof(uint8_t), bytes_to_read, fp) )
        {
            printf("Error in reading %s\n", in_fname);
            return FAIL;
        }

        fclose(fp);
        
        return SUCCESS;
    }

private:
    size_t width;
    size_t height;
    uint8_t* buf;
};

