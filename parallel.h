#pragma once
#include <complex>
#include <thread>
#include <future>
#include <mutex>
using complex = std::complex<double>;

const double RMIN = -2, RMAX = 1, IMIN = -1, IMAX = 1;
const int W = 12*1024;
const double RESN = W / (RMAX - RMIN);
const int H = (IMAX - IMIN) * RESN;
const int MI = 1;
const int SLICE = 32;
class Parallel {
public:
	static int Mandelbrot(complex&& c) {
		complex z;
		for (int k = 256 * MI - 1; k >= 0; --k) {
			z = z * z + c;
			if (std::norm(z) > 4) return k / MI;
		}
		return 0;
	}
	template<typename Iterator>
	static int Mandelbrot(Iterator first,const int W, const int index, const int h_num) {
		Iterator it = first;
		for (int h = 0; h < h_num; h++) {
			auto l_index = index + h;
			for (int i = 0; i < W; it++, i++) {
				*it = Mandelbrot(complex{ RMIN + i / RESN, IMIN + l_index / RESN });
			}
		}

		return 0;
	}
	template<typename Iterator> 
	static int Mandelbrot_pall(Iterator first, Iterator last, const int W, int H, const int index) {
		const unsigned long length = std::distance(first, last) ;
		if (!length)
			return 0;
		if (length <= W * SLICE)
		{
			Mandelbrot(first, W, index, SLICE);
			return 0;
		}
		else {
			int half_length = H / 2;
			Iterator mid = first + half_length * W;
			std::future<int> first_half = std::async(&Mandelbrot_pall<Iterator>, first, mid, W, half_length, index);
			Mandelbrot_pall<Iterator>(mid, last, W, H - half_length, index + half_length);

		}
	}

};
