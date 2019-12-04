#include "y_sad.h"
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <fstream>

template <typename T>
    class Buf2D
{
public:
    Buf2D(std::string& fname, int in_w, int in_h)
        : w(in_w), h(in_h),
        buf(new T[w*h])
    {
        std::ifstream fid(fname.c_str(), std::ios::binary);
        fid.read((char*)buf, sizeof(T)*w*h);
    }

    ~Buf2D()
    {
        delete[] buf;
    }
public:
    int w;
    int h;
    T* buf;
};

int main(int argc, const char* argv[])
{
    std::string ref_fname(argv[1]);
    std::string next_fname(argv[2]);
    const int width(std::atoi(argv[3]));
    const int height(std::atoi(argv[4]));
    std::string out_txt_fname(argv[5]);

    try 
    {
        Buf2D<uint8_t> ref(ref_fname, width, height);
        Buf2D<uint8_t> next(next_fname, width, height);

        YSad sad(width, height);
        sad.Proc(ref.buf, next.buf);

        sad.Write(out_txt_fname);
    } 
    catch (std::ofstream::failure e)
    {
        std::cerr << "Exception in reading";
        return 1;
    }

    return 0;
}