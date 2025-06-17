#pragma once
#ifndef KYRNNESS_MATRIX_HPP
#define KYRNNESS_MATRIX_HPP

#include "Core/Vector.hpp"

class FMatrix
{
public:
    float M[4][4];

    // Construtores
    FMatrix() { *this = Identity(); }

    FMatrix(float diagonal)
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                M[i][j] = (i == j) ? diagonal : 0.0f;
    }

    FMatrix(const glm::mat4& mat)
    {
        for (int row = 0; row < 4; ++row)
            for (int col = 0; col < 4; ++col)
                M[row][col] = mat[col][row]; // glm::mat4 usa column-major
    }

    // Operadores aritméticos
    FMatrix operator*(const FMatrix& other) const
    {
        FMatrix result(0.0f);
        for (int row = 0; row < 4; ++row)
            for (int col = 0; col < 4; ++col)
                for (int k = 0; k < 4; ++k)
                    result.M[row][col] += M[row][k] * other.M[k][col];
        return result;
    }


    // Multiplicação por vetor
    FVector4 operator*(const FVector4& vec) const
    {
        float x = M[0][0] * vec.X + M[0][1] * vec.Y + M[0][2] * vec.Z + M[0][3] * vec.W;
        float y = M[1][0] * vec.X + M[1][1] * vec.Y + M[1][2] * vec.Z + M[1][3] * vec.W;
        float z = M[2][0] * vec.X + M[2][1] * vec.Y + M[2][2] * vec.Z + M[2][3] * vec.W;
        float w = M[3][0] * vec.X + M[3][1] * vec.Y + M[3][2] * vec.Z + M[3][3] * vec.W;
        return FVector4(x, y, z, w);
    }

    FVector operator*(const FVector& vec) const
    {
        FVector4 temp = (*this) * FVector4(vec.X, vec.Y, vec.Z, 1.0f);
        return FVector(temp.X, temp.Y, temp.Z);
    }

    // Comparação
    bool operator==(const FMatrix& other) const
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                if (M[i][j] != other.M[i][j])
                    return false;
        return true;
    }

    bool operator!=(const FMatrix& other) const { return !(*this == other); }

    operator glm::mat4() const {
        glm::mat4 result(1.0f);
        for (int row = 0; row < 4; ++row)
            for (int col = 0; col < 4; ++col)
                result[col][row] = M[row][col]; // glm é column-major
        return result;
    }

    // Utilitários
    static FMatrix Identity() { return FMatrix(1.0f); }

    // Transformações
    static FMatrix Translate(const FVector& v)
    {
        FMatrix result = Identity();
        result.M[0][3] = v.X;
        result.M[1][3] = v.Y;
        result.M[2][3] = v.Z;
        return result;
    }
    static FMatrix Translate(const FMatrix& matrix, const FVector& translation)
    {
        FMatrix result = matrix;
        result.M[3][0] += translation.X * result.M[0][0] + translation.Y * result.M[1][0] + translation.Z * result.M[2][0];
        result.M[3][1] += translation.X * result.M[0][1] + translation.Y * result.M[1][1] + translation.Z * result.M[2][1];
        result.M[3][2] += translation.X * result.M[0][2] + translation.Y * result.M[1][2] + translation.Z * result.M[2][2];
        result.M[3][3] += translation.X * result.M[0][3] + translation.Y * result.M[1][3] + translation.Z * result.M[2][3];
        return result;
    }

    static FMatrix Scale(const FMatrix& matrix, const FVector& scale)
    {
        FMatrix result = matrix;
        result.M[0][0] *= scale.X;
        result.M[0][1] *= scale.X;
        result.M[0][2] *= scale.X;
        result.M[0][3] *= scale.X;

        result.M[1][0] *= scale.Y;
        result.M[1][1] *= scale.Y;
        result.M[1][2] *= scale.Y;
        result.M[1][3] *= scale.Y;

        result.M[2][0] *= scale.Z;
        result.M[2][1] *= scale.Z;
        result.M[2][2] *= scale.Z;
        result.M[2][3] *= scale.Z;

        return result;
    }

    static FMatrix Scale(const FVector& scale)
    {
        FMatrix result = Identity();
        result.M[0][0] = scale.X;
        result.M[1][1] = scale.Y;
        result.M[2][2] = scale.Z;
        return result;
    }

    static FMatrix Rotate(float radians, const FVector& axis)
    {
        FVector a = axis.Normalize();
        float c = cosf(radians);
        float s = sinf(radians);
        float t = 1.0f - c;

        FMatrix result;

        result.M[0][0] = c + a.X * a.X * t;
        result.M[1][0] = a.X * a.Y * t - a.Z * s;
        result.M[2][0] = a.X * a.Z * t + a.Y * s;
        result.M[3][0] = 0.0f;

        result.M[0][1] = a.Y * a.X * t + a.Z * s;
        result.M[1][1] = c + a.Y * a.Y * t;
        result.M[2][1] = a.Y * a.Z * t - a.X * s;
        result.M[3][1] = 0.0f;

        result.M[0][2] = a.Z * a.X * t - a.Y * s;
        result.M[1][2] = a.Z * a.Y * t + a.X * s;
        result.M[2][2] = c + a.Z * a.Z * t;
        result.M[3][2] = 0.0f;

        result.M[0][3] = 0.0f;
        result.M[1][3] = 0.0f;
        result.M[2][3] = 0.0f;
        result.M[3][3] = 1.0f;

        return result;
    }

    static FMatrix LookAt(const FVector& eye, const FVector& target, const FVector& up)
    {
        FVector f = (target - eye).Normalize();
        FVector s = FVector::Cross(f, up).Normalize();
        FVector u = FVector::Cross(s, f);

        FMatrix result = Identity();

        result.M[0][0] = s.X;
        result.M[1][0] = s.Y;
        result.M[2][0] = s.Z;

        result.M[0][1] = u.X;
        result.M[1][1] = u.Y;
        result.M[2][1] = u.Z;

        result.M[0][2] = -f.X;
        result.M[1][2] = -f.Y;
        result.M[2][2] = -f.Z;

        result.M[3][0] = -FVector::Dot(s, eye);
        result.M[3][1] = -FVector::Dot(u, eye);
        result.M[3][2] = FVector::Dot(f, eye);

        return result;
    }

    //Incompativel com o Vulkan
    //static FMatrix Perspective(float fovRadians, float aspectRatio, float nearPlane, float farPlane)
    //{
    //    float tanHalfFov = std::tan(fovRadians / 2.0f);

    //    FMatrix result{};
    //    result.M[0][0] = 1.0f / (aspectRatio * tanHalfFov);
    //    result.M[1][1] = 1.0f / tanHalfFov;
    //    result.M[2][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);
    //    result.M[2][3] = -1.0f;
    //    result.M[3][2] = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);
    //    result.M[3][3] = 0.0f;
    //    return result;
    //}
    static FMatrix Perspective(float fovRadians, float aspectRatio, float nearPlane, float farPlane)
    {
        
        float tanHalfFov = std::tan(fovRadians / 2.0f);

        FMatrix result{};
        result.M[0][0] = 1.0f / (aspectRatio * tanHalfFov);
        result.M[1][1] = 1.0f / tanHalfFov;
        result.M[2][2] = farPlane / (nearPlane - farPlane);
        result.M[2][3] = -1.0f;
        result.M[3][2] = (farPlane * nearPlane) / (nearPlane - farPlane);
        result.M[3][3] = 0.0f;
        return result;
    }
};

#endif // KYRNNESS_MATRIX_HPP