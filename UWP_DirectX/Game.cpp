#include "pch.h"
#include "Game.h"


using namespace Windows::ApplicationModel::Core;
using namespace Windows::UI::Core;

//Initializes and prepares direct3d
void CGame::Initialize()
{
	/*  Direct3D Initialization */
	ComPtr<ID3D11Device>tempDevice;
	ComPtr<ID3D11DeviceContext>tempDeviceContext;
	

	D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&tempDevice,
		nullptr,
		&tempDeviceContext

	);
	tempDevice.As(&device);
	tempDeviceContext.As(&deviceContext);
	//
	/*
	* To create Swap Chains
	* Step 1 Convert the IDXGIDevice1 interface to IDXGIDevice1
	* Step 2 Utilize the IDXGIDevice1 interface to get the access of the adapter
	* Step 3 Use the IDXGIAdapter interface to get access to the parent.
	* Step 4 Start presenting the Swap Chains
	*/
	// Step 1 of Swap Chain
	ComPtr<IDXGIDevice1> dxgiDevice;
	device.As(&dxgiDevice);

	//Step 2 of Swap Chain 
	ComPtr<IDXGIAdapter> dxgiAdapter;
	dxgiDevice->GetAdapter(&dxgiAdapter);
	 
	// Step 3 of Swap Chain

	ComPtr<IDXGIFactory2> dxgiFactory;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), &dxgiFactory);

	/*XXXXXXXXXXXXXXXXXXXXXX   SWAP CHAINS  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/
	//Set up the swap chain description
	DXGI_SWAP_CHAIN_DESC1 swapChainDescription = { 0 };
	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // how the swap chain should be utilized
	swapChainDescription.BufferCount = 2;                               //Front Buffer and Back Buffer
	swapChainDescription.Format = DXGI_FORMAT_B8G8R8A8_UNORM;           // UNORM - unsigned normalized integer Common Swap chain format
	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; //recommended flip mode
	swapChainDescription.SampleDesc.Count = 1;                          // Disable Anti-Aliasing
	

	CoreWindow^ Window = CoreWindow::GetForCurrentThread();

	dxgiFactory->CreateSwapChainForCoreWindow(
	    device.Get(),
		reinterpret_cast<IUnknown*>(Window),
		&swapChainDescription,
		nullptr,
		&swapChain);
	/* XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX SWAP CHAINS END XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/

	/* XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX Render Targert XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/
	/*
	* To create a Render Target 
	1. Get a Direct pointer to the Back Buffer
	2. Create a Render Target
	3. It needs to set as current render target
	*/
	// Step 1 for Render Target

	ComPtr<ID3D11Texture2D> backBuffer;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);

	//Step 2 for Render Target

	device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTarget);

	/* XXXXXXXXXXXXXXXXXXXXXXXXXXXX Render Target End XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/
}

//Perform updates to the game states
void CGame::Update()
{

}

//renders a single frame of 3d graphcis
void CGame::Render()
{
	/* Step 3 for Render Target*/

	deviceContext->OMSetRenderTargets(1, renderTarget.GetAddressOf(), nullptr);
	// clear the backbuffer to some color
	float color[4] = { 0.0f, 0.1f, 0.3f, 1.0f };
	deviceContext->ClearRenderTargetView(renderTarget.Get(), color);


	//Step 4 of Swap Chain
	swapChain->Present(1, 0);
}

