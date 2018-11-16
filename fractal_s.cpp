#include <stdint.h>

class Bmp256 {
#pragma pack(2)
	struct Header {
		uint16_t bfType = 0x4D42;
		uint32_t bfSize;
		uint16_t bfReserved1 = 0;
		uint16_t bfReserved2 = 0;
		uint32_t bfOffBits = 54 + 256 * 4;
		uint32_t biSize = 40;
		int32_t  biWidth;
		int32_t  biHeight;
		uint16_t biPlanes = 1;
		uint16_t biBitCount = 8;
		uint32_t biCompression = 0;
		uint32_t biSizeImage = 0;
		int32_t  biXPelsPerMeter = 0;
		int32_t  biYPelsPerMeter = 0;
		uint32_t biClrUsed = 256;
		uint32_t biClrImportant = 256;
	} header;
#pragma pack()
	int32_t rowSize;
	struct {
		uint8_t B, G, R, A;
		void set(uint8_t r, uint8_t g, uint8_t b) { R = r; G = g; B = b; A = 0; }
	} palette[256];
	uint8_t *buffer = nullptr;

	void calc_palette();

public:
	Bmp256(int width, int height);
	~Bmp256() { delete[] buffer; }
	int width()  const { return header.biWidth; }
	int height() const { return header.biHeight; }
	uint8_t& operator()(int row, int col) { return buffer[row * rowSize + col]; }	// get/set the pixel
	void save(const char* file);
};

Bmp256::Bmp256(int width, int height) {
	header.biWidth = width;
	header.biHeight = height;
	rowSize = ((width + 3) / 4) * 4;
	int buffSize = rowSize * height;
	header.bfSize = header.bfOffBits + buffSize;
	calc_palette();
	buffer = new uint8_t[buffSize];
}

void Bmp256::calc_palette() {
	for (int i = 0; i < 64; ++i) {
		palette[i].set(255, 255 - i * 4, 0);
		palette[i + 64].set(255 - i * 2, 0, i * 2);
		palette[i + 128].set(127 - i * 2, 0, 128 + i * 2);
		palette[i + 192].set(0, 0, 255 - i * 3);
	}
	palette[0].set(0, 0, 0);
}

#include <iostream>
#include <fstream>
void Bmp256::save(const char* file_name) {
	std::ofstream of(file_name, std::ios::binary);
	of.write((char *)&header, 54);
	of.write((char *)palette, 256 * 4);
	char* p = (char *)buffer;
	for (int i = 0; i < header.biHeight; ++i) {
		of.write(p, rowSize);
		p += rowSize;
	}
}


#include <complex>
using complex = std::complex<double>;

const double RMIN = -2, RMAX = 1, IMIN = -1, IMAX = 1;
const int W = 12 * 1024;
const double RESN = W / (RMAX - RMIN);
const int H = (IMAX - IMIN) * RESN;
const int MI = 1;

int Mandelbrot(complex c) {
	complex z;
	for (int k = 256 * MI - 1; k >= 0; --k) {
		z = z * z + c;
		if (std::norm(z) > 4) return k / MI;
	}
	return 0;
}

#include <chrono>
using namespace std::chrono;

int main() {
	Bmp256 bmp{W, H};

	auto t1 = steady_clock::now();
	for (int j = 0; j < H; ++j)
	for (int i = 0; i < W; ++i)
		bmp(j, i) = Mandelbrot(complex{RMIN + i / RESN, IMIN + j / RESN});
	auto t2 = steady_clock::now();

	bmp.save("Mandelbrot12k.bmp");
	std::cout << "run time: " << duration_cast<duration<double>>(t2 - t1).count() << " seconds.\n";
}
