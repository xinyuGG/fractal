mandelbrot : fractal.o parallel.h
	g++ -o mandelbrot fractal.o parallel.h main.cpp -std=c++11 -pthread  -march=native -O3

fractal.o : fractal.cpp
	g++ -c fractal.cpp -std=c++11

# parallel.h.gch : parallel.h complex.h
# 	g++ -c parallel.h -std=c++11