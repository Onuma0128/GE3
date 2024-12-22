#include "Quaternion.h"

#include <cmath>

#include "imgui.h"

Quaternion::Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

void Quaternion::ImGuiQuaternion(const std::string& imguiName) const
{
	ImGui::Text("%.3f, %.3f, %.3f, %.3f : ", x, y, z, w);
	ImGui::SameLine();
	ImGui::Text(imguiName.c_str());
}

//Quaternion Quaternion::Multiply(const Quaternion& lhs, const Quaternion& rhs)
//{
//	return {
//		lhs.y * rhs.z - lhs.z * rhs.y + rhs.w * lhs.x + lhs.w * rhs.x,
//		lhs.z * rhs.x - lhs.x * rhs.z + rhs.w * lhs.y + lhs.w * rhs.y,
//		lhs.x * rhs.y - lhs.y * rhs.x + rhs.w * lhs.z + lhs.w * rhs.z,
//		lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z
//	};
//}

Quaternion Quaternion::IdentityQuaternion()
{
	return { 0.0f,0.0f,0.0f,1.0f };
}

Quaternion Quaternion::Conjugate() const
{
	return { -x,-y,-z,w };
}

float Quaternion::Norm() const
{
	return std::sqrt(x * x + y * y + z * z + w * w);
}

Quaternion& Quaternion::Normalize()
{
	float norm = Norm();
	if (norm > 0.0f)
	{
		x /= norm;
		y /= norm;
		z /= norm;
		w /= norm;
	}
	return *this;
}

Quaternion Quaternion::Inverse() const
{
	float norm = Norm() * Norm();

	Quaternion conjugate = Conjugate();

	return {
		conjugate.x / norm,
		conjugate.y / norm,
		conjugate.z / norm,
		conjugate.w / norm
	};
}

Quaternion Quaternion::operator*(const Quaternion& q) const
{
	return {
		y * q.z - z * q.y + q.w * x + w * q.x,
		z * q.x - x * q.z + q.w * y + w * q.y,
		x * q.y - y * q.x + q.w * z + w * q.z,
		w * q.w - x * q.x - y * q.y - z * q.z
	};
}

Quaternion& Quaternion::operator*=(const Quaternion& q)
{
	*this = *this * q;
	return *this;
}
