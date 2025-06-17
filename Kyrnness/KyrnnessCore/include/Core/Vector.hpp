#pragma once
#ifndef KYRNNESS_VECTOR_HPP
#define KYRNNESS_VECTOR_HPP

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "PxPhysicsAPI.h"

#include <cmath>

class FVector
{
public:
	float X, Y, Z;

	FVector() : X(0.f), Y(0.f), Z(0.f) {}
	FVector(float value) : X(value), Y(value), Z(value) {}
	FVector(float xVal, float yVal, float zVal) : X(xVal), Y(yVal), Z(zVal) {}

	FVector(const glm::vec3& value) : X(value.x), Y(value.y), Z(value.z) {}
	FVector(const physx::PxVec3& value) : X(value.x), Y(value.y), Z(value.z) {}
	FVector(const physx::PxExtendedVec3& value) : X(static_cast<float>(value.x)), Y(static_cast<float>(value.y)), Z(static_cast<float>(value.z)) {}

	operator glm::vec3() const { return glm::vec3(X, Y, Z); }
	operator physx::PxVec3() const { return physx::PxVec3(X, Y, Z); }
	operator physx::PxExtendedVec3() const { return physx::PxExtendedVec3(X, Y, Z); }

    FVector operator+(const FVector& other) const { return FVector(X + other.X, Y + other.Y, Z + other.Z); }
    FVector operator-(const FVector& other) const { return FVector(X - other.X, Y - other.Y, Z - other.Z); }
    FVector operator*(float scalar) const { return FVector(X * scalar, Y * scalar, Z * scalar); }
    FVector operator/(float scalar) const { return FVector(X / scalar, Y / scalar, Z / scalar); }
    FVector operator-() const { return FVector(-X, -Y, -Z); }

    FVector& operator+=(const FVector& other) { X += other.X; Y += other.Y; Z += other.Z; return *this; }
    FVector& operator-=(const FVector& other) { X -= other.X; Y -= other.Y; Z -= other.Z; return *this; }
    FVector& operator*=(float scalar) { X *= scalar; Y *= scalar; Z *= scalar; return *this; }
    FVector& operator/=(float scalar) { X /= scalar; Y /= scalar; Z /= scalar; return *this; }

    // Comparação
    bool operator==(const FVector& other) const { return X == other.X && Y == other.Y && Z == other.Z; }
    bool operator!=(const FVector& other) const { return !(*this == other); }

    // Utilitários
    float Length() const { return std::sqrt(X * X + Y * Y + Z * Z); }
    float LengthSquared() const { return X * X + Y * Y + Z * Z; }

    FVector Normalize() const
    {
        float len = Length();
        if (len > 0.0f)
            return FVector(X / len, Y / len, Z / len);
        return *this;
    }
    inline static FVector Normalize(const FVector& v)
    {
        return v.Normalize();
    }

    static float Dot(const FVector& a, const FVector& b)
    {
        return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
    }

    static FVector Cross(const FVector& a, const FVector& b)
    {
        return FVector(
            a.Y * b.Z - a.Z * b.Y,
            a.Z * b.X - a.X * b.Z,
            a.X * b.Y - a.Y * b.X
        );
    }

    static FVector Min(const FVector& a, const FVector& b)
    {
        return FVector(
            (a.X < b.X) ? a.X : b.X,
            (a.Y < b.Y) ? a.Y : b.Y,
            (a.Z < b.Z) ? a.Z : b.Z
        );
    }
    static FVector Max(const FVector& a, const FVector& b)
    {
        return FVector(
            (a.X > b.X) ? a.X : b.X,
            (a.Y > b.Y) ? a.Y : b.Y,
            (a.Z > b.Z) ? a.Z : b.Z
        );
    }

};

class FVector2
{
public:
    float X, Y;

    FVector2() : X(0.f), Y(0.f) {}
    FVector2(float value) : X(value), Y(value) {}
    FVector2(float xVal, float yVal) : X(xVal), Y(yVal) {}

    // Construtores para tipos externos
    FVector2(const glm::vec2& value) : X(value.x), Y(value.y) {}
    FVector2(const physx::PxVec2& value) : X(value.x), Y(value.y) {}

    // Conversões implícitas
    operator glm::vec2() const { return glm::vec2(X, Y); }
    operator physx::PxVec2() const { return physx::PxVec2(X, Y); }

    // Operadores
    FVector2 operator+(const FVector2& other) const { return FVector2(X + other.X, Y + other.Y); }
    FVector2 operator-(const FVector2& other) const { return FVector2(X - other.X, Y - other.Y); }
    FVector2 operator*(float scalar) const { return FVector2(X * scalar, Y * scalar); }
    FVector2 operator/(float scalar) const { return FVector2(X / scalar, Y / scalar); }
    FVector2 operator-() const { return FVector2(-X, -Y); }

    FVector2& operator+=(const FVector2& other) { X += other.X; Y += other.Y; return *this; }
    FVector2& operator-=(const FVector2& other) { X -= other.X; Y -= other.Y; return *this; }
    FVector2& operator*=(float scalar) { X *= scalar; Y *= scalar; return *this; }
    FVector2& operator/=(float scalar) { X /= scalar; Y /= scalar; return *this; }

    // Comparação
    bool operator==(const FVector2& other) const { return X == other.X && Y == other.Y; }
    bool operator!=(const FVector2& other) const { return !(*this == other); }

    // Utilitários
    float Length() const { return std::sqrt(X * X + Y * Y); }
    float LengthSquared() const { return X * X + Y * Y; }

    FVector2 Normalize() const
    {
        float len = Length();
        if (len > 0.0f)
            return FVector2(X / len, Y / len);
        return *this;
    }

