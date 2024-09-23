#include "DirectXEngine.h"
#include <cassert>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include <format>
#include "DepthStencilTexture.h"
#include "DescriptorHeap.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
#include "TextureManager.h"

using Microsoft::WRL::ComPtr;

const uint32_t DirectXEngine::kMaxSRVCount = 512;

DirectXEngine::~DirectXEngine()
{
	delete logger_;
	delete stringUtility_;
	delete vertexResource_;
	delete textureResource_;
	delete pipelineState_;

	//解放の処理
	CloseHandle(fenceEvent_);
}

void DirectXEngine::Initialize(WinApp* winApp)
{
	assert(winApp);
	winApp_ = winApp;

	// デバイスの初期化
	DeviceInitialize();
	// コマンド関連の初期化
	CommandInitialize();
	// スワップチェーンの初期化
	SwapChainInitialize();
	// 深度バッファの初期化
	DepthStencilInitialize();
	// 各種デスクリプタヒープの初期化
	DescriptorHeapInitialize();
	// レンダーターゲットビューの初期化
	RTVInitialize();
	// フェンスの初期化
	FenceInitialize();
	// ビューポートの初期化
	ViewportInitialize();
	// シザー矩形の初期化
	RectInitialize();
	// DXCコンパイラの初期化
	DxcCompilerInitialize();
	// ImGuiの初期化
	ImGuiInitialize();
	// VertexResourceの初期化
	VertexResourceInitialize();
	// TextureResourceの初期化
	TextureResourceInitialize();
	// InstancingSRVの初期化
	InstancingSrvInitialize();
	// IncludeHandlerの初期化
	IncludeHandlerInitialize();
	// PipelineStateの初期化
	PipelineStateInitialize();
}

void DirectXEngine::DeviceInitialize()
{
	//デバックレイヤー
#ifdef _DEBUG
	ComPtr<ID3D12Debug1> debugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		//デバックレイヤーを有効化にする
		debugController->EnableDebugLayer();
		//さらにGPU側でもチェックを行うようにする
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif // DEBUG

	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(hr));

	//使用するアダプタ用の変数。最初にnullptrを入れておく
	ComPtr<IDXGIAdapter4> useAdapter = nullptr;
	//良い順にアダプタを頼む
	for (UINT i = 0; dxgiFactory_->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) !=
		DXGI_ERROR_NOT_FOUND; ++i) {
		//アダプタの情報を取得する
		DXGI_ADAPTER_DESC3 adapteDesc{};
		hr = useAdapter->GetDesc3(&adapteDesc);
		assert(SUCCEEDED(hr));
		//ソフトウェアアダプタでなければ採用
		if (!(adapteDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			//採用したアダプタの情報をログに出力。wstringの方なので注意
			logger_->Log(stringUtility_->ConvertString(std::format(L"Use Adapater : {}\n", adapteDesc.Description)));
			//Log(std::format(L"Use Adapater : {}\n", adapteDesc.Description));
			break;
		}
		useAdapter = nullptr; //ソフトウェアアダプタの場合は見えなかったことにする
	}
	//適切なアダプタが見つからなかったので起動できない
	assert(useAdapter != nullptr);

	//機能レベルとログ出力用の文字列
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
	};
	const char* featureLevelStrings[] = { "12.2","12.1","12.0" };
	//高い順に生成できるか試していく
	for (size_t i = 0; i < _countof(featureLevels); ++i) {
		//採用したアダプタでデバイスを生成
		hr = D3D12CreateDevice(useAdapter.Get(), featureLevels[i], IID_PPV_ARGS(&device_));
		//指定した機能レベルでデバイスが生成できたかを確認
		if (SUCCEEDED(hr)) {
			//生成できたのでログ出力を行ってループを抜ける
			logger_->Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
			break;
		}
	}
	//デバイスの生成がうまくいかなかったので起動できない
	assert(device_ != nullptr);
	logger_->Log("Complete create D3D12Device!!!\n");
