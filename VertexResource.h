#pragma once
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
#include "math/MT3.h"
#include "LoadObjFile.h"
#include "wrl.h"
#include <iostream>
#include <random>
#include <numbers>
#include <list>
#include <array>
#include "ResourceObject.h"

using Microsoft::WRL::ComPtr;

const uint32_t kNumMaxInstance = 100;

ResourceObject CreateBufferResource(ComPtr<ID3D12Device> device, size_t sizeInBytes);

class VertexResource
{
public:
	void Initialize(ComPtr<ID3D12Device> device);
	uint32_t GetNumInstance() { return numInstance; }
	bool& GetuseMonsterBall() { return useMonsterBall_; }
	bool& GetuseCircle() { return useCircle_; }

	void Update();

	ModelData& GetModelData() { return modelData_; }
	ModelData& GetModelDataObject() { return modelDataObject_[objIndex_]; }
	D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() { return vertexBufferView_; }
	D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferViewObject() { return vertexBufferViewObject_[objIndex_]; }
	D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferViewSphere() { return vertexBufferViewSphere_; }
	/*D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferViewSprite() { return vertexBufferViewSprite_; }
	D3D12_INDEX_BUFFER_VIEW& GetIndexBufferViewSprite() { return indexBufferViewSprite_; }*/
	ComPtr<ID3D12Resource> GetDirectionalLightResource() { return directionalLightResource_; }
	ComPtr<ID3D12Resource> GetPointLightResource() { return pointLightResource_; }
	ComPtr<ID3D12Resource> GetSpotLightResource() { return spotLightResource_; }

	ComPtr<ID3D12Resource> GetMaterialResource() { return materialResource_; }
	ComPtr<ID3D12Resource> GetMaterialResourceObject() { return materialResourceObject_; }
	ComPtr<ID3D12Resource> GetMaterialResourceSphere() { return materialResourceSphere_; }
	//ComPtr<ID3D12Resource> GetMaterialResourceSprite() { return materialResourceSprite_; }
	ComPtr<ID3D12Resource> GetInstancingResource() { return instancingResource_; }
	ComPtr<ID3D12Resource> GetwvpResourceObject() { return wvpResourceObject_; }
	ComPtr<ID3D12Resource> GetwvpResourceSphere() { return wvpResourceSphere_; }
	//ComPtr<ID3D12Resource> GetTransformationMatrixResourceSprite() { return transformationMatrixResourceSprite_; }
	ComPtr<ID3D12Resource> GetCameraResource() { return cameraResource_; }

	void ImGui();

private:
	// モデル読み込み
	ModelData modelData_;
	ModelData modelDataObject_[5];
	// Particleの頂点リソースを作る
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	// Object3Dの頂点リソースを作る
	uint32_t objIndex_ = 0;
	std::array<ComPtr<ID3D12Resource>, 5> vertexResourceObject_ = {};
	// Sphere用の頂点リソースを作る
	ComPtr<ID3D12Resource> vertexResourceSphere_ = nullptr;
	// Sprite用の頂点リソースを作る
	/*ComPtr<ID3D12Resource> vertexResourceSprite_ = nullptr;
	ComPtr<ID3D12Resource> indexResourceSprite_ = nullptr;*/
	// 平行光源用のリソースを作る
	ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;
	// ポイントライト用
	ComPtr<ID3D12Resource> pointLightResource_ = nullptr;
	// スポットライト用
	ComPtr<ID3D12Resource> spotLightResource_ = nullptr;

	///=================================================================

	// VertexBufferViewを作成する
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	std::array<D3D12_VERTEX_BUFFER_VIEW, 5> vertexBufferViewObject_{};
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSphere_{};
	/*D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite_{};
	D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite_{};*/
	D3D12_VERTEX_BUFFER_VIEW directionalLightBufferView_{};
	D3D12_VERTEX_BUFFER_VIEW pointLightBufferView_{};
	D3D12_VERTEX_BUFFER_VIEW spotLightBufferView_{};

	///=================================================================

	//頂点リソースにデータを書き込む
	VertexData* vertexData_ = nullptr;
	VertexData* vertexDataObject_ = nullptr;
	VertexData* vertexDataSphere_ = nullptr;
	/*VertexData* vertexDataSprite_ = nullptr;
	uint32_t* indexDataSprite_ = nullptr;*/
	DirectionalLight* directionalLightData_ = nullptr;
	PointLight* pointLightData_ = nullptr;
	SpotLight* spotLightData_ = nullptr;
	//分割数(球体)
	uint32_t vertexCount_ = 16;

	///=================================================================

	//マテリアル用のリソースを作る
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	ComPtr<ID3D12Resource> materialResourceObject_ = nullptr;
	ComPtr<ID3D12Resource> materialResourceSphere_ = nullptr;
	ComPtr<ID3D12Resource> materialResourceSprite_ = nullptr;
	//マテリアルにデータを書き込む
	Material* materialData_ = nullptr;
	Material* materialDataObject_ = nullptr;
	Material* materialDataSphere_ = nullptr;
	//Material* materialDataSprite_ = nullptr;
	// WVP用のリソースを作る
	// Instancing用のTransformationMatrixリソースを作る
	ComPtr<ID3D12Resource> instancingResource_ = nullptr;
	// Object用
	ComPtr<ID3D12Resource> wvpResourceObject_ = nullptr;
	// Sphere用
	ComPtr<ID3D12Resource> wvpResourceSphere_ = nullptr;
	// Sprite用
	//ComPtr<ID3D12Resource> transformationMatrixResourceSprite_ = nullptr;
	//
	ComPtr<ID3D12Resource> cameraResource_ = nullptr;
	// データを書き込む
	TransformationMatrix* wvpDataObject_ = nullptr;
	TransformationMatrix* wvpDataSphere_ = nullptr;
	//TransformationMatrix* transformationMatrixDataSprite_ = nullptr;
	ParticleForGPU* instancingData_ = nullptr;
	CameraForGPU* cameraData_ = nullptr;

	///=================================================================

	//Transform変数を作る
	std::list<Particle> particles_{};
	AccelerationField accelerationField_{};
	Emitter emitter_{};
	const float kDeltaTime = 1.0f / 60.0f;
	uint32_t numInstance = 0;
	//乱数生成器の初期化
	std::random_device seedGenerator_;
	bool moveStart_ = false;
	bool isFieldStart_ = false;
	// Object
	Transform transformObject_{ {1.0f,1.0f,1.0f},{0.0f,3.14f,0.0f},{1.0f,0.0f,0.0f} };
	bool objectLight_ = true;
	// Sphere
	Transform transformSphere_{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{-1.0f,0.0f,0.0f} };
	Transform uvTransformSphere_{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	bool useMonsterBall_ = true;
	bool sphereLight_ = true;
	// Sprite
	//Transform transformSprite_{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	bool useCircle_ = true;
	//Camera変数を作る
	Transform cameraTransform_{ {1.0f,1.0f,1.0f},{0.26f,0.0f,0.0f},{0.0f,4.0f,-15.0f} };
	//UVTransform変数
	Transform uvTransformSprite_{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Matrix4x4 uvTransformMatrix_{};
};