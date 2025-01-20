#include "TrailEffect.h"

#include <numbers>

#include "DirectXEngine.h"
#include "TrailEffectBase.h"
#include "TextureManager.h"
#include "SrvManager.h"

#include "Camera.h"
#include "CameraManager.h"

void TrailEffect::Init(std::vector<Vector3> pos)
{
	trailEffectBase_ = TrailEffectBase::GetInstance();
	positions_ = pos;

	SetTexture("resources", "white1x1.png");

	CreateBufferResource(vertexResource_, sizeof(VertexData) * 4);
	CreateVertexBufferView(4);
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
	SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(2, textureData_.textureIndex);

	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void TrailEffect::InitSphere(uint32_t kSubdivision)
{
	kSubdivision_ = kSubdivision;
	trailEffectBase_ = TrailEffectBase::GetInstance();

	SetTexture("resources", "white1x1.png");

	uint32_t startIndex = kSubdivision * kSubdivision * 6;
	CreateBufferResource(vertexResource_, sizeof(VertexData) * startIndex);
	CreateVertexBufferView(startIndex);
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	vertexData_ = CreateSphereVertexData(vertexData_, kSubdivision);

	CreateBufferResource(materialResource_, sizeof(Vector4));
	CreateMaterialData();

	CreateBufferResource(wvpResource_, sizeof(Matrix4x4));
	CreateWVPData();
}

void TrailEffect::DrawSphere()
{
	auto commandList = trailEffectBase_->GetDxEngine()->GetCommandList();

	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(2, textureData_.textureIndex);

	commandList->DrawInstanced(kSubdivision_ * kSubdivision_ * 6, 1, 0, 0);
}

void TrailEffect::SetPosition(std::vector<Vector3> pos)
{
	vertexData_[0].position = { pos[0].x,pos[0].y,pos[0].z,1.0f };
	vertexData_[1].position = { pos[1].x,pos[1].y,pos[1].z,1.0f };
	vertexData_[2].position = { pos[2].x,pos[2].y,pos[2].z,1.0f };
	vertexData_[3].position = { pos[3].x,pos[3].y,pos[3].z,1.0f };

	vertexData_[0].texcoord = { 0.0f,0.0f };
	vertexData_[1].texcoord = { 1.0f,0.0f };
	vertexData_[2].texcoord = { 0.0f,1.0f };
	vertexData_[3].texcoord = { 1.0f,1.0f };
}

void TrailEffect::SetTexture(const std::string& directoryPath, const std::string& filePath)
{
	textureData_.directoryPath = directoryPath;
	textureData_.filePath = filePath;

	TextureManager::GetInstance()->LoadTexture(textureData_.directoryPath + "/" + textureData_.filePath);
	textureData_.textureIndex = TextureManager::GetInstance()->GetSrvIndex(textureData_.directoryPath + "/" + textureData_.filePath);
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

void TrailEffect::CreateVertexBufferView(uint32_t kVertexSize)
{
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * kVertexSize;
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
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

	vertexData_[0].position = { positions_[0].x,positions_[0].y,positions_[0].z,1.0f };
	vertexData_[1].position = { positions_[1].x,positions_[1].y,positions_[1].z,1.0f };
	vertexData_[2].position = { positions_[2].x,positions_[2].y,positions_[2].z,1.0f };
	vertexData_[3].position = { positions_[3].x,positions_[3].y,positions_[3].z,1.0f };

	vertexData_[0].texcoord = { 0.0f,0.0f };
	vertexData_[1].texcoord = { 1.0f,0.0f };
	vertexData_[2].texcoord = { 0.0f,1.0f };
	vertexData_[3].texcoord = { 1.0f,1.0f };

	/*vertexData_[0] = { -0.5f,-0.5f,0.0f,1.0f };*/
	/*vertexData_[1] = { -0.5f,0.5f,0.0f,1.0f };*/
	/*vertexData_[2] = { 0.5f,-0.5f,0.0f,1.0f };*/
	/*vertexData_[3] = { 0.5f,0.5f,0.0f,1.0f };*/
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

TrailEffect::VertexData* TrailEffect::CreateSphereVertexData(VertexData* vertexData, uint32_t kSubdivision)
{
	const float pi = static_cast<float>(std::numbers::pi);
	const float kLonEvery = 2 * pi / float(kSubdivision); // 経度
	const float kLatEvery = pi / float(kSubdivision);     // 緯度

	for (uint32_t latIndex = 0; latIndex < kSubdivision; latIndex++) {
		float lat0 = -pi / 2.0f + kLatEvery * latIndex; // 緯度の方向に分割
		float lat1 = lat0 + kLatEvery;
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; lonIndex++) {
			uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;
			float lon0 = kLonEvery * lonIndex; // 経度の方向に分割
			float lon1 = lon0 + kLonEvery;
			float u0 = float(lonIndex) / float(kSubdivision);
			float u1 = float(lonIndex + 1) / float(kSubdivision);
			float v0 = 1.0f - float(latIndex) / float(kSubdivision);
			float v1 = 1.0f - float(latIndex + 1) / float(kSubdivision);
			//頂点座標を計算
			vertexData[start].position = { std::cos(lat0) * std::cos(lon0), std::sin(lat0), std::cos(lat0) * std::sin(lon0), 1.0f };
			vertexData[start].texcoord = { u0, v0 };
			vertexData[start + 1].position = { std::cos(lat1) * std::cos(lon0), std::sin(lat1), std::cos(lat1) * std::sin(lon0), 1.0f };
			vertexData[start + 1].texcoord = { u0, v1 };
			vertexData[start + 2].position = { std::cos(lat0) * std::cos(lon1), std::sin(lat0), std::cos(lat0) * std::sin(lon1), 1.0f };
			vertexData[start + 2].texcoord = { u1, v0 };
			vertexData[start + 3].position = { std::cos(lat1) * std::cos(lon0), std::sin(lat1), std::cos(lat1) * std::sin(lon0), 1.0f };
			vertexData[start + 3].texcoord = { u0, v1 };
			vertexData[start + 4].position = { std::cos(lat1) * std::cos(lon1), std::sin(lat1), std::cos(lat1) * std::sin(lon1), 1.0f };
			vertexData[start + 4].texcoord = { u1, v1 };
			vertexData[start + 5].position = { std::cos(lat0) * std::cos(lon1), std::sin(lat0), std::cos(lat0) * std::sin(lon1), 1.0f };
			vertexData[start + 5].texcoord = { u1, v0 };
		}
	}
	return vertexData;
}

