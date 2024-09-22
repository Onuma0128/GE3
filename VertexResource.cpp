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
	modelDataObject_[0] = LoadObjFile("resources", "terrain.obj");
	modelDataObject_[1] = LoadObjFile("resources", "multiMesh.obj");
	modelDataObject_[2] = LoadObjFile("resources", "teapot.obj");
	modelDataObject_[3] = LoadObjFile("resources", "bunny.obj");
	modelDataObject_[4] = LoadObjFile("resources", "suzanne.obj");

	// 実際に頂点リソースを作る
	for (uint32_t i = 0; i < 5; i++) {
		vertexResourceObject_[i] = CreateBufferResource(device, sizeof(VertexData) * modelDataObject_[i].vertices.size()).Get();
		vertexBufferViewObject_[i].BufferLocation = vertexResourceObject_[i]->GetGPUVirtualAddress();
		vertexBufferViewObject_[i].SizeInBytes = UINT(sizeof(VertexData) * modelDataObject_[i].vertices.size());
		vertexBufferViewObject_[i].StrideInBytes = sizeof(VertexData);

		vertexResourceObject_[i]->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataObject_));
		std::memcpy(vertexDataObject_, modelDataObject_[i].vertices.data(), sizeof(VertexData) * modelDataObject_[i].vertices.size());
	}

	///=============================================================================================================

	// Sphere用の頂点リソース
	vertexResourceSphere_ = CreateBufferResource(device, sizeof(VertexData) * 1536).Get();
	vertexBufferViewSphere_.BufferLocation = vertexResourceSphere_->GetGPUVirtualAddress();
	vertexBufferViewSphere_.SizeInBytes = sizeof(VertexData) * 1536;
	vertexBufferViewSphere_.StrideInBytes = sizeof(VertexData);

	// Sphere
	vertexResourceSphere_->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSphere_));
	vertexDataSphere_ = DrawSphere(vertexDataSphere_, vertexCount_);
	// 法線情報の追加
	for (uint32_t index = 0; index < 1536; ++index) {
		vertexDataSphere_[index].normal = Normalize(vertexDataSphere_[index].position);
	}

	///=============================================================================================================

	// Sprite用の頂点リソースを作る
	//vertexResourceSprite_ = CreateBufferResource(device, sizeof(VertexData) * 4).Get();
	//indexResourceSprite_ = CreateBufferResource(device, sizeof(uint32_t) * 6).Get();
	//vertexBufferViewSprite_.BufferLocation = vertexResourceSprite_->GetGPUVirtualAddress();
	//indexBufferViewSprite_.BufferLocation = indexResourceSprite_->GetGPUVirtualAddress();
	//vertexBufferViewSprite_.SizeInBytes = sizeof(VertexData) * 4;
	//indexBufferViewSprite_.SizeInBytes = sizeof(uint32_t) * 6;
	//vertexBufferViewSprite_.StrideInBytes = sizeof(VertexData);
	//indexBufferViewSprite_.Format = DXGI_FORMAT_R32_UINT;
	//// 書き込むためのアドレスを取得
	//vertexResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite_));
	//// 四角形の4つの頂点
	//vertexDataSprite_[0].position = { 0.0f,360.0f,0.0f,1.0f };//左下
	//vertexDataSprite_[0].texcoord = { 0.0f,1.0f };
	//vertexDataSprite_[1].position = { 0.0f,0.0f,0.0f,1.0f };//左上
	//vertexDataSprite_[1].texcoord = { 0.0f,0.0f };
	//vertexDataSprite_[2].position = { 640.0f,360.0f,0.0f,1.0f };//右下
	//vertexDataSprite_[2].texcoord = { 1.0f,1.0f };
	//vertexDataSprite_[3].position = { 640.0f,0.0f,0.0f,1.0f };//右上
	//vertexDataSprite_[3].texcoord = { 1.0f,0.0f };
	//// 法線情報の追加
	//vertexDataSprite_[0].normal = { 0.0f,0.0f,-1.0f };
	//// IndexData
	//indexResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSprite_));
	//indexDataSprite_[0] = 0; indexDataSprite_[1] = 1; indexDataSprite_[2] = 2;
	//indexDataSprite_[3] = 1; indexDataSprite_[4] = 3; indexDataSprite_[5] = 2;

	/*===================================== Light =====================================*/

	// 平行光源用のリソースを作る
	directionalLightResource_ = CreateBufferResource(device, sizeof(DirectionalLight)).Get();
	directionalLightBufferView_.BufferLocation = directionalLightResource_->GetGPUVirtualAddress();
	directionalLightBufferView_.SizeInBytes = sizeof(DirectionalLight);
	directionalLightBufferView_.StrideInBytes = sizeof(DirectionalLight);
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	// デフォルト値はとりあえず以下のようにする
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = { 0.0f,1.0f,0.0f };
	directionalLightData_->intensity = 0.0f;

	// ポイントライト用
	pointLightResource_ = CreateBufferResource(device, sizeof(PointLight)).Get();
	pointLightBufferView_.BufferLocation = pointLightResource_->GetGPUVirtualAddress();
	pointLightBufferView_.SizeInBytes = sizeof(PointLight);
	pointLightBufferView_.StrideInBytes = sizeof(PointLight);
	pointLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData_));
	// デフォルト値はとりあえず以下のようにする
	pointLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	pointLightData_->position = { 0.0f,2.0f,0.0f };
	pointLightData_->intensity = 0.0f;
	pointLightData_->radius = 6.0f;
	pointLightData_->decay = 2.0f;

	// スポットライト用
	spotLightResource_ = CreateBufferResource(device, sizeof(SpotLight)).Get();
	spotLightBufferView_.BufferLocation = spotLightResource_->GetGPUVirtualAddress();
	spotLightBufferView_.SizeInBytes = sizeof(SpotLight);
	spotLightBufferView_.StrideInBytes = sizeof(SpotLight);
	spotLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&spotLightData_));
	// デフォルト値はとりあえず以下のようにする
	spotLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	spotLightData_->position = { 2.0f,1.25f,0.0f };
	spotLightData_->distance = 7.0f;
	spotLightData_->direction = Normalize(Vector3{ -1.0f,-1.0f,0.0f });
	spotLightData_->intensity = 4.0f;
	spotLightData_->decay = 2.0f;
	spotLightData_->cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
	spotLightData_->cosFalloffStart = std::cos(std::numbers::pi_v<float> / 5.0f);

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

	// Object用のマテリアルリソースを作る
	materialResourceObject_ = CreateBufferResource(device, sizeof(Material)).Get();
	// 書き込むためのアドレスを取得
	materialResourceObject_->Map(0, nullptr, reinterpret_cast<void**>(&materialDataObject_));
	// 今回は白を書き込んでいく
	materialDataObject_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialDataObject_->enableLighting = true;
	materialDataObject_->uvTransform = MakeIdentity4x4();
	materialDataObject_->shininess = 8.0f;

	// Sphere用のマテリアルリソースを作る
	materialResourceSphere_ = CreateBufferResource(device, sizeof(Material)).Get();
	// 書き込むためのアドレスを取得
	materialResourceSphere_->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSphere_));
	// 今回は白を書き込んでいく
	materialDataSphere_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialDataSphere_->enableLighting = true;
	materialDataSphere_->uvTransform = MakeIdentity4x4();
	materialDataSphere_->shininess = 16.0f;

	// Sprite用のマテリアルリソースを作る
	//materialResourceSprite_ = CreateBufferResource(device, sizeof(Material)).Get();
	//// 書き込むためのアドレスを取得
	//materialResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSprite_));
	//// 今回は白を書き込んでいく
	//materialDataSprite_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	//materialDataSprite_->enableLighting = false;
	//materialDataSprite_->uvTransform = MakeIdentity4x4();

	///=============================================================================================================

	// WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	wvpResourceObject_ = CreateBufferResource(device, sizeof(Matrix4x4)).Get();
	wvpResourceSphere_ = CreateBufferResource(device, sizeof(Matrix4x4)).Get();
	//transformationMatrixResourceSprite_ = CreateBufferResource(device, sizeof(Matrix4x4)).Get();
	cameraResource_ = CreateBufferResource(device, sizeof(Vector3)).Get();

	// 書き込むためのアドレスを取得
	wvpResourceObject_->Map(0, nullptr, reinterpret_cast<void**>(&wvpDataObject_));
	wvpResourceSphere_->Map(0, nullptr, reinterpret_cast<void**>(&wvpDataSphere_));
	//transformationMatrixResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite_));
	cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));

	// 単位行列を書き込んでおく
	wvpDataObject_->WVP = MakeIdentity4x4();
	wvpDataObject_->World = MakeIdentity4x4();
	wvpDataSphere_->WVP = MakeIdentity4x4();
	wvpDataSphere_->World = MakeIdentity4x4();
	/*transformationMatrixDataSprite_->WVP = MakeIdentity4x4();
	transformationMatrixDataSprite_->World = MakeIdentity4x4();*/
	cameraData_->worldPosition = cameraTransform_.translate;

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
	//カメラのワールド座標を取得
	Matrix4x4 cameraMatrix = MakeAfineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.0f);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	cameraData_->worldPosition = cameraTransform_.translate;

	// エミッターによるパーティクルの発生
	if (moveStart_) {
		emitter_.frequencyTime += kDeltaTime;
	}
	if (emitter_.frequency <= emitter_.frequencyTime) {
		std::mt19937 randomEngine_(seedGenerator_());
		particles_.splice(particles_.end(), Emit(emitter_, randomEngine_));
		emitter_.frequencyTime -= emitter_.frequency;
	}

	// パーティクルの更新
	numInstance = 0;
	for (std::list<Particle>::iterator particleIterator = particles_.begin();
		particleIterator != particles_.end();) {
		if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
			particleIterator = particles_.erase(particleIterator);
			continue;
		}
		Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
		Matrix4x4 billboardMatrix = Multiply(backToFrontMatrix, cameraMatrix);
		billboardMatrix.m[3][0] = 0.0f; billboardMatrix.m[3][1] = 0.0f; billboardMatrix.m[3][2] = 0.0f;
		Matrix4x4 worldMatrix = MakeScaleMatrix(particleIterator->transform.scale) * billboardMatrix * MakeTranslateMatrix(particleIterator->transform.translate);
		Matrix4x4 worldViewMatrix = Multiply(worldMatrix, viewMatrix);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldViewMatrix, projectionMatrix);
		//パーティクルの動き
		if (moveStart_) {
			if (IsCollision(accelerationField_.area, particleIterator->transform.translate) && isFieldStart_) {
				particleIterator->velocity = accelerationField_.acceleration * kDeltaTime + particleIterator->velocity;
			}
			particleIterator->transform.translate = particleIterator->velocity * kDeltaTime + particleIterator->transform.translate;
			particleIterator->currentTime += kDeltaTime;
		}
		float alpha = 1.0f - (particleIterator->currentTime / particleIterator->lifeTime);
		if (numInstance < kNumMaxInstance) {
			instancingData_[numInstance].WVP = worldViewProjectionMatrix;
			instancingData_[numInstance].World = worldViewMatrix;
			instancingData_[numInstance].color = particleIterator->color;
			instancingData_[numInstance].color.w = alpha;
			++numInstance;
		}
		++particleIterator;
	}

	// Object用
	Matrix4x4 worldMatrixObject = MakeAfineMatrix(transformObject_.scale, transformObject_.rotate, transformObject_.translate);
	Matrix4x4 worldViewMatrixObject = Multiply(worldMatrixObject, viewMatrix); // カメラから見たワールド座標に変換
	Matrix4x4 worldViewProjectionMatrixObject = Multiply(worldViewMatrixObject, projectionMatrix); // 射影行列を適用してワールドビュープロジェクション行列を計算
	wvpDataObject_->WVP = worldViewProjectionMatrixObject; // ワールドビュープロジェクション行列を更新
	wvpDataObject_->World = worldViewMatrixObject; // ワールド座標行列を更新
	wvpDataObject_->WorldInverseTranspose = Inverse(worldViewMatrixObject);

	//Sphere用
	Matrix4x4 worldMatrixSphere = MakeAfineMatrix(transformSphere_.scale, transformSphere_.rotate, transformSphere_.translate);
	Matrix4x4 worldViewMatrixSphere = Multiply(worldMatrixSphere, viewMatrix); // カメラから見たワールド座標に変換
	Matrix4x4 worldViewProjectionMatrixSphere = Multiply(worldViewMatrixSphere, projectionMatrix); // 射影行列を適用してワールドビュープロジェクション行列を計算
	wvpDataSphere_->WVP = worldViewProjectionMatrixSphere; // 球体のワールドビュープロジェクション行列を更新
	wvpDataSphere_->World = worldViewMatrixSphere; // 球体のワールド座標行列を更新
	wvpDataSphere_->WorldInverseTranspose = Inverse(worldViewMatrixSphere);

	uvTransformMatrix_ = MakeScaleMatrix(uvTransformSphere_.scale);
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeRotateZMatrix(uvTransformSphere_.rotate.z));
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeTranslateMatrix(uvTransformSphere_.translate));
	materialDataSphere_->uvTransform = uvTransformMatrix_;

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

	//LightのNormalize
	directionalLightData_->direction = Normalize(directionalLightData_->direction);

	spotLightData_->direction = Normalize(spotLightData_->direction);
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

		if (ImGui::TreeNodeEx("Object", flag)) {
			ImGui::DragFloat3("Scale", &transformObject_.scale.x, 0.01f);
			ImGui::DragFloat3("Rotate", &transformObject_.rotate.x, 0.01f);
			ImGui::DragFloat3("Translate", &transformObject_.translate.x, 0.01f);
			ImGui::ColorEdit4("Color", (float*)&materialDataObject_->color.x);
			ImGui::Checkbox("Light", &objectLight_);
			materialDataObject_->enableLighting = objectLight_;
			ImGui::Text("objIndex %d", objIndex_);
			if (ImGui::Button("plane")) {
				objIndex_ = 0;
			}
			if (ImGui::Button("multiMesh")) {
				objIndex_ = 1;
			}
			if (ImGui::Button("teapot")) {
				objIndex_ = 2;
			}
			if (ImGui::Button("bunny")) {
				objIndex_ = 3;
			}
			if (ImGui::Button("suzannu")) {
				objIndex_ = 4;
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("Sphere", flag)) {
			ImGui::DragFloat3("Scale", &transformSphere_.scale.x, 0.01f);
			ImGui::DragFloat3("Rotate", &transformSphere_.rotate.x, 0.01f);
			ImGui::DragFloat3("Translate", &transformSphere_.translate.x, 0.01f);
			ImGui::DragFloat2("UVScale", &uvTransformSphere_.scale.x, 0.01f, -10.0f, 10.0f);
			ImGui::SliderAngle("UVRotate", &uvTransformSphere_.rotate.z);
			ImGui::DragFloat2("UVTranslate", &uvTransformSphere_.translate.x, 0.01f, -10.0f, 10.0f);
			ImGui::ColorEdit4("Color", (float*)&materialDataSphere_->color.x);
			ImGui::Checkbox("MonsterBall", &useMonsterBall_);
			ImGui::Checkbox("Light", &sphereLight_);
			materialDataSphere_->enableLighting = sphereLight_;
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

		if (ImGui::TreeNodeEx("Camera & Light", flag)) {
			ImGui::DragFloat3("Rotate", &cameraTransform_.rotate.x, 0.01f);
			ImGui::DragFloat3("Translate", &cameraTransform_.translate.x, 0.01f);
			if (ImGui::TreeNodeEx("Directional Light", flag)) {
				ImGui::ColorEdit4("LightColor", (float*)&directionalLightData_->color.x);
				ImGui::DragFloat3("DirectionalLightData.Direction", &directionalLightData_->direction.x, 0.01f);
				ImGui::DragFloat("DirectionalLightData.Intensity", &directionalLightData_->intensity, 0.01f);
				ImGui::TreePop();
			}
			if (ImGui::TreeNodeEx("Point Light", flag)) {
				ImGui::DragFloat3("PointLightData.pos", &pointLightData_->position.x, 0.01f);
				ImGui::DragFloat("PointLightRadius", &pointLightData_->radius, 0.01f);
				ImGui::DragFloat("PointLightDecay", &pointLightData_->decay, 0.01f);
				ImGui::DragFloat("PointLightIntensity", &pointLightData_->intensity, 0.01f);
				ImGui::TreePop();
			}
			if (ImGui::TreeNodeEx("Spot Light", flag)) {
				ImGui::DragFloat3("SpotLightData.pos", &spotLightData_->position.x, 0.01f);
				ImGui::DragFloat3("SpotLightData.direction", &spotLightData_->direction.x, 0.01f);
				ImGui::DragFloat("SpotLightData.intensity", &spotLightData_->intensity, 0.01f);
				ImGui::DragFloat("SpotLightData.cosAngle", &spotLightData_->cosAngle, 0.01f);
				ImGui::DragFloat("SpotLightData.cosFalloffStart", &spotLightData_->cosFalloffStart, 0.01f);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

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
