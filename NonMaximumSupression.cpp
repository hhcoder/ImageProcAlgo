// Practice: Non-maximum suppression
//   https://en.wikipedia.org/wiki/Canny_edge_detector#Non-maximum_suppression

// Command line usage:
//  a.out ../source_images/nonmaximum_suppression.y ../processed_results/nonmaximum_suppression.y 426 319

#include <stdio.h>
#include <stdlib.h>

class ImageType
{
public:
    ImageType(const size_t in_w, const size_t in_h)
        : width(in_w), height(in_h), length(width*height), p(NULL)
    {
        p = (unsigned char*)malloc(length);
        if(NULL==p)
        {
            printf("Error in allocation\n");
        }
    }

    ~ImageType()
    {
        if(NULL!=p)
            free(p);
    }

    void Read(const char* ifname)
    {
        FILE* fp = fopen(ifname, "rb");
        if(NULL==fp)
        {
            printf("Error opening: %s\n", ifname);
            return;
        }
        fread(p, sizeof(unsigned char), length, fp);
        fclose(fp);
    }

    void Write(const char* ofname)
    {
        FILE* fp = fopen(ofname, "wb");
        if(NULL==fp)
        {
            printf("Error opening: %s\n", ofname);
            return;
        }
        fwrite(p, sizeof(unsigned char), length, fp);
        fclose(fp);
    }

    unsigned char* GetLoc(unsigned int x, unsigned int y)
    {
        return p+(y*width+x);
    }

    static void NonMaximumSupression(ImageType& src, ImageType& dst)
    {
        for(unsigned int j=3; j<src.height-4; j++)
        {
            for(unsigned int i=3; i<src.width-4; i++)
            {
                unsigned char max_val = 0;
                //Find the max
                for(int s=-3; s<=3; s++)
                {
                    for(int t=-3; t<=3; t++)
                    {
                        unsigned char* p = src.GetLoc(i+t, j+s);
                        if(*p>max_val)
                            max_val = *p;
                    }
                }

                unsigned char* src_ptr = src.GetLoc(i, j);
                unsigned char* dst_ptr = dst.GetLoc(i, j);
                if(*src_ptr!=max_val)
                    *dst_ptr = 0;
                else
                    *dst_ptr = max_val;
            }
        }
    }
private:
    unsigned int width;
    unsigned int height;
    unsigned int length;
    unsigned char* p;
};

int main(int argc, char* argv[])
{
    const char* ifname = argv[1];
    const char* ofname = argv[2];
    const unsigned int in_width = atoi(argv[3]);
    const unsigned int in_height = atoi(argv[4]);

    ImageType im(in_width, in_height);
    ImageType im2(in_width, in_height);

    im.Read(ifname);
    ImageType::NonMaximumSupression(im, im2);

    im2.Write(ofname);

    return 0;
}
