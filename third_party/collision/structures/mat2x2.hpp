#pragma once

#include <collision/structures/vec2.hpp>
#include <limits>
#include <cstddef>

namespace collision
{
template <typename T = float, precision P = defaultp>
struct tmat2x2
{
	typedef collision::vec2<float> col_type;
	typedef collision::vec2<float> row_type;
	typedef tmat2x2<T, P> type;
	typedef tmat2x2<T, P> transpose_type;
	typedef T value_type;

  private:
	col_type value[2];

  public:
	// -- Constructors --

	tmat2x2();
	tmat2x2(tmat2x2<T, P> const &m);
	template <precision Q>
	tmat2x2(tmat2x2<T, Q> const &m);

	explicit tmat2x2(ctor);
	explicit tmat2x2(T scalar);
	tmat2x2(
		T const &x1, T const &y1,
		T const &x2, T const &y2);
	tmat2x2(
		col_type const &v1,
		col_type const &v2);

	// -- Conversions --

	template <typename U, typename V, typename M, typename N>
	tmat2x2(
		U const &x1, V const &y1,
		M const &x2, N const &y2);

	template <typename U, typename V>
	tmat2x2(
		collision::vec2<float> const &v1,
		collision::vec2<float> const &v2);

	// -- Matrix conversions --

	template <typename U, precision Q>
	tmat2x2(tmat2x2<U, Q> const &m);

	// -- Accesses --

	typedef unsigned short length_type;
	length_type length() const;

	col_type &operator[](length_type i);
	col_type const &operator[](length_type i) const;

	// -- Unary arithmetic operators --

	tmat2x2<T, P> &operator=(tmat2x2<T, P> const &v);

	template <typename U>
	tmat2x2<T, P> &operator=(tmat2x2<U, P> const &m);
	template <typename U>
	tmat2x2<T, P> &operator+=(U s);
	template <typename U>
	tmat2x2<T, P> &operator+=(tmat2x2<U, P> const &m);
	template <typename U>
	tmat2x2<T, P> &operator-=(U s);
	template <typename U>
	tmat2x2<T, P> &operator-=(tmat2x2<U, P> const &m);
	template <typename U>
	tmat2x2<T, P> &operator*=(U s);
	template <typename U>
	tmat2x2<T, P> &operator*=(tmat2x2<U, P> const &m);
	template <typename U>
	tmat2x2<T, P> &operator/=(U s);
	template <typename U>
	tmat2x2<T, P> &operator/=(tmat2x2<U, P> const &m);

	// -- Increment and decrement operators --

