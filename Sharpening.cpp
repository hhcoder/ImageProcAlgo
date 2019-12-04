// Commandline usage:
//  a.out ../source_images/sharpening_1280x720.y ../processed_result/sharpening_1280x720.y 1280 720

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

template <typename T> class ImageTemplate
{
public:
    class Size
    {
    public:
        Size(const size_t in_width, const size_t in_height)
            : width(in_width), height(in_height)
        {
        }
        Size(const Size& in_size)
        {
            width = in_size.width;
            height = in_size.height;
        }
        inline size_t Length() const { return width*height; }
        inline size_t Width() const { return width; }
        inline size_t Height() const { return height; }
    private:
        size_t width;
        size_t height;
    };

public:
    ImageTemplate(const Size& in_size)
        : size(in_size), ptr(NULL)
    {
        buf = (unsigned char*)malloc(size.Length()*sizeof(unsigned char));
        if(NULL==buf)
            printf("Error in allocation!\n");
        ptr = (T*)malloc(size.Length()*sizeof(T));
        if(NULL==ptr)
            printf("Error in allocation!\n");
    }

    ~ImageTemplate()
    {
        if(NULL!=buf)
            free(buf);
        if(NULL!=ptr)
            free(ptr);
    }

    void Read(const char* in_fname)
    {
        FILE* fp = fopen(in_fname, "rb");
        if(NULL==fp)
            printf("Error opening: %s\n", in_fname);
        fread(buf, sizeof(unsigned char), size.Length(), fp);
        for(size_t i=0; i<size.Length(); i++)
        {
            ptr[i] = (T)(buf[i]);
        }
        fclose(fp);
    }

    void Write(const char* out_fname)
    {
        FILE* fp = fopen(out_fname, "wb");
        if(NULL==fp)
            printf("Error opening: %s\n", out_fname);
        for(size_t i=0; i<size.Length(); i++)
        {
            buf[i] = Clamp(ptr[i]);
        }
        fwrite(buf, sizeof(unsigned char), size.Length(), fp);
        fclose(fp);
    }

    inline Size GetSize() const
    {
        return size;
    }

private:
    inline T GetValue(const size_t x, const size_t y) const
    {
        return *(ptr+(y*size.Width()+x));
    }

    inline T GetValue(const size_t i) const
    {
        return *(ptr+i);
    }

    inline unsigned char Clamp(const T in_val)
    {
        const T max_v = 255.0f;
        const T min_v = 0.0f;
        const unsigned char max_c = 255;
        const unsigned char min_c = 0;

        if(in_val>max_v)
            return max_c;
        if(in_val<min_v)
            return min_c;
        return (unsigned char)(in_val);
    }

    inline void SetValue(const size_t x, const size_t y, const T in_val)
    {
        *(ptr+y*size.Width()+x) = in_val;
    }

    inline void SetValue(const size_t i, const T in_val)
    {
        *(ptr+i) = in_val;
    }

    T Max() const
    {
        T mx = -1000;
        for(size_t i=0; i<size.Length(); i++)
        {
            T t = GetValue(i);
            if(t>mx)
                mx = t;
        }

        T mn = 1000;
        for(size_t i=0; i<size.Length(); i++)
        {
            T t = GetValue(i);
            if(t<mn)
                mn = t;
        }

        if(abs(mx)>abs(mn))
            return abs(mx);
        else
            return abs(mn);
    }

public:
    inline static T KernelMultiply(const float kernel_val, const T in_val) 
    {
        float t = kernel_val * float(in_val);
        return T(t);
    }

    static void LowPassFilter(const ImageTemplate& src, ImageTemplate& dst)
    {
        const size_t kernel_size = 9;
        const size_t kernel_range = floor(kernel_size/2);
        const size_t j_st = kernel_range;
        const size_t j_ed = src.size.Height()-kernel_range;
        const size_t i_st = kernel_range;
        const size_t i_ed = src.size.Width()-kernel_range;

        const int t_st = -kernel_range;
        const int t_ed = kernel_range;
        const int s_st = -kernel_range;
        const int s_ed = kernel_range; 

        const float kernel[81] = 
        {   0.0095,    0.0104,    0.0112,    0.0117,    0.0118,    0.0117,    0.0112,    0.0104,    0.0095,
            0.0104,    0.0115,    0.0123,    0.0128,    0.0130,    0.0128,    0.0123,    0.0115,    0.0104,
            0.0112,    0.0123,    0.0132,    0.0138,    0.0140,    0.0138,    0.0132,    0.0123,    0.0112,
            0.0117,    0.0128,    0.0138,    0.0144,    0.0146,    0.0144,    0.0138,    0.0128,    0.0117,
            0.0118,    0.0130,    0.0140,    0.0146,    0.0148,    0.0146,    0.0140,    0.0130,    0.0118,
            0.0117,    0.0128,    0.0138,    0.0144,    0.0146,    0.0144,    0.0138,    0.0128,    0.0117,
            0.0112,    0.0123,    0.0132,    0.0138,    0.0140,    0.0138,    0.0132,    0.0123,    0.0112,
            0.0104,    0.0115,    0.0123,    0.0128,    0.0130,    0.0128,    0.0123,    0.0115,    0.0104,
            0.0095,    0.0104,    0.0112,    0.0117,    0.0118,    0.0117,    0.0112,    0.0104,    0.0095 };

        for(size_t j=j_st; j<j_ed; j++)
        {
            for(size_t i=i_st; i<i_ed; i++)
            {
                T sum = T(0.0f);
                unsigned int idx = 0;

                for(int t=t_st; t<=t_ed; t++)
                {
                    for(int s=s_st; s<=s_ed; s++)
                    {
                        sum += KernelMultiply(kernel[idx], src.GetValue(i+s, j+t));
                        idx++;
                    }
                }

                dst.SetValue(i, j, sum);
            }
        }
    }

    static void Subtract(const ImageTemplate& src1, const ImageTemplate& src2, ImageTemplate& dst)
    {
        const size_t j_st = 0;
        const size_t j_ed = src1.size.Height();
        const size_t i_st = 0;
        const size_t i_ed = src1.size.Width();

        for(size_t j=j_st; j<j_ed; j++)
        {
            for(size_t i=i_st; i<i_ed; i++)
            {
                T u = src1.GetValue(i, j);
                T v = src2.GetValue(i, j);
                T d = u-v;

                dst.SetValue(i, j, d);
            }
        }
    }

    static void Add(const ImageTemplate& src1, const ImageTemplate& src2, ImageTemplate& dst)
    {
        const size_t j_st = 0;
        const size_t j_ed = src1.size.Height();
        const size_t i_st = 0;
        const size_t i_ed = src1.size.Width();

        for(size_t j=j_st; j<j_ed; j++)
        {
            for(size_t i=i_st; i<i_ed; i++)
            {
                T u = src1.GetValue(i, j);
                T v = src2.GetValue(i, j);
                T d = u+v;

                dst.SetValue(i, j, d);
            }
        }
    }

    static T Power(const T s, const T mx, const float theta)
    {
        float m;

        if(s<0)
            m = -1.0f*(float)mx;
        else
            m = (float)mx;

        float as = abs(s);
        float v = (float)as/(float)mx;

        T t = m*pow(v, theta);
        return t;
    }

    static void Enhance(ImageTemplate& img)
    {
        T mx = img.Max();

        for(size_t i=0; i<img.size.Length(); i++)
        {
            T s = img.GetValue(i);

            T t = Power(s, mx, 0.55f);

            img.SetValue(i, t);
        }
    }

private:
    Size size;
    unsigned char* buf;
    T* ptr;
};

typedef ImageTemplate<float> Image;
int main(int argc, char* argv[])
{
    const char* in_fname = argv[1];
    const char* out_fname = argv[2];
    const size_t in_width = atoi(argv[3]);
    const size_t in_height = atoi(argv[4]);

    const Image::Size src_size(in_width, in_height);

    Image src(src_size);

    src.Read(in_fname);

    Image lpf(src.GetSize());

    Image::LowPassFilter(src, lpf);

    Image hpf(src.GetSize());
    Image::Subtract(src, lpf, hpf);

    Image::Enhance(hpf);

    Image dst(src.GetSize());
    Image::Add(hpf, lpf, dst);

    dst.Write(out_fname);

    return 0;
}
