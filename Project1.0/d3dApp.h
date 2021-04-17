//***************************************************************************************
//LU SHUAI QIAO 2020
//Frame reference by Introduction to 3D Game Programming with Directx 12
//***************************************************************************************
#pragma once

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "d3dUtil.h"
#include "GameTimer.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

class D3DApp
{
protected:

    D3DApp(HINSTANCE hInstance);
    D3DApp(const D3DApp& rhs) = delete;
    D3DApp& operator=(const D3DApp& rhs) = delete;
    virtual ~D3DApp();

public:

    static D3DApp* GetApp();
    
	HINSTANCE AppInst()const;
	HWND      MainWnd()const;
	float     AspectRatio()const;
	//设置MSAA
    bool Get4xMsaaState()const;
    void Set4xMsaaState(bool value);
	//主循环
	int Run();
	//初始化
    virtual bool Initialize();
	//消息循环
    virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	//创建交换链的RTV DSV
    virtual void CreateRtvAndDsvDescriptorHeaps();
	//更新大小
	virtual void OnResize(); 
	//每帧更新
	virtual void Update(const GameTimer& gt)=0;
	//渲染
    virtual void Draw(const GameTimer& gt)=0;

	//鼠标控制
	virtual void OnMouseDown(WPARAM btnState, int x, int y){ }
	virtual void OnMouseUp(WPARAM btnState, int x, int y)  { }
	virtual void OnMouseMove(WPARAM btnState, int x, int y){ }

protected:
	//初始化窗口
	bool InitMainWindow();
	//初始化D3D
	bool InitDirect3D();
	//创建命令列表
	void CreateCommandObjects();
	//创建交换链
    void CreateSwapChain();
	//围栏
	void FlushCommandQueue();
	//后台缓冲区
	ID3D12Resource* CurrentBackBuffer()const;
	//后台缓冲区描述符
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
	//Depth Stencil描述符
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;
	//FPS计算
	void CalculateFrameStats();
	//获取适配器（显卡）
    void LogAdapters();
    void LogAdapterOutputs(IDXGIAdapter* adapter);
    void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

protected:

    static D3DApp* mApp;
	//僂傿儞僪僂
    HINSTANCE mhAppInst = nullptr; //窗口实例
    HWND      mhMainWnd = nullptr; //窗口句柄
	bool      mAppPaused = false;  //暂停判断
	bool      mMinimized = false;  // 窗口最小化
	bool      mMaximized = false;  // 最大化
	bool      mResizing = false;   // 唤醒
    bool      mFullscreenState = false;// 全屏

	//MSAA
    bool      m4xMsaaState = false;
    UINT      m4xMsaaQuality = 0; //质量等级

	//计时器
	GameTimer mTimer;
	
    Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
    Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;//交换链
    Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice; //D3D设备
	//围栏
    Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
	//当前围栏索引
    UINT64 mCurrentFence = 0;
	
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;//命令列队
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;//命令列表

	static const int SwapChainBufferCount = 2;//交换链数目
	int mCurrBackBuffer = 0;//当前后台缓冲区索引

    Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];	//交换链BUffer
    Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;//深度模板缓冲区BUFFer

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;
	//视口窗口矩形
    D3D12_VIEWPORT mScreenViewport; 
    D3D12_RECT mScissorRect;
	//描述符数量
	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvUavDescriptorSize = 0;

	//窗口名
	std::wstring mMainWndCaption = L"d3d App";
	//硬驱动模式
	D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
    DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;//后台缓冲区格式
    DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;//深度模板缓冲区格式
	//窗口长宽
	int mClientWidth = 1600;
	int mClientHeight = 900;
};

