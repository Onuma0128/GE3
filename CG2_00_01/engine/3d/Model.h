#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <wrl.h>
#include <vector>
#include <string>

#include "assimp//Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

using Microsoft::WRL::ComPtr;

class ModelBase;

class Model
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

	struct MaterialData {
		std::string textureFilePath;
		uint32_t textureIndex = 0;
	};

	struct Node {
		Matrix4x4 localMatrix;
		std::string name;
		std::vector<Node> children;
	};

	struct ModelData {
		std::vector<VertexData> vertices;
		MaterialData material;
		Node rootNode;
	};

public:
	/*==================== メンバ関数 ====================*/

		// 初期化
	void Initialize(const std::string& filename);

	// 描画
	void Draw();

	// 頂点データの作成
	void MakeVertexData();

	// マテリアルデータの作成
	void MakeMaterialData();

	const ModelData GetModelData()const { return modelData_; }

private:

	static std::wstring s2ws(const std::string& str);

	static Model::ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

	static Model::MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	static Model::Node ReadNode(aiNode* node);

private:

	ModelBase* modelBase_ = nullptr;

	/*==================== モデルのデータ ====================*/

	ModelData modelData_;

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

};