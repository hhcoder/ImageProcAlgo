#pragma once

#include <stdint.h>
#include <vector>

namespace Filter2D
{
class LineBuf 
{
public:
	LineBuf(const int32_t in_width, const int32_t in_height)
		: len(in_height), width(in_width), buf_2d(len),
		head(0), tail(0), is_empty(true)
	{
		for (size_t i = 0; i < buf_2d.size(); i++)
			buf_2d[i] = new uint8_t[width];
	}

	~LineBuf()
	{
		for (size_t i = 0; i < buf_2d.size(); i++)
			delete[] buf_2d[i];
	}

	void Attach(uint8_t* next_row)
	{
		if (IsFull())
		{
			return;
		}
		else
		{
			memcpy(buf_2d[head], next_row, sizeof(uint8_t)*width);
			head = (head + 1) % len;
			is_empty = false;
		}
	}

	uint8_t* Remove()
	{
		if (IsEmpty())
			return NULL;

		int32_t ret = tail;
		tail = (tail + 1) % len;

		if (tail == head)
			is_empty = true;

		return buf_2d[ret];
	}

private:
	bool IsEmpty() const { return is_empty; }
	bool IsFull() const { return (!is_empty && head == tail); }

private:
	int32_t len;
	int32_t width;
	std::vector<uint8_t*> buf_2d;

private:
	int32_t head;
	int32_t tail;
	bool is_empty;
};

template <typename T>
	class BoxAvg
{
public:
	BoxAvg(const int32_t in_im_width, const int32_t in_im_height, const int32_t in_width, const int32_t in_height)
		: im_width(in_im_width), im_height(in_im_height), 
		width(in_width), height(in_height),
		vert_sum(im_width), hori_sum(im_width)
	{
		for (std::vector<typename T>::iterator i=vert_sum.begin(); i!=vert_sum.end(); i++)
			*i = 0;
	}

	void AddRow(const uint8_t* in_row)
	{
		const uint8_t* p = in_row;
		for (std::vector<typename T>::iterator i=vert_sum.begin(); i!=vert_sum.end(); i++)
		{
			*i += (*p);
			p++;
		}
	}

	void SubstractRow(const uint8_t* in_row)
	{
		const uint8_t* p = in_row;
		for (std::vector<typename T>::iterator i=vert_sum.begin(); i!=vert_sum.end(); i++)
		{
			*i -= (*p);
			p++;
		}
	}

	void Set(uint8_t* dst_row)
	{
		HorizontalSum();

		uint8_t* dst = dst_row;

		using Iterator = std::vector<typename T>::iterator;
		for (Iterator i=hori_sum.begin(); i!=hori_sum.end(); i++)
		{
			*dst = static_cast<uint8_t>(*i); 
			dst++;
		}
	}

	int32_t VertSrcTail() const { return 0; }
	int32_t VertSrcHead() const { return height; }
	int32_t VertDstStart() const { return (height >> 1); }
	int32_t VertDstEnd() const { return im_height - (height >> 1); }

private:
	int32_t HorSrcHead() const { return width; }
	int32_t HorSrcTail() const { return 0; }
	int32_t HorDstStart() const { return (width >> 1); }
	int32_t HorDstEnd() const { return im_width - (width >> 1); }

private:
	void HorizontalSum()
	{
		using Iterator = std::vector<typename T>::iterator;
		//typedef std::vector<typename T>::iterator Iterator;

		T s = 0;

		// Fill in the first part
		std::vector<typename T>::iterator src_init = vert_sum.begin();
		for (int32_t i = HorSrcTail(); i < HorSrcHead(); i++)
		{
			s += *src_init;
			src_init++;
		}

		const uint16_t dividor = width * height;
		const int32_t half_width = width >> 1;

		Iterator src_tail = vert_sum.begin() + HorSrcTail();
		Iterator src_head = vert_sum.begin() + HorSrcHead();
		Iterator  dst = hori_sum.begin() + HorDstStart();
		for (int32_t i = HorDstStart(); i < HorDstEnd(); i++)
		{
			*dst = static_cast<T>(std::floor(s/dividor));

			s -= *src_tail;
			s += *src_head;

			src_head++;
			src_tail++;
			dst++;
		}

		for (int32_t i = 0; i < HorDstStart(); i++)
		{
			hori_sum[i] = static_cast<uint16_t>(std::floor(vert_sum[i] / height));
		}

		for (int32_t i = HorDstEnd(); i < im_width; i++)
		{
			hori_sum[i] = static_cast<uint16_t>(std::floor(vert_sum[i] / height));
		}
	}

private:
	int32_t im_width;
	int32_t im_height;
	int32_t width;
	int32_t height;
	std::vector<T> vert_sum;
	std::vector<T> hori_sum;
private:
};

class BoxFilterBase
{
public:
	BoxFilterBase(
		const int32_t in_im_width,
		const int32_t in_im_height,
		const int32_t in_im_stride,
		const int32_t in_box_width,
		const int32_t in_box_height)
		: im_width(in_im_width), im_height(in_im_height), im_stride(in_im_stride),
		box_width(in_box_width), box_height(in_box_height),
		box_avg(im_width, im_height, box_width, box_height)
	{
	}

