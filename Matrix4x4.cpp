// Matrix4x4.cpp
// Created by xiaojl Jan/19/2021
// 四阶矩阵

#include"Matrix4x4.h"

Matrix4x4& Matrix4x4::operator *(const Matrix4x4 &rhs)
{
    Matrix4x4 m;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m.Values[j][i] =
                Values[j][0] * rhs.Values[0][i] +
                Values[j][1] * rhs.Values[1][i] +
                Values[j][2] * rhs.Values[2][i] +
                Values[j][3] * rhs.Values[3][i];
        }
    }

    return m;
}

// https://msdn.microsoft.com/en-us/library/bb205342(v=vs.85).aspx
Matrix4x4 Matrix4x4::LookAtLH(Vector3 eye, Vector3 forward, Vector3 up)
{
    Vector3 zaxis = forward.Normalize();
    Vector3 xaxis = up.Cross(zaxis).Normalize();
    Vector3 yaxis = zaxis.Cross(xaxis).Normalize();

    return Matrix4x4
    {
        xaxis.x,         yaxis.x,         zaxis.x,         0,
        xaxis.y,         yaxis.y,         zaxis.y,         0,
        xaxis.z,         yaxis.z,         zaxis.z,         0,
        -xaxis.Dot(eye), -yaxis.Dot(eye), -zaxis.Dot(eye), 1
    };
}

// https://msdn.microsoft.com/en-us/library/bb205350(VS.85).aspx
Matrix4x4 Matrix4x4::PerspectiveFovLH(float fov, float aspect, float znear, float zfar)
{
    float yScale = 1 / (float)std::tanf(fov / 2);
    float xScale = yScale / aspect;
    return Matrix4x4
    {
        xScale, 0,      0,                              0,
        0,      yScale, 0,                              0,
        0,      0,      zfar / (zfar - znear),          1,
        0,      0,      -znear * zfar / (zfar - znear), 0
    };
}

Matrix4x4 Matrix4x4::Rotation(Vector3 r)
{
    Matrix4x4 x = RotationX(r.x);
    Matrix4x4 y = RotationY(r.y);
    Matrix4x4 z = RotationZ(r.z);
    return z * x * y;
}

Matrix4x4 Matrix4x4::RotationX(float angle)
{
    float s = std::sinf(angle);
    float c = std::cosf(angle);

    return Matrix4x4
    {
        1, 0,  0, 0,
        0, c,  s, 0,
        0, -s, c, 0,
        0, 0,  0, 1
    };
}

Matrix4x4 Matrix4x4::RotationY(float angle)
{
    float s = std::sinf(angle);
    float c = std::cosf(angle);

    return Matrix4x4
    {
        c, 0,  -s, 0,
        0, 1,   0, 0,
        s, 0,   c, 0,
        0, 0,   0, 1
    };
}

Matrix4x4 Matrix4x4::RotationZ(float angle)
{
    float s = std::sinf(angle);
    float c = std::cosf(angle);

    return Matrix4x4
    {
        c,  s,  0, 0,
        -s, c,  0, 0,
        0,  0,  1, 0,
        0,  0,  0, 1
    };
}

Matrix4x4 Matrix4x4::Translation(Vector3 &rhs)
{
    return Matrix4x4
    {
        1,     0,     0,     0,
        0,     1,     0,     0,
        0,     0,     1,     0,
        rhs.x, rhs.y, rhs.z, 0
    };
}
