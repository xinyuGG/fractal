﻿#include "fractal.h"

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

uint8_t* Bmp256::begin() {
	return &buffer[0];
}
uint8_t* Bmp256::end() {
	return &buffer[0] + header.biWidth * header.biHeight;
}





