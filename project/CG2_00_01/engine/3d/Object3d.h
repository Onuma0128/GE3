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

class Object3dBase;

class Object3d
{
public:

	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Matrix4x4 WorldInverseTranspose;
	};

public:
	/*==================== メンバ関数 ====================*/

	// 初期化
	void Initialize(const std::string& filePath);

	// 更新
	void Update();

	// 描画
	void Draw();

	// 座標変換行列のデータ作成
	void MakeWvpData();

	/*==================== アクセッサ ====================*/

	void SetModel(const std::string& filePath);
	void SetTexture(const std::string& directoryPath, const std::string& filePath);

	// サイズ
	const Vector3& GetScale()const { return transform_.scale; }
	void SetScale(const Vector3& scale) { transform_.scale = scale; }

	// 回転
	const Vector3& GetRotation()const { return transform_.rotate; }
	void SetRotation(const Vector3& rotation) { transform_.rotate = rotation; }

	// 座標
	const Vector3& GetPosition()const { return transform_.translate; }
	void SetPosition(const Vector3& position) { transform_.translate = position; }

	void SetParent(Object3d* obj) { parent_ = obj; }

	const Vector3 GetWorldPosition();

	const Matrix4x4& GetWorldMatrix();

private:

	Object3dBase* object3dBase_ = nullptr;

	Model* model_ = nullptr;

	/*==================== トランスフォーム ====================*/

	Transform transform_;

	Matrix4x4 worldMatrix_{};

	Object3d* parent_;

	/*==================== 座標変換行列 ====================*/

	// バッファリソース
	ComPtr<ID3D12Resource> wvpResource_ = nullptr;
	// バッファリソース内のデータを指すポインタ
	TransformationMatrix* wvpData_ = nullptr;

};