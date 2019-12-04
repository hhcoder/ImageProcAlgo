//Practice: Histogram matching between 2 images

// Command line usage: 
//  a.out ../source_images/histogram_matching_ref.y ../source_images/histogram_matching_target.y 640 480 ../processed_results/histogram_matching.y

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <float.h>

bool ReadFromFile(unsigned char* in_src, const size_t in_size, const char* in_fname)
{
    FILE* fp = fopen(in_fname, "rb");
    if(NULL==fp)
        return false;
    fread(in_src, sizeof(unsigned char), in_size, fp);
    fclose(fp);
}

bool WriteToFile(unsigned char* in_src, const size_t in_size, const char* in_fname)
{
    FILE* fp = fopen(in_fname, "wb");
    if(NULL==fp)
        return false;
    fwrite(in_src, sizeof(unsigned char), in_size, fp);
    fclose(fp);
}

static void ComputeHistogram(const uint8_t *in,
                 const uint32_t width,
                 const uint32_t height,
                 const uint32_t stride,
                 uint32_t* hist,
                 const uint32_t num_levels)
{
	memset(hist, 0, sizeof(uint32_t)*num_levels);

	for (uint32_t i=0; i<height; i++)
	{
		for (uint32_t j=0; j<width; j++)
		{
			hist[in[i*stride+j]]++;
		}
	}
}

static void ComputeCdfFromHistogram(const uint32_t* hist, float* cdf, const uint32_t num_levels)
{
	// CDF, not normalized
    cdf[0] = hist[0];
	for (uint32_t i=1; i<num_levels; i++)
	{
        cdf[i]  = hist[i];
		cdf[i]  = cdf[i] + cdf[i-1];
	}
}

static int ComputeMatchingLut(const float    *src_cdf,
                       const float    *dst_cdf,
                       uint32_t *curr_hist,
                       uint8_t* match_lut,
                       const uint32_t dst_width,
                       const uint32_t dst_height,
	                   const uint32_t num_levels)
{
    const float sqrt_eps = 0.0000000149011611938476560f;
    const float err_threshold = -sqrt_eps * dst_width * dst_height;

    curr_hist[0] = curr_hist[num_levels-1] = 0;

	// NOTE: histogram equalization is based on Matlab's histeq() function
	for(uint32_t i=0; i<num_levels; i++)
	{
        uint32_t min_idx = 0;
        float min_err = FLT_MAX;

		for(uint32_t j=0;j<num_levels; j++) 
        {
            float curr_err = (src_cdf[j]-dst_cdf[i]+(curr_hist[i]/2.0f));

            if (curr_err >= err_threshold && curr_err <= min_err) 
            {
                min_err = curr_err;
                min_idx = j;
			}
		}
		match_lut[i] = min_idx;
	}
}

static void ApplyLut(uint8_t *in,
                     const uint32_t width,
                     const uint32_t height,
                     const uint32_t stride,
                     const uint8_t  *match_lut)
{
	// match histogram via LUT
	for (uint32_t i=0; i<height; i++)
	{
		for (uint32_t j=0; j<width; j++)
		{
			in[i*stride+j] = match_lut[in[i*stride+j]];
		}
	}
}

int MatchHistogram(const uint8_t* src_buf,
                     const uint32_t src_width,
                     const uint32_t src_height,
                     const uint32_t src_stride,
                     uint8_t* dst_buf,
                     const uint32_t dst_width,
                     const uint32_t dst_height,
                     const uint32_t dst_stride)
{
    const size_t num_levels = 256;

    // 1. Build histogram of reference image. Then build normalized CDF Cr.
    uint32_t src_hist[num_levels];
    ComputeHistogram(src_buf, src_width, src_height, src_stride, &src_hist[0], num_levels);

    float src_cdf[num_levels];
    ComputeCdfFromHistogram(&src_hist[0], &src_cdf[0], num_levels);

    // // 2. Build histogram of given image. Then build normalized CDF Ci.
    uint32_t dst_hist[num_levels];
    ComputeHistogram(dst_buf, dst_width, dst_height, dst_stride, &dst_hist[0], num_levels);

    float dst_cdf[num_levels];
    ComputeCdfFromHistogram(&dst_hist[0], &dst_cdf[0], num_levels);

    // 3. Create look-up table such that Cr[j] == Ci[i] 
	uint8_t match_lut[num_levels];
    ComputeMatchingLut(&src_cdf[0], &dst_cdf[0], &dst_hist[0], &match_lut[0], 
        dst_width, dst_height, num_levels);
    
    // 4. Step through given image pixels. Replace Ci[i] with j such that Cr[j] == Ci[i]
    ApplyLut(dst_buf, dst_width, dst_height, dst_stride, &match_lut[0]);
                                                    
    return 0;
}

int main(int argc, char* argv[])
{
    const char* in_src_fname = argv[1];
    const size_t in_width = atoi(argv[2]);
    const size_t in_height = atoi(argv[3]);
    const char* in_dst_fname = argv[4];
    const char* out_fname = argv[5];
    const size_t in_size = in_width*in_height;
    
    unsigned char* src = (unsigned char*)malloc(in_size);
    unsigned char* dst = (unsigned char*)malloc(in_size);
   
    ReadFromFile(src, in_size, in_src_fname); 
    ReadFromFile(dst, in_size, in_dst_fname);

    MatchHistogram(src, in_width, in_height, in_width,
                   dst, in_width, in_height, in_width);

    WriteToFile(dst, in_size, out_fname);

    free(src);
    free(dst);
}
