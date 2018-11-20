#pragma once
#include <complex>
#include <thread>
#include <future>
#include <mutex>
#include "complex.h"
using precision = double;
using Complex = Feng::complex<precision>;

const precision RMIN = -2, RMAX = 1, IMIN = -1, IMAX = 1;
const int W = 12*1024;
const precision RESN = W / (RMAX - RMIN);
const int H = (IMAX - IMIN) * RESN;
const int MI = 1;
const int SLICE = 32;
//static Complex z;
class Parallel {
public:
	static int Mandelbrot(Complex&& c) {
		Complex z;
		register int k;
		__m128d c128 = _mm_load_pd((const double*)(&c));

		for (k = 256 * MI - 1; k >= 0; --k) {
			z.squareAdd(c128);
			//z = z * z + c;
			if (Feng::norm(z) > 4) return k / MI;
		}
		return 0;
	}

	//static int Mandelbrot(Complex&& c) {
	//	Complex z;
	//	register int k;
	//	__m128d c128 = _mm_load_pd((const double*)(&c));
	//	__m128d a128 = _mm_setzero_pd();
	//	__m128d b128 = _mm_setzero_pd();
	//	return Feng::squareAdd(a128, b128, c128, 255);
	//	return 0;
	//}

	template<typename Iterator>
	static int Mandelbrot(Iterator first,const int W, const int index, const int h_num) {
		Iterator it = first;
		for (int h = 0; h < h_num; h++) {
			auto l_index = index + h;
			for (int i = 0; i < W; it++, i++) {

				*it = Mandelbrot(Complex(RMIN + i / RESN, IMIN + l_index / RESN));

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
			std::future<int> first_half = std::async(std::launch::async | std::launch::deferred,&Mandelbrot_pall<Iterator>, first, mid, W, half_length, index);
			Mandelbrot_pall<Iterator>(mid, last, W, H - half_length, index + half_length);

		}
	}

};
