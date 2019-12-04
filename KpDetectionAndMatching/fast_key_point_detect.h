#ifndef _FAST_KEY_POINT_DETECTION_H_
#define _FAST_KEY_POINT_DETECTION_H_

void fast10_detect(
	const uint8_t* im, 
	uint32_t xsize, 
	uint32_t ysize, 
	uint32_t stride, 
	int b, 
    uint32_t max_count,
    int32_t* out_x,
    int32_t* out_y,
    int32_t* out_len
	);

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
    bool* out_valid);

#endif //_FAST_KEY_POINT_DETECTION_H_
