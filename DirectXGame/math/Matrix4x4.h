#pragma once
#include "Vector3.h"
#include <cmath>
/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];
};

/// <summary>
/// 4x4行列の掛け算
/// </summary>
Matrix4x4 Multiply(Matrix4x4 m1, Matrix4x4 m2);

/// <summary>
/// 4x4行列のアフィン変換
/// </summary>
Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotation, Vector3 translation);
