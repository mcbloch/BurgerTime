#pragma once
#include "MathUtilities.h"

#include <iostream>

namespace dae
{
#define ZeroVector2 Vector2()
#define UnitVector2 Vector2(1.f,1.f)

	//Vector 2D
	struct Vector2
	{
		//=== Data Members ===
		float x = 0.0f;
		float y = 0.0f;

		//=== Constructors ===
		Vector2() = default;

		Vector2(const float _x, const float _y) : x(_x), y(_y)
		{
		}

		//=== Arithmetic Operators ===
		auto operator-(const Vector2& v) const
		{
			return Vector2(x - v.x, y - v.y);
		}

		auto operator-() const
		{
			return Vector2(-x, -y);
		}

		auto operator*(float scale) const
		{
			return Vector2(x * scale, y * scale);
		}

		auto operator/(float scale) const
		{
			const auto revScale = 1.0f / scale;
			return Vector2(x * revScale, y * revScale);
		}

		//=== Compound Assignment Operators === //auto& for type deduction
		auto& operator+=(const Vector2& v)
		{
			x += v.x;
			y += v.y;
			return *this;
		}

		auto& operator-=(const Vector2& v)
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}

		auto& operator*=(const float scale)
		{
			x *= scale;
			y *= scale;
			return *this;
		}

		auto& operator/=(const float scale)
		{
			const auto revScale = 1.0f / scale;
			x *= revScale;
			y *= revScale;
			return *this;
		}


		//=== Relational Operators ===
		auto operator==(const Vector2& v) const /*Check if both components are equal*/
		{
			return AreEqual(x, v.x) && AreEqual(y, v.y);
		}

		auto operator!=(const Vector2& v) const /*Check if one or both components are NOT equal*/
		{
			return !(*this == v);
		}

		//=== Member Access Operators ===
		float operator[](const unsigned int i) const
		{
			return ((i == 0) ? x : y);
			//if (i >= 0 && i < 2)
			//	return ((i == 0) ? x : y);
			//throw; /*TODO : specify error thrown;*/
		}

		float& operator[](const unsigned int i)
		{
			return ((i == 0) ? x : y);
			//if (i >= 0 && i < 2)
			//	return ((i == 0) ? x : y);
			//throw; /*TODO : specify error thrown;*/
		}

		//=== Internal Vector Functions ===
		[[nodiscard]] auto Dot(const Vector2& v) const
		{
			return x * v.x + y * v.y;
		}

		[[nodiscard]] auto Cross(const Vector2& v) const
		{
			return x * v.y - y * v.x;
		}

		[[nodiscard]] auto GetAbs() const
		{
			return Vector2(abs(x), abs(y));
		}

		[[nodiscard]] auto MagnitudeSquared() const
		{
			return x * x + y * y;
		}

		[[nodiscard]] auto Magnitude() const
		{
			return sqrtf(MagnitudeSquared());
		}

		float Normalize()
		{
			const auto m = Magnitude();
			if (AreEqual(m, 0.f))
			{
				*this = ZeroVector2;
				return 0.f;
			}

			auto invM = 1.f / m;
			x *= invM;
			y *= invM;

			return m;
		}

		[[nodiscard]] Vector2 GetNormalized() const
		/*! Returns a normalized copy of this vector. This vector does not change.*/
		{
			auto v = Vector2(*this);
			v.Normalize();
			return v;
		}

		[[nodiscard]] auto DistanceSquared(const Vector2& v) const
		{
			return Square(v.x - x) + Square(v.y - y);
		}

		[[nodiscard]] auto Distance(const Vector2& v) const
		{
			return sqrtf(DistanceSquared(v));
		}

		auto Clamp(float max)
		{
			auto scale = max / Magnitude();
			scale      = scale < 1.f ? scale : 1.f;
			return *this * scale;
		}
	};

	//=== Global Vector Operators ===
#pragma region GlobalVectorOperators
	inline auto operator+(const Vector2& v, const Vector2& v2)
	{
		return Vector2(v.x + v2.x, v.y + v2.y);
	}

	inline auto operator*(float s, const Vector2& v)
	{
		return Vector2(s * v.x, s * v.y);
	}

	inline auto operator*(const Vector2& a, const Vector2& b)
	{
		return Vector2(a.x * b.x, a.y * b.y);
	}

	inline auto operator/(float s, const Vector2& v)
	{
		const auto revScale = 1.0f / s;
		return Vector2(revScale * v.x, revScale * v.y);
	}

	inline std::ostream& operator<<(std::ostream& os, const Vector2& rhs)
	{
		os << "(" << rhs.x << ", " << rhs.y << " )";
		return os;
	}
#pragma endregion //GlobalVectorOperators

	//=== Global Vector Functions ===
#pragma region GlobalVectorFunctions
	inline auto Dot(const Vector2& v1, const Vector2& v2)
	{
		return v1.Dot(v2);
	}

	inline auto Cross(const Vector2& v1, const Vector2& v2)
	{
		return v1.Cross(v2);
	}

	inline auto GetAbs(const Vector2& v)
	{
		return v.GetAbs();
	}

	inline void Abs(Vector2& v) /*! Make absolute Vector2 of this Vector2 */
	{
		v = v.GetAbs();
	}

	inline void Normalize(Vector2& v)
	{
		v.Normalize();
	}

	inline auto GetNormalized(const Vector2& v)
	{
		return v.GetNormalized();
	}

	inline auto DistanceSquared(const Vector2& v1, const Vector2& v2)
	{
		return v1.DistanceSquared(v2);
	}

	inline auto Distance(const Vector2& v1, const Vector2& v2)
	{
		return v1.Distance(v2);
	}

	inline auto Clamp(const Vector2& v, float max)
	{
		auto scale = max / v.Magnitude();
		scale      = scale < 1.f ? scale : 1.f;
		return v * scale;
	}
#pragma endregion //GlobalVectorFunctions

#pragma region ExtraFunctions
	/*! Random Vector2 */
	inline Vector2 randomVector2(float max = 1.f)
	{
		return {RandomBinomial(max), RandomBinomial(max)};
	}

	inline Vector2 randomVector2(float min, float max)
	{
		return {RandomFloat(min, max), RandomFloat(min, max)};
	}

	/*! Orientation to a Vector2 */
	inline Vector2 OrientationToVector(float orientation)
	{
		orientation -= static_cast<float>(PI_2);
		return Vector2(cos(orientation), sin(orientation));
	}

	/*! Get orientation from an a velocity vector */
	inline float GetOrientationFromVelocity(const Vector2& velocity)
	{
		if (velocity.Magnitude() == 0)
			return 0.f;

		return atan2f(velocity.x, -velocity.y);
	}

	/*! Get Angle Between 2 vectors*/
	inline float AngleBetween(const Vector2& v1, const Vector2& v2)
	{
		float x = v1.Dot(v2);
		float y = v1.Cross(v2);
		return atan2(y, x);
	}

#pragma endregion //ExtraFunctions
}
