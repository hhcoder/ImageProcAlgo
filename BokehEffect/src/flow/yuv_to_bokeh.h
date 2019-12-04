#pragma once

#include <stdint.h>

#include "../../../dual_pd_core/include/common/dpd_dim.h"

#include "../../../dual_pd_core/include/common/dpd_buffer.h"

#include "../proc/box_filter/box_filter.h"

class FlowYuvToBokehYuv
{
public:
struct Info
{
	int32_t yuv_width;
	int32_t yuv_height;
	int32_t y_stride;
	int32_t uv_stride;
	int32_t dense_width;
	int32_t dense_height;
};

public:
	FlowYuvToBokehYuv()
		: box_filter(NULL), 
		filtered_y(NULL),
		merged_y(NULL)
	{
	}

	~FlowYuvToBokehYuv()
	{
		if(NULL!=filtered_y)
			delete filtered_y;

		if(NULL!=merged_y)
			delete merged_y;
	}

	DimWH Config(const Info& in)
	{
		info = in;

		const int32_t box_width = 20;
		const int32_t box_height = 20;

		box_filter = new Filter2D::BoxFilterSrc2Dst(
			info.yuv_width, info.yuv_height, info.y_stride,
			box_width, box_height);

		filtered_y = new Internal2DBuffer<uint8_t>(info.yuv_width, info.yuv_height);

		merged_y = new External2DBuffer<uint8_t>(info.yuv_width, info.yuv_height, info.yuv_width);

		return DimWH(info.yuv_width, info.yuv_height);
	}

	void Process(uint8_t* yuv_src_dst, const uint8_t* dense_src)
	{
		box_filter->ProcY(yuv_src_dst, filtered_y->GetBuf());

		filtered_y->DumpInfo(std::string("blurry_y"));

		const float target_width = 112;
		const float target_height = 112;
		
		const float hor_scale = info.yuv_width / target_width;
		const float vert_scale = info.yuv_height / target_height;
		Merge(yuv_src_dst, dense_src, hor_scale, vert_scale);

		merged_y->SetExternalBuffer(yuv_src_dst);
		merged_y->DumpInfo(std::string("merged_y"));
	}

private:
	void Merge(uint8_t* yuv_src_dst, const uint8_t* dense_src, const float hor_scale, const float vert_scale)
	{
		for (int32_t j = 0; j < info.yuv_height; j++)
		{
			const uint32_t j_dense = static_cast<uint32_t>(std::floor(j / vert_scale));

			for (int32_t i = 0; i < info.yuv_width; i++)
			{
				uint8_t* p_dst = yuv_src_dst + j * info.y_stride + i;

				const uint32_t i_dense = static_cast<uint32_t>(std::floor(i / hor_scale));

				const uint8_t pixel_clear = *p_dst;
#define MASK_MODE 0
#if MASK_MODE 
				const uint8_t pixel_blur = 0; 
#else
				const uint8_t pixel_blur = *(filtered_y->GetBuf() + j * filtered_y->Stride() + i);
#endif
				const uint8_t pixel_far = *(dense_src + j_dense * info.dense_width + i_dense);
				const uint8_t pixel_near = 255 - pixel_far;

				const uint32_t pixel_merged = pixel_clear * pixel_near + pixel_blur * pixel_far;
				const uint32_t pixel_shifted = 0xFF & (pixel_merged >> 8);

				*p_dst = static_cast<uint8_t>(pixel_shifted);
			}
		}
	}

private:
	Info info;
	Filter2D::BoxFilterSrc2Dst* box_filter;
	Internal2DBuffer<uint8_t>* filtered_y;
	External2DBuffer<uint8_t>* merged_y;
};