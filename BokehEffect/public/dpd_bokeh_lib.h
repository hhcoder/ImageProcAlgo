#pragma once

#include "../../dual_pd_api/public/dualpd_system.h"

class FlowYuvToBokehYuv;

class BokehCtrl
{
public:
	BokehCtrl();

	uint32_t initialize(dpdsys_bokeh_init_t* in);

	uint32_t Process(dpdsys_bokeh_result_t* out);

	~BokehCtrl();

private:
	FlowYuvToBokehYuv* yuv_to_bokeh;
};