#ifdef _DEBUG
	ID3D12InfoQueue* infoQueue = nullptr;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		//やばいエラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		//エラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		//警告時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
		//解放
		infoQueue->Release();
		//抑制するメッセージのID
		D3D12_MESSAGE_ID denyIds[] = {
			//Windows11でのDXGIデバックレイヤーとDX12デバックレイヤーの相互作用バグによるエラーメッセージ
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		//抑制するレベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		//指定したメッセージの表示を抑制する
		infoQueue->PushStorageFilter(&filter);
	}
#endif // DEBUG
}

void DirectXEngine::CommandInitialize()
{
	HRESULT hr{};

	//コマンドキューを生成する
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	hr = device_->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue_));
	assert(SUCCEEDED(hr));

	//コマンドアロケータを生成する
	hr = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	assert(SUCCEEDED(hr));

	//コマンドリストを生成する
	hr = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr,
		IID_PPV_ARGS(&commandList_));
}

void DirectXEngine::SwapChainInitialize()
{
	HRESULT hr{};

	//スワップチェーンを生成する
	swapChainDesc_.Width = winApp_->kClientWidth; //画面の幅
	swapChainDesc_.Height = winApp_->kClientHeight; //画面の高さ
	swapChainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //色の形式
	swapChainDesc_.SampleDesc.Count = 1; //マルチサンプルしない
	swapChainDesc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //描画のターゲットとして利用する
	swapChainDesc_.BufferCount = 2; //ダブルバッファ
	swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; //モニタにうつしたら、中身を破棄
	//コマンドキュー、ウィンドウハンドル、設定を渡して生成する
	hr = dxgiFactory_->CreateSwapChainForHwnd(commandQueue_.Get(), winApp_->GetHwnd(), &swapChainDesc_, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain_.GetAddressOf()));
	assert(SUCCEEDED(hr));
}

void DirectXEngine::DepthStencilInitialize()
{
	//DepthStencilTextureをウィンドウのサイズで作成
	depthStencilResource_ = CreateDepthStencilTextureResource(device_, winApp_->kClientWidth, winApp_->kClientHeight);
	//DSV用のヒープでディスクリプタの数は1。DSVはShader内で触るものではないので、ShaderVisibleはflase
	dsvDescriptorHeap_ = CreateDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);
	//DSVの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//Format。基本的にはResourceに合わせる
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;//2dTexture
	//DSVHeapの先頭にDSVを作る
	device_->CreateDepthStencilView(depthStencilResource_.Get(), &dsvDesc, dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart());
}

void DirectXEngine::DescriptorHeapInitialize()
{
	//ディスクリプタヒープの生成(RTV,SRV)
	rtvDescriptorHeap_ = CreateDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);
	srvDescriptorHeap_ = CreateDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxSRVCount, true);
	//DescriptorSizeを取得しておく
	descriptorSizeSRV_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	descriptorSizeRTV_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	descriptorSizeDSV_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

void DirectXEngine::RTVInitialize()
{
	HRESULT hr{};

	//SwapChainからResourceを引っ張ってくる
	hr = swapChain_->GetBuffer(0, IID_PPV_ARGS(&swapChainResources_[0]));
	//うまく取得できなければ起動できない
	assert(SUCCEEDED(hr));
	hr = swapChain_->GetBuffer(1, IID_PPV_ARGS(&swapChainResources_[1]));
	assert(SUCCEEDED(hr));

	//RTVの設定
	rtvDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; //出力結果をSRGBに変換して書き込む
	rtvDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D; //2Dテクスチャとして書き込む
	//ディスクリプタの先頭を取得する
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle = GetCPUDescriptorHandle(rtvDescriptorHeap_, descriptorSizeRTV_, 0);
	//まず1つ目を作る。1つ目は最初の所に作る。作る場所をこちらで指摘してあげる必要がある
	rtvHandles_[0] = rtvStartHandle;
	rtvHandles_[1].ptr = rtvHandles_[0].ptr + device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	for (uint32_t i = 0; i < 2; i++) {
		device_->CreateRenderTargetView(swapChainResources_[i].Get(), &rtvDesc_, rtvHandles_[i]);
	}
}

