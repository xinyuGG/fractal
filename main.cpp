//#include "fractal.h"
//#include "parallel.h"
//using namespace std::chrono;
//using namespace Feng;
//int main() {
//	Bmp256 bmp{ W, H };
//	int num = 1;
//	auto t1 = steady_clock::now();
//
//	for (int i = 0; i < num; i++) {
//
//		//for (int j = 0; j < H; ++j)
//		//	for (int i = 0; i < W; ++i)
//		//		bmp(j, i) = Parallel::Mandelbrot(complex{ RMIN + i / RESN, IMIN + j / RESN });
//
//		Parallel::Mandelbrot_pall<uint8_t*>(bmp.begin(), bmp.end(), W, H, 0);
//	}
//
//	auto t2 = steady_clock::now();
//
//	bmp.save("Mandelbrot12k.bmp");
//	std::cout << "run time: " << duration_cast<duration<double>>(t2 - t1).count() / (double)num << " seconds.\n";
//
//
//	//int num = 80000000;
//	//auto t1 = steady_clock::now();
//	//for (int i = 0; i < num; i++) {
//	//	Feng::complex<double> a(rand(), rand()), b(rand(), rand());
//	//	auto c = a.squareAdd(b);
//
//	//}
//	//auto t2 = steady_clock::now();
//	//std::cout << "run time: " << duration_cast<duration<double>>(t2 - t1).count()  << " seconds.\n";
//
//	//auto t3 = steady_clock::now();
//	//for (int i = 0; i < num; i++) {
//	//	Feng::complex<double> a(rand(), rand()), b(rand(), rand());
//
//	//	auto d = a* a + b;
//
//
//	//}
//	//auto t4 = steady_clock::now();
//	//std::cout << "run time: " << duration_cast<duration<double>>(t4 - t3).count()<< " seconds.\n";
//
//
//
//	system("pause");
//}
//
//
//
//
//
//
////#include <immintrin.h>
////#include <iostream>
////
//////#ifdef __AVX__
//////
//////
//////#else
//////#define VEC_SIZE 2
//////typedef __m128d Vec;
//////#define VEC_INIT(value) (Vec){value,value}
//////#endif
////
////#define VEC_SIZE 4
////typedef __m256d Vec;
////#define VEC_INIT(value) (Vec){value,value,value,value}
////
////#define LOOP_SIZE (8/VEC_SIZE)
////using namespace std;
////
////int numDigits(long n)
////{
////	auto len = 0;
////	while (n)
////	{
////		n = n / 10;
////		len++;
////	}
////	return len;
////}
////
////// Return true iff any of 8 members of double vector v is 
////// less than or equal to f.
////bool vec_le(double *v, double f)
////{
////	return (
////		v[0] <= f ||
////		v[1] <= f ||
////		v[2] <= f ||
////		v[3] <= f ||
////		v[4] <= f ||
////		v[5] <= f ||
////		v[6] <= f ||
////		v[7] <= f);
////}
////
////// Return 8 bit value with bits set iff cooresponding 
////// member of double vector v is less than or equal to f.
////int8_t pixels(double *v, double f)
////{
////	int8_t res = 0;
////	if (v[0] <= f) res |= 0b10000000;
////	if (v[1] <= f) res |= 0b01000000;
////	if (v[2] <= f) res |= 0b00100000;
////	if (v[3] <= f) res |= 0b00010000;
////	if (v[4] <= f) res |= 0b00001000;
////	if (v[5] <= f) res |= 0b00000100;
////	if (v[6] <= f) res |= 0b00000010;
////	if (v[7] <= f) res |= 0b00000001;
////	return res;
////}
////
//////
////// Do one iteration of mandelbrot calculation for a vector of eight 
////// complex values.  Using Vec to work with groups of doubles speeds
////// up computations.
//////
////inline void calcSum(double *r, double *i, double *sum, double const *init_r, Vec&  init_i)
////{
////	auto r_v = (double*)r;
////	auto i_v = (double*)i;
////	auto sum_v = (double*)sum;
////	auto init_r_v = (double const *)init_r;
////
////	for (int vec = 0; vec<LOOP_SIZE; vec++)
////	{
////		auto r2 = r_v[vec] * r_v[vec];
////		auto i2 = i_v[vec] * i_v[vec];
////		auto ri = r_v[vec] * i_v[vec];
////
////		sum_v[vec] = r2 + i2;
////
////		r_v[vec] = r2 - i2 + init_r_v[vec];
////		i_v[vec] = ri + ri + i[0];
////	}
////}
////
//////
////// Do 50 iterations of mandelbrot calculation for a vector of eight 
////// complex values.  Check occasionally to see if the iterated results
////// have wandered beyond the point of no return (> 4.0).
//////
////inline int8_t mand8(double *init_r, double iy)
////{
////	double r[8], i[8], sum[8];
////	for (auto k = 0; k<8; k++)
////	{
////		r[k] = init_r[k];
////		i[k] = iy;
////	}
////
////	auto init_i = __m256d{iy, iy, iy,iy};
////
////	int8_t pix = 0xff;
////
////	for (auto j = 0; j < 10; j++)
////	{
////		for (auto k = 0; k<5; k++)
////			calcSum(r, i, sum, init_r, init_i);
////
////		if (!vec_le(sum, 4.0))
////		{
////			pix = 0x00;
////			break;
////		}
////	}
////	if (pix)
////	{
////		pix = pixels(sum, 4.0);
////	}
////
////	return pix;
////}
////
////int main(int argc, char ** argv)
////{
////	// get width/height from arguments
////
////	//auto wid_ht = 16000;
////	//if (argc >= 2)
////	//{
////	//	wid_ht = atoi(argv[1]);
////	//}
////
////	// round up to multiple of 8
////	const int wid_ht = (16000 + 7) & ~7;
////
////	// allocate memory for pixels.
////	auto dataLength = wid_ht * (wid_ht >> 3);
////	auto pixels = new char[dataLength];
////
////	// calculate initial x values, store in r0
////	double r0[wid_ht];
////	for (auto x = 0; x<wid_ht; x++)
////	{
////		r0[x] = 2.0 / wid_ht * x - 1.5;
////	}
////
////	// generate the bitmap
////
////	// process 8 pixels (one byte) at a time    
////#pragma omp parallel for schedule(guided)
////	for (auto y = 0; y<wid_ht; y++)
////	{
////		// all 8 pixels have same y value (iy).
////		auto iy = 2.0 / wid_ht * y - 1.0;
////		auto rowstart = y * wid_ht / 8;
////		for (auto x = 0; x<wid_ht; x += 8)
////		{
////			pixels[rowstart + x / 8] = mand8(r0 + x, iy);
////		}
////	}
////
////	// write the data
////	cout << "P4\n" << wid_ht << " " << wid_ht << "\n";
////	cout.write(pixels, dataLength);
////	delete[] pixels;
////
////	return 0;
////}
#define max(a, b )  a>b? a:b
#include <cstdio>
#include <cstdlib>
#include <vector>

