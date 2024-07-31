#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "Logger.h"
#include "StringUtility.h"
#include "WinApp.h"
#include <dxcapi.h>

using Microsoft::WRL::ComPtr;

class DirectXEngine
{
public:
	// 初期化
	void Initialize(WinApp* winApp);
	// デバイスの初期化
	void DeviceInitialize();
	// コマンド関連の初期化
	void CommandInitialize();
	// スワップチェーンの初期化
	void SwapChainInitialize();
	// 深度バッファの初期化
	void DepthStencilInitialize();
	// 各種デスクリプタヒープの初期化
	void DescriptorHeapInitialize();
	// レンダーターゲットビューの初期化
	void RTVInitialize();
	// フェンスの初期化
	void FenceInitialize();
	// ビューポートの初期化
	void ViewportInitialize();
	// シザー矩形の初期化
	void RectInitialize();
	// DXCコンパイラの初期化
	void DxcCompilerInitialize();
	// ImGuiの初期化
	void ImGuiInitialize();
	
private:
	// Logger
	Logger* logger_ = nullptr;
	// StringUtility
	StringUtility* stringUtility_ = nullptr;
	// WindowsAPI
	WinApp* winApp_ = nullptr;

	///==============================================================

	// D3D12Deviceの作成
	ComPtr<ID3D12Device> device_ = nullptr;
	// DXGIファクトリーの生成
	ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;
	//コマンドキューを生成する
	ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	//コマンドリストを生成する
	ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	//コマンドアロケータを生成する
	ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
	//スワップチェーンを生成する
	ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
	ComPtr<ID3D12Resource> swapChainResources_[2] = {nullptr};
	// 深度バッファの生成
	ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap_ = nullptr;
	// 各種でスクリプタヒープの生成
	ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap_ = nullptr;
	ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap_ = nullptr;
	uint32_t descriptorSizeSRV_ = NULL;
	uint32_t descriptorSizeRTV_ = NULL;
	uint32_t descriptorSizeDSV_ = NULL;
	//RTVを2つ作るのでディスクリプタを2つ用意
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];
	// フェンスを生成
	ComPtr<ID3D12Fence> fence_ = nullptr;
	uint64_t fenceValue_ = 0;
	HANDLE fenceEvent_{};
	// ビューポートの生成
	D3D12_VIEWPORT viewport_{};
	// シザー矩形の生成
	D3D12_RECT scissorRect_{};
	// DxcCompilerの生成
	ComPtr<IDxcUtils> dxcUtils_ = nullptr;
	ComPtr<IDxcCompiler3> dxcCompiler_ = nullptr;


};

