#include "TrailEffect.h"

#include "DirectXEngine.h"
#include "TrailEffectBase.h"

#include "Camera.h"
#include "CameraManager.h"

void TrailEffect::Init(std::vector<Vector3> pos)
{
	trailEffectBase_ = TrailEffectBase::GetInstance();
	positions_ = pos;

	CreateBufferResource(vertexResource_, sizeof(Vector4) * 4);
	CreateVertexBufferView();
	CreateVertexData();

	CreateBufferResource(indexResource_, sizeof(uint32_t) * 6);
	CreateIndexBufferView();
	CreateIndexData();

	CreateBufferResource(materialResource_, sizeof(Vector4));
	CreateMaterialData();

	CreateBufferResource(wvpResource_, sizeof(Matrix4x4));
	CreateWVPData();
}

void TrailEffect::Update()
{
	Matrix4x4 matWorld = Matrix4x4::Affine(transform_.scale, transform_.rotate, transform_.translate);

	*wvpData_ = matWorld * CameraManager::GetInstance()->GetActiveCamera()->GetViewProjectionMatrix();
}

void TrailEffect::Draw()
{
	auto commandList = trailEffectBase_->GetDxEngine()->GetCommandList();

	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	commandList->IASetIndexBuffer(&indexBufferView_);
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());

	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void TrailEffect::CreateBufferResource(ComPtr<ID3D12Resource>& resource, size_t size)
{
	// 頂点リソースのヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties = {};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

	// 頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc = {};
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = size;
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点リソースを作成する
	HRESULT hr = trailEffectBase_->GetDxEngine()->GetDevice()->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&resource));

	assert(SUCCEEDED(hr)); // エラーチェック
}

void TrailEffect::CreateVertexBufferView()
{
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(Vector4) * 4;
	vertexBufferView_.StrideInBytes = sizeof(Vector4);
}

void TrailEffect::CreateIndexBufferView()
{
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
}

void TrailEffect::CreateVertexData()
{
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	vertexData_[0] = { positions_[0].x,positions_[0].y,positions_[0].z,1.0f };
	vertexData_[1] = { positions_[1].x,positions_[1].y,positions_[1].z,1.0f };
	vertexData_[2] = { positions_[2].x,positions_[2].y,positions_[2].z,1.0f };
	vertexData_[3] = { positions_[3].x,positions_[3].y,positions_[3].z,1.0f };

	/*vertexData_[0] = { -0.5f,-0.5f,0.0f,1.0f };
	vertexData_[1] = { -0.5f,0.5f,0.0f,1.0f };
	vertexData_[2] = { 0.5f,-0.5f,0.0f,1.0f };
	vertexData_[3] = { 0.5f,0.5f,0.0f,1.0f };*/
}

void TrailEffect::CreateIndexData()
{
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	indexData_[0] = 0; indexData_[1] = 1; indexData_[2] = 2;
	indexData_[3] = 1; indexData_[4] = 3; indexData_[5] = 2;
}

void TrailEffect::CreateMaterialData()
{
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	*materialData_ = { 1.0f,1.0f,1.0f,1.0f };
}

void TrailEffect::CreateWVPData()
{
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));

	*wvpData_ = Matrix4x4::Identity();
}

