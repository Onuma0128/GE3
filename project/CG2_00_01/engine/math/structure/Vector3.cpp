#include "Vector3.h"

#include <random>

// コンストラクタ
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

// 加算
Vector3 Vector3::Add(const Vector3& v1, const Vector3& v2) {
    return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

// 減算
Vector3 Vector3::Subtract(const Vector3& v1, const Vector3& v2) {
    return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

// スカラー倍
Vector3 Vector3::Multiply(float scalar, const Vector3& v) {
    return { v.x * scalar, v.y * scalar, v.z * scalar };
}

float Vector3::Length() const
{
    return std::sqrt(x * x + y * y + z * z);
}

// 内積
float Vector3::Dot(const Vector3& v1, const Vector3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// 外積
Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2) {
    return {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    };
}

// 正規化
Vector3 Vector3::Normalize() const {
    float length = Length();
    assert(length != 0 && "正規化中にゼロ除算が発生しました。");
    return { x / length, y / length, z / length };
}

// 行列による変換（仮実装）
Vector3 Vector3::Transform(const Matrix4x4& matrix) const {
    Vector3 result{};
    result.x = x * matrix.m[0][0] + y * matrix.m[1][0] + z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
    result.y = x * matrix.m[0][1] + y * matrix.m[1][1] + z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
    result.z = x * matrix.m[0][2] + y * matrix.m[1][2] + z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
    float w = x * matrix.m[0][3] + y * matrix.m[1][3] + z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
    assert(w != 0.0f);
    result.x /= w;
    result.y /= w;
    result.z /= w;
    return result;
}

Vector3 Vector3::Shake(float time, float duration, float intensity)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

    // 振動が終了している場合、ゼロを返す
    if (time >= duration) {
        return { 0.0f, 0.0f, 0.0f };
    }

    // 残り時間による振幅の減衰
    float progress = time / duration;  // 0.0 (開始) から 1.0 (終了)
    float damping = 1.0f - progress;   // 1.0 から 0.0 への線形減衰

    // ランダムな振動の生成
    return Vector3{
        dist(gen) * intensity * damping,
        dist(gen) * intensity * damping,
        dist(gen) * intensity * damping
    };
}

// 単項演算子オーバーロード
Vector3 Vector3::operator+() const {
    return *this;
}

Vector3 Vector3::operator-() const {
    return { -x, -y, -z };
}

// 二項演算子オーバーロード
Vector3 Vector3::operator+(const Vector3& v) const {
    return { x + v.x, y + v.y, z + v.z };
}

Vector3 Vector3::operator-(const Vector3& v) const {
    return { x - v.x, y - v.y, z - v.z };
}

Vector3 Vector3::operator*(float scalar) const {
    return { x * scalar, y * scalar, z * scalar };
}

// 複合代入演算子オーバーロード
Vector3& Vector3::operator+=(const Vector3& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

Vector3& Vector3::operator-=(const Vector3& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

Vector3& Vector3::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

// フレンド関数：スカラー倍の演算子オーバーロード
Vector3 operator*(float scalar, const Vector3& v) {
    return v * scalar;
}