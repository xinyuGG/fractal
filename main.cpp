#include "fractal.h"
#include "parallel.h"
using namespace std::chrono;
using namespace Feng;

int main() {
	Bmp256 bmp{ W, H };
	int num = 5;
	auto t1 = steady_clock::now();

	for (int i = 0; i < num; i++) {
		for (int j = 0; j < H; ++j)
			for (int i = 0; i < W; ++i)
				bmp(j, i) = Parallel::Mandelbrot(Complex{ RMIN + i / RESN, IMIN + j / RESN });
	}

	auto t2 = steady_clock::now();
	std::cout << "origin run time: " << duration_cast<duration<double>>(t2 - t1).count() / (double)num << " seconds.\n";

	t1 = steady_clock::now();
	for (int i = 0; i < num; i++)
		Parallel::Mandelbrot_pall<uint8_t*>(bmp.begin(), bmp.end(), W, H, 0);
		t2 = steady_clock::now();
	std::cout << "para run time: " << duration_cast<duration<double>>(t2 - t1).count() / (double)num << " seconds.\n";

	bmp.save("Mandelbrot12k.bmp");


// #ifdef _MSC_VER
// 	system("pause");
// #else	
// 	getchar();
// #endif
}








