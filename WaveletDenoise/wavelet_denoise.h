#include "base_image.h"

class Band
{
public:
    enum BandIDType
    {
        LL, LH, HL, HH
    };

    Band(const size_t in_width, const size_t in_height)
        : orig_w(in_width), orig_h(in_height), 
          band_w(in_width/2), band_h(in_height/2),
          ll(NULL), lh(NULL), hl(NULL), hh(NULL), child(NULL)
    {
        ll = new Image(band_w, band_h);
        lh = new Image(band_w, band_h);
        hl = new Image(band_w, band_h);
        hh = new Image(band_w, band_h);
    }

    Image* GetImage(BandIDType id)
    {
        switch(id)
        {
            case LL: return ll;
            case LH: return lh;
            case HL: return hl;
            case HH: return hh;
        }
    }

public:
    Band* child;

private:
    size_t orig_w;
    size_t orig_h;
    size_t band_w;
    size_t band_h;

    Image* ll;
    Image* lh;
    Image* hl;
    Image* hh;
};

const static int low_analysis_numerator[] = {-1, 2, 6, 2, -1};
const static int low_analysis_denominator = 8;
const static size_t low_analysis_length = 5;
const static int low_analysis_start = -2;
const static int low_analysis_end = 2;
             
const static int low_synthesis_numerator[] = {1, 2, 1};
const static int low_synthesis_denominator = 2;
const static size_t low_synthesis_length = 3;
const static int low_synthesis_start = -1;
const static int low_synthesis_end = 1;

const static int high_analysis_numerator[] = {-1, 2, -1};
const static int high_analysis_denominator = 2;
const static size_t high_analysis_length = 3;
const static int high_analysis_start = -1;
const static int high_analysis_end = 1;

const static int high_synthesis_numerator[] = {-1, -2, 6, -2, -1};
const static int high_synthesis_denominator = 8;
const static size_t high_synthesis_length = 5;
const static int high_synthesis_start = -2;
const static int high_synthesis_end = 2;
             
class Kernel
{
public:
    enum Type
    {
        High,
        Low,
        InvLow,
        InvHigh
    };

    Kernel(Type ik)
        : k(ik)
    {
    }

    int Start() 
    {
        switch(k)
        {
            case High: return high_analysis_start;
            case Low:  return low_analysis_start;
            case InvLow: return low_synthesis_start;
            case InvHigh: return high_synthesis_start;
        }
    }

    int End() 
    {
        switch(k)
        {
            case High: return high_analysis_end;
            case Low:  return low_analysis_end;
            case InvLow: return low_synthesis_end;
            case InvHigh: return high_synthesis_end;
        }
    }

    int GetNumerator(const int32_t idx) 
    {
        switch(k)
        {
            case High: return high_analysis_numerator[idx];
            case Low:  return low_analysis_numerator[idx];
            case InvLow: return low_synthesis_numerator[idx];
            case InvHigh: return high_synthesis_numerator[idx];
        }
    }

    int GetDenominator() const
    {
        switch(k)
        {
            case High: return high_analysis_denominator;
            case Low:  return low_analysis_denominator;
            case InvLow: return low_synthesis_denominator;
            case InvHigh: return high_synthesis_denominator;
        }
    }

private:
    Type k;
};


class WaveletPyramid
{
public:
    WaveletPyramid(const Image* src)
        : base(NULL), source(src), 
          kernel_low(Kernel::Low), 
          kernel_high(Kernel::High),
          kernel_inv_low(Kernel::InvLow),
          kernel_inv_high(Kernel::InvHigh)
    {
    } 

    ReturnType Decompose()
    {
        base = Decompose(source);

        base->child = Decompose(base->GetImage(Band::LL));

        base->child->child = Decompose(base->child->GetImage(Band::LL));

        return SUCCESS;
    }

private:
    Band* Decompose(const Image* src)
    {
        Band* b = new Band(src->GetWidth(), src->GetHeight());

        const size_t w = src->GetWidth()/2;
        const size_t h = src->GetHeight();

        Image* tmp_vert_low = new Image(w, h);
        VerticalAnalysis(src, tmp_vert_low, kernel_low);
        tmp_vert_low->Write("VertLow");

        Image* tmp_vert_high = new Image(w, h);
        VerticalAnalysis(src, tmp_vert_high, kernel_high);
        tmp_vert_high->Write("VertHigh");

        Image* test = new Image(src->GetWidth(), src->GetHeight());
        VerticalSynthesis(
            tmp_vert_low, kernel_inv_low,
            tmp_vert_high, kernel_inv_high,
            test);

        test->Write("VertSyn");
        delete test;

        // Image* ll = b->GetImage(Band::LL);
        // HorizontalAnalysis(tmp_vert_low, ll, kernel_low);
        // ll->Write("VertLowHortLow");
        //
        // Image* hl = b->GetImage(Band::HL);
        // HorizontalAnalysis(tmp_vert_low, hl, kernel_high);
        // hl->Write("VertLowHortHigh");
        //
        // Image* lh = b->GetImage(Band::LH);
        // HorizontalAnalysis(tmp_vert_high, lh, kernel_low);
        // lh->Write("VertHighHortLow");
        //
        // Image* hh = b->GetImage(Band::HH);
        // HorizontalAnalysis(tmp_vert_high, hh, kernel_high);
        // hh->Write("VertHighHortHigh");

        delete tmp_vert_low;
        delete tmp_vert_high;

        return b;
    }