	tmat2x2<T, P> &operator++();
	tmat2x2<T, P> &operator--();
	tmat2x2<T, P> operator++(int);
	tmat2x2<T, P> operator--(int);
};

// -- Unary operators --

template <typename T, precision P>
tmat2x2<T, P> operator+(tmat2x2<T, P> const &m);

template <typename T, precision P>
tmat2x2<T, P> operator-(tmat2x2<T, P> const &m);

// -- Binary operators --

template <typename T, precision P>
tmat2x2<T, P> operator+(tmat2x2<T, P> const &m, T scalar);

template <typename T, precision P>
tmat2x2<T, P> operator+(T scalar, tmat2x2<T, P> const &m);

template <typename T, precision P>
tmat2x2<T, P> operator+(tmat2x2<T, P> const &m1, tmat2x2<T, P> const &m2);

template <typename T, precision P>
tmat2x2<T, P> operator-(tmat2x2<T, P> const &m, T scalar);

template <typename T, precision P>
tmat2x2<T, P> operator-(T scalar, tmat2x2<T, P> const &m);

template <typename T, precision P>
tmat2x2<T, P> operator-(tmat2x2<T, P> const &m1, tmat2x2<T, P> const &m2);

template <typename T, precision P>
tmat2x2<T, P> operator*(tmat2x2<T, P> const &m, T scalar);

template <typename T, precision P>
tmat2x2<T, P> operator*(T scalar, tmat2x2<T, P> const &m);

template <typename T, precision P>
typename tmat2x2<T, P>::col_type operator*(tmat2x2<T, P> const &m, typename tmat2x2<T, P>::row_type const &v);

template <typename T, precision P>
typename tmat2x2<T, P>::row_type operator*(typename tmat2x2<T, P>::col_type const &v, tmat2x2<T, P> const &m);

template <typename T, precision P>
tmat2x2<T, P> operator*(tmat2x2<T, P> const &m1, tmat2x2<T, P> const &m2);

template <typename T, precision P>
tmat2x2<T, P> operator/(tmat2x2<T, P> const &m, T scalar);

template <typename T, precision P>
tmat2x2<T, P> operator/(T scalar, tmat2x2<T, P> const &m);

template <typename T, precision P>
typename tmat2x2<T, P>::col_type operator/(tmat2x2<T, P> const &m, typename tmat2x2<T, P>::row_type const &v);

template <typename T, precision P>
typename tmat2x2<T, P>::row_type operator/(typename tmat2x2<T, P>::col_type const &v, tmat2x2<T, P> const &m);

template <typename T, precision P>
tmat2x2<T, P> operator/(tmat2x2<T, P> const &m1, tmat2x2<T, P> const &m2);

// -- Boolean operators --

template <typename T, precision P>
bool operator==(tmat2x2<T, P> const &m1, tmat2x2<T, P> const &m2);

template <typename T, precision P>
bool operator!=(tmat2x2<T, P> const &m1, tmat2x2<T, P> const &m2);

namespace detail
{
template <typename T = float, precision P = defaultp>
tmat2x2<T, P> compute_inverse(tmat2x2<T, P> const &m)
{
	T OneOverDeterminant = static_cast<T>(1) / (+m[0][0] * m[1][1] - m[1][0] * m[0][1]);

	tmat2x2<T, P> Inverse(
		+m[1][1] * OneOverDeterminant,
		-m[0][1] * OneOverDeterminant,
		-m[1][0] * OneOverDeterminant,
		+m[0][0] * OneOverDeterminant);

	return Inverse;
}
} //namespace detail

// -- Constructors --

template <typename T, precision P>
tmat2x2<T, P>::tmat2x2()
{
	this->value[0] = col_type(1, 0);
	this->value[1] = col_type(0, 1);
}

template <typename T, precision P>
template <precision Q>
tmat2x2<T, P>::tmat2x2(tmat2x2<T, Q> const &m)
{
	this->value[0] = m.value[0];
	this->value[1] = m.value[1];
}

template <typename T, precision P>
tmat2x2<T, P>::tmat2x2(ctor)
{
}

template <typename T, precision P>
tmat2x2<T, P>::tmat2x2(T scalar)
{
	this->value[0] = col_type(scalar, 0);
	this->value[1] = col_type(0, scalar);
}

template <typename T, precision P>
tmat2x2<T, P>::tmat2x2(
	T const &x0, T const &y0,
	T const &x1, T const &y1)
{
	this->value[0] = col_type(x0, y0);
	this->value[1] = col_type(x1, y1);
}

template <typename T, precision P>
tmat2x2<T, P>::tmat2x2(col_type const &v0, col_type const &v1)
{
	this->value[0] = v0;
	this->value[1] = v1;
}

// -- Conversion constructors --

template <typename T, precision P>
template <typename X1, typename Y1, typename X2, typename Y2>
tmat2x2<T, P>::tmat2x2(
	X1 const &x1, Y1 const &y1,
	X2 const &x2, Y2 const &y2)
{
	this->value[0] = col_type(static_cast<T>(x1), value_type(y1));
	this->value[1] = col_type(static_cast<T>(x2), value_type(y2));
}

template <typename T, precision P>
template <typename V1, typename V2>
tmat2x2<T, P>::tmat2x2(collision::vec2<float> const &v1, collision::vec2<float> const &v2)
{
	this->value[0] = col_type(v1);
	this->value[1] = col_type(v2);
}

// -- mat2x2 matrix conversions --

template <typename T, precision P>
template <typename U, precision Q>
tmat2x2<T, P>::tmat2x2(tmat2x2<U, Q> const &m)
{
	this->value[0] = col_type(m[0]);
	this->value[1] = col_type(m[1]);
}

// -- Accesses --

template <typename T, precision P>
typename tmat2x2<T, P>::length_type tmat2x2<T, P>::length() const
{
	return 2;
}

template <typename T, precision P>
typename tmat2x2<T, P>::col_type &tmat2x2<T, P>::operator[](typename tmat2x2<T, P>::length_type i)
{
	assert(i < this->length());
	return this->value[i];
}

template <typename T, precision P>
typename tmat2x2<T, P>::col_type const &tmat2x2<T, P>::operator[](typename tmat2x2<T, P>::length_type i) const
{
	assert(i < this->length());
	return this->value[i];
}

// -- Unary updatable operators --

template <typename T, precision P>
tmat2x2<T, P> &tmat2x2<T, P>::operator=(tmat2x2<T, P> const &m)
{
	this->value[0] = m[0];
	this->value[1] = m[1];
	return *this;
}

template <typename T, precision P>
template <typename U>
tmat2x2<T, P> &tmat2x2<T, P>::operator=(tmat2x2<U, P> const &m)
{
	this->value[0] = m[0];
	this->value[1] = m[1];
	return *this;
}

template <typename T, precision P>
template <typename U>
tmat2x2<T, P> &tmat2x2<T, P>::operator+=(U scalar)
{
	this->value[0] += scalar;
	this->value[1] += scalar;
	return *this;
}

template <typename T, precision P>
template <typename U>
tmat2x2<T, P> &tmat2x2<T, P>::operator+=(tmat2x2<U, P> const &m)
{
	this->value[0] += m[0];
	this->value[1] += m[1];
	return *this;
}

template <typename T, precision P>
template <typename U>
tmat2x2<T, P> &tmat2x2<T, P>::operator-=(U scalar)
{
	this->value[0] -= scalar;
	this->value[1] -= scalar;
	return *this;
}

template <typename T, precision P>
template <typename U>
tmat2x2<T, P> &tmat2x2<T, P>::operator-=(tmat2x2<U, P> const &m)
{
	this->value[0] -= m[0];
	this->value[1] -= m[1];
	return *this;
}

template <typename T, precision P>
template <typename U>
tmat2x2<T, P> &tmat2x2<T, P>::operator*=(U scalar)
{
	this->value[0] *= scalar;
	this->value[1] *= scalar;
	return *this;
}

template <typename T, precision P>
template <typename U>
tmat2x2<T, P> &tmat2x2<T, P>::operator*=(tmat2x2<U, P> const &m)
{
	return (*this = *this * m);
}

template <typename T, precision P>
template <typename U>
tmat2x2<T, P> &tmat2x2<T, P>::operator/=(U scalar)
{
	this->value[0] /= scalar;
	this->value[1] /= scalar;
	return *this;
}

template <typename T, precision P>
template <typename U>
tmat2x2<T, P> &tmat2x2<T, P>::operator/=(tmat2x2<U, P> const &m)
{
	return (*this = *this * detail::compute_inverse<T, P>(m));
}

// -- Increment and decrement operators --

template <typename T, precision P>
tmat2x2<T, P> &tmat2x2<T, P>::operator++()
{
	++this->value[0];
	++this->value[1];
	return *this;
}

template <typename T, precision P>
tmat2x2<T, P> &tmat2x2<T, P>::operator--()
{
	--this->value[0];
	--this->value[1];
	return *this;
}

template <typename T, precision P>
tmat2x2<T, P> tmat2x2<T, P>::operator++(int)
{
	tmat2x2<T, P> Result(*this);
	++*this;
	return Result;
}

template <typename T, precision P>
tmat2x2<T, P> tmat2x2<T, P>::operator--(int)
{
	tmat2x2<T, P> Result(*this);
	--*this;
	return Result;
}

// -- Unary arithmetic operators --

template <typename T, precision P>
tmat2x2<T, P> operator+(tmat2x2<T, P> const &m)
{
	return m;
}

template <typename T, precision P>
tmat2x2<T, P> operator-(tmat2x2<T, P> const &m)
{
	return tmat2x2<T, P>(
		-m[0],
		-m[1]);
}

// -- Binary arithmetic operators --

template <typename T, precision P>
tmat2x2<T, P> operator+(tmat2x2<T, P> const &m, T scalar)
{
	return tmat2x2<T, P>(
		m[0] + scalar,
		m[1] + scalar);
}

template <typename T, precision P>
tmat2x2<T, P> operator+(T scalar, tmat2x2<T, P> const &m)
{
	return tmat2x2<T, P>(
		m[0] + scalar,
		m[1] + scalar);
}

template <typename T, precision P>
tmat2x2<T, P> operator+(tmat2x2<T, P> const &m1, tmat2x2<T, P> const &m2)
{
	return tmat2x2<T, P>(
		m1[0] + m2[0],
		m1[1] + m2[1]);
}

template <typename T, precision P>
tmat2x2<T, P> operator-(tmat2x2<T, P> const &m, T scalar)
{
	return tmat2x2<T, P>(
		m[0] - scalar,
		m[1] - scalar);
}

template <typename T, precision P>
tmat2x2<T, P> operator-(T scalar, tmat2x2<T, P> const &m)
{
	return tmat2x2<T, P>(
		scalar - m[0],
		scalar - m[1]);
}

template <typename T, precision P>
tmat2x2<T, P> operator-(tmat2x2<T, P> const &m1, tmat2x2<T, P> const &m2)
{
	return tmat2x2<T, P>(
		m1[0] - m2[0],
		m1[1] - m2[1]);
}

template <typename T, precision P>
tmat2x2<T, P> operator*(tmat2x2<T, P> const &m, T scalar)
{
	return tmat2x2<T, P>(
		m[0] * scalar,
		m[1] * scalar);
}

template <typename T, precision P>
tmat2x2<T, P> operator*(T scalar, tmat2x2<T, P> const &m)
{
	return tmat2x2<T, P>(
		m[0] * scalar,
		m[1] * scalar);
}

template <typename T, precision P>
typename tmat2x2<T, P>::col_type operator*(
	tmat2x2<T, P> const &m,
	typename tmat2x2<T, P>::row_type const &v)
{
	return collision::vec2<float>(
		m[0][0] * v.x + m[1][0] * v.y,
		m[0][1] * v.x + m[1][1] * v.y);
}

template <typename T, precision P>
typename tmat2x2<T, P>::row_type operator*(
	typename tmat2x2<T, P>::col_type const &v,
	tmat2x2<T, P> const &m)
{
	return collision::vec2<float>(
		v.x * m[0][0] + v.y * m[0][1],
		v.x * m[1][0] + v.y * m[1][1]);
}

template <typename T, precision P>
tmat2x2<T, P> operator*(tmat2x2<T, P> const &m1, tmat2x2<T, P> const &m2)
{
	return tmat2x2<T, P>(
		m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1],
		m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1],
		m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1],
		m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1]);
}

