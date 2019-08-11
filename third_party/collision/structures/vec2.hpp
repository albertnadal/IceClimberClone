#pragma once

namespace collision
{
enum ctor
{
	uninitialize
};

typedef int int_t;
typedef unsigned short uint_t;
typedef unsigned int uint;

enum precision
{
	highp,
	mediump,
	lowp,
	simd,
	defaultp = highp
};

template <typename T, precision P>
struct vec2;
typedef vec2<float, highp> fvec2;
typedef vec2<uint, highp> uvec2;

template <typename T = float, precision P = defaultp>
struct vec2
{
	// -- Implementation detail --

	typedef T value_type;
	typedef vec2<float, highp> type;
	typedef vec2<bool, highp> bool_type;

	// -- Data --

	union {
		T x, r, s;
	};
	union {
		T y, g, t;
	};

	// -- Component accesses --

	/// Return the count of components of the vector
	typedef unsigned short length_type;
	unsigned short length() const;

	T &operator[](unsigned short i);
	T const &operator[](unsigned short i) const;

	// -- Implicit basic constructors --

	vec2();
	vec2(vec2<T, P> const &v);
	template <precision Q>
	vec2(vec2<T, Q> const &v);

	// -- Explicit basic constructors --

	explicit vec2(ctor);
	explicit vec2(T scalar);
	vec2(T s1, T s2);

	// -- Conversion constructors --

	/// Explicit converions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
	template <typename A, typename B>
	vec2(A x, B y);

	/// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
	template <typename U, precision Q>
	vec2(vec2<U, Q> const &v);

	// -- Unary arithmetic operators --

	vec2<T, P> &operator=(vec2<T, P> const &v);

	template <typename U>
	vec2<T, P> &operator=(vec2<U, P> const &v);
	template <typename U>
	vec2<T, P> &operator+=(U scalar);
	template <typename U>
	vec2<T, P> &operator+=(vec2<U, P> const &v);
	template <typename U>
	vec2<T, P> &operator-=(U scalar);
	template <typename U>
	vec2<T, P> &operator-=(vec2<U, P> const &v);
	template <typename U>
	vec2<T, P> &operator*=(U scalar);
	template <typename U>
	vec2<T, P> &operator*=(vec2<U, P> const &v);
	template <typename U>
	vec2<T, P> &operator/=(U scalar);
	template <typename U>
	vec2<T, P> &operator/=(vec2<U, P> const &v);

	// -- Increment and decrement operators --

	vec2<T, P> &operator++();
	vec2<T, P> &operator--();
	vec2<T, P> operator++(int);
	vec2<T, P> operator--(int);

	// -- Unary bit operators --

