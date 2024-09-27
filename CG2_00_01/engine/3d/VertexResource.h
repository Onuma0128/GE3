#pragma once
#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"
#include "MT3.h"
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
	bool& GetuseCircle() { return useCircle_; }

	void Update();

	ModelData& GetModelData() { return modelData_; }
	D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() { return vertexBufferView_; }

	ComPtr<ID3D12Resource> GetMaterialResource() { return materialResource_; }
	ComPtr<ID3D12Resource> GetInstancingResource() { return instancingResource_; }
	ComPtr<ID3D12Resource> GetCameraResource() { return cameraResource_; }

	Matrix4x4 GetCameraView()const {
		Matrix4x4 cameraMatrix = MakeAfineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		return viewMatrix;
	}

	void ImGui();

private:
	// モデル読み込み
	ModelData modelData_;
	// Particleの頂点リソースを作る
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;

	///=================================================================

	// VertexBufferViewを作成する
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	///=================================================================

	//頂点リソースにデータを書き込む
	VertexData* vertexData_ = nullptr;

	///=================================================================

	//マテリアル用のリソースを作る
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	//マテリアルにデータを書き込む
	Material* materialData_ = nullptr;
	// WVP用のリソースを作る
	// Instancing用のTransformationMatrixリソースを作る
	ComPtr<ID3D12Resource> instancingResource_ = nullptr;
	//
	ComPtr<ID3D12Resource> cameraResource_ = nullptr;
	// データを書き込む
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
	bool useCircle_ = true;
	//Camera変数を作る
	Transform cameraTransform_{ {1.0f,1.0f,1.0f},{0.26f,0.0f,0.0f},{0.0f,4.0f,-15.0f} };
	//UVTransform変数
	Transform uvTransformSprite_{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Matrix4x4 uvTransformMatrix_{};
};