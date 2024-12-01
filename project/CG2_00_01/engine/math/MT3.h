#pragma once
#include <vector>
#include <xstring>
#include <random>
#include <cassert>
#include <list>
#include "structure/Vector2.h"
#include "structure/Vector3.h"
#include "structure/Vector4.h"
#include "structure/Matrix3x3.h"
#include "structure/Matrix4x4.h"
#include "structure/Transform.h"

struct AABB {
	Vector3 min;
	Vector3 max;
};
//struct VertexData {
//	Vector4 position;
//	Vector2 texcoord;
//	Vector3 normal;
//};
//struct MaterialData {
//	std::string textureFilePath;
//};
//struct ModelData {
//	std::vector<VertexData> vertices;
//	MaterialData material;
//};
//struct Sphere {
//	Vector3 center;
//	float radius;
//};
//struct Material {
//	Vector4 color;
//	int32_t enableLighting;
//	float padding[3];
//	Matrix4x4 uvTransform;
//	float shininess;
//};
//struct Particle {
//	Transform transform;
//	Vector3 velocity;
//	Vector4 color;
//	float lifeTime;
//	float currentTime;
//};
//struct ParticleForGPU {
//	Matrix4x4 WVP;
//	Matrix4x4 World;
//	Vector4 color;
//};
//struct Emitter {
//	Transform transform;
//	uint32_t count;
//	float frequency;
//	float frequencyTime;
//};
//struct AccelerationField {
//	Vector3 acceleration;
//	AABB area;
//};
struct CameraForGPU {
	Vector3 worldPosition;
};

const float pi = 3.141592653589793238462643383279f;

// 正規化
Vector3 Normalize(const Vector4& v);

////Sphere
//VertexData* DrawSphere(VertexData* vertexData, uint32_t kSubdivision);
////Particle生成関数
//Particle MakeNewParticle(std::mt19937& randomEngine,const Vector3& translate);
////Emit関数
//std::list<Particle> Emit(const Emitter& emitter, std::mt19937& randomEngine);
// AABBとpoint
bool IsCollision(const AABB& aabb, const Vector3& point);