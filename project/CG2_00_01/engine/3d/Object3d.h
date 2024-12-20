#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <wrl.h>
#include <vector>
#include <string>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Transform.h"
#include "Model.h"
#include "Camera.h"

using Microsoft::WRL::ComPtr;

class WorldTransform;

class Object3dBase;

class Object3d
{
public:

	struct Material {
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
		float shininess;
	};

public:
	/*==================== メンバ関数 ====================*/

	// 初期化
	void Initialize(const std::string& filePath, WorldTransform* transform);

	// 更新
	void Update();

	// 描画
	void Draw();

	/*==================== アクセッサ ====================*/

	void SetModel(const std::string& filePath);
	void SetTexture(const std::string& directoryPath, const std::string& filePath);
	void SetColor(const Vector4& color);

private:

	// マテリアルデータの作成
	void MakeMaterialData();

private:

	Object3dBase* object3dBase_ = nullptr;

	Model* model_ = nullptr;

	WorldTransform* transform_ = nullptr;

	/*==================== マテリアル ====================*/

	// バッファリソース
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	// バッファリソース内のデータを指すポインタ
	Material* materialData_ = nullptr;

};