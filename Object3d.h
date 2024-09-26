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

using Microsoft::WRL::ComPtr;

class Object3dBase;

class Object3d
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

	struct MaterialData {
		std::string textureFilePath;
		uint32_t textureIndex = 0;
	};

	struct ModelData {
		std::vector<VertexData> vertices;
		MaterialData material;
	};

public:
	/*==================== メンバ関数 ====================*/

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 頂点データの作成
	void MakeVertexData();

	// マテリアルデータの作成
	void MakeMaterialData();

	// 座標変換行列のデータ作成
	void MakeWvpData();

private:

	static std::wstring s2ws(const std::string& str);

	static Object3d::ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

	static Object3d::MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);


private:

	Object3dBase* object3dBase_ = nullptr;

	/*==================== モデルのデータ ====================*/

	ModelData modelData_;

	/*==================== トランスフォーム ====================*/

	Transform transform_;

	/*==================== 頂点データ ====================*/

	// バッファリソース
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	// バッファリソース内のデータを指すポインタ
	VertexData* vertexData_ = nullptr;
	// バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	/*==================== マテリアル ====================*/

	// バッファリソース
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	// バッファリソース内のデータを指すポインタ
	Material* materialData_ = nullptr;

	/*==================== 座標変換行列 ====================*/

	// バッファリソース
	ComPtr<ID3D12Resource> wvpResource_ = nullptr;
	// バッファリソース内のデータを指すポインタ
	TransformationMatrix* wvpData_ = nullptr;

};