#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include "wrl.h"
#include <vector>
#include <string>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Transform.h"

using Microsoft::WRL::ComPtr;

class TrailEffectBase;

class TrailEffect
{
public:

	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
	};
	struct TextureData {
		std::string directoryPath;
		std::string filePath;
		uint32_t textureIndex = 0;
	};

	/* =============== トレイルエフェクト =============== */

	void Init(std::vector<Vector3> pos);

	void Update();

	void Draw();

	/* =============== 球面 =============== */

	void InitSphere(uint32_t kSubdivision);

	void DrawSphere();

	/* =============== アクセッサ(トレイル用) =============== */

	void SetPosition(std::vector<Vector3> pos);

	/* =============== アクセッサ(球体用) =============== */

	const Transform& GetTransform() { return transform_; }
	void SetTransform(Transform transform) { transform_ = transform; }

	/* =============== アクセッサ(全体) =============== */

	void SetTexture(const std::string& directoryPath, const std::string& filePath);
	void SetColor(const Vector3& color) { 
		materialData_->x = color.x;
		materialData_->y = color.y;
		materialData_->z = color.z;
	}
	void SetAlpha(const float alpha) { materialData_->w = alpha; }

private:

	void CreateBufferResource(ComPtr<ID3D12Resource>& resource, size_t size);

	void CreateVertexBufferView(uint32_t kVertexSize);
	void CreateIndexBufferView();

	void CreateVertexData();
	void CreateIndexData();
	void CreateMaterialData();
	void CreateWVPData();

	// 球面の頂点計算
	VertexData* CreateSphereVertexData(VertexData* vertexData, uint32_t kSubdivision);

private:

	TrailEffectBase* trailEffectBase_ = nullptr;

	/* =============== 頂点 =============== */

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	VertexData* vertexData_ = nullptr;

	/* =============== index頂点 =============== */
	
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};
	ComPtr<ID3D12Resource> indexResource_ = nullptr;
	uint32_t* indexData_ = nullptr;

	/* =============== マテリアル =============== */

	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	Vector4* materialData_ = nullptr;
	// TextureData
	TextureData textureData_;

	/* =============== 座標変換行列 =============== */

	ComPtr<ID3D12Resource> wvpResource_ = nullptr;
	Matrix4x4* wvpData_ = nullptr;


	Transform transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	std::vector<Vector3> positions_;

	uint32_t kSubdivision_;
};