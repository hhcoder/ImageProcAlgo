#ifndef _Y_SAD_H_
#define _Y_SAD_H_

#include <vector>
#include <utility>
#include <string>
#include <fstream>
#include <stdint.h>
#include <iostream>

class YSad
{
private:
    typedef std::pair<int, int> OffsetUnit;

public:
    YSad(int in_width, int in_height)
        :   width(in_width),
            height(in_height),
            kernel_width(9), kernel_height(9),
            search_range_width(13), search_range_height(13),
            hor_skip(19), vert_skip(19),
            offset(static_cast<size_t>(width*height))
    {
        hor_lim = kernel_width + search_range_width;
        ver_lim = kernel_height + search_range_height;
    }

    void Proc(const uint8_t* ref, const uint8_t* in)
    {
        for (int j = ver_lim; j < height-ver_lim; j+=vert_skip)
        {
            for (int i = hor_lim; i < width-hor_lim; i+=hor_skip)
            {
                OffsetUnit min_idx;

                for (int l = -search_range_height; l < search_range_height; l++)
                {
                    for (int k = -search_range_width; k < search_range_width; k++)
                    {
                        int sum = 0;
                        int min_sum = INT_MAX; //SAD is always positive, so we choose a random negative number as minimal seed.

                        for (int n = -kernel_height; n < kernel_height; n++)
                        {
                            for (int m = -kernel_width; m < kernel_width; m++)
                            {
                                // Reduce domain 2
                                const uint8_t* p_ref = ref + (j + k + n) * width + (i + l + m);
                                const uint8_t* p_in = in + (j + k + n) * width + (i + l + m);

                                int v_ref = *p_ref;
                                int v_in = *p_in;

                                sum += abs(v_ref - v_in);
                            }
                        }

                        // Reduce domain 1
                        if (sum < min_sum)
                        {
                            min_sum = sum;
                            min_idx = OffsetUnit(k, l);
                        }
                    }
                }

                // Reduce domain 0 (output) 
                offset[j*width + i] = min_idx;
            }
            std::cout << "processed " << j << "lines." << std::endl;
        }

    }

    void Write(std::string ofname)
    {
        std::ofstream fid(ofname.c_str(), std::ios::out);

        for (int j = ver_lim; j < height - ver_lim; j += vert_skip)
        {
            for (int i = hor_lim; i < width-hor_lim; i+=hor_skip)
            {
                OffsetUnit v = offset[j*width + i];
                fid << "(" << v.first << "," << v.second << ")" << std::endl;
            }
            std::cout << "writing " << j << "lines." << std::endl;
        }
    }

    ~YSad()
    {
    }


private:
    int width;
    int height;
    std::vector<OffsetUnit> offset;
private:
    int kernel_width;
    int kernel_height;
    int search_range_width;
    int search_range_height;
    int hor_skip;
    int vert_skip;
    int hor_lim;
    int ver_lim;
        
};

#endif //_Y_SAD_H_
