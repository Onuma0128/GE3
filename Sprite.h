#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <wrl.h>
#include <stdint.h>

#include "math/structure/Vector2.h"
#include "math/structure/Vector3.h"
#include "math/structure/Vector4.h"
#include "math/structure/Matrix4x4.h"
#include "math/structure/Transform.h"

using Microsoft::WRL::ComPtr;

class SpriteBase;

class Sprite
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

	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Matrix4x4 WorldInverseTranspose;
	};

	void Initialize(SpriteBase* spriteBase);

	void Update();

	void Draw();

private:

	void VertexDataInitialize();

	void MaterialDataInitialize();

	void TransformationMatrixDataInitialize();

private:

	SpriteBase* spriteBase_ = nullptr;

	// バッファリソース
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	ComPtr<ID3D12Resource> indexResource_ = nullptr;
	// バッファリソース内のデータ
	VertexData* vertexData_ = nullptr;
	uint32_t* indexData_ = nullptr;
	// バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	// マテリアルリソース
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	// マテリアルデータ
	Material* materialData_ = nullptr;

	// 座標変換行列
	ComPtr<ID3D12Resource> transformationMatrixResource_ = nullptr;
	TransformationMatrix* transformationMatrixData_ = nullptr;

	// トランスフォーム
	Transform transform_ { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

};