    ReturnType VerticalAnalysis(const Image* src, Image* dst, Kernel& k)
    {
        printf("VerticalAnalysis\n");
        printf("Src (w,h)=(%d, %d), Dst (w,h)=(%d,%d)\n", src->GetWidth(), src->GetHeight(), dst->GetWidth(), dst->GetHeight());

        printf("Start: %d, End: %d\n", k.Start(), k.End());

        for(int32_t j=0-k.Start(); j<src->GetHeight()-k.End(); j++)
        {
            for(int32_t i=0; i<src->GetWidth(); i+=2)
            {
                int32_t s = 0;

                int32_t idx = 0;
                for(int32_t t=k.Start(); t<=k.End(); t++)
                {
                    int32_t n = k.GetNumerator(idx);
                    idx++;
                    int32_t p = src->GetPixel(i, j+t);

                    s += n*p;
                }

                float denom = k.GetDenominator();

                s = s/(float)denom;

                dst->SetPixel(i/2, j, s);
            }
        }
        return SUCCESS;
    }

    ReturnType HorizontalAnalysis(const Image* src, Image* dst, Kernel& k)
    {
        printf("HorizontalAnalysis\n");
        printf("Src (w,h)=(%d, %d), Dst (w,h)=(%d,%d)\n", src->GetWidth(), src->GetHeight(), dst->GetWidth(), dst->GetHeight());

        printf("Start: %d, End: %d\n", k.Start(), k.End());

        for(int32_t j=0; j<src->GetHeight(); j+=2)
        {
            for(int32_t i=0-k.Start(); i<src->GetWidth()-k.End(); i++)
            {
                int32_t s = 0;

                int32_t idx = 0;
                for(int32_t t=k.Start(); t<=k.End(); t++)
                {
                    int32_t n = k.GetNumerator(idx);
                    idx++;
                    int32_t p = src->GetPixel(i+t, j);

                    s += n*p;
                }

                float denom = k.GetDenominator();

                s = s/(float)denom;

                dst->SetPixel(i, j/2, s);
            }
        }
        return SUCCESS;
    }

    ReturnType VerticalSynthesis(
        const Image* src_l, Kernel& al,
        const Image* src_h, Kernel& ah,
        Image* dst)
    {
        printf("VerticalSynthesis\n");
        printf("Src1(w,h)=(%d,%d), Src2(w,h)=(%d,%d), Dst(w,h)=(%d,%d)\n",
        src_l->GetWidth(), src_l->GetHeight(),
        src_h->GetWidth(), src_h->GetHeight(),
        dst->GetWidth(), dst->GetHeight());

        Image tl(src_l->GetWidth(), src_l->GetHeight());
        for(int32_t j=0-al.Start(); j<tl.GetHeight()-al.End(); j++)
        {
            for(int32_t i=0; i<tl.GetWidth(); i++)
            {
                int32_t s = 0;
                int32_t idx = 0;
                for(int32_t t=al.Start(); t<=al.End(); t++)
                {
                    int32_t n = al.GetNumerator(idx);
                    idx++;
                    int32_t p = src_l->GetPixel(i+t, j);
                    s += n*p;
                }
                float denom = al.GetDenominator();
                s = s/denom;
                tl.SetPixel(i, j, s);
            }
        }

        Image th(src_h->GetWidth(), src_h->GetHeight());
        for(int32_t j=0-ah.Start(); j<th.GetHeight()-ah.End(); j++)
        {
            for(int32_t i=0; i<th.GetWidth(); i++)
            {
                int32_t s = 0;
                int32_t idx = 0;
                for(int32_t t=ah.Start(); t<=ah.End(); t++)
                {
                    int32_t n = ah.GetNumerator(idx);
                    idx++;
                    int32_t p = src_h->GetPixel(i+t, j);
                    s+= n*p;
                }
                float denom = ah.GetDenominator();
                s = s/denom;
                th.SetPixel(i, j, s);
            }
        }

        for(int32_t j=0; j<dst->GetWidth(); j++)
        {
            for(int32_t i=0; i<dst->GetHeight(); i++)
            {
                if(i%2==0)
                    dst->SetPixel(i, j, tl.GetPixel(i/2, j));
                else
                    dst->SetPixel(i, j, th.GetPixel(i/2, j));
            }
        }

        return SUCCESS;
    }

private:
    const Image* source;
    Band* base;
    Kernel kernel_low;
    Kernel kernel_high;
    Kernel kernel_inv_low;
    Kernel kernel_inv_high;
};

