#include "../public/dpd_bokeh_lib.h"

#include "flow\yuv_to_bokeh.h"

BokehCtrl::BokehCtrl()
	: yuv_to_bokeh(NULL)
{
}

uint32_t BokehCtrl::initialize(dpdsys_bokeh_init_t * in)
{
	yuv_to_bokeh = new FlowYuvToBokehYuv();

	FlowYuvToBokehYuv::Info info;
	info.yuv_width = in->yuv_width;
	info.yuv_height = in->yuv_height;
	info.y_stride = in->y_stride;
	info.uv_stride = in->uv_stride;
	info.dense_width = in->dense_width;
	info.dense_height = in->dens_height;

	yuv_to_bokeh->Config(info);

	return DPDSYSTEM_RESULT_SUCCESS;
}

uint32_t BokehCtrl::Process(dpdsys_bokeh_result_t * out)
{
	yuv_to_bokeh->Process(
		static_cast<uint8_t*>(out->yuv_src_dst), 
		static_cast<const uint8_t*>(out->src_dense_depth_map));

	return DPDSYSTEM_RESULT_SUCCESS;
}

BokehCtrl::~BokehCtrl()
{
}