typedef unsigned char Byte;

using namespace std;

int main(int argc, char* argv[])
{
	const int    N = max(0, (argc > 1) ? atoi(argv[1]) : 0);
	const int    width = N;
	const int    height = N;
	const int    max_x = (width + 7) / 8;
	const int    max_iterations = 50;
	const double limit = 2.0;
	const double limit_sq = limit * limit;

	vector<Byte> buffer(height * max_x);

	vector<double> cr0(8 * max_x);
#pragma omp parallel for
	for (int x = 0; x < max_x; ++x)
	{
		for (int k = 0; k < 8; ++k)
		{
			const int xk = 8 * x + k;
			cr0[xk] = (2.0 * xk) / width - 1.5;
		}
	}

#pragma omp parallel for schedule(guided)
	for (int y = 0; y < height; ++y)
	{
		Byte* line = &buffer[y * max_x];

		const double ci0 = 2.0 * y / height - 1.0;

		for (int x = 0; x < max_x; ++x)
		{
			const double* cr0_x = &cr0[8 * x];
			double cr[8];
			double ci[8];
			for (int k = 0; k < 8; ++k)
			{
				cr[k] = cr0_x[k];
				ci[k] = ci0;
			}

			Byte bits = 0xFF;
			for (int i = 0; bits && i < max_iterations; ++i)
			{
				Byte bit_k = 0x80;
				for (int k = 0; k < 8; ++k)
				{
					if (bits & bit_k)
					{
						const double cr_k = cr[k];
						const double ci_k = ci[k];
						const double cr_k_sq = cr_k * cr_k;
						const double ci_k_sq = ci_k * ci_k;

						cr[k] = cr_k_sq - ci_k_sq + cr0_x[k];
						ci[k] = 2.0 * cr_k * ci_k + ci0;

						if (cr_k_sq + ci_k_sq > limit_sq)
						{
							bits ^= bit_k;
						}
					}
					bit_k >>= 1;
				}
			}
			line[x] = bits;
		}
	}

	//FILE* out = (argc == 3) ? fopen(argv[2], "wb") : stdout;
	//fprintf(out, "P4\n%u %u\n", width, height);
	//fwrite(&buffer[0], buffer.size(), 1, out);

	//if (out != stdout)
	//{
	//	fclose(out);
	//}

	return 0;
}