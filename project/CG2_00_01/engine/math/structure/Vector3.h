#pragma once
#include <cmath>
#include <cassert>

#include "Matrix4x4.h"

// Vector3 クラスの定義
class Vector3 
{
public:
    float x, y, z;

    // コンストラクタ
    Vector3(float x = 0, float y = 0, float z = 0);

    // 加算
    static Vector3 Add(const Vector3& v1, const Vector3& v2);

    // 減算
    static Vector3 Subtract(const Vector3& v1, const Vector3& v2);

    // スカラー倍
    static Vector3 Multiply(float scalar, const Vector3& v);

    float Length()const;

    // 内積
    static float Dot(const Vector3& v1, const Vector3& v2);

    // 外積
    static Vector3 Cross(const Vector3& v1, const Vector3& v2);

    // 正規化
    Vector3 Normalize() const;

    // 行列による変換
    Vector3 Transform(const Matrix4x4& matrix) const;

    // シェーク
    Vector3 Shake(float time, float duration, float intensity);

    // 単項演算子オーバーロード
    Vector3 operator+() const;
    Vector3 operator-() const;

    // 二項演算子オーバーロード
    Vector3 operator+(const Vector3& v) const;
    Vector3 operator-(const Vector3& v) const;
    Vector3 operator*(float scalar) const;

    // 複合代入演算子オーバーロード
    Vector3& operator+=(const Vector3& v);
    Vector3& operator-=(const Vector3& v);
    Vector3& operator*=(float scalar);

    // フレンド関数：スカラー倍の演算子オーバーロード
    friend Vector3 operator*(float scalar, const Vector3& v);
};