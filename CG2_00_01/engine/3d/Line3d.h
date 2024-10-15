#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include "wrl.h"
#include "vector"

#include "PrimitiveDrawer.h"

#include "Vector3.h"
#include "Vector4.h"

using Microsoft::WRL::ComPtr;

class Line3d
{
public:
	struct VertexLineData {
		Vector4 position;
		//Vector4 color;
	};
public:

	void Initialize(Vector3 startPos, Vector3 endPos);

	void Initialize(const std::vector<Vector3>& positions);

	void Update();

	void Draw();

	void Draws();

	/*==================== メンバ関数 ====================*/

	void CreatVertexResource();

	void CreatVertexResource(const std::vector<Vector3>& positions);

	void CreatVertexBufferView();

	void CreatVertexBufferViews();

private:

	PrimitiveDrawer* primitiveDrawer_ = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	ComPtr<ID3D12Resource> wvpResource_ = nullptr;

	VertexLineData* vertexData_ = nullptr;
	Matrix4x4* wvpData_ = nullptr;
	uint32_t lineCount_ = 0;

	Vector3 startPos_ = {};
	Vector3 endPos_ = {};

};

