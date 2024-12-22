#pragma once

#include <string>

class Quaternion
{
public:
	float x, y, z, w;

    // コンストラクタ
    Quaternion(float x = 0, float y = 0, float z = 0, float w = 1);

    // QuaternionのImGui表示
    void ImGuiQuaternion(const std::string& imguiName) const;

    // 単位Quaternion
    static Quaternion IdentityQuaternion();

    // 共役Quaternion
    Quaternion Conjugate() const;

    // ノルムQuaternion
    float Norm() const;

    // 正規化Quaternion
    Quaternion& Normalize();

    // 逆Quaternion
    Quaternion Inverse() const;



    // 二項演算子オーバーロード
    Quaternion operator*(const Quaternion& q) const;

    // 複合代入演算子オーバーロード
    Quaternion& operator*=(const Quaternion& q);
};