	template <typename U>
	vec2<T, P> &operator%=(U scalar);
	template <typename U>
	vec2<T, P> &operator%=(vec2<U, P> const &v);
	template <typename U>
	vec2<T, P> &operator&=(U scalar);
	template <typename U>
	vec2<T, P> &operator&=(vec2<U, P> const &v);
	template <typename U>
	vec2<T, P> &operator|=(U scalar);
	template <typename U>
	vec2<T, P> &operator|=(vec2<U, P> const &v);
	template <typename U>
	vec2<T, P> &operator^=(U scalar);
	template <typename U>
	vec2<T, P> &operator^=(vec2<U, P> const &v);
	template <typename U>
	vec2<T, P> &operator<<=(U scalar);
	template <typename U>
	vec2<T, P> &operator<<=(vec2<U, P> const &v);
	template <typename U>
	vec2<T, P> &operator>>=(U scalar);
	template <typename U>
	vec2<T, P> &operator>>=(vec2<U, P> const &v);
};

// -- Unary operators --

template <typename T, precision P>
vec2<T, P> operator+(vec2<T, P> const &v);
template <typename T, precision P>
vec2<T, P> operator-(vec2<T, P> const &v);
// -- Binary operators --

template <typename T, precision P>
vec2<T, P> operator+(vec2<T, P> const &v, T scalar);
template <typename T, precision P>
vec2<T, P> operator+(T scalar, vec2<T, P> const &v);
template <typename T, precision P>
vec2<T, P> operator+(vec2<T, P> const &v1, vec2<T, P> const &v2);
template <typename T, precision P>
vec2<T, P> operator-(vec2<T, P> const &v, T scalar);
template <typename T, precision P>
vec2<T, P> operator-(T scalar, vec2<T, P> const &v);
template <typename T, precision P>
vec2<T, P> operator-(vec2<T, P> const &v1, vec2<T, P> const &v2);
template <typename T, precision P>
vec2<T, P> operator*(vec2<T, P> const &v, T scalar);
template <typename T, precision P>
vec2<T, P> operator*(T scalar, vec2<T, P> const &v);
template <typename T, precision P>
vec2<T, P> operator*(vec2<T, P> const &v1, vec2<T, P> const &v2);
template <typename T, precision P>
vec2<T, P> operator/(vec2<T, P> const &v, T scalar);
template <typename T, precision P>
vec2<T, P> operator/(T scalar, vec2<T, P> const &v);
template <typename T, precision P>
vec2<T, P> operator/(vec2<T, P> const &v1, vec2<T, P> const &v2);
template <typename T, precision P>
vec2<T, P> operator%(vec2<T, P> const &v, T scalar);
template <typename T, precision P>
vec2<T, P> operator%(T scalar, vec2<T, P> const &v);
template <typename T, precision P>
vec2<T, P> operator%(vec2<T, P> const &v1, vec2<T, P> const &v2);
template <typename T, precision P>
vec2<T, P> operator&(vec2<T, P> const &v, T scalar);
template <typename T, precision P>
vec2<T, P> operator&(T scalar, vec2<T, P> const &v);
template <typename T, precision P>
vec2<T, P> operator&(vec2<T, P> const &v1, vec2<T, P> const &v2);
template <typename T, precision P>
vec2<T, P> operator|(vec2<T, P> const &v, T scalar);
template <typename T, precision P>
vec2<T, P> operator|(T scalar, vec2<T, P> const &v);
template <typename T, precision P>
vec2<T, P> operator|(vec2<T, P> const &v1, vec2<T, P> const &v2);
template <typename T, precision P>
vec2<T, P> operator^(vec2<T, P> const &v, T scalar);
template <typename T, precision P>
vec2<T, P> operator^(T scalar, vec2<T, P> const &v);
template <typename T, precision P>
vec2<T, P> operator^(vec2<T, P> const &v1, vec2<T, P> const &v2);
template <typename T, precision P>
vec2<T, P> operator<<(vec2<T, P> const &v, T scalar);
template <typename T, precision P>
vec2<T, P> operator<<(T scalar, vec2<T, P> const &v);
template <typename T, precision P>
vec2<T, P> operator<<(vec2<T, P> const &v1, vec2<T, P> const &v2);
template <typename T, precision P>
vec2<T, P> operator>>(vec2<T, P> const &v, T scalar);
template <typename T, precision P>
vec2<T, P> operator>>(T scalar, vec2<T, P> const &v);
template <typename T, precision P>
vec2<T, P> operator>>(vec2<T, P> const &v1, vec2<T, P> const &v2);
template <typename T, precision P>
vec2<T, P> operator~(vec2<T, P> const &v);

// -- Boolean operators --

template <typename T, precision P>
bool operator==(vec2<T, P> const &v1, vec2<T, P> const &v2);
template <typename T, precision P>
bool operator!=(vec2<T, P> const &v1, vec2<T, P> const &v2);
template <precision P>
vec2<bool, P> operator&&(vec2<bool, P> const &v1, vec2<bool, P> const &v2);
template <precision P>
vec2<bool, P> operator||(vec2<bool, P> const &v1, vec2<bool, P> const &v2);

// -- Implicit basic constructors --

template <typename T, precision P>
vec2<T, P>::vec2()
	: x(0), y(0)
{
}

template <typename T, precision P>
vec2<T, P>::vec2(vec2<T, P> const &v)
	: x(v.x), y(v.y)
{
}

template <typename T, precision P>
template <precision Q>
vec2<T, P>::vec2(vec2<T, Q> const &v)
	: x(v.x), y(v.y)
{
}

// -- Explicit basic constructors --

template <typename T, precision P>
vec2<T, P>::vec2(ctor)
{
}

template <typename T, precision P>
vec2<T, P>::vec2(T scalar)
	: x(scalar), y(scalar)
{
}

template <typename T, precision P>
vec2<T, P>::vec2(T s1, T s2)
	: x(s1), y(s2)
{
}

// -- Conversion scalar constructors --

template <typename T, precision P>
template <typename A, typename B>
vec2<T, P>::vec2(A a, B b)
	: x(static_cast<T>(a)), y(static_cast<T>(b))
{
}

// -- Conversion vector constructors --

template <typename T, precision P>
template <typename U, precision Q>
vec2<T, P>::vec2(vec2<U, Q> const &v)
	: x(static_cast<T>(v.x)), y(static_cast<T>(v.y))
{
}

// -- Component accesses --

template <typename T, precision P>
typename vec2<T, P>::length_type vec2<T, P>::length() const
{
	return 2;
}

template <typename T, precision P>
T &vec2<T, P>::operator[](unsigned short i)
{
	return (&x)[i];
}

template <typename T, precision P>
T const &vec2<T, P>::operator[](unsigned short i) const
{
	return (&x)[i];
}

// -- Unary arithmetic operators --

template <typename T, precision P>
vec2<T, P> &vec2<T, P>::operator=(vec2<T, P> const &v)
{
	this->x = v.x;
	this->y = v.y;
	return *this;
}

template <typename T, precision P>
template <typename U>
vec2<T, P> &vec2<T, P>::operator=(vec2<U, P> const &v)
{
	this->x = static_cast<T>(v.x);
	this->y = static_cast<T>(v.y);
	return *this;
}

template <typename T, precision P>
template <typename U>
vec2<T, P> &vec2<T, P>::operator+=(U scalar)
{
	this->x += static_cast<T>(scalar);
	this->y += static_cast<T>(scalar);
	return *this;
}

template <typename T, precision P>
template <typename U>
vec2<T, P> &vec2<T, P>::operator+=(vec2<U, P> const &v)
{
	this->x += static_cast<T>(v.x);
	this->y += static_cast<T>(v.y);
	return *this;
}

template <typename T, precision P>
template <typename U>
vec2<T, P> &vec2<T, P>::operator-=(U scalar)
{
	this->x -= static_cast<T>(scalar);
	this->y -= static_cast<T>(scalar);
	return *this;
}

template <typename T, precision P>
template <typename U>
vec2<T, P> &vec2<T, P>::operator-=(vec2<U, P> const &v)
{
	this->x -= static_cast<T>(v.x);
	this->y -= static_cast<T>(v.y);
	return *this;
}

template <typename T, precision P>
template <typename U>
vec2<T, P> &vec2<T, P>::operator*=(U scalar)
{
	this->x *= static_cast<T>(scalar);
	this->y *= static_cast<T>(scalar);
	return *this;
}

template <typename T, precision P>
template <typename U>
vec2<T, P> &vec2<T, P>::operator*=(vec2<U, P> const &v)
{
	this->x *= static_cast<T>(v.x);
	this->y *= static_cast<T>(v.y);
	return *this;
}

template <typename T, precision P>
template <typename U>
vec2<T, P> &vec2<T, P>::operator/=(U scalar)
{
	this->x /= static_cast<T>(scalar);
	this->y /= static_cast<T>(scalar);
	return *this;
}

template <typename T, precision P>
template <typename U>
vec2<T, P> &vec2<T, P>::operator/=(vec2<U, P> const &v)
{
	this->x /= static_cast<T>(v.x);
	this->y /= static_cast<T>(v.y);
	return *this;
}

// -- Increment and decrement operators --

template <typename T, precision P>
vec2<T, P> &vec2<T, P>::operator++()
{
	++this->x;
	++this->y;
	return *this;
}

template <typename T, precision P>
vec2<T, P> &vec2<T, P>::operator--()
{
	--this->x;
	--this->y;
	return *this;
}

template <typename T, precision P>
vec2<T, P> vec2<T, P>::operator++(int)
{
	vec2<T, P> Result(*this);
	++*this;
	return Result;
}

template <typename T, precision P>
vec2<T, P> vec2<T, P>::operator--(int)
{
	vec2<T, P> Result(*this);
	--*this;
	return Result;
}

// -- Unary bit operators --

template <typename T, precision P>
template <typename U>
vec2<T, P> &vec2<T, P>::operator%=(U scalar)
{
	this->x %= static_cast<T>(scalar);
	this->y %= static_cast<T>(scalar);
	return *this;
}

template <typename T, precision P>
template <typename U>
vec2<T, P> &vec2<T, P>::operator%=(vec2<U, P> const &v)
{
	this->x %= static_cast<T>(v.x);
	this->y %= static_cast<T>(v.y);
	return *this;
}

template <typename T, precision P>
template <typename U>
vec2<T, P> &vec2<T, P>::operator&=(U scalar)
{
	this->x &= static_cast<T>(scalar);
	this->y &= static_cast<T>(scalar);
	return *this;
}

template <typename T, precision P>
template <typename U>
vec2<T, P> &vec2<T, P>::operator&=(vec2<U, P> const &v)
{
	this->x &= static_cast<T>(v.x);
	this->y &= static_cast<T>(v.y);
	return *this;
}

template <typename T, precision P>
template <typename U>
vec2<T, P> &vec2<T, P>::operator|=(U scalar)
{
	this->x |= static_cast<T>(scalar);
	this->y |= static_cast<T>(scalar);
	return *this;
}

template <typename T, precision P>
template <typename U>
vec2<T, P> &vec2<T, P>::operator|=(vec2<U, P> const &v)
{
	this->x |= static_cast<T>(v.x);
	this->y |= static_cast<T>(v.y);
	return *this;
}

template <typename T, precision P>
template <typename U>
vec2<T, P> &vec2<T, P>::operator^=(U scalar)
{
	this->x ^= static_cast<T>(scalar);
	this->y ^= static_cast<T>(scalar);
	return *this;
}

template <typename T, precision P>
template <typename U>
vec2<T, P> &vec2<T, P>::operator^=(vec2<U, P> const &v)
{
	this->x ^= static_cast<T>(v.x);
	this->y ^= static_cast<T>(v.y);
	return *this;
}

template <typename T, precision P>
template <typename U>
vec2<T, P> &vec2<T, P>::operator<<=(U scalar)
{
	this->x <<= static_cast<T>(scalar);
	this->y <<= static_cast<T>(scalar);
	return *this;
}

template <typename T, precision P>
template <typename U>
vec2<T, P> &vec2<T, P>::operator<<=(vec2<U, P> const &v)
{
	this->x <<= static_cast<T>(v.x);
	this->y <<= static_cast<T>(v.y);
	return *this;
}

template <typename T, precision P>
template <typename U>
vec2<T, P> &vec2<T, P>::operator>>=(U scalar)
{
	this->x >>= static_cast<T>(scalar);
	this->y >>= static_cast<T>(scalar);
	return *this;
}

template <typename T, precision P>
template <typename U>
vec2<T, P> &vec2<T, P>::operator>>=(vec2<U, P> const &v)
{
	this->x >>= static_cast<T>(v.x);
	this->y >>= static_cast<T>(v.y);
	return *this;
}

// -- Unary arithmetic operators --

template <typename T, precision P>
vec2<T, P> operator+(vec2<T, P> const &v)
{
	return v;
}

template <typename T, precision P>
vec2<T, P> operator-(vec2<T, P> const &v)
{
	return vec2<T, P>(
		-v.x,
		-v.y);
}

// -- Binary arithmetic operators --

template <typename T, precision P>
vec2<T, P> operator+(vec2<T, P> const &v, T scalar)
{
	return vec2<T, P>(
		v.x + scalar,
		v.y + scalar);
}

template <typename T, precision P>
vec2<T, P> operator+(T scalar, vec2<T, P> const &v)
{
	return vec2<T, P>(
		scalar + v.x,
		scalar + v.y);
}

template <typename T, precision P>
vec2<T, P> operator+(vec2<T, P> const &v1, vec2<T, P> const &v2)
{
	return vec2<T, P>(
		v1.x + v2.x,
		v1.y + v2.y);
}

template <typename T, precision P>
vec2<T, P> operator-(vec2<T, P> const &v, T scalar)
{
	return vec2<T, P>(
		v.x - scalar,
		v.y - scalar);
}

template <typename T, precision P>
vec2<T, P> operator-(T scalar, vec2<T, P> const &v)
{
	return vec2<T, P>(
		scalar - v.x,
		scalar - v.y);
}

template <typename T, precision P>
vec2<T, P> operator-(vec2<T, P> const &v1, vec2<T, P> const &v2)
{
	return vec2<T, P>(
		v1.x - v2.x,
		v1.y - v2.y);
}

template <typename T, precision P>
vec2<T, P> operator*(vec2<T, P> const &v, T scalar)
{
	return vec2<T, P>(
		v.x * scalar,
		v.y * scalar);
}

template <typename T, precision P>
vec2<T, P> operator*(T scalar, vec2<T, P> const &v)
{
	return vec2<T, P>(
		scalar * v.x,
		scalar * v.y);
}

template <typename T, precision P>
vec2<T, P> operator*(vec2<T, P> const &v1, vec2<T, P> const &v2)
{
	return vec2<T, P>(
		v1.x * v2.x,
		v1.y * v2.y);
}

template <typename T, precision P>
vec2<T, P> operator/(vec2<T, P> const &v, T scalar)
{
	return vec2<T, P>(
		v.x / scalar,
		v.y / scalar);
}

template <typename T, precision P>
vec2<T, P> operator/(T scalar, vec2<T, P> const &v)
{
	return vec2<T, P>(
		scalar / v.x,
		scalar / v.y);
}

template <typename T, precision P>
vec2<T, P> operator/(vec2<T, P> const &v1, vec2<T, P> const &v2)
{
	return vec2<T, P>(
		v1.x / v2.x,
		v1.y / v2.y);
}

// -- Binary bit operators --

template <typename T, precision P>
vec2<T, P> operator%(vec2<T, P> const &v, T scalar)
{
	return vec2<T, P>(
		v.x % scalar,
		v.y % scalar);
}

template <typename T, precision P>
vec2<T, P> operator%(T scalar, vec2<T, P> const &v)
{
	return vec2<T, P>(
		scalar % v.x,
		scalar % v.y);
}

template <typename T, precision P>
vec2<T, P> operator%(vec2<T, P> const &v1, vec2<T, P> const &v2)
{
	return vec2<T, P>(
		v1.x % v2.x,
		v1.y % v2.y);
}

template <typename T, precision P>
vec2<T, P> operator&(vec2<T, P> const &v, T scalar)
{
	return vec2<T, P>(
		v.x & scalar,
		v.y & scalar);
}

template <typename T, precision P>
vec2<T, P> operator&(T scalar, vec2<T, P> const &v)
{
	return vec2<T, P>(
		scalar & v.x,
		scalar & v.y);
}

template <typename T, precision P>
vec2<T, P> operator&(vec2<T, P> const &v1, vec2<T, P> const &v2)
{
	return vec2<T, P>(
		v1.x & v2.x,
		v1.y & v2.y);
}

template <typename T, precision P>
vec2<T, P> operator|(vec2<T, P> const &v, T scalar)
{
	return vec2<T, P>(
		v.x | scalar,
		v.y | scalar);
}

template <typename T, precision P>
vec2<T, P> operator|(T scalar, vec2<T, P> const &v)
{
	return vec2<T, P>(
		scalar | v.x,
		scalar | v.y);
}

template <typename T, precision P>
vec2<T, P> operator|(vec2<T, P> const &v1, vec2<T, P> const &v2)
{
	return vec2<T, P>(
		v1.x | v2.x,
		v1.y | v2.y);
}

template <typename T, precision P>
vec2<T, P> operator^(vec2<T, P> const &v, T scalar)
{
	return vec2<T, P>(
		v.x ^ scalar,
		v.y ^ scalar);
}

template <typename T, precision P>
vec2<T, P> operator^(T scalar, vec2<T, P> const &v)
{
	return vec2<T, P>(
		scalar ^ v.x,
		scalar ^ v.y);
}

template <typename T, precision P>
vec2<T, P> operator^(vec2<T, P> const &v1, vec2<T, P> const &v2)
{
	return vec2<T, P>(
		v1.x ^ v2.x,
		v1.y ^ v2.y);
}

template <typename T, precision P>
vec2<T, P> operator<<(vec2<T, P> const &v, T scalar)
{
	return vec2<T, P>(
		v.x << scalar,
		v.y << scalar);
}

template <typename T, precision P>
vec2<T, P> operator<<(T scalar, vec2<T, P> const &v)
{
	return vec2<T, P>(
		scalar << v.x,
		scalar << v.y);
}

template <typename T, precision P>
vec2<T, P> operator<<(vec2<T, P> const &v1, vec2<T, P> const &v2)
{
	return vec2<T, P>(
		v1.x << v2.x,
		v1.y << v2.y);
}

template <typename T, precision P>
vec2<T, P> operator>>(vec2<T, P> const &v, T scalar)
{
	return vec2<T, P>(
		v.x >> scalar,
		v.y >> scalar);
}

template <typename T, precision P>
vec2<T, P> operator>>(T scalar, vec2<T, P> const &v)
{
	return vec2<T, P>(
		scalar >> v.x,
		scalar >> v.y);
}

template <typename T, precision P>
vec2<T, P> operator>>(vec2<T, P> const &v1, vec2<T, P> const &v2)
{
	return vec2<T, P>(
		v1.x >> v2.x,
		v1.y >> v2.y);
}

template <typename T, precision P>
vec2<T, P> operator~(vec2<T, P> const &v)
{
	return vec2<T, P>(
		~v.x,
		~v.y);
}

// -- Boolean operators --

template <typename T, precision P>
bool operator==(vec2<T, P> const &v1, vec2<T, P> const &v2)
{
	return (v1.x == v2.x) && (v1.y == v2.y);
}

template <typename T, precision P>
bool operator!=(vec2<T, P> const &v1, vec2<T, P> const &v2)
{
	return (v1.x != v2.x) || (v1.y != v2.y);
}

template <precision P>
vec2<bool, P> operator&&(vec2<bool, P> const &v1, vec2<bool, P> const &v2)
{
	return vec2<bool, P>(v1.x && v2.x, v1.y && v2.y);
}

template <precision P>
vec2<bool, P> operator||(vec2<bool, P> const &v1, vec2<bool, P> const &v2)
{
	return vec2<bool, P>(v1.x || v2.x, v1.y || v2.y);
}
} // namespace collision
