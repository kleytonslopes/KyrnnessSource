#pragma once
#ifndef KYRNNESS_QUATERNION_HPP
#define KYRNNESS_QUATERNION_HPP

#include "Core/Vector.hpp"
#include "Core/Math.hpp"
#include <cmath>
#include <PxPhysicsAPI.h>

class FQuaternion
{
public:
	float X, Y, Z, W;

	FQuaternion() : X(0.f), Y(0.f), Z(0.f), W(1.f) {}
	FQuaternion(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}

	FQuaternion(const physx::PxQuat& quat): X(quat.x), Y(quat.y), Z(quat.z), W(quat.w) {}

	// Construtor a partir de um vetor de Euler (graus)
	FQuaternion(const FVector& eulerDegrees)
	{
		FVector radians = FVector(
			FMath::Radians(eulerDegrees.X),
			FMath::Radians(eulerDegrees.Y),
			FMath::Radians(eulerDegrees.Z)
		);

		float cx = FMath::Cos(radians.X * 0.5f);
		float sx = FMath::Sin(radians.X * 0.5f);
		float cy = FMath::Cos(radians.Y * 0.5f);
		float sy = FMath::Sin(radians.Y * 0.5f);
		float cz = FMath::Cos(radians.Z * 0.5f);
		float sz = FMath::Sin(radians.Z * 0.5f);

		W = cx * cy * cz + sx * sy * sz;
		X = sx * cy * cz - cx * sy * sz;
		Y = cx * sy * cz + sx * cy * sz;
		Z = cx * cy * sz - sx * sy * cz;
	}

	// Converte para glm::quat (caso ainda use glm internamente em algum ponto)
	operator glm::quat() const
	{
		return glm::quat(W, X, Y, Z);
	}

	// Conversão implícita para PxQuat
	operator physx::PxQuat() const
	{
		return physx::PxQuat(X, Y, Z, W);
	}
	// Atribuição de PxQuat
	FQuaternion& operator=(const physx::PxQuat& quat)
	{
		X = quat.x;
		Y = quat.y;
		Z = quat.z;
		W = quat.w;
		return *this;
	}

	// Multiplicação de quaternions (rotação composta)
	FQuaternion operator*(const FQuaternion& q) const
	{
		return FQuaternion(
			W * q.X + X * q.W + Y * q.Z - Z * q.Y,
			W * q.Y - X * q.Z + Y * q.W + Z * q.X,
			W * q.Z + X * q.Y - Y * q.X + Z * q.W,
			W * q.W - X * q.X - Y * q.Y - Z * q.Z
		);
	}

	// Rotaciona um vetor por este quaternion
	FVector RotateVector(const FVector& v) const
	{
		FVector qVec(X, Y, Z);
		FVector uv = FVector::Cross(qVec, v);
		FVector uuv = FVector::Cross(qVec, uv);
		uv *= (2.0f * W);
		uuv *= 2.0f;
		return v + uv + uuv;
	}

	// Retorna o conjugado do quaternion
	FQuaternion Conjugate() const
	{
		return FQuaternion(-X, -Y, -Z, W);
	}

	// Normaliza o quaternion
	void Normalize()
	{
		float mag = std::sqrt(X * X + Y * Y + Z * Z + W * W);
		if (mag > 0.0f)
		{
			X /= mag;
			Y /= mag;
			Z /= mag;
			W /= mag;
		}
	}

	// Retorna o quaternion normalizado
	FQuaternion Normalized() const
	{
		FQuaternion q = *this;
		q.Normalize();
		return q;
	}
};

#endif // KYRNNESS_QUATERNION_HPP
