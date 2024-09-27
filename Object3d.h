#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <wrl.h>
#include <vector>
#include <string>

#include "math/structure/Vector2.h"
#include "math/structure/Vector3.h"
#include "math/structure/Vector4.h"
#include "math/structure/Matrix4x4.h"
#include "math/structure/Transform.h"
#include "Model.h"

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
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 座標変換行列のデータ作成
	void MakeWvpData();

	/*==================== アクセッサ ====================*/

	void SetModel(Model* model) { this->model_ = model; }

	// サイズ
	const Vector3& GetScale()const { return transform_.scale; }
	void SetScale(const Vector3& scale) { transform_.scale = scale; }

	// 回転
	const Vector3& GetRotation()const { return transform_.rotate; }
	void SetRotation(const Vector3& rotation) { transform_.rotate = rotation; }

	// 座標
	const Vector3& GetPosition()const { return transform_.translate; }
	void SetPosition(const Vector3& position) { transform_.translate = position; }

private:

	Object3dBase* object3dBase_ = nullptr;

	Model* model_ = nullptr;

	/*==================== トランスフォーム ====================*/

	Transform transform_;

	/*==================== 座標変換行列 ====================*/

	// バッファリソース
	ComPtr<ID3D12Resource> wvpResource_ = nullptr;
	// バッファリソース内のデータを指すポインタ
	TransformationMatrix* wvpData_ = nullptr;

};