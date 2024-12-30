#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include "wrl.h"
#include <iostream>
#include <numbers>
#include <memory>
#include <list>
#include <unordered_map>

#include "DirectXEngine.h"
#include "SrvManager.h"
#include "Model.h"
#include "ModelManager.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Transform.h"

using Microsoft::WRL::ComPtr;

// パーティクルのMAX値(上げすぎ注意!!)
const uint32_t kNumMaxInstance = 1024;

class ParticleEmitter;

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

	struct ParticleGroup {
		std::string textureFilePath;
		uint32_t srvIndex;
		uint32_t textureIndex;
		std::list<Particle> particles;
		uint32_t instancingIndex;
		ComPtr<ID3D12Resource> instancingResource;
		uint32_t instanceCount;
		ParticleForGPU* instancingData;
		ParticleEmitter* emitter;
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

	// グループのクリア処理
	void Clear();

	void Finalize();

	void CreateParticleGroup(const std::string name, const std::string textureFilePath, ParticleEmitter* emitter);

	void Emit(const std::string name);

private:

	void CreateVertexResource();

	void CreateMatrialResource();

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

	// パーティクルグループコンテナ
	std::unordered_map<std::string, ParticleGroup> particleGroups_;

	// 頂点リソース,データを作成
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	VertexData* vertexData_ = nullptr;

	// 頂点バッファビューを作成
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	//マテリアル用のリソース,データを作成
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	Material* materialData_ = nullptr;

};