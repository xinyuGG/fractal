#include "fractal.h"
#include "parallel.h"
using namespace std::chrono;
int main() {
	Bmp256 bmp{ W, H };
	int num = 3;
	auto t1 = steady_clock::now();

	for (int i = 0; i < num; i++) {

		//for (int j = 0; j < H; ++j)
		//	for (int i = 0; i < W; ++i)
		//		bmp(j, i) = Parallel::Mandelbrot(complex{ RMIN + i / RESN, IMIN + j / RESN });

		Parallel::Mandelbrot_pall<uint8_t*>(bmp.begin(), bmp.end(), W, H, 0);
	}

	auto t2 = steady_clock::now();

	bmp.save("Mandelbrot12k.bmp");
	std::cout << "run time: " << duration_cast<duration<double>>(t2 - t1).count() / (double)num << " seconds.\n";

	system("pause");
}




