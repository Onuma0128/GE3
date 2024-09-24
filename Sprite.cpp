#include "Sprite.h"
#include "SpriteBase.h"
#include "WinApp.h"
#include "TextureManager.h"

void Sprite::Initialize(std::string textureFilePath)
{
	this->spriteBase_ = SpriteBase::GetInstance()->GetInstance();

	TextureManager::GetInstance()->LoadTexture("resources/" + textureFilePath);

	textureIndex_ = TextureManager::GetInstance()->GetTextureIndexByFilePath("resources/" + textureFilePath);

	VertexDataInitialize();

	MaterialDataInitialize();

	TransformationMatrixDataInitialize();
}

void Sprite::Update()
{
	// セッターで貰った値を格納
	AccessorUpdate();

	UpdateMatrix();
}

void Sprite::Draw()
{
	spriteBase_->GetDxEngine()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	spriteBase_->GetDxEngine()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	spriteBase_->GetDxEngine()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	spriteBase_->GetDxEngine()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
	spriteBase_->GetDxEngine()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(textureIndex_));
	spriteBase_->GetDxEngine()->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void Sprite::VertexDataInitialize()
{
	vertexResource_ = CreateBufferResource(spriteBase_->GetDxEngine()->GetDevice(), sizeof(VertexData) * 4).Get();
	indexResource_ = CreateBufferResource(spriteBase_->GetDxEngine()->GetDevice(), sizeof(uint32_t) * 6).Get();
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 4;
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
	// 書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	// 四角形の4つの頂点
	vertexData_[0].position = { 0.0f,1.0f,0.0f,1.0f };//左下
	vertexData_[0].texcoord = { 0.0f,1.0f };
	vertexData_[1].position = { 0.0f,0.0f,0.0f,1.0f };//左上
	vertexData_[1].texcoord = { 0.0f,0.0f };
	vertexData_[2].position = { 1.0f,1.0f,0.0f,1.0f };//右下
	vertexData_[2].texcoord = { 1.0f,1.0f };
	vertexData_[3].position = { 1.0f,0.0f,0.0f,1.0f };//右上
	vertexData_[3].texcoord = { 1.0f,0.0f };
	// 法線情報の追加
	vertexData_[0].normal = { 0.0f,0.0f,-1.0f };
	// IndexData
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	indexData_[0] = 0; indexData_[1] = 1; indexData_[2] = 2;
	indexData_[3] = 1; indexData_[4] = 3; indexData_[5] = 2;
}

void Sprite::MaterialDataInitialize()
{
	materialResource_ = CreateBufferResource(spriteBase_->GetDxEngine()->GetDevice(), sizeof(Material)).Get();
	
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = false;
	materialData_->uvTransform = MakeIdentity4x4();
}

void Sprite::TransformationMatrixDataInitialize()
{
	transformationMatrixResource_ = CreateBufferResource(spriteBase_->GetDxEngine()->GetDevice(), sizeof(Matrix4x4)).Get();
	transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	transformationMatrixData_->WVP = MakeIdentity4x4();
	transformationMatrixData_->World = MakeIdentity4x4();
}

void Sprite::AccessorUpdate()
{
	// トランスフォームを設定
	transform_.scale = { size_.x,size_.y,1.0f };
	transform_.rotate = { 0.0f,0.0f,rotation_ };
	transform_.translate = { position_.x,position_.y,0.0f };

	// アンカーポイントを設定
	float left = 0.0f - anchorPoint_.x;
	float right = 1.0f - anchorPoint_.x;
	float top = 0.0f - anchorPoint_.y;
	float bottom = 1.0f - anchorPoint_.y;

	// フリップを設定(反転する)
	// 左右反転
	if (isFlipX_) {
		left = -left;
		right = -right;
	}
	// 上下反転
	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}

	// アンカーポイントを設定
	vertexData_[0].position = { left,bottom,0.0f,1.0f };
	vertexData_[1].position = { left,top,0.0f,1.0f };
	vertexData_[2].position = { right,bottom,0.0f,1.0f };
	vertexData_[3].position = { right,top,0.0f,1.0f };
}

void Sprite::UpdateMatrix()
{
	Matrix4x4 worldMatrix = MakeAfineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	Matrix4x4 viewMatrix = MakeIdentity4x4();
	Matrix4x4 projectionMatrix = MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kClientWidth), float(WinApp::kClientHeight), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	transformationMatrixData_->WVP = worldViewProjectionMatrix;
	transformationMatrixData_->World = worldViewProjectionMatrix;
	transformationMatrixData_->WorldInverseTranspose = Inverse(worldViewProjectionMatrix);
}
