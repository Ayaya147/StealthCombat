#pragma once
#include <DirectXMath.h>
#include <cmath>

//XMFLOAT3
static inline void operator+= (DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;
}

static inline void operator-= (DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;
}

static inline void operator*= (DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
	v1.x *= v2.x;
	v1.y *= v2.y;
	v1.z *= v2.z;
}

static inline void operator/= (DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
	v1.x /= v2.x;
	v1.y /= v2.y;
	v1.z /= v2.z;
}

static inline void operator%= (DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
	v1.x = ::fmodf(v1.x, v2.x);
	v1.y = ::fmodf(v1.y, v2.y);
	v1.z = ::fmodf(v1.z, v2.z);
}

_NODISCARD static inline constexpr auto operator+ (const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
	return DirectX::XMFLOAT3{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

_NODISCARD static inline constexpr auto operator- (const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
	return DirectX::XMFLOAT3{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

_NODISCARD static inline constexpr auto operator* (const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
	return DirectX::XMFLOAT3{ v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
}

_NODISCARD static inline constexpr auto operator/ (const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
	return DirectX::XMFLOAT3{ v1.x / v2.x, v1.y / v2.y, v1.z / v2.z };
}

_NODISCARD static inline constexpr auto operator% (const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
	return DirectX::XMFLOAT3{ ::fmodf(v1.x, v2.x), ::fmodf(v1.y, v2.y), ::fmodf(v1.z, v2.z) };
}

static inline void operator+= (DirectX::XMFLOAT3& v1, const float num)
{
	v1.x += num;
	v1.y += num;
	v1.z += num;
}

static inline void operator-= (DirectX::XMFLOAT3& v1, const float num)
{
	v1.x -= num;
	v1.y -= num;
	v1.z -= num;
}

static inline void operator/= (DirectX::XMFLOAT3& v1, const float num)
{
	v1.x /= num;
	v1.y /= num;
	v1.z /= num;
}

static inline void operator*= (DirectX::XMFLOAT3& v1, const float num)
{
	v1.x *= num;
	v1.y *= num;
	v1.z *= num;
}

static inline void operator%= (DirectX::XMFLOAT3& v1, const float num)
{
	v1.x = ::fmodf(v1.x, num);
	v1.y = ::fmodf(v1.y, num);
	v1.z = ::fmodf(v1.z, num);
}

_NODISCARD static inline constexpr auto operator+ (const DirectX::XMFLOAT3& v1, const float num)
{
	return DirectX::XMFLOAT3{ v1.x + num, v1.y + num, v1.z + num };
}

_NODISCARD static inline constexpr auto operator- (const DirectX::XMFLOAT3& v1, const float num)
{
	return DirectX::XMFLOAT3{ v1.x - num, v1.y - num, v1.z - num };
}

_NODISCARD static inline constexpr auto operator* (const DirectX::XMFLOAT3& v1, const float num)
{
	return DirectX::XMFLOAT3{ v1.x * num, v1.y * num, v1.z * num };
}

_NODISCARD static inline constexpr auto operator% (const DirectX::XMFLOAT3& v1, const float num)
{
	return DirectX::XMFLOAT3{ ::fmodf(v1.x, num), ::fmodf(v1.y, num), ::fmodf(v1.z, num) };
}

_NODISCARD static inline constexpr auto operator/ (const DirectX::XMFLOAT3& v1, const float num)
{
	return DirectX::XMFLOAT3{ v1.x / num, v1.y / num, v1.z / num };
}

_NODISCARD static inline constexpr auto operator+ (const float num, DirectX::XMFLOAT3& v1)
{
	return DirectX::XMFLOAT3{ num + v1.x, num + v1.y, num + v1.z };
}

_NODISCARD static inline constexpr auto operator- (const float num, DirectX::XMFLOAT3& v1)
{
	return DirectX::XMFLOAT3{ num - v1.x, num - v1.y, num - v1.z };
}

_NODISCARD static inline constexpr auto operator* (const float num, DirectX::XMFLOAT3& v1)
{
	return DirectX::XMFLOAT3{ num * v1.x, num * v1.y, num * v1.z };
}

_NODISCARD static inline constexpr auto operator/ (const float num, DirectX::XMFLOAT3& v1)
{
	return DirectX::XMFLOAT3{ num / v1.x, num / v1.y, num / v1.z };
}

_NODISCARD static inline constexpr auto operator% (const float num, DirectX::XMFLOAT3& v1)
{
	return DirectX::XMFLOAT3{ fmodf(num, v1.x), fmodf(num, v1.y), fmodf(num, v1.z) };
}

_NODISCARD static inline constexpr bool operator< (const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
	return ((v1.x < v2.x) && (v1.y < v2.y) && (v1.z < v2.z));
}

_NODISCARD static inline constexpr bool operator> (const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
	return ((v1.x > v2.x) && (v1.y > v2.y) && (v1.z > v2.z));
}

_NODISCARD static inline constexpr auto operator- (const DirectX::XMFLOAT3& v1)
{
	return DirectX::XMFLOAT3{ -v1.x, -v1.y, -v1.z };
}

//XMFLOAT2
static inline void operator+= (DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
}

static inline void operator-= (DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
}

static inline void operator/= (DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
	v1.x /= v2.x;
	v1.y /= v2.y;
}

static inline void operator*= (DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
	v1.x *= v2.x;
	v1.y *= v2.y;
}

static inline void operator%= (DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
	v1.x = ::fmodf(v1.x, v2.x);
	v1.y = ::fmodf(v1.y, v2.y);
}

_NODISCARD static inline constexpr auto operator+ (const DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
	return DirectX::XMFLOAT2{ v1.x + v2.x, v1.y + v2.y };
}

_NODISCARD static inline constexpr auto operator- (const DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
	return DirectX::XMFLOAT2{ v1.x - v2.x, v1.y - v2.y };
}

_NODISCARD static inline constexpr auto operator* (const DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
	return DirectX::XMFLOAT2{ v1.x * v2.x, v1.y * v2.y };
}

_NODISCARD static inline constexpr auto operator/ (const DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
	return DirectX::XMFLOAT2{ v1.x / v2.x, v1.y / v2.y };
}

_NODISCARD static inline constexpr auto operator% (const DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
	return DirectX::XMFLOAT2{ ::fmodf(v1.x, v2.x), ::fmodf(v1.y, v2.y) };
}

static inline void operator+= (DirectX::XMFLOAT2& v1, const float num)
{
	v1.x += num;
	v1.y += num;
}

static inline void operator-= (DirectX::XMFLOAT2& v1, const float num)
{
	v1.x -= num;
	v1.y -= num;
}

static inline void operator/= (DirectX::XMFLOAT2& v1, const float num)
{
	v1.x /= num;
	v1.y /= num;
}

static inline void operator*= (DirectX::XMFLOAT2& v1, const float num)
{
	v1.x *= num;
	v1.y *= num;
}

static inline void operator%= (DirectX::XMFLOAT2& v1, const float num)
{
	v1.x = ::fmodf(v1.x, num);
	v1.y = ::fmodf(v1.y, num);
}

_NODISCARD static inline constexpr auto operator+ (const DirectX::XMFLOAT2& v1, const float num)
{
	return DirectX::XMFLOAT2{ v1.x + num, v1.y + num };
}

_NODISCARD static inline constexpr auto operator- (const DirectX::XMFLOAT2& v1, const float num)
{
	return DirectX::XMFLOAT2{ v1.x - num, v1.y - num };
}

_NODISCARD static inline constexpr auto operator* (const DirectX::XMFLOAT2& v1, const float num)
{
	return DirectX::XMFLOAT2{ v1.x * num, v1.y * num };
}

_NODISCARD static inline constexpr auto operator/ (const DirectX::XMFLOAT2& v1, const float num)
{
	return DirectX::XMFLOAT2{ v1.x / num, v1.y / num };
}

_NODISCARD static inline constexpr auto operator% (const DirectX::XMFLOAT2& v1, const float num)
{
	return DirectX::XMFLOAT2{ ::fmodf(v1.x, num), ::fmodf(v1.y, num) };
}

_NODISCARD static inline constexpr auto operator+ (const float num, DirectX::XMFLOAT2& v1)
{
	return DirectX::XMFLOAT2{ num + v1.x, num + v1.y };
}

_NODISCARD static inline constexpr auto operator- (const float num, DirectX::XMFLOAT2& v1)
{
	return DirectX::XMFLOAT2{ num - v1.x, num - v1.y };
}

_NODISCARD static inline constexpr auto operator* (const float num, DirectX::XMFLOAT2& v1)
{
	return DirectX::XMFLOAT2{ num * v1.x, num * v1.y };
}

_NODISCARD static inline constexpr auto operator/ (const float num, DirectX::XMFLOAT2& v1)
{
	return DirectX::XMFLOAT2{ num / v1.x, num / v1.y };
}

_NODISCARD static inline constexpr auto operator% (const float num, DirectX::XMFLOAT2& v1)
{
	return DirectX::XMFLOAT2{ fmodf(num, v1.x), fmodf(num, v1.y) };
}

_NODISCARD static inline constexpr bool operator< (const DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
	return ((v1.x < v2.x) && (v1.y < v2.y));
}

_NODISCARD static inline constexpr bool operator> (const DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
	return ((v1.x > v2.x) && (v1.y > v2.y));
}

_NODISCARD static inline constexpr auto operator- (const DirectX::XMFLOAT2& v1)
{
	return DirectX::XMFLOAT2{ -v1.x, -v1.y };
}

namespace DXMath
{
	//XMFLOAT3
	_NODISCARD static inline float LengthSq(const DirectX::XMFLOAT3& vf3)
	{
		float len{};

		auto&& vec{ DirectX::XMLoadFloat3(&vf3) };

		DirectX::XMStoreFloat(&len, DirectX::XMVector3LengthSq(vec));

		return len;
	}

	_NODISCARD static inline float Length(const DirectX::XMFLOAT3& vf3)
	{
		float len{};

		auto&& vec{ DirectX::XMLoadFloat3(&vf3) };

		DirectX::XMStoreFloat(&len, DirectX::XMVector3Length(vec));

		return len;
	}

	_NODISCARD static inline auto Dot(const DirectX::XMFLOAT3& vec1, const DirectX::XMFLOAT3& vec2)
	{
		float rv{};

		const auto&& v1{ DirectX::XMLoadFloat3(&vec1) }, &&v2{ DirectX::XMLoadFloat3(&vec2) };

		DirectX::XMStoreFloat(&rv, DirectX::XMVector3Dot(v1, v2));

		return rv;
	}

	_NODISCARD static inline auto Cross(const DirectX::XMFLOAT3& vec1, const DirectX::XMFLOAT3& vec2)
	{
		DirectX::XMFLOAT3 rv{};

		const auto&& v1{ DirectX::XMLoadFloat3(&vec1) }, &&v2{ DirectX::XMLoadFloat3(&vec2) };

		DirectX::XMStoreFloat3(&rv, DirectX::XMVector3Cross(v1, v2));

		return rv;
	}

	_NODISCARD static inline auto Normalize(const DirectX::XMFLOAT3& vf3)
	{
		DirectX::XMFLOAT3 rv{ vf3 };

		auto&& vec{ DirectX::XMLoadFloat3(&rv) };

		DirectX::XMStoreFloat3(&rv, DirectX::XMVector3Normalize(vec));

		return rv;
	}

	//XMFLOAT2
	_NODISCARD static inline float LengthSq(const DirectX::XMFLOAT2& vf2)
	{
		float len{};

		auto&& vec{ DirectX::XMLoadFloat2(&vf2) };

		DirectX::XMStoreFloat(&len, DirectX::XMVector2LengthSq(vec));

		return len;
	}

	_NODISCARD static inline float Length(const DirectX::XMFLOAT2& vf2)
	{
		float len{};

		auto&& vec{ DirectX::XMLoadFloat2(&vf2) };

		DirectX::XMStoreFloat(&len, DirectX::XMVector2Length(vec));

		return len;
	}

	_NODISCARD static inline auto Dot(const DirectX::XMFLOAT2& vec1, const DirectX::XMFLOAT2& vec2)
	{
		float rv{};

		const auto&& v1{ DirectX::XMLoadFloat2(&vec1) }, &&v2{ DirectX::XMLoadFloat2(&vec2) };

		DirectX::XMStoreFloat(&rv, DirectX::XMVector2Dot(v1, v2));

		return rv;
	}

	_NODISCARD static inline auto Cross(const DirectX::XMFLOAT2& vec1, const DirectX::XMFLOAT2& vec2)
	{
		float rv{};

		const auto&& v1{ DirectX::XMLoadFloat2(&vec1) }, &&v2{ DirectX::XMLoadFloat2(&vec2) };

		DirectX::XMStoreFloat(&rv, DirectX::XMVector2Cross(v1, v2));

		return rv;
	}

	_NODISCARD static inline auto Normalize(const DirectX::XMFLOAT2& vf2)
	{
		DirectX::XMFLOAT2 rv{ vf2 };

		auto&& vec{ DirectX::XMLoadFloat2(&rv) };

		DirectX::XMStoreFloat2(&rv, DirectX::XMVector2Normalize(vec));

		return rv;
	}
}
