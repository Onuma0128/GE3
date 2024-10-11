#include "Line3d.h"
#include "Camera.h"

void Line3d::Initialize(Vector3 startPos, Vector3 endPos)
{
	primitiveDrawer_ = PrimitiveDrawer::GetInstance();

	CreatVertexResource();

	CreatVertexBufferView();

	startPos_ = startPos;
	endPos_ = endPos;

	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	vertexData_[0].position = { startPos_.x ,startPos_.y,startPos_.z,1.0f };
	vertexData_[1].position = { endPos_.x,endPos_.y,endPos_.z,1.0f };

	wvpResource_ = CreateBufferResource(primitiveDrawer_->GetDxEngine()->GetDevice(), sizeof(Matrix4x4)).Get();

	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
}

void Line3d::Update()
{
	*wvpData_ = Camera::GetInstance()->GetViewProjectionMatrix();
}

void Line3d::Draw()
{
	primitiveDrawer_->GetDxEngine()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	primitiveDrawer_->GetDxEngine()->GetCommandList()->SetGraphicsRootConstantBufferView(0, wvpResource_->GetGPUVirtualAddress());
	primitiveDrawer_->GetDxEngine()->GetCommandList()->DrawInstanced(2, 1, 0, 0);
}

void Line3d::CreatVertexResource()
{
	// 頂点ヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

	// 頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeof(VertexLineData) * 2;
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT hr = primitiveDrawer_->GetDxEngine()->GetDevice()->CreateCommittedResource(
		&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &vertexResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexResource_)
	);
	assert(SUCCEEDED(hr));
}

void Line3d::CreatVertexBufferView()
{
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(VertexLineData) * 2;
	vertexBufferView_.StrideInBytes = sizeof(VertexLineData);
}
