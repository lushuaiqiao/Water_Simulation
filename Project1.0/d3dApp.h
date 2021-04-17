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
	//����MSAA
    bool Get4xMsaaState()const;
    void Set4xMsaaState(bool value);
	//��ѭ��
	int Run();
	//��ʼ��
    virtual bool Initialize();
	//��Ϣѭ��
    virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	//������������RTV DSV
    virtual void CreateRtvAndDsvDescriptorHeaps();
	//���´�С
	virtual void OnResize(); 
	//ÿ֡����
	virtual void Update(const GameTimer& gt)=0;
	//��Ⱦ
    virtual void Draw(const GameTimer& gt)=0;

	//������
	virtual void OnMouseDown(WPARAM btnState, int x, int y){ }
	virtual void OnMouseUp(WPARAM btnState, int x, int y)  { }
	virtual void OnMouseMove(WPARAM btnState, int x, int y){ }

protected:
	//��ʼ������
	bool InitMainWindow();
	//��ʼ��D3D
	bool InitDirect3D();
	//���������б�
	void CreateCommandObjects();
	//����������
    void CreateSwapChain();
	//Χ��
	void FlushCommandQueue();
	//��̨������
	ID3D12Resource* CurrentBackBuffer()const;
	//��̨������������
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
	//Depth Stencil������
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;
	//FPS����
	void CalculateFrameStats();
	//��ȡ���������Կ���
    void LogAdapters();
    void LogAdapterOutputs(IDXGIAdapter* adapter);
    void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

protected:

    static D3DApp* mApp;
	//�E�B���h�E
    HINSTANCE mhAppInst = nullptr; //����ʵ��
    HWND      mhMainWnd = nullptr; //���ھ��
	bool      mAppPaused = false;  //��ͣ�ж�
	bool      mMinimized = false;  // ������С��
	bool      mMaximized = false;  // ���
	bool      mResizing = false;   // ����
    bool      mFullscreenState = false;// ȫ��

	//MSAA
    bool      m4xMsaaState = false;
    UINT      m4xMsaaQuality = 0; //�����ȼ�

	//��ʱ��
	GameTimer mTimer;
	
    Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
    Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;//������
    Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice; //D3D�豸
	//Χ��
    Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
	//��ǰΧ������
    UINT64 mCurrentFence = 0;
	
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;//�����ж�
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;//�����б�

	static const int SwapChainBufferCount = 2;//��������Ŀ
	int mCurrBackBuffer = 0;//��ǰ��̨����������

    Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];	//������BUffer
    Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;//���ģ�建����BUFFer

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;
	//�ӿڴ��ھ���
    D3D12_VIEWPORT mScreenViewport; 
    D3D12_RECT mScissorRect;
	//����������
	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvUavDescriptorSize = 0;

	//������
	std::wstring mMainWndCaption = L"d3d App";
	//Ӳ����ģʽ
	D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
    DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;//��̨��������ʽ
    DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;//���ģ�建������ʽ
	//���ڳ���
	int mClientWidth = 1600;
	int mClientHeight = 900;
};

