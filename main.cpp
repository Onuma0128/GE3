#include <dxgidebug.h>
#pragma comment(lib,"dxguid.lib")
#include "wrl.h"
#include "Input.h"
#include "DirectXEngine.h"
#include "D3DResourceLeakChecker.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	D3DResourceLeakChecker leakChecker;

	WinApp* winApp_ = nullptr;
	winApp_ = new WinApp();
	winApp_->Initialize();

	DirectXEngine* directXEngine_ = nullptr;
	directXEngine_ = new DirectXEngine();
	directXEngine_->Initialize(winApp_);

	///===================================================================================

	////VertexResourceを作る
	//VertexResource* vertexResource_ = new VertexResource();
	//vertexResource_->Initialize(device);

	////TextureResourceを作る
	//TextureResource* textureResource_ = new TextureResource();
	//textureResource_->Initialize(device, srvDescriptorHeap, descriptorSizeSRV);
	//D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU[5]{
	//	textureResource_->GetTextureSrvHandleGPU(vertexResource_->GetModelData().material.textureFilePath,1),
	//	textureResource_->GetTextureSrvHandleGPU("resources/uvChecker.png",2),
	//	textureResource_->GetTextureSrvHandleGPU("resources/checkerBoard.png",3),
	//	textureResource_->GetTextureSrvHandleGPU("resources/circle.png",4),
	//	textureResource_->GetTextureSrvHandleGPU("resources/monsterBall.png",5)
	//};

	//D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	//instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	//instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	//instancingSrvDesc.Buffer.FirstElement = 0;
	//instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	//instancingSrvDesc.Buffer.NumElements = kNumMaxInstance;
	//instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);
	//D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU = GetCPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 6);
	//D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU = GetGPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 6);
	//device->CreateShaderResourceView(vertexResource_->GetInstancingResource().Get(), &instancingSrvDesc, instancingSrvHandleCPU);

	////現時点でincludeはしないが、includeに対応するための設定を行っておく
	//ComPtr<IDxcIncludeHandler> includeHandler = nullptr;
	//hr = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	//assert(SUCCEEDED(hr));

	//// 新しいパイプライン
	//PipelineState* pipelineState_ = new PipelineState();
	//pipelineState_->Initialize(device, dxcUtils, dxcCompiler, includeHandler);
	//// Object3d
	//ComPtr<ID3D12RootSignature> object3dRootSignature = pipelineState_->CreateObject3dRootSignature();
	//ComPtr<ID3D12PipelineState> object3dPipelineState = pipelineState_->CreateObject3dPipelineState();
	//// Particle
	//ComPtr<ID3D12RootSignature> ParticleRootSignature = pipelineState_->CreateParticleRootSignature();
	//ComPtr<ID3D12PipelineState> ParticlePipelineState = pipelineState_->CreateParticlePipelineState();

	Input* input_ = nullptr;
	input_ = new Input();
	input_->Initialize(winApp_);

	//// オーディオ
	//ComPtr<IXAudio2> xAudio2;
	//IXAudio2MasteringVoice* masterVoice;
	//// XAudio2エンジンを生成
	//hr = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	//hr = xAudio2->CreateMasteringVoice(&masterVoice);
	//// 音声読み込み
	//SoundData soundData1 = SoundLoadWave("resources/fanfare.wav");
	//SoundPlayWave(xAudio2.Get(), soundData1);

	//ウィンドウの×ボタンが押されるまでループ
	while (true) {
		//Windowにメッセージが来てたら最優先で処理させる
		if (winApp_->ProcessMessage()) {
			break;
		}
		else {
			// 入力の更新
			input_->Update();
			////ImGuiの開始処理
			//ImGui_ImplDX12_NewFrame();
			//ImGui_ImplWin32_NewFrame();
			//ImGui::NewFrame();
			//これから書き込むバックバッファのインデックスを取得
			//UINT backBufferIndex = swapChain->GetCurrentBackBufferIndex();
			////TransitionBarrierの設定
			//D3D12_RESOURCE_BARRIER barrier{};
			////今回のバリアはTransition
			//barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			////Noneにしておく
			//barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			////バリアを張る対象のリソース。現在のバックバッファに対して行う
			//barrier.Transition.pResource = swapChainResources[backBufferIndex].Get();
			////遷移前(現在)のResourceState
			//barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
			////遷移後のResourceState
			//barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
			////TransitionBarrierを張る
			//commandList->ResourceBarrier(1, &barrier);
			////描画先のRTVとDSVを設定する
			//D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			//commandList->OMSetRenderTargets(1, &rtvHandles[backBufferIndex], false, &dsvHandle);
			////指定した深度で画面全体をクリアする
			//commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
			////指定した色で画面全体をクリアする
			//float clearColor[] = { 0.1f,0.25f,0.5f,1.0f }; //青っぽい色。RGBAの順
			//commandList->ClearRenderTargetView(rtvHandles[backBufferIndex], clearColor, 0, nullptr);
			////開発用UIの処理.
			//ImGui::ShowDemoWindow();
			//vertexResource_->ImGui(textureResource_->GetuseMonsterBall());

			////ImGuiの内部コマンドを生成
			//ImGui::Render();
			////描画用のDescriptorHeapの設定
			//ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] = { srvDescriptorHeap };
			//commandList->SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());
			////コマンドを積む
			//commandList->RSSetViewports(1, &viewport);
			//commandList->RSSetScissorRects(1, &scissorRect);
			/////==============================================================================================
			////RootSignatureを設定。PSOに設定しているけど別途設定が必要(Particle.hlsl)
			//commandList->SetGraphicsRootSignature(ParticleRootSignature.Get());
			//commandList->SetPipelineState(ParticlePipelineState.Get());
			//commandList->IASetVertexBuffers(0, 1, &vertexResource_->GetVertexBufferView());
			/////==============================================================================================
			////形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えて置けばいい
			//commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			////マテリアルCBufferの場所を設定
			//commandList->SetGraphicsRootConstantBufferView(0, vertexResource_->GetMaterialResource()->GetGPUVirtualAddress());
			////wvp用のCBufferの場所を設定
			//commandList->SetGraphicsRootConstantBufferView(1, vertexResource_->GetwvpResource()->GetGPUVirtualAddress());
			////SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である
			//commandList->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU[3]);
			////Lightの描画
			//commandList->SetGraphicsRootConstantBufferView(3, vertexResource_->GetDirectionalLightResource()->GetGPUVirtualAddress());
			////instancing用のDataを読むためにStructuredBufferのSRVを設定
			//commandList->SetGraphicsRootDescriptorTable(4, instancingSrvHandleGPU);
			////描画
			//commandList->DrawInstanced(UINT(vertexResource_->GetModelData().vertices.size()), vertexResource_->GetNumInstance(), 0, 0);
			/////==============================================================================================
			//// 新しいパイプラインステートを設定(Object3d.hlsl)
			//commandList->SetGraphicsRootSignature(object3dRootSignature.Get());
			//commandList->SetPipelineState(object3dPipelineState.Get());
			/////==============================================================================================
			////Sphere
			//commandList->IASetVertexBuffers(0, 1, &vertexResource_->GetVertexBufferViewSphere());
			//commandList->SetGraphicsRootConstantBufferView(0, vertexResource_->GetMaterialResourceSphere()->GetGPUVirtualAddress());
			//commandList->SetGraphicsRootConstantBufferView(1, vertexResource_->GetwvpResourceSphere()->GetGPUVirtualAddress());
			//commandList->SetGraphicsRootDescriptorTable(2, textureResource_->GetuseMonsterBall() ? textureSrvHandleGPU[4] : textureSrvHandleGPU[1]);
			//commandList->SetGraphicsRootConstantBufferView(3, vertexResource_->GetDirectionalLightResource()->GetGPUVirtualAddress());
			//commandList->SetGraphicsRootConstantBufferView(4, vertexResource_->GetCameraResource()->GetGPUVirtualAddress());
			////描画
			//commandList->DrawInstanced(1536, 1, 0, 0);

			////Spriteの描画
			//commandList->IASetVertexBuffers(0, 1, &vertexResource_->GetVertexBufferViewSprite());
			//commandList->IASetIndexBuffer(&vertexResource_->GetIndexBufferViewSprite());
			////TransformtionMatrixCBufferの場所を設定
			//commandList->SetGraphicsRootConstantBufferView(0, vertexResource_->GetMaterialResourceSprite()->GetGPUVirtualAddress());
			//commandList->SetGraphicsRootConstantBufferView(1, vertexResource_->GetTransformationMatrixResourceSprite()->GetGPUVirtualAddress());
			//commandList->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU[1]);
			////スプライトの描画
			//commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
			////実際のnommandListのImGuiの描画コマンドを積む
			//ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList.Get());
			////画面に描く処理は全て終わり、画面に映すので、状態を遷移
			////今回はRenderTargetからPresentにする
			//barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			//barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
			////TransitionBarrierを張る
			//commandList->ResourceBarrier(1, &barrier);
			////コマンドリストの内容を確定させる。すべてのコマンドを積んでからCloseすること
			//hr = commandList->Close();
			//assert(SUCCEEDED(hr));
			////GPUにコマンドリストの実行を行わせる
			//ComPtr<ID3D12CommandList> commandLists[] = { commandList };
			//commandQueue->ExecuteCommandLists(1, commandLists->GetAddressOf());
			////GPUとOSに画面の交換を行うよう通知をする
			//swapChain->Present(1, 0);
			////FenceValue値を更新
			//fenceValue++;
			////GPUがここまでたどり着いた時に、Fenceの値を指定した値に代入するようにSignalを送る
			//commandQueue->Signal(fence.Get(), fenceValue);
			////Fenceの値が指定したSignal値にたどりついているか確認する
			////GetCompleteValueの初期値はFence作成時に渡した初期化
			//if (fence->GetCompletedValue() < fenceValue) {
			//	//指定したSignelにたどり着いてないので、たどり着くまで待つようにイベントを設定
			//	fence->SetEventOnCompletion(fenceValue, fenceEvent);
			//	//イベントを待つ
			//	WaitForSingleObject(fenceEvent, INFINITE);
			//}
			////次のフレーム用のコマンドリストを準備
			//hr = commandAllocator->Reset();
			//assert(SUCCEEDED(hr));
			//hr = commandList->Reset(commandAllocator.Get(), nullptr);
			//assert(SUCCEEDED(hr));

			//vertexResource_->Update();

			if (input_->TriggerKey(DIK_S)) {
				OutputDebugStringA("Hit S\n");
			}
		}
	}
	/*xAudio2.Reset();
	SoundUnload(&soundData1);*/
	delete input_;
	delete directXEngine_;

	winApp_->Finalize();
	delete winApp_;
	//ImGuiの終了処理
	/*ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();*/

	return 0;
}