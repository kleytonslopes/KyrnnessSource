/*********************************************************************
 *   File: Math.hpp
 *  Brief:
 *
 * Author: Kleyton Lopes
 *   Date: junho 2025
 *
 * Copyright (c) 2025 Kyrnness. All rights reserved.
 *********************************************************************/

#pragma once
#ifndef K_MATH_HPP
#define K_MATH_HPP

#include "Core/Vector.hpp"
#include <cmath>
#include <limits>
#include <glm/gtc/noise.hpp>

class FMath
{
public:
	static inline float Abs(float value) { return std::fabs(value); }
	static inline float Cos(float value) { return std::cos(value); }
	static inline float Sin(float value) { return std::sin(value); }
	static inline float Tan(float value) { return std::tan(value); }
	static inline float Acos(float value) { return std::acos(value); }
	static inline float Asin(float value) { return std::asin(value); }
	static inline float Atan(float value) { return std::atan(value); }
	static inline float Atan2(float y, float x) { return std::atan2(y, x); }

	static inline float Sqrt(float value) { return std::sqrt(value); }
	static inline float Pow(float base, float exponent) { return std::pow(base, exponent); }
	static inline float Log(float value) { return std::log(value); }
	static inline float Exp(float value) { return std::exp(value); }

	static inline float Radians(float degrees) { return degrees * (3.14159265358979323846f / 180.0f); }
	static inline float Degrees(float radians) { return radians * (180.0f / 3.14159265358979323846f); }

	static inline float Clamp(float value, float min, float max)
	{
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

	static inline float Lerp(float a, float b, float t)
	{
		return a + t * (b - a);
	}

	static inline bool IsNearlyEqual(float a, float b, float epsilon = std::numeric_limits<float>::epsilon())
	{
		return std::fabs(a - b) <= epsilon;
	}

	static inline int32_t Fade(int32_t t)
	{
		// Fun��o fade suave 6t^5 - 15t^4 + 10t^3
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	static inline float Fade(float t)
	{
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	static inline int32_t Grad(int32_t hash, float x, float y)
	{
		int32_t h = hash & 7;      // usa os 3 bits menos significativos
		float u = h < 4 ? x : y;
		float v = h < 4 ? y : x;
		return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
	}

	static float PerlinGLM(const FVector2& value)
	{
		return glm::perlin(glm::vec2{ value });
	}

	static float Perlin(float x, float y)
	{
		// Encontra o "grid cell" que o ponto est�
		int X = (int)std::floor(x) & 255;
		int Y = (int)std::floor(y) & 255;

		// Posi��es relativas dentro da c�lula
		float xf = x - std::floor(x);
		float yf = y - std::floor(y);

		float u = Fade(xf);
		float v = Fade(yf);

		// Hash dos cantos da c�lula
		int aa = permutation[permutation[X] + Y];
		int ab = permutation[permutation[X] + Y + 1];
		int ba = permutation[permutation[X + 1] + Y];
		int bb = permutation[permutation[X + 1] + Y + 1];

		// Interpola��o do gradiente nos 4 cantos
		float x1 = Lerp(Grad(aa, xf, yf), Grad(ba, xf - 1, yf), u);
		float x2 = Lerp(Grad(ab, xf, yf - 1), Grad(bb, xf - 1, yf - 1), u);
		float result = Lerp(x1, x2, v);

		return result;
	}

private:
	

	static const int permutation[256];
};

#endif //K_MATH_HPP