template <typename T, precision P>
tmat2x2<T, P> operator/(tmat2x2<T, P> const &m, T scalar)
{
	return tmat2x2<T, P>(
		m[0] / scalar,
		m[1] / scalar);
}

template <typename T, precision P>
tmat2x2<T, P> operator/(T scalar, tmat2x2<T, P> const &m)
{
	return tmat2x2<T, P>(
		scalar / m[0],
		scalar / m[1]);
}

template <typename T, precision P>
typename tmat2x2<T, P>::col_type operator/(tmat2x2<T, P> const &m, typename tmat2x2<T, P>::row_type const &v)
{
	return detail::compute_inverse<T, P>(m) * v;
}

template <typename T, precision P>
typename tmat2x2<T, P>::row_type operator/(typename tmat2x2<T, P>::col_type const &v, tmat2x2<T, P> const &m)
{
	return v * detail::compute_inverse<T, P>(m);
}

template <typename T, precision P>
tmat2x2<T, P> operator/(tmat2x2<T, P> const &m1, tmat2x2<T, P> const &m2)
{
	tmat2x2<T, P> m1_copy(m1);
	return m1_copy /= m2;
}

// -- Boolean operators --

template <typename T, precision P>
bool operator==(tmat2x2<T, P> const &m1, tmat2x2<T, P> const &m2)
{
	return (m1[0] == m2[0]) && (m1[1] == m2[1]);
}

template <typename T, precision P>
bool operator!=(tmat2x2<T, P> const &m1, tmat2x2<T, P> const &m2)
{
	return (m1[0] != m2[0]) || (m1[1] != m2[1]);
}
} // namespace collision
