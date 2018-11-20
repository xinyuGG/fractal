#pragma once
#include <intrin.h>
namespace Feng {
	double a[2];
	int squareAdd(__m128d a, __m128d ra, __m128d c, int num);
#pragma pack(16)

	template<typename T>
	class complex {
		template<typename Ty>
		friend Ty norm(const complex<Ty>& other);
	public:
		complex();
		complex(T real, T imag);
		complex(const complex&);
		//complex(complex&&);
		complex& operator = (const complex&);
		complex& operator = (const T);
		complex& operator * (const complex&);
		complex& operator + (const complex&);
		bool operator == (const complex&);
		complex& squareAdd(const complex&);
		complex& assign(const T, const T);
		complex<double>& squareAdd(__m128d c);

	private:
		T _real;
		T _imag;
		T _placeholder;
	};

	template<typename T>
	complex<T>::complex() : _real(0), _imag(0) {}

	template<typename T>
	inline complex<T>::complex(T real, T imag) : _real(real), _imag(imag) {}

	template<typename T>
	inline complex<T>::complex(const complex& other) {
		_real = other._real;
		_imag = other._imag;
	}

	template<typename T>
	inline complex<T>& complex<T>::operator = (const complex<T>& other) {
		_real = other._real;
		_imag = other._imag;
		return *this;
	}

	template<typename T>
	inline complex<T>& complex<T>::operator = (const T real) {
		_real = real;
		_imag = (T)0;
		return *this;
	}

	template<typename T>
	inline complex<T>& complex<T>::operator + (const complex<T>& other) {
		_real += other._real;
		_imag += other._imag;
		return *this;
	}

	template<typename T>
	inline bool complex<T>::operator == (const complex<T>& other) {

		return (this->_imag == other._imag) && (this->_real== other._real);
	}


	template<typename T>
	inline complex<T>& complex<T>::operator * (const complex<T>& other) {
		auto tmp = _real * other._real - _imag * other._imag;
		_imag = _real * other._imag + _imag * other._real;
		_real = tmp;
		return *this;
	}

	template<typename T>
	inline T norm(const complex<T>& other) {
		return other._real * other._real + other._imag * other._imag;
	}

	template<typename T>
	complex<T>& complex<T>::squareAdd(const complex<T>& add_one) {
		(*this) = (*this) * (*this);
		(*this) = (*this) + add_one;
		return *this;
	}

	template<typename T>
	complex<T>& complex<T>::assign(const T real, const T imag) {
		this->_real = real;
		this->_imag = imag;
		return *this;
	}


	template<>
	complex<double>& complex<double>::squareAdd(const complex<double>& add_one) {
		
		__m128d a;
		__m128d ra;

		__m128d a_a;
		__m128d a_ra;
		__m128d ra_ra;
		a = _mm_load_pd((const double*)this); //a,b
		ra = _mm_loadr_pd((const double*)(this)); //b,a
		__m128d c = _mm_load_pd((const double*)(&add_one));

		a_a = _mm_mul_pd(a, a); // a*a, b*b
		a_ra = _mm_mul_pd(a, ra);// a*b , b*
		ra_ra = _mm_mul_pd(ra, ra);
		
		a_a = _mm_sub_pd(a_a, ra_ra);
		ra = _mm_add_pd(a_ra, a_ra);
		a_a = _mm_add_pd(a_a, c);
		ra = _mm_add_pd(ra, c);

		_mm_stream_pd((double*)(this), a_a);
		_mm_store_pd((double*)(&(this->_imag)), ra);
		this->_imag = this->_placeholder;
		return *this;
	}

	template<>
	complex<double>& complex<double>::squareAdd(__m128d c) {

		__m128d a;
		__m128d ra;

		__m128d a_a;
		__m128d a_ra;
		__m128d ra_ra;
		a = _mm_load_pd((const double*)this); //a,b
		ra = _mm_loadr_pd((const double*)(this)); //b,a

		a_a = _mm_mul_pd(a, a); // a*a, b*b
		a_ra = _mm_mul_pd(a, ra);// a*b , b*
		ra_ra = _mm_mul_pd(ra, ra);

		a_a = _mm_sub_pd(a_a, ra_ra);
		ra = _mm_add_pd(a_ra, a_ra);
		a_a = _mm_add_pd(a_a, c);
		ra = _mm_add_pd(ra, c);

		_mm_store_pd((double*)(this), a_a);
		_mm_store_pd((double*)(&(this->_imag)), ra);
		this->_imag = this->_placeholder;
		return *this;
	}



	int squareAdd(__m128d a, __m128d ra, __m128d c , int num) {
		if (!num)
			return 0;


		__m128d a_a;
		__m128d a_ra;
		__m128d ra_ra;

		a_a = _mm_mul_pd(a, a); // a*a, b*b
		a_ra = _mm_mul_pd(a, ra);// a*b , b*
		ra_ra = _mm_mul_pd(ra, ra);

		a = _mm_sub_pd(a_a, ra_ra);
		ra = _mm_add_pd(a_ra, a_ra);


		a = _mm_add_pd(a, c);
		ra = _mm_add_pd(ra, c);
		

		a_a = _mm_mul_pd(a, a);
		ra_ra = _mm_mul_pd(ra, ra);

		ra_ra = _mm_add_pd(a_a, ra_ra);  //加起来

		_MM_ALIGN16 double result[2];
		_mm_store_pd(result, c);
		if (result[1] > 4)
			return 0;
		else
			return squareAdd(a,  ra,  c,  --num);

	}
}

