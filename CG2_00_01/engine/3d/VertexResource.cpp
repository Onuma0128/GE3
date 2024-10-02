#include "VertexResource.h"
#include <cassert>
#include "WinApp.h"

void VertexResource::Initialize(ComPtr<ID3D12Device> device)
{
	modelData_ = LoadObjFile("resources", "plane.obj");

	// 実際に頂点リソースを作る
	vertexResource_ = CreateBufferResource(device, sizeof(VertexData) * modelData_.vertices.size()).Get();
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());

	///=============================================================================================================

	// Instancing用
	instancingResource_ = CreateBufferResource(device, sizeof(ParticleForGPU) * kNumMaxInstance).Get();
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
	for (uint32_t index = 0; index < kNumMaxInstance; ++index) {
		instancingData_[index].color = Vector4{ 1.0f,1.0f,1.0f,1.0f };
	}


	///=============================================================================================================

	// マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource_ = CreateBufferResource(device, sizeof(Material)).Get();
	// 書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	// 今回は白を書き込んでいく
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = false;
	materialData_->uvTransform = MakeIdentity4x4();

	///=============================================================================================================

	//// WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	//cameraResource_ = CreateBufferResource(device, sizeof(Vector3)).Get();

	//// 書き込むためのアドレスを取得
	//cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));

	//// 単位行列を書き込んでおく
	//cameraData_->worldPosition = cameraTransform_.translate;

	///=============================================================================================================

	std::mt19937 randomEngine_(seedGenerator_());

	emitter_.transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	emitter_.frequency = 0.5f;
	emitter_.frequencyTime = 0.0f;
	accelerationField_.acceleration = { 0.0f,10.0f,0.0f };
	accelerationField_.area.min = { -1.0f,-1.0f,-1.0f };
	accelerationField_.area.max = { 1.0f,1.0f,1.0f };
	// emitter_.frequencyごとに出すパーティクルの個数
	emitter_.count = 3;
	particles_.splice(particles_.end(), Emit(emitter_, randomEngine_));

	for (uint32_t index = 0; index < kNumMaxInstance; ++index) {
		instancingData_[index].WVP = MakeIdentity4x4();
		instancingData_[index].World = MakeIdentity4x4();
	}
	moveStart_ = false;
	isFieldStart_ = false;
}

void VertexResource::Update()
{
	////カメラのワールド座標を取得
	//Matrix4x4 cameraMatrix = MakeAfineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	//Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.0f);
	//Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	//cameraData_->worldPosition = cameraTransform_.translate;

	// エミッターによるパーティクルの発生
	//if (moveStart_) {
	//	emitter_.frequencyTime += kDeltaTime;
	//}
	//if (emitter_.frequency <= emitter_.frequencyTime) {
	//	std::mt19937 randomEngine_(seedGenerator_());
	//	particles_.splice(particles_.end(), Emit(emitter_, randomEngine_));
	//	emitter_.frequencyTime -= emitter_.frequency;
	//}

	//// パーティクルの更新
	//numInstance = 0;
	//for (std::list<Particle>::iterator particleIterator = particles_.begin();
	//	particleIterator != particles_.end();) {
	//	if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
	//		particleIterator = particles_.erase(particleIterator);
	//		continue;
	//	}
	//	Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
	//	Matrix4x4 billboardMatrix = Multiply(backToFrontMatrix, cameraMatrix);
	//	billboardMatrix.m[3][0] = 0.0f; billboardMatrix.m[3][1] = 0.0f; billboardMatrix.m[3][2] = 0.0f;
	//	Matrix4x4 worldMatrix = MakeScaleMatrix(particleIterator->transform.scale) * billboardMatrix * MakeTranslateMatrix(particleIterator->transform.translate);
	//	Matrix4x4 worldViewMatrix = Multiply(worldMatrix, viewMatrix);
	//	Matrix4x4 worldViewProjectionMatrix = Multiply(worldViewMatrix, projectionMatrix);
	//	//パーティクルの動き
	//	if (moveStart_) {
	//		if (IsCollision(accelerationField_.area, particleIterator->transform.translate) && isFieldStart_) {
	//			particleIterator->velocity = accelerationField_.acceleration * kDeltaTime + particleIterator->velocity;
	//		}
	//		particleIterator->transform.translate = particleIterator->velocity * kDeltaTime + particleIterator->transform.translate;
	//		particleIterator->currentTime += kDeltaTime;
	//	}
	//	float alpha = 1.0f - (particleIterator->currentTime / particleIterator->lifeTime);
	//	if (numInstance < kNumMaxInstance) {
	//		instancingData_[numInstance].WVP = worldViewProjectionMatrix;
	//		instancingData_[numInstance].World = worldViewMatrix;
	//		instancingData_[numInstance].color = particleIterator->color;
	//		instancingData_[numInstance].color.w = alpha;
	//		++numInstance;
	//	}
	//	++particleIterator;
	//}

	//Sprite用
	//Matrix4x4 worldMatrixSprite = MakeAfineMatrix(transformSprite_.scale, transformSprite_.rotate, transformSprite_.translate);
	//Matrix4x4 viewMatrixSprite = MakeIdentity4x4();
	//Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kClientWidth), float(WinApp::kClientHeight), 0.0f, 100.0f);
	//Matrix4x4 worldViewProjectionMatrixSprite = Multiply(worldMatrixSprite, Multiply(viewMatrixSprite, projectionMatrixSprite));
	//transformationMatrixDataSprite_->WVP = worldViewProjectionMatrixSprite;
	//transformationMatrixDataSprite_->World = worldViewProjectionMatrixSprite;
	//transformationMatrixDataSprite_->WorldInverseTranspose = Inverse(worldViewProjectionMatrixSprite);

	////UVの行列を生成
	//uvTransformMatrix_ = MakeScaleMatrix(uvTransformSprite_.scale);
	//uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeRotateZMatrix(uvTransformSprite_.rotate.z));
	//uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeTranslateMatrix(uvTransformSprite_.translate));
	//materialDataSprite_->uvTransform = uvTransformMatrix_;

}

void VertexResource::ImGui()
{
	ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen;

	if (ImGui::TreeNodeEx("Settings", flag)) {

		if (ImGui::TreeNodeEx("Particle", flag)) {
			uint32_t min = 0;
			uint32_t max = 20;
			ImGui::SliderScalar("Emitter_count", ImGuiDataType_U32, &emitter_.count, &min, &max);
			ImGui::DragFloat3("EmitterTranslate", &emitter_.transform.translate.x, 0.01f, -100.0f, 100.0f);
			ImGui::ColorEdit4("Color", (float*)&materialData_->color.x);
			ImGui::Checkbox("move", &moveStart_);
			ImGui::Checkbox("field", &isFieldStart_);
			ImGui::Checkbox("Circle", &useCircle_);
			ImGui::Text("%d", numInstance);
			if (ImGui::Button("Add Particle")) {
				std::mt19937 randomEngine_(seedGenerator_());
				particles_.splice(particles_.end(), Emit(emitter_, randomEngine_));
			}
			ImGui::TreePop();
		}

		/*if (ImGui::TreeNodeEx("Sprite", flag)) {
			ImGui::DragFloat3("Scale", &transformSprite_.scale.x, 0.01f);
			ImGui::DragFloat3("Rotate", &transformSprite_.rotate.x, 0.01f);
			ImGui::DragFloat3("Translate", &transformSprite_.translate.x, 5.0f);
			ImGui::DragFloat2("UVTranslate", &uvTransformSprite_.translate.x, 0.01f, -10.0f, 10.0f);
			ImGui::DragFloat2("UVScale", &uvTransformSprite_.scale.x, 0.01f, -10.0f, 10.0f);
			ImGui::SliderAngle("UVRotate", &uvTransformSprite_.rotate.z);
			ImGui::TreePop();
		}*/

		/*if (ImGui::TreeNodeEx("Camera", flag)) {
			ImGui::DragFloat3("Rotate", &cameraTransform_.rotate.x, 0.01f);
			ImGui::DragFloat3("Translate", &cameraTransform_.translate.x, 0.01f);
			ImGui::TreePop();
		}*/

		ImGui::TreePop();
	}
}

ResourceObject CreateBufferResource(ComPtr<ID3D12Device> device, size_t sizeInBytes)
{
	// 頂点リソースのヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties = {};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

	// 頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc = {};
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeof(Material) * sizeInBytes;
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点リソースを作成する
	ID3D12Resource* vertexResource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexResource));

	assert(SUCCEEDED(hr)); // エラーチェック

	return vertexResource;

}