void DirectXEngine::FenceInitialize()
{
	HRESULT hr{};

	//初期値0でFenceを作る
	hr = device_->CreateFence(fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(hr));
	//FenceのSignalを持つためのイベントを作成する
	fenceEvent_ = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent_ != nullptr);
}

void DirectXEngine::ViewportInitialize()
{
	//クライアント領域のサイズと一緒にして画面全体に表示
	viewport_.Width = winApp_->kClientWidth;
	viewport_.Height = winApp_->kClientHeight;
	viewport_.TopLeftX = 0;
	viewport_.TopLeftY = 0;
	viewport_.MinDepth = 0.0f;
	viewport_.MaxDepth = 1.0f;
}

void DirectXEngine::RectInitialize()
{
	//基本的にビューポート同じ矩形が構成されるようにする
	scissorRect_.left = 0;
	scissorRect_.right = winApp_->kClientWidth;
	scissorRect_.top = 0;
	scissorRect_.bottom = winApp_->kClientHeight;
}

void DirectXEngine::DxcCompilerInitialize()
{
	HRESULT hr{};

	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(hr));
}

void DirectXEngine::ImGuiInitialize()
{
	//InGuiの初期化。
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winApp_->GetHwnd());
	ImGui_ImplDX12_Init(device_.Get(),
		swapChainDesc_.BufferCount,
		rtvDesc_.Format,
		srvDescriptorHeap_.Get(),
		srvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart(),
		srvDescriptorHeap_->GetGPUDescriptorHandleForHeapStart()
	);
}

void DirectXEngine::VertexResourceInitialize()
{
	vertexResource_ = new VertexResource();
	vertexResource_->Initialize(device_);
}

void DirectXEngine::TextureResourceInitialize()
{
	textureResource_ = new TextureResource();
	textureResource_->Initialize(device_, srvDescriptorHeap_, descriptorSizeSRV_);
	textureSrvHandleGPU_[0] = textureResource_->GetTextureSrvHandleGPU(vertexResource_->GetModelData().material.textureFilePath, 1);
	textureSrvHandleGPU_[1] = textureResource_->GetTextureSrvHandleGPU(vertexResource_->GetModelDataObject().material.textureFilePath, 2);
	textureSrvHandleGPU_[2] = textureResource_->GetTextureSrvHandleGPU("resources/uvChecker.png", 3);
	textureSrvHandleGPU_[3] = textureResource_->GetTextureSrvHandleGPU("resources/checkerBoard.png", 4);
	textureSrvHandleGPU_[4] = textureResource_->GetTextureSrvHandleGPU("resources/circle.png", 5);
	textureSrvHandleGPU_[5] = textureResource_->GetTextureSrvHandleGPU("resources/monsterBall.png", 6);
}

void DirectXEngine::InstancingSrvInitialize()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = kNumMaxInstance;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);
	instancingSrvHandleCPU_ = GetCPUDescriptorHandle(srvDescriptorHeap_, descriptorSizeSRV_, 10);
	instancingSrvHandleGPU_ = GetGPUDescriptorHandle(srvDescriptorHeap_, descriptorSizeSRV_, 10);
	device_->CreateShaderResourceView(vertexResource_->GetInstancingResource().Get(), &instancingSrvDesc, instancingSrvHandleCPU_);
}

void DirectXEngine::IncludeHandlerInitialize()
{
	HRESULT hr{};

	//現時点でincludeはしないが、includeに対応するための設定を行っておく
	hr = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(hr));
}

