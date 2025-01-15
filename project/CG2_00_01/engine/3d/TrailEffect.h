#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include "wrl.h"
#include <vector>

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Transform.h"

using Microsoft::WRL::ComPtr;

class TrailEffectBase;

class TrailEffect
{
public:

	void Init(std::vector<Vector3> pos);

	void Update();

	void Draw();


	void SetColor(const Vector4& color) { *materialData_ = color; }
	void SetAlpha(const float alpha) { materialData_->w = alpha; }

private:

	void CreateBufferResource(ComPtr<ID3D12Resource>& resource, size_t size);

	void CreateVertexBufferView();
	void CreateIndexBufferView();

	void CreateVertexData();
	void CreateIndexData();
	void CreateMaterialData();
	void CreateWVPData();

private:

	TrailEffectBase* trailEffectBase_ = nullptr;

	/* =============== 頂点 =============== */

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	Vector4* vertexData_ = nullptr;

	/* =============== index頂点 =============== */
	
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};
	ComPtr<ID3D12Resource> indexResource_ = nullptr;
	uint32_t* indexData_ = nullptr;

	/* =============== マテリアル =============== */

	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	Vector4* materialData_ = nullptr;

	/* =============== 座標変換行列 =============== */

	ComPtr<ID3D12Resource> wvpResource_ = nullptr;
	Matrix4x4* wvpData_ = nullptr;


	Transform transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	std::vector<Vector3> positions_;
};