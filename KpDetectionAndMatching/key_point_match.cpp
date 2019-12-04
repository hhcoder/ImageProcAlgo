#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <climits>

static int CalcMSE(
    uint8_t* src, uint8_t* dst, int32_t stride, 
    int32_t kpx, int32_t kpy,
    int32_t hor, int32_t vert, 
    int32_t block_left, int32_t block_right,
    int32_t block_top, int32_t block_bottom)
{
    int mse = 0;

    for(int32_t j=-block_top; j<=block_bottom; j++)
    {
        uint8_t* psrc = src + (kpx-block_top+j)*stride + (kpy-block_left);
        uint8_t* pdst = dst + (vert-block_top+j)*stride + (hor-block_left);

        for(int32_t i=-block_left; i<=block_right; i++)
        {
            int src_square = (*psrc) * (*psrc);
            int dst_squre = (*pdst) * (*pdst);

            mse += abs(src_square - dst_squre);

            psrc++;
            pdst++;
        }
    }

    return mse;
}

void KeyPointMatch(
    uint8_t* im_src,
    const uint32_t xsize,
    const uint32_t ysize,
    const uint32_t stride,
    uint8_t* im_dst,
    int32_t block_left,
    int32_t block_right,
    int32_t block_top,
    int32_t block_bottom,
    int32_t range_left,
    int32_t range_right,
    int32_t range_top,
    int32_t range_bottom,
    int32_t* kp_x,
    int32_t* kp_y,
    int32_t kp_len,
    int32_t* out_x,
    int32_t* out_y,
    bool* out_valid)
{
    for(int i=0; i<kp_len; i++)
    {
        int hor_orig = kp_x[i];
        int vert_orig = kp_y[i];
        int hor_start = hor_orig-range_left;
        int hor_end = hor_orig+range_right;
        int vert_start = vert_orig-range_top;
        int vert_end = vert_orig+range_bottom;

        if(hor_start<=block_left||hor_end>xsize-block_right ||
           vert_start<=block_top||vert_end>ysize-block_bottom)
        {
            out_valid[i] = false;
            continue;
        }
        else
        {
            out_valid[i] = true;
        }

        int min = INT_MAX;
        int min_hor = -1;
        int min_vert = -1;
        uint8_t* src = im_src;
        uint8_t* dst = im_dst;

        for(int hor_scan=hor_start; hor_scan<hor_end; hor_scan++)
        {
            for(int vert_scan=vert_start; vert_scan<vert_end; vert_scan++)
            {
                int v = CalcMSE(
                    src, dst, stride, 
                    hor_orig, vert_orig,
                    hor_scan, vert_scan, 
                    block_left, block_right, block_top, block_bottom); 

                if(v<min)
                {
                    min = v;
                    min_hor = hor_scan;
                    min_vert = vert_scan;
                }
            }
        }

        out_x[i] = min_hor;
        out_y[i] = min_vert;
    }
}

