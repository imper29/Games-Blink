#pragma once
#include "float2.h"
#include "float3.h"

/// <summary>
/// Row-major 3x3 matrix.
/// </summary>
class float3x3
{
private:
	static constexpr float det2x2(float a, float b, float c, float d) {
		return a * d - b * c;
	}

public:
	float3 x;
	float3 y;
	float3 z;

	/// <summary>
	/// Constructs the matrix.
	/// </summary>
	constexpr float3x3() : x(), y(), z() {

	}
	/// <summary>
	/// Constructs the matrix.
	/// </summary>
	/// <param name="x">The 1st row.</param>
	/// <param name="y">The 2nd row.</param>
	/// <param name="z">The 3rd row.</param>
	constexpr float3x3(float3 x, float3 y, float3 z) : x(x), y(y), z(z) {

	}

	/// <summary>
	/// Returns the determinant of the matrix.
	/// </summary>
	/// <returns>The determinant of the matrix.</returns>
	constexpr float determinant() const {
		return x.x * float3x3::det2x2(y.y, y.z, z.y, z.z) + x.y * float3x3::det2x2(y.x, y.z, z.x, z.z) + x.z * float3x3::det2x2(y.x, y.y, z.x, z.y);
	}
	/// <summary>
	/// Returns the adjugate of the matrix.
	/// </summary>
	/// <returns>The adjugate of the matrix.</returns>
	constexpr float3x3 adjugate() const {
		float3x3 t = float3x3(float3(x.x, y.x, z.x),
			float3(x.y, y.y, z.y),
			float3(x.z, y.z, z.z));
		return float3x3(float3(float3x3::det2x2(t.y.y, t.y.z, t.z.y, t.z.z), -float3x3::det2x2(t.y.x, t.y.z, t.z.x, t.z.z), float3x3::det2x2(t.y.x, t.y.y, t.z.x, t.z.y)),
			float3(-float3x3::det2x2(t.x.y, t.x.z, t.z.y, t.z.z), float3x3::det2x2(t.x.x, t.x.z, t.z.x, t.z.z), -float3x3::det2x2(t.x.x, t.x.y, t.z.x, t.z.y)),
			float3(float3x3::det2x2(t.x.y, t.x.z, t.y.y, t.y.z), -float3x3::det2x2(t.x.x, t.x.z, t.y.x, t.y.z), float3x3::det2x2(t.x.x, t.x.y, t.y.x, t.y.y)));
	}
	/// <summary>
	/// Returns the inverse of the matrix.
	/// </summary>
	/// <returns>The inverse of the matrix.</returns>
	constexpr float3x3 inverse() const {
		float3x3 r = adjugate();
		float d = determinant();
		return float3x3(r.x / d, r.y / d, r.z / d);
	}

	/// <summary>
	/// Returns the product of the left and right operands.
	/// </summary>
	/// <param name="r">The right operand.</param>
	/// <returns>The product of the left and right operands.</returns>
	constexpr float3x3 operator *(const float3x3& r) const {
		float3x3 lt = float3x3(float3(x.x, y.x, z.x),
			float3(x.y, y.y, z.y),
			float3(x.z, y.z, z.z));
		return float3x3(float3(float3::dot(lt.x, r.x), float3::dot(lt.y, r.x), float3::dot(lt.z, r.x)),
			float3(float3::dot(lt.x, r.y), float3::dot(lt.y, r.y), float3::dot(lt.z, r.y)),
			float3(float3::dot(lt.x, r.z), float3::dot(lt.y, r.z), float3::dot(lt.z, r.z)));
	}
	/// <summary>
	/// Returns the product of the left and right operands.
	/// </summary>
	/// <param name="r">The right operand.</param>
	/// <returns>The product of the left and right operands.</returns>
	constexpr float3 operator *(const float3& r) const {
		return float3(float3::dot(x, r), float3::dot(y, r), float3::dot(z, r));
	}
};