void DirectXEngine::PipelineStateInitialize()
{
	// 新しいパイプライン
	pipelineState_ = new PipelineState();
	pipelineState_->Initialize(device_, dxcUtils_, dxcCompiler_, includeHandler_);
	// Object3d
	object3dRootSignature_ = pipelineState_->CreateObject3dRootSignature();
	object3dPipelineState_ = pipelineState_->CreateObject3dPipelineState(false);
	// Particle
	ParticleRootSignature_ = pipelineState_->CreateParticleRootSignature();
	ParticlePipelineState_ = pipelineState_->CreateParticlePipelineState();
}

void DirectXEngine::PreDraw()
{
	//ImGuiの開始処理
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	// これから書き込むバックバッファのインデックスを取得
	UINT backBufferIndex = swapChain_->GetCurrentBackBufferIndex();
	//TransitionBarrierの設定
	//今回のバリアはTransition
	barrier_.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	barrier_.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier_.Transition.pResource = swapChainResources_[backBufferIndex].Get();
	//遷移前(現在)のResourceState
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	//遷移後のResourceState
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//TransitionBarrierを張る
	commandList_->ResourceBarrier(1, &barrier_);
	//描画先のRTVとDSVを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	commandList_->OMSetRenderTargets(1, &rtvHandles_[backBufferIndex], false, &dsvHandle);
	//指定した深度で画面全体をクリアする
	commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	//指定した色で画面全体をクリアする
	float clearColor[] = { 0.1f,0.25f,0.5f,1.0f }; //青っぽい色。RGBAの順
	commandList_->ClearRenderTargetView(rtvHandles_[backBufferIndex], clearColor, 0, nullptr);
	//描画用のDescriptorHeapの設定
	ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] = { srvDescriptorHeap_ };
	commandList_->SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());
	//コマンドを積む
	commandList_->RSSetViewports(1, &viewport_);
	commandList_->RSSetScissorRects(1, &scissorRect_);
	//開発用UIの処理.
	ImGui::ShowDemoWindow();
}

