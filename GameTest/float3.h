#pragma once
#include "float2.h"
#include "math.h"

class float3
{
public:
	float x;
	float y;
	float z;

	/// <summary>
	/// Constructs a vector.
	/// </summary>
	constexpr float3() : x(), y(), z() {

	}
	/// <summary>
	/// Constructs a vector.
	/// </summary>
	/// <param name="xy">The x and y vector components.</param>
	/// <param name="y">The z vector component.</param>
	constexpr float3(float2 xy, float z) : x(xy.x), y(xy.y), z(z) {

	}
	/// <summary>
	/// Constructs a vector.
	/// </summary>
	/// <param name="xy">The x and y vector components.</param>
	/// <param name="y">The z vector component.</param>
	constexpr float3(float x, float2 yz) : x(x), y(yz.x), z(yz.y) {

	}
	/// <summary>
	/// Constructs a vector.
	/// </summary>
	/// <param name="x">The x vector component.</param>
	/// <param name="y">The y vector component.</param>
	/// <param name="y">The z vector component.</param>
	constexpr float3(float x, float y, float z) : x(x), y(y), z(z) {

	}

	/// <summary>
	/// Returns the dot product of the left and right operands.
	/// </summary>
	/// <param name="l">The left operand.</param>
	/// <param name="r">The right operand.</param>
	/// <returns>The dot product of the left and right operands.</returns>
	constexpr static float dot(const float3& l, const float3& r) {
		return l.x * r.x + l.y * r.y + l.z * r.z;
	}
	/// <summary>
	/// Returns the length of the operand.
	/// </summary>
	/// <param name="o">The operand.</param>
	/// <returns>The length of the operand.</returns>
	static float length(const float3& o) {
		return sqrtf(float3::lengthSq(o));
	}
	/// <summary>
	/// Returns the squared length of the operand.
	/// </summary>
	/// <param name="o">The operand.</param>
	/// <returns>The squared length of the operand.</returns>
	constexpr static float lengthSq(const float3& o) {
		return float3::dot(o, o);
	}
	/// <summary>
	/// Returns the normalized version of the operand.
	/// </summary>
	/// <param name="o">The operand.</param>
	/// <returns>The normalized version of the operand.</returns>
	static float3 normalize(const float3& o) {
		return o / float3::length(o);
	}
	/// <summary>
	/// Returns the cross product of the left and right operands.
	/// </summary>
	/// <param name="l">The left operand.</param>
	/// <param name="r">The right operand.</param>
	/// <returns>The cross product of the left and right operands.</returns>
	constexpr static float3 cross(const float3& l, const float3& r) {
		return float3(l.y * r.z - l.z * r.y,
			l.z * r.x - l.x * r.z,
			l.x * r.y - l.y * r.x);
	}

	/// <summary>
	/// Returns the per-component sum of the left and right operands.
	/// </summary>
	/// <param name="r">The right operand.</param>
	/// <returns>The per-component sum of the left and right operands.</returns>
	constexpr float3 operator +(const float3& r) const {
		return float3(x + r.x, y + r.y, z + r.z);
	};
	/// <summary>
	/// Returns the per-component difference of the left and right operands.
	/// </summary>
	/// <param name="r">The right operand.</param>
	/// <returns>The per-component difference of the left and right operands.</returns>
	constexpr float3 operator -(const float3& r) const {
		return float3(x - r.x, y - r.y, z - r.z);
	};
	/// <summary>
	/// Returns the per-component product of the left and right operands.
	/// </summary>
	/// <param name="r">The right operand.</param>
	/// <returns>The per-component product of the left and right operands.</returns>
	constexpr float3 operator *(const float3& r) const {
		return float3(x * r.x, y * r.y, z * r.z);
	};
	/// <summary>
	/// Returns the per-component quotient of the left and right operands.
	/// </summary>
	/// <param name="r">The right operand.</param>
	/// <returns>The per-component quotient of the left and right operands.</returns>
	constexpr float3 operator /(const float3& r) const {
		return float3(x / r.x, y / r.y, z / r.z);
	};

	/// <summary>
	/// Returns the per-component sum of the left and right operands.
	/// </summary>
	/// <param name="r">The right operand.</param>
	/// <returns>The per-component sum of the left and right operands.</returns>
	constexpr float3 operator +(const float& r) const {
		return float3(x + r, y + r, z + r);
	}
	/// <summary>
	/// Returns the per-component difference of the left and right operands.
	/// </summary>
	/// <param name="r">The right operand.</param>
	/// <returns>The per-component difference of the left and right operands.</returns>
	constexpr float3 operator -(const float& r) const {
		return float3(x - r, y - r, z - r);
	}
	/// <summary>
	/// Returns the per-component product of the left and right operands.
	/// </summary>
	/// <param name="r">The right operand.</param>
	/// <returns>The per-component product of the left and right operands.</returns>
	constexpr float3 operator *(const float& r) const {
		return float3(x * r, y * r, z * r);
	}
	/// <summary>
	/// Returns the per-component quotient of the left and right operands.
	/// </summary>
	/// <param name="r">The right operand.</param>
	/// <returns>The per-component quotient of the left and right operands.</returns>
	constexpr float3 operator /(const float& r) const {
		return float3(x / r, y / r, z / r);
	}

	/// <summary>
	/// Returns true if the x, y, and z components of the left and right operands are equal.
	/// </summary>
	/// <param name="r">The right operand.</param>
	/// <returns>True if the x, y, and z components of the left and right operands are equal.</returns>
	constexpr bool operator==(const float3& r) const {
		return x == r.x && y == r.y && z == r.z;
	}
	/// <summary>
	/// Returns true if the x, y, and z components of the left and right operands are not equal.
	/// </summary>
	/// <param name="r">The right operand.</param>
	/// <returns>True if the x, y, and z components of the left and right operands are not equal.</returns>
	constexpr bool operator!=(const float3& r) const {
		return x != r.x || y != r.y || z != r.z;
	}
};
