#include "pch.h"
#include "Game.h"
#include<fstream>

Array<byte>^ LoadShaderFile(std::string File)
{
	Array<byte>^ FileData = nullptr;
	std::ifstream VertexFile(File, std::ios::in | std::ios::binary | std::ios::ate);

	//check whether file open succeeded

	if (VertexFile.is_open())
	{
		//find the file length

		int length = (int)VertexFile.tellg();

		FileData = ref new Array<byte>(length);
		VertexFile.seekg(0, std::ios::beg);
		VertexFile.read(reinterpret_cast<char*>(FileData->Data), length);
		VertexFile.close();
	}
	return FileData;
}


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

	//Set up the view port
	D3D11_VIEWPORT viewPort = { 0 };
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = Window->Bounds.Width;
	viewPort.Height = Window->Bounds.Height;
	deviceContext->RSSetViewports(1, &viewPort);
	InitGraphics();
	InitPipeline();

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

	/*  To Finally draw the triangle
	* 1. Setting up the vertex buffer we intend to use;
	* 2. Setting up the type of primitive we intend to use.
	* 3. Draw the Primitive
	*/

	//setting up the vertex buffer 
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	//setting up the primitive topology
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//draw the primitive
	deviceContext->Draw(3, 0);

	//Step 4 of Swap Chain
	swapChain->Present(1, 0);
}

void CGame::InitGraphics()
{
	/* Triangle Details
	*/
	VERTEX vertices[] = { {0.0f, 0.5f, 0.0f},
						{0.5f, -0.5f, 0.0f},
						{-0.45f, -0.5f, 0.0f} };

	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.ByteWidth = sizeof(VERTEX) * ARRAYSIZE(vertices); //Buffer Length, how big the buffer should be
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //This tells Direct3D what kind of buffer we want .
	D3D11_SUBRESOURCE_DATA subResourceData = { vertices, 0, 0 };
	device->CreateBuffer(&bufferDesc, &subResourceData, &vertexBuffer);

	/* Now we have to create a shader for it.
	*  1.Create a vertex shader file and a pixel shaer file
	*  2.Load the shaders fom a .cso file
	*  3.Encapsulate the shaders into objects
	*  4.Set the shaders as active shaders.
	*/

}

void CGame::InitPipeline()
{
	/* Initializes the GPU settings and prepares it for rendering
	*/

	//Load Shader files

	Array<byte>^VSFile = LoadShaderFile("VertexShader.cso");
	Array<byte>^PSFile = LoadShaderFile("PixelShader.cso");

	//creating the shader objects

	device->CreateVertexShader(VSFile->Data, VSFile->Length, nullptr, &vertexShader);
	device->CreatePixelShader(PSFile->Data, PSFile->Length, nullptr, &pixelShader);

	//Set the shaders as active shaders

	deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);

	// Initialize Input Layout

	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},

	};

	//creating input layout

	device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), VSFile->Data, VSFile->Length, &inputLayout);
	
	//Set the input layout

	deviceContext->IASetInputLayout(inputLayout.Get());

	//deviceContext->IASetVertexBuffers();

}

