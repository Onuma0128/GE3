#pragma once

#include <string>

class Vector3;
class Matrix4x4;

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
    static Quaternion Conjugate(const Quaternion& quaternion);

    // ノルムQuaternion
    static float Norm(const Quaternion& quaternion);

    // 正規化Quaternion
    static Quaternion Normalize(const Quaternion& quaternion);

    // 逆Quaternion
    static Quaternion Inverse(const Quaternion& quaternion);

    // 任意軸回転を表すQuaternion
    static Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);

    // Vector3をQuaternionで回転させた結果のVector3を求める
    static Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);

    // Quaternionから回転行列を求める
    static Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);



    // 二項演算子オーバーロード
    Quaternion operator*(const Quaternion& q) const;

    // 複合代入演算子オーバーロード
    Quaternion& operator*=(const Quaternion& q);
};