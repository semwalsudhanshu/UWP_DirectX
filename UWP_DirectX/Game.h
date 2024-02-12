#pragma once
#include<wrl.h>
#include<windows.graphics.directx.h>

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace Platform;
using namespace DirectX;

class CGame
{
public:
	ComPtr<ID3D11Device1> device; //device interface virtual representation of the video adapter
	
	ComPtr<ID3D11DeviceContext1> deviceContext; // the control panel for the virtual GPU
	ComPtr<IDXGISwapChain1> swapChain;
	ComPtr<ID3D11RenderTargetView>renderTarget; //Render Target 

	ComPtr<ID3D11Buffer> vertexBuffer;

	ComPtr<ID3D11VertexShader>vertexShader;
	ComPtr<ID3D11PixelShader>pixelShader;

	ComPtr<ID3D11InputLayout>inputLayout;


	struct VERTEX
	{
		float X, Y, Z;
	};



	void Initialize();	//starting up code
	void InitGraphics();
	void InitPipeline();
	void Update(); // code that manipulates the game such as timer and input
	void Render(); // draw graphics
	/*
	 Swap Chain Creation Process
	1. Obtain a pointer to DXGI Factory
	2. We customize the swapchain by filling up the swapchain description structure.
	3. Use DXGI Factory to create a swap chain
	*/


	/*
	* To render a blank frame window.(Paint window with some color)
	* 1.We need to tell the GPU where in the memory to create the final image (Back buffer)
	* 2. Tell GPU Where in back buffer should it draw the image.
	* 3. FIrst thing is establish a Render Target 
	* 
	*/

	/* Rendering a Triangle
	* 1. Create three vertices
	* 2. Storing the vertices in vRAM.
	* 3. Inform the GPU how to read the vertices.
	* 4. Inform the GPU how to translate the vertices into a flat image.
	* 5. Inform the GPU where on the Back Buffer should image appear
	* 6. Render the triangle
	*/

};
