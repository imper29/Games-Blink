#pragma once
#include "float3.h"

/// <summary>
/// Column vector.
/// </summary>
struct float4
{
	float x, y, z, w;

	constexpr float4() : x(), y(), z(), w() {}
	constexpr float4(float xyzw) : x(xyzw), y(xyzw), z(xyzw), w(xyzw) {};
	constexpr float4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	constexpr float4(const float2& xy, float z, float w) : x(xy.x), y(xy.y), z(z), w(w) {}
	constexpr float4(float x, const float2& yz, float w) : x(x), y(yz.x), z(yz.y), w(w) {}
	constexpr float4(float x, float y, const float2& zw) : x(x), y(y), z(zw.x), w(zw.y) {}
	constexpr float4(const float2& xy, const float2& zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}
	constexpr float4(const float3& xyz, float w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
	constexpr float4(float x, const float3& yzw) : x(x), y(yzw.x), z(yzw.y), w(yzw.z) {}

	constexpr float4(const float3& r) : x(r.x), y(r.y), z(r.z), w() {}
	constexpr explicit operator float3() { return float3(x, y, z); }
	constexpr float4(const float2& r) : x(r.x), y(r.y), z(), w() {}
	constexpr explicit operator float2() { return float2(x, y); }

	constexpr float dot(const float4& r) const {
		return x * r.x + y * r.y + z * r.z + w * r.w;
	}
	constexpr float lengthSq() const {
		return dot(*this);
	}
	float length() const {
		return sqrtf(lengthSq());
	}
	float4 normalized() const {
		return *this / length();
	}

	constexpr float4 operator-() const {
		return float4(-x, -y, -z, -w);
	}
	constexpr float4 operator*(const float4& r) const {
		return float4(x * r.x, y * r.y, z * r.z, w * r.w);
	}
	constexpr float4 operator*(const float& r) const {
		return float4(x * r, y * r, z * r, w * r);
	}
	constexpr float4 operator/(const float4& r) const {
		return float4(x / r.x, y / r.y, z / r.z, w / r.w);
	}
	constexpr float4 operator/(const float& r) const {
		return float4(x / r, y / r, z / r, w / r);
	}
	constexpr float4 operator+(const float4& r) const {
		return float4(x + r.x, y + r.y, z + r.z, w + r.w);
	}
	constexpr float4 operator+(const float& r) const {
		return float4(x + r, y + r, z + r, w + r);
	}
	constexpr float4 operator-(const float4& r) const {
		return float4(x - r.x, y - r.y, z - r.z, w - r.w);
	}
	constexpr float4 operator-(const float& r) const {
		return float4(x - r, y - r, z - r, w - r);
	}

	constexpr bool operator==(const float4& r) const
	{
		return x == r.x && y == r.y && z == r.z && w == r.w;
	}
};
