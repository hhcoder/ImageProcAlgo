// Practice: MSE-based block matching, user can specify block size and search range

// Commandline Usage:
//  TBD

#include <stdio.h>
#include <stdlib.h>

void BlockMatching(
    const Image* src_img, 
    const Image* dst_img,
    vector<loc_pair>& src_points,
    vector<loc_pair>& dst_points,
    const MatchingConfig& cfg)
{
    for(size_t i=0; i<src_points.length(); i++)
    {
        dst_points[i] = FindPointMatch(src_img, dst_img, src_points[i], cfg);
    }
}

loc_pair FindPointMatch(
    const Image* src_img,
    const Image* dst_img,
    const loc_pair& src_loc,
    const MatchingConfig& cfg)
{
    int32_t min_mse = INT_MAX;
    uint32_t min_x = INT_MAX;
    uint32_t min_y = INT_MAX;
    for(size_t j=src_loc.x-cfg.hor_range/2; j<=src_loc.x+cfg.hor_range/2; j++)
    {
        for(size_t i=src_loc.x-cfg.ver_range/2; i<=src_loc.x+cfg.ver_range/2; i++)
        {
            loc_pair dst_loc(i, j);
            int32_t v = BlockMSE(src_img, dst_img, src_loc, dst_loc, cfg.block_size);
            if(v < min_mse)
            {
                min_mse = v;
                min_x = i;
                min_y = j;
            }
        }
    }

    loc_pair result(i, j);

    return result;
}

int32_t BlockMSE(
    const Image* src_img, 
    const Image* dst_img, 
    const loc_pair& src_loc, 
    const loc_pair& dst_loc, 
    const uint32_t block_size)
{
    uint32_t mse = 0;

    uint8_t* psrc = src_img.GetPtr(src_loc.x - block_size/2, src_loc.y - block_size/2);
    uint8_t* pdst  = dst_img.GetPtr(dst_loc.x - block_size/2, dst_loc.y - block_size/2);

    for(uint32_t i=0; i<block_size; i++)
    {
        for(uint32_t j=0; j<block_size; j++)
        {
            mse += abs((*psrc) * (*psrc) - (*pdst) * (*pdst))
        }
    }

    return mse;
}

int main(int argc, char* argv[])
{
    return 0;
}