    static float Dot(const FVector2& a, const FVector2& b)
    {
        return a.X * b.X + a.Y * b.Y;
    }

    static FVector2 Min(const FVector& a, const FVector& b)
    {
        return FVector2(
            (a.X < b.X) ? a.X : b.X,
            (a.Y < b.Y) ? a.Y : b.Y
        );
    }
    static FVector2 Max(const FVector& a, const FVector& b)
    {
        return FVector2(
            (a.X > b.X) ? a.X : b.X,
            (a.Y > b.Y) ? a.Y : b.Y
        );
    }
};

class FVector4
{
public:
    float X, Y, Z, W;

    FVector4() : X(0.f), Y(0.f), Z(0.f), W(0.f) {}
    FVector4(float value) : X(value), Y(value), Z(value), W(value) {}
    FVector4(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}

    // Construtores a partir de glm
    FVector4(const glm::vec4& value) : X(value.x), Y(value.y), Z(value.z), W(value.w) {}

    // Operadores de conversão
    operator glm::vec4() const { return glm::vec4(X, Y, Z, W); }

    // Operadores aritméticos
    FVector4 operator+(const FVector4& other) const { return FVector4(X + other.X, Y + other.Y, Z + other.Z, W + other.W); }
    FVector4 operator-(const FVector4& other) const { return FVector4(X - other.X, Y - other.Y, Z - other.Z, W - other.W); }
    FVector4 operator*(float scalar) const { return FVector4(X * scalar, Y * scalar, Z * scalar, W * scalar); }
    FVector4 operator/(float scalar) const { return FVector4(X / scalar, Y / scalar, Z / scalar, W / scalar); }
    FVector4 operator-() const { return FVector4(-X, -Y, -Z, -W); }

    FVector4& operator+=(const FVector4& other) { X += other.X; Y += other.Y; Z += other.Z; W += other.W; return *this; }
    FVector4& operator-=(const FVector4& other) { X -= other.X; Y -= other.Y; Z -= other.Z; W -= other.W; return *this; }
    FVector4& operator*=(float scalar) { X *= scalar; Y *= scalar; Z *= scalar; W *= scalar; return *this; }
    FVector4& operator/=(float scalar) { X /= scalar; Y /= scalar; Z /= scalar; W /= scalar; return *this; }

    // Comparação
    bool operator==(const FVector4& other) const { return X == other.X && Y == other.Y && Z == other.Z && W == other.W; }
    bool operator!=(const FVector4& other) const { return !(*this == other); }

    // Utilitários
    float Length() const { return std::sqrt(X * X + Y * Y + Z * Z + W * W); }
    float LengthSquared() const { return X * X + Y * Y + Z * Z + W * W; }

    FVector4 Normalize() const
    {
        float len = Length();
        if (len > 0.0f)
            return FVector4(X / len, Y / len, Z / len, W / len);
        return *this;
    }

    static inline FVector4 Normalize(const FVector4& v)
    {
        return v.Normalize();
    }

    static float Dot(const FVector4& a, const FVector4& b)
    {
        return a.X * b.X + a.Y * b.Y + a.Z * b.Z + a.W * b.W;
    }
};

//class FMatrix
//{
//public:
//    glm::mat4 matrix;
//
//    // Construtores
//    FMatrix() : matrix(1.0f) {} // Matriz identidade
//    FMatrix(const glm::mat4& mat) : matrix(mat) {}
//
//    // Conversão implícita
//    operator glm::mat4() const { return matrix; }
//
//    // Operadores aritméticos
//    FMatrix operator*(const FMatrix& other) const { return FMatrix(matrix * other.matrix); }
//
//    // Multiplicação por vetor
//    glm::vec4 operator*(const glm::vec4& vec) const { return matrix * vec; }
//    glm::vec3 operator*(const glm::vec3& vec) const { return glm::vec3(matrix * glm::vec4(vec, 1.0f)); }
//
//    // Comparação
//    bool operator==(const FMatrix& other) const { return matrix == other.matrix; }
//    bool operator!=(const FMatrix& other) const { return matrix != other.matrix; }
//
//    // Utilitários
//    FMatrix Inverse() const { return FMatrix(glm::inverse(matrix)); }
//    FMatrix Transpose() const { return FMatrix(glm::transpose(matrix)); }
//
//    static FMatrix Identity() { return FMatrix(glm::mat4(1.0f)); }
//
//    // Transformações comuns
//    static FMatrix Translate(const glm::vec3& v) { return FMatrix(glm::translate(glm::mat4(1.0f), v)); }
//    static FMatrix Rotate(float radians, const glm::vec3& axis) { return FMatrix(glm::rotate(glm::mat4(1.0f), radians, axis)); }
//    static FMatrix Scale(const glm::vec3& scale) { return FMatrix(glm::scale(glm::mat4(1.0f), scale)); }
//    static FMatrix LookAt(const FVector& eye, const FVector& target, const FVector& up)
//    {
//        FVector f = (target - eye).Normalize();
//        FVector s = FVector::Cross(f, up).Normalize();
//        FVector u = FVector::Cross(s, f);
//
//        glm::mat4 result(1.0f);
//        result[0][0] = s.X;
//        result[1][0] = s.Y;
//        result[2][0] = s.Z;
//
//        result[0][1] = u.X;
//        result[1][1] = u.Y;
//        result[2][1] = u.Z;
//
//        result[0][2] = -f.X;
//        result[1][2] = -f.Y;
//        result[2][2] = -f.Z;
//
//        result[3][0] = -FVector::Dot(s, eye);
//        result[3][1] = -FVector::Dot(u, eye);
//        result[3][2] = FVector::Dot(f, eye);
//
//        return FMatrix(result);
//    }
//};

#endif KYRNNESS_VECTOR_HPP
