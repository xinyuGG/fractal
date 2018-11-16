#include <stdint.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <complex>


//extern const double RMIN, RMAX , IMIN , IMAX;
//extern const int W ;
//const double RESN = W / (RMAX - RMIN);
//const int H = (IMAX - IMIN) * RESN;
//extern const int MI;
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
	uint8_t* begin();

	uint8_t* end();

};