	~BoxFilterBase()
	{
	}

protected:
	int32_t im_width;
	int32_t im_height;
	int32_t im_stride;
	int32_t box_width;
	int32_t box_height;

protected:
	BoxAvg<uint32_t> box_avg;
};

class BoxFilterInplace : public BoxFilterBase
{
public:
	BoxFilterInplace(
		const int32_t in_im_width,
		const int32_t in_im_height,
		const int32_t in_im_stride,
		const int32_t in_box_width,
		const int32_t in_box_height)
		: BoxFilterBase(in_im_width, in_im_height, in_im_stride, in_box_width, in_box_height),
		cache(in_im_width, in_box_height)
	{
	}

	~BoxFilterInplace()
	{
	}

public:
	void ProcY(uint8_t* src_dst)
	{
		const int32_t src_tail = box_avg.VertSrcTail();
		const int32_t src_head = box_avg.VertSrcHead();
		const int32_t dst_start = box_avg.VertDstStart();
		const int32_t dst_end = box_avg.VertDstEnd();

		uint8_t* init_row = src_dst;
		for (int32_t i = src_tail; i < src_head; i++)
		{
			box_avg.AddRow(init_row);

			cache.Attach(init_row);

			init_row += im_stride;
		}

		uint8_t* src_row_head = src_dst + (src_head * im_stride);
		uint8_t* src_row_tail = NULL;
		uint8_t* dst_row = src_dst + (dst_start * im_stride);
		for (int32_t i = dst_start; i < dst_end; i++)
		{
			box_avg.Set(dst_row);

			src_row_tail = cache.Remove();

			box_avg.SubstractRow(src_row_tail);

			box_avg.AddRow(src_row_head);

			cache.Attach(src_row_head);

			src_row_head += im_stride;
			dst_row += im_stride;
		}
	}

private:
	LineBuf cache;
};

class BoxFilterSrc2Dst : public BoxFilterBase
{
public:
	BoxFilterSrc2Dst(
		const int32_t in_im_width,
		const int32_t in_im_height,
		const int32_t in_im_stride,
		const int32_t in_box_width,
		const int32_t in_box_height)
		: BoxFilterBase(in_im_width, in_im_height, in_im_stride, in_box_width, in_box_height)
	{
	}

	void ProcY(uint8_t* src, uint8_t* dst)
	{
		const int32_t src_tail = box_avg.VertSrcTail();
		const int32_t src_head = box_avg.VertSrcHead();
		const int32_t dst_start = box_avg.VertDstStart();
		const int32_t dst_end = box_avg.VertDstEnd();

		uint8_t* init_row = src;
		for (int32_t i = src_tail; i < src_head; i++)
		{
			box_avg.AddRow(init_row);

			init_row += im_stride;
		}

		uint8_t* src_row_head = src + (src_head * im_stride);
		uint8_t* src_row_tail = src + (src_tail * im_stride);
		uint8_t* dst_row = dst + (dst_start * im_stride);
		for (int32_t i = dst_start; i < dst_end; i++)
		{
			box_avg.Set(dst_row);

			box_avg.SubstractRow(src_row_tail);

			box_avg.AddRow(src_row_head);

			src_row_head += im_stride;
			src_row_tail += im_stride;
			dst_row += im_stride;
		}
	}
	~BoxFilterSrc2Dst()
	{
	}
};

};