void DirectXEngine::Draw()
{
	vertexResource_->ImGui();
	vertexResource_->Update();

	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えて置けばいい
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	///==============================================================================================
	// RootSignatureを設定。PSOに設定しているけど別途設定が必要(Particle.hlsl)
	commandList_->SetGraphicsRootSignature(ParticleRootSignature_.Get());
	commandList_->SetPipelineState(ParticlePipelineState_.Get());
	///==============================================================================================
	// Particle
	commandList_->IASetVertexBuffers(0, 1, &vertexResource_->GetVertexBufferView());
	commandList_->SetGraphicsRootConstantBufferView(0, vertexResource_->GetMaterialResource()->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(1, vertexResource_->GetInstancingResource()->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootDescriptorTable(2, vertexResource_->GetuseCircle() ? textureSrvHandleGPU_[4] : textureSrvHandleGPU_[0]);
	commandList_->SetGraphicsRootConstantBufferView(3, vertexResource_->GetDirectionalLightResource()->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootDescriptorTable(4, instancingSrvHandleGPU_);
	// 描画
	commandList_->DrawInstanced(UINT(vertexResource_->GetModelData().vertices.size()), vertexResource_->GetNumInstance(), 0, 0);
	///==============================================================================================
	// 新しいパイプラインステートを設定(Object3d.hlsl)
	commandList_->SetGraphicsRootSignature(object3dRootSignature_.Get());
	commandList_->SetPipelineState(object3dPipelineState_.Get());
	///==============================================================================================
	// Object
	commandList_->IASetVertexBuffers(0, 1, &vertexResource_->GetVertexBufferViewObject());
	commandList_->SetGraphicsRootConstantBufferView(0, vertexResource_->GetMaterialResourceObject()->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(1, vertexResource_->GetwvpResourceObject()->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_[2]);
	commandList_->SetGraphicsRootConstantBufferView(3, vertexResource_->GetDirectionalLightResource()->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(4, vertexResource_->GetPointLightResource()->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(5, vertexResource_->GetSpotLightResource()->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(6, vertexResource_->GetCameraResource()->GetGPUVirtualAddress());
	// 描画
	commandList_->DrawInstanced(UINT(vertexResource_->GetModelDataObject().vertices.size()), 1, 0, 0);
	///==============================================================================================
	// Sphere
	commandList_->IASetVertexBuffers(0, 1, &vertexResource_->GetVertexBufferViewSphere());
	commandList_->SetGraphicsRootConstantBufferView(0, vertexResource_->GetMaterialResourceSphere()->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(1, vertexResource_->GetwvpResourceSphere()->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootDescriptorTable(2, vertexResource_->GetuseMonsterBall() ? textureSrvHandleGPU_[5] : textureSrvHandleGPU_[2]);
	commandList_->SetGraphicsRootConstantBufferView(3, vertexResource_->GetDirectionalLightResource()->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(4, vertexResource_->GetPointLightResource()->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(5, vertexResource_->GetSpotLightResource()->GetGPUVirtualAddress());
	commandList_->SetGraphicsRootConstantBufferView(6, vertexResource_->GetCameraResource()->GetGPUVirtualAddress());
	// 描画
	commandList_->DrawInstanced(1536, 1, 0, 0);
	///==============================================================================================
	// Sprite
	//commandList_->IASetVertexBuffers(0, 1, &vertexResource_->GetVertexBufferViewSprite());
	//commandList_->IASetIndexBuffer(&vertexResource_->GetIndexBufferViewSprite());
	//commandList_->SetGraphicsRootConstantBufferView(0, vertexResource_->GetMaterialResourceSprite()->GetGPUVirtualAddress());
	//commandList_->SetGraphicsRootConstantBufferView(1, vertexResource_->GetTransformationMatrixResourceSprite()->GetGPUVirtualAddress());
	//commandList_->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_[2]);
	//// 描画
	//commandList_->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void DirectXEngine::PostDraw()
{
	//ImGuiの内部コマンドを生成
	ImGui::Render();

	HRESULT hr{};
	UINT backBufferIndex = swapChain_->GetCurrentBackBufferIndex();
	//実際のnommandListのImGuiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList_.Get());
	//画面に描く処理は全て終わり、画面に映すので、状態を遷移
	//今回はRenderTargetからPresentにする
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	//TransitionBarrierを張る
	commandList_->ResourceBarrier(1, &barrier_);
	//コマンドリストの内容を確定させる。すべてのコマンドを積んでからCloseすること
	hr = commandList_->Close();
	assert(SUCCEEDED(hr));
	//GPUにコマンドリストの実行を行わせる
	ComPtr<ID3D12CommandList> commandLists[] = { commandList_ };
	commandQueue_->ExecuteCommandLists(1, commandLists->GetAddressOf());
	//GPUとOSに画面の交換を行うよう通知をする
	swapChain_->Present(1, 0);
	//FenceValue値を更新
	fenceValue_++;
	//GPUがここまでたどり着いた時に、Fenceの値を指定した値に代入するようにSignalを送る
	commandQueue_->Signal(fence_.Get(), fenceValue_);
	//Fenceの値が指定したSignal値にたどりついているか確認する
	//GetCompleteValueの初期値はFence作成時に渡した初期化
	if (fence_->GetCompletedValue() < fenceValue_) {
		//指定したSignelにたどり着いてないので、たどり着くまで待つようにイベントを設定
		fence_->SetEventOnCompletion(fenceValue_, fenceEvent_);
		//イベントを待つ
		WaitForSingleObject(fenceEvent_, INFINITE);
	}
	//次のフレーム用のコマンドリストを準備
	hr = commandAllocator_->Reset();
	assert(SUCCEEDED(hr));
	hr = commandList_->Reset(commandAllocator_.Get(), nullptr);
	assert(SUCCEEDED(hr));
}