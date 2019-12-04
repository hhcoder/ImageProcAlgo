/*
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "src\proc\box_filter\box_filter.h"

int main(int argc, char* argv[])
{
	const char* in_depth_map_name = argv[1];
	const int32_t in_depth_map_width = atoi(argv[2]);
	const int32_t in_depth_map_height = atoi(argv[3]);
	const char* in_yuv_name = argv[4];
	const int32_t in_yuv_width = atoi(argv[5]);
	const int32_t in_yuv_height = atoi(argv[6]);
	const char* out_bokeh_name = argv[7];
	
	const int32_t out_width = in_yuv_width;
	const int32_t out_height = in_yuv_height;

	const size_t in_depth_map_size = sizeof(uint8_t) * in_depth_map_width * in_depth_map_height;
	const size_t in_yuv_size = sizeof(uint8_t) * in_yuv_width * in_yuv_height + sizeof(uint8_t) * in_yuv_width * in_yuv_height / 2;
	const size_t out_size = in_yuv_size;

	void* in_depth_map_data = malloc(in_depth_map_size);
	if (NULL == in_depth_map_data)
	{
		printf("\n Error allocating raw data buffer!\n");
		return -1;
	}

	void* in_yuv_data = malloc(in_yuv_size);
	if (NULL == in_yuv_data)
	{
		printf("\n Error allocating yuv data buffer!\n");
		return -1;
	}

	FILE* fp_raw_in = fopen(in_depth_map_name, "rb");
	if (NULL == fp_raw_in)
	{
		printf("\n Error opening %s! \n", in_depth_map_name);
		return -1;
	}

	if (in_depth_map_size != fread(in_depth_map_data, sizeof(uint8_t), in_depth_map_size, fp_raw_in) )
	{
		printf("\n Error reading %s!\n", in_depth_map_name);
		fclose(fp_raw_in);
		return -1;
	}

	fclose(fp_raw_in);

	FILE* fp_yuv_in = fopen(in_yuv_name, "rb");
	if (NULL == fp_yuv_in)
	{
		printf("\n Error opening %s! \n", in_yuv_name);
		return -1;
	}

	if (in_yuv_size != fread(in_yuv_data, sizeof(uint8_t), in_yuv_size, fp_yuv_in) )
	{
		printf("\n Error reading %s!\n", in_yuv_name);
		fclose(fp_yuv_in);
		return -1;
	}

	fclose(fp_yuv_in);

	uint8_t* out_bokeh_data = (uint8_t*)malloc(out_size);
	if (NULL == out_bokeh_data)
	{
		printf("\n Error allocating dmap data buffer!\n");
		return -1;
	}

	printf("\n Initialize\n");

	printf("\n Start Processing... \n");

	memcpy(out_bokeh_data, in_yuv_data, out_size);

	const int32_t box_width = 16;
	const int32_t box_height = 16;

#if 0
	Filter2D::BoxFilterInplace box_filter(
		out_width, 
		300, 
		out_width,
		box_width, 
		box_height);

	box_filter.ProcY(out_bokeh_data);

#else
	Filter2D::BoxFilterSrc2Dst box_filter(
		out_width, 
		300, 
		out_width,
		box_width, 
		box_height);

	box_filter.ProcY((uint8_t*)in_yuv_data, out_bokeh_data);

#endif

	FILE* fp_dmap = fopen(out_bokeh_name, "wb");
	if (NULL == fp_dmap)
	{
		printf("\n Error opening %s\n", out_bokeh_name);
		return -1;
	}
	if (out_size != fwrite(out_bokeh_data, sizeof(uint8_t), out_size, fp_dmap))
	{
		printf("\n Error writing %s\n", out_bokeh_name);
		fclose(fp_dmap);
		return -1;
	}
	fclose(fp_dmap);

	printf("\n ... Finish Processing \n");

	free(out_bokeh_data);

	free(in_yuv_data);

	free(in_depth_map_data);

	return 0;
}
*/