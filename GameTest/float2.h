#pragma once
#include <math.h>

class float2
{
public:
	float x;
	float y;

	/// <summary>
	/// Constructs a vector.
	/// </summary>
	constexpr float2() : x(), y() {

	}
	/// <summary>
	/// Constructs a vector.
	/// </summary>
	/// <param name="x">The x vector component.</param>
	/// <param name="y">The y vector component.</param>
	constexpr float2(float x, float y) : x(x), y(y) {

	}

	/// <summary>
	/// Returns the dot product of the left and right operands.
	/// </summary>
	/// <param name="l">The left operand.</param>
	/// <param name="r">The right operand.</param>
	/// <returns>The dot product of the left and right operands.</returns>
	constexpr static float dot(const float2& l, const float2& r) {
		return l.x * r.x + l.y * r.y;
	}
	/// <summary>
	/// Returns the length of the operand.
	/// </summary>
	/// <param name="o">The operand.</param>
	/// <returns>The length of the operand.</returns>
	static float length(const float2& o) {
		return sqrtf(dot(o, o));
	}
	/// <summary>
	/// Returns the squared length of the operand.
	/// </summary>
	/// <param name="o">The operand.</param>
	/// <returns>The squared length of the operand.</returns>
	constexpr static float lengthSq(const float2& o) {
		return dot(o, o);
	}
	/// <summary>
	/// Returns the normalized version of the operand.
	/// </summary>
	/// <param name="o">The operand.</param>
	/// <returns>The normalized version of the operand.</returns>
	static float2 normalize(const float2& o) {
		return o / float2::length(o);
	}
	/// <summary>
	/// Returns the normalized version of the operand.
	/// </summary>
	/// <param name="o">The operand.</param>
	/// <param name="result">The result.</param>
	/// <returns>True if normalized.</returns>
	static bool normalizeSafe(const float2& o, float2& result) {
		float l = float2::length(o);
		if (l < 0.001f) {
			return false;
		}
		result = o / l;
		return true;
	}

	/// <summary>
	/// Returns the negative of the vector.
	/// </summary>
	/// <returns>The negative of the vector.</returns>
	constexpr float2 operator-() const {
		return float2(-x, -y);
	};
	/// <summary>
	/// Returns the per-component sum of the left and right operands.
	/// </summary>
	/// <param name="r">The right operand.</param>
	/// <returns>The per-component sum of the left and right operands.</returns>
	constexpr float2 operator +(const float2& r) const {
		return float2(x + r.x, y + r.y);
	};
	/// <summary>
	/// Returns the per-component difference of the left and right operands.
	/// </summary>
	/// <param name="r">The right operand.</param>
	/// <returns>The per-component difference of the left and right operands.</returns>
	constexpr float2 operator -(const float2& r) const {
		return float2(x - r.x, y - r.y);
	};
	/// <summary>
	/// Returns the per-component product of the left and right operands.
	/// </summary>
	/// <param name="r">The right operand.</param>
	/// <returns>The per-component product of the left and right operands.</returns>
	constexpr float2 operator *(const float2& r) const {
		return float2(x * r.x, y * r.y);
	};
	/// <summary>
	/// Returns the per-component quotient of the left and right operands.
	/// </summary>
	/// <param name="r">The right operand.</param>
	/// <returns>The per-component quotient of the left and right operands.</returns>
	constexpr float2 operator /(const float2& r) const {
		return float2(x / r.x, y / r.y);
	};

	/// <summary>
	/// Returns the per-component sum of the left and right operands.
	/// </summary>
	/// <param name="r">The right operand.</param>
	/// <returns>The per-component sum of the left and right operands.</returns>
	constexpr float2 operator +(const float& r) const {
		return float2(x + r, y + r);
	};
	/// <summary>
	/// Returns the per-component difference of the left and right operands.
	/// </summary>
	/// <param name="r">The right operand.</param>
	/// <returns>The per-component difference of the left and right operands.</returns>
	constexpr float2 operator -(const float& r) const {
		return float2(x - r, y - r);
	};
	/// <summary>
	/// Returns the per-component product of the left and right operands.
	/// </summary>
	/// <param name="r">The right operand.</param>
	/// <returns>The per-component product of the left and right operands.</returns>
	constexpr float2 operator *(const float& r) const {
		return float2(x * r, y * r);
	};
	/// <summary>
	/// Returns the per-component quotient of the left and right operands.
	/// </summary>
	/// <param name="r">The right operand.</param>
	/// <returns>The per-component quotient of the left and right operands.</returns>
	constexpr float2 operator /(const float& r) const {
		return float2(x / r, y / r);
	};

	/// <summary>
	/// Returns true if the x, and y components of the left and right operands are equal.
	/// </summary>
	/// <param name="r">The right operand.</param>
	/// <returns>True if the x, and y components of the left and right operands are equal.</returns>
	constexpr bool operator==(const float2& r) const {
		return x == r.x && y == r.y;
	}
	/// <summary>
	/// Returns true if the x, and y components of the left and right operands are not equal.
	/// </summary>
	/// <param name="r">The right operand.</param>
	/// <returns>True if the x, and y components of the left and right operands are not equal.</returns>
	constexpr bool operator!=(const float2& r) const {
		return x != r.x || y != r.y;
	}
};
