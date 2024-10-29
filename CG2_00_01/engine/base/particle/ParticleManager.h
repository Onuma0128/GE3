#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include "wrl.h"
#include <iostream>
#include <random>
#include <numbers>
#include <memory>
#include <list>

#include "DirectXEngine.h"
#include "SrvManager.h"
#include "Model.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Transform.h"

using Microsoft::WRL::ComPtr;

// パーティクルのMAX値(上げすぎ注意!!)
const uint32_t kNumMaxInstance = 10;

class ParticleManager
{
public:

	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};
	struct Material {
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
		float shininess;
	};

	struct Particle {
		Transform transform;
		Vector3 velocity;
		Vector4 color;
		float lifeTime;
		float currentTime;
	};
	struct ParticleForGPU {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Vector4 color;
	};
	struct Emitter {
		Transform transform;
		uint32_t count;
		float frequency;
		float frequencyTime;
	};
	struct AABB {
		Vector3 min;
		Vector3 max;
	};
	struct AccelerationField {
		Vector3 acceleration;
		AABB area;
	};

private:
	static ParticleManager* instance_;

	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(ParticleManager&) = delete;
	ParticleManager& operator=(ParticleManager&) = delete;
public:
	// シングルトンインスタンスの取得
	static ParticleManager* GetInstance();

	void Initialize(DirectXEngine* dxEngine);

	void Update();

	void Draw();

	void Finalize();

	ComPtr<ID3D12Resource> GetInstancingResource()const { return instancingResource_; }

private:

	static std::list<ParticleManager::Particle> Emit(const Emitter& emitter, std::mt19937& randomEngine);

	void CreateVertexResource();

	void CreateMatrialResource();

	void CreateInstancingResource();

	void CreateEmit();

	static Particle MakeNewParticle(std::mt19937& randomEngine, const Vector3& translate);

	bool IsCollision(const AABB& aabb, const Vector3& point);

private:

	// 基盤ポインタ
	DirectXEngine* dxEngine_ = nullptr;
	SrvManager* srvManager_ = nullptr;
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	// パイプラインステート
	ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;

	/*==================== メンバ変数 ====================*/

	// モデル読み込み
	Model::ModelData modelData_;

	// 頂点リソース,データを作成
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	VertexData* vertexData_ = nullptr;

	// 頂点バッファビューを作成
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	//マテリアル用のリソース,データを作成
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	Material* materialData_ = nullptr;

	// Instancing用リソース,データを作成
	ComPtr<ID3D12Resource> instancingResource_ = nullptr;
	ParticleForGPU* instancingData_ = nullptr;

	/*==================== エミッター ====================*/

	//乱数生成器の初期化
	std::random_device seedGenerator_;

	//Transform変数を作る
	std::list<Particle> particles_{};
	AccelerationField accelerationField_{};
	Emitter emitter_{};
	const float kDeltaTime = 1.0f / 60.0f;
	bool moveStart_ = false;
	bool isFieldStart_ = false;

};