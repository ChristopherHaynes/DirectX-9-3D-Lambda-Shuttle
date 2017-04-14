//Direct 3D Assignment - Star Wars Lambda Shuttle - Chris Haynes
#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )

#define SCREEN_WIDTH 1920	//screen resolution width
#define SCREEN_HEIGHT 1080	//screen resolution height
#define POINTS 312			//number of verticies
#define FILLMODE 0			//toggle between wireframe and textured

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0) //handle keyboard inputs
#define KEY_UP  (vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

LPDIRECT3D9             g_pD3D = NULL;			// Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL;	// Our rendering device
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;			// Buffer to hold vertices
LPDIRECT3DTEXTURE9      g_pTexture = NULL;		// Our texture

struct CUSTOMVERTEX		 // A structure for a custom vertex type. Contains the position, normal, base colour and texture co-ordinates for a vertex.
  {
  D3DXVECTOR3 position; // The vector3 (x,y,z) position
  D3DVECTOR NORMAL;		// The normal of the surface
  D3DCOLOR color;       // The base colour in RGB
  FLOAT tu, tv;         // The texture coordinates
  };

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

float xrot=1;		// the x rotation of the camera
float yrot=1;		// the y rotation of the camera
float zoom = -50;	// the zoom of the camera (z-axis adjustment)

//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Create the Direct3D device for the given window size
//-----------------------------------------------------------------------------
HRESULT InitD3D(HWND hWnd)//Initializes Direct3D
  {
  //Create the D3D object.
  if(NULL==(g_pD3D=Direct3DCreate9(D3D_SDK_VERSION))) return E_FAIL;

  // Set up the structure used to create the D3DDevice. Since we are now
  // using more complex geometry, we will create a device with a zbuffer.
  D3DPRESENT_PARAMETERS d3dpp;
  ZeroMemory(&d3dpp,sizeof(d3dpp));
  d3dpp.Windowed = TRUE;
  d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
  d3dpp.BackBufferWidth = SCREEN_WIDTH;
  d3dpp.BackBufferHeight = SCREEN_HEIGHT;
  d3dpp.EnableAutoDepthStencil = TRUE;
  d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    // Create the D3DDevice
    if( FAILED( g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&g_pd3dDevice ) ) )
      {
      return E_FAIL;
      }

    // Turn off culling (culling determines whether the textures are drawn to one of both sides of each polygon)
    g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
    // Turn on D3D lighting
    g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
    // Turn on the zbuffer
    g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	
	g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(70, 70, 70));//ambient light
	g_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE); //handle normals when scaling the object
	g_pd3dDevice->SetRenderState(D3DRS_FILLMODE,0);//0=solid fill, 2=wireframe
	
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: InitGeometry()
// Desc: Create the textures and vertex buffers
//-----------------------------------------------------------------------------
HRESULT InitGeometry()
{
	typedef struct D3DVECTOR { //type defenition of a vector3 to hold the normal values in the custom vertex
		float x, y, z;
	} D3DVECTOR, *LPD3DVECTOR;

	CUSTOMVERTEX* v; 

	// Use D3DX to create a texture from a file based image
	if (FAILED (D3DXCreateTextureFromFile(g_pd3dDevice, L"shuttle.bmp", &g_pTexture))) {

		MessageBox( NULL, L"Could not find shuttle.bmp",L"Textures.exe", MB_OK );
		return E_FAIL;
	}

	// Create the vertex buffer.
	if (FAILED (g_pd3dDevice->CreateVertexBuffer(POINTS*sizeof(CUSTOMVERTEX), 0 ,D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL))) {
	
		return E_FAIL;
	}

	// Fill the vertex buffer. We are setting the tu and tv texture
	if (FAILED(g_pVB->Lock(0,0,(void**)&v,0))) return E_FAIL;

	// The following lines (112 - 551) contain the information required for each vertex. They are seperated into groups of the induvidual triangles
	// which they will form. Each point has to have a Vector3 position, RGB colour value, Vector2 Texture co-ordinates and a Vector3 Normal.

	//main body ##################################################################################################
	//front face of body
	//triangle 1 2 3 
	//                            x     y    z                                R    G    B         Texture x      Texture y			       Normal   x    y    z							
	v[0].position = D3DXVECTOR3(-3.0, -1.0, 0.0); v[0].color = D3DCOLOR_XRGB(255, 255, 255); v[0].tu = 0.0; v[0].tv = 0.0; v[0].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[1].position = D3DXVECTOR3(-4.0, 0.0, 0.0); v[1].color = D3DCOLOR_XRGB(255, 255, 255); v[1].tu = 1.0; v[1].tv = 0.0; v[1].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[2].position = D3DXVECTOR3(-3.0, 1.0, 0.0); v[2].color = D3DCOLOR_XRGB(255, 255, 255); v[2].tu = 1.0; v[2].tv = 1.0; v[2].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	//triangle 1 3 5
	v[3].position = D3DXVECTOR3(-3.0, -1.0, 0.0); v[3].color = D3DCOLOR_XRGB(255, 255, 255); v[3].tu = 1.0; v[3].tv = 0.0; v[3].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[4].position = D3DXVECTOR3(-3.0, 1.0, 0.0); v[4].color = D3DCOLOR_XRGB(255, 255, 255); v[4].tu = 0.0; v[4].tv = 0.0; v[4].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[5].position = D3DXVECTOR3(-1.0, -1.0, 0.0); v[5].color = D3DCOLOR_XRGB(255, 255, 255); v[5].tu = 1.0; v[5].tv = 1.0; v[5].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	//triangle 3 4 5
	v[6].position = D3DXVECTOR3(-3.0, 1.0, 0.0); v[6].color = D3DCOLOR_XRGB(255, 255, 255); v[6].tu = 0.0; v[6].tv = 0.0; v[6].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[7].position = D3DXVECTOR3(-1.0, 1.0, 0.0); v[7].color = D3DCOLOR_XRGB(255, 255, 255); v[7].tu = 1.0; v[7].tv = 0.0; v[7].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[8].position = D3DXVECTOR3(-1.0, -1.0, 0.0); v[8].color = D3DCOLOR_XRGB(255, 255, 255); v[8].tu = 1.0; v[8].tv = 1.0; v[8].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	//triangle 5 4 9
	v[9].position = D3DXVECTOR3(-1.0, -1.0, 0.0); v[9].color = D3DCOLOR_XRGB(255, 255, 255); v[9].tu = 1.0; v[9].tv = 0.0; v[9].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[10].position = D3DXVECTOR3(-1.0, 1.0, 0.0); v[10].color = D3DCOLOR_XRGB(255, 255, 255); v[10].tu = 0.0; v[10].tv = 0.0; v[10].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[11].position = D3DXVECTOR3(1.0, -1.0, 0.0); v[11].color = D3DCOLOR_XRGB(255, 255, 255); v[11].tu = 1.0; v[11].tv = 1.0; v[11].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	//triangle 4 6 9
	v[12].position = D3DXVECTOR3(-1.0, 1.0, 0.0); v[12].color = D3DCOLOR_XRGB(255, 255, 255); v[12].tu = 0.0; v[12].tv = 0.0; v[12].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[13].position = D3DXVECTOR3(0.0, 2.0, 0.0); v[13].color = D3DCOLOR_XRGB(255, 255, 255); v[13].tu = 1.0; v[13].tv = 0.0; v[13].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[14].position = D3DXVECTOR3(1.0, -1.0, 0.0); v[14].color = D3DCOLOR_XRGB(255, 255, 255); v[14].tu = 1.0; v[14].tv = 1.0; v[14].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	//triangle 6 7 9
	v[15].position = D3DXVECTOR3(0.0, 2.0, 0.0); v[15].color = D3DCOLOR_XRGB(255, 255, 255); v[15].tu = 0.0; v[15].tv = 0.0; v[15].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[16].position = D3DXVECTOR3(2.0, 2.0, 0.0); v[16].color = D3DCOLOR_XRGB(255, 255, 255); v[16].tu = 1.0; v[16].tv = 1.0; v[16].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[17].position = D3DXVECTOR3(1.0, -1.0, 0.0); v[17].color = D3DCOLOR_XRGB(255, 255, 255); v[17].tu = 0.0; v[17].tv = 1.0; v[17].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	//triangle 7 8 9
	v[18].position = D3DXVECTOR3(2.0, 2.0, 0.0); v[18].color = D3DCOLOR_XRGB(255, 255, 255); v[18].tu = 0.0; v[18].tv = 0.0; v[18].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[19].position = D3DXVECTOR3(3.0, 1.0, 0.0); v[19].color = D3DCOLOR_XRGB(255, 255, 255); v[19].tu = 1.0; v[19].tv = 0.0; v[19].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[20].position = D3DXVECTOR3(1.0, -1.0, 0.0); v[20].color = D3DCOLOR_XRGB(255, 255, 255); v[20].tu = 1.0; v[20].tv = 1.0; v[20].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	//triangle 9 8 10
	v[21].position = D3DXVECTOR3(1.0, -1.0, 0.0); v[21].color = D3DCOLOR_XRGB(255, 255, 255); v[21].tu = 0.0; v[21].tv = 0.0; v[21].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[22].position = D3DXVECTOR3(3.0, 1.0, 0.0); v[22].color = D3DCOLOR_XRGB(255, 255, 255); v[22].tu = 1.0; v[22].tv = 1.0; v[22].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[23].position = D3DXVECTOR3(3.0, -1.0, 0.0); v[23].color = D3DCOLOR_XRGB(255, 255, 255); v[23].tu = 0.0; v[23].tv = 1.0; v[23].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	//triangle 9 8 11
	v[24].position = D3DXVECTOR3(3.0, -1.0, 0.0); v[24].color = D3DCOLOR_XRGB(255, 255, 255); v[24].tu = 0.0; v[24].tv = 0.0; v[24].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[25].position = D3DXVECTOR3(5.0, 1.0, 0.0); v[25].color = D3DCOLOR_XRGB(255, 255, 255); v[25].tu = 1.0; v[25].tv = 1.0; v[25].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[26].position = D3DXVECTOR3(3.0, 1.0, 0.0); v[26].color = D3DCOLOR_XRGB(255, 255, 255); v[26].tu = 0.0; v[26].tv = 1.0; v[26].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	//triangle 9 11 10
	v[27].position = D3DXVECTOR3(3.0, -1.0, 0.0); v[27].color = D3DCOLOR_XRGB(255, 255, 255); v[27].tu = 0.0; v[27].tv = 0.0; v[27].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[28].position = D3DXVECTOR3(5.0, 1.0, 0.0); v[28].color = D3DCOLOR_XRGB(255, 255, 255); v[28].tu = 1.0; v[28].tv = 1.0; v[28].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[29].position = D3DXVECTOR3(5.0, -1.0, 0.0); v[29].color = D3DCOLOR_XRGB(255, 255, 255); v[29].tu = 0.0; v[29].tv = 1.0; v[29].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	//triangle 10 11 12
	v[30].position = D3DXVECTOR3(5.0, -1.0, 0.0); v[30].color = D3DCOLOR_XRGB(255, 255, 255); v[30].tu = 0.0; v[30].tv = 0.0; v[30].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[31].position = D3DXVECTOR3(5.0, 1.0, 0.0); v[31].color = D3DCOLOR_XRGB(255, 255, 255); v[31].tu = 1.0; v[31].tv = 1.0; v[31].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[32].position = D3DXVECTOR3(6.0, 0.0, 0.0); v[32].color = D3DCOLOR_XRGB(255, 255, 255); v[32].tu = 0.0; v[32].tv = 1.0; v[32].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);

	//back face of body
	//triangle 1 2 3 
	v[33].position = D3DXVECTOR3(-3.0, -1.0, -7.0); v[33].color = D3DCOLOR_XRGB(255, 255, 255); v[33].tu = 0.0; v[33].tv = 0.0; v[33].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[34].position = D3DXVECTOR3(-4.0, 0.0, -7.0); v[34].color = D3DCOLOR_XRGB(255, 255, 255); v[34].tu = 1.0; v[34].tv = 1.0; v[34].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[35].position = D3DXVECTOR3(-3.0, 1.0, -7.0); v[35].color = D3DCOLOR_XRGB(255, 255, 255); v[35].tu = 0.0; v[35].tv = 1.0; v[35].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	//triangle 1 3 5
	v[36].position = D3DXVECTOR3(-3.0, -1.0, -7.0); v[36].color = D3DCOLOR_XRGB(255, 255, 255); v[36].tu = 1.0; v[36].tv = 0.0; v[36].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[37].position = D3DXVECTOR3(-3.0, 1.0, -7.0); v[37].color = D3DCOLOR_XRGB(255, 255, 255); v[37].tu = 0.0; v[37].tv = 0.0; v[37].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[38].position = D3DXVECTOR3(-1.0, -1.0, -7.0); v[38].color = D3DCOLOR_XRGB(255, 255, 255); v[38].tu = 1.0; v[38].tv = 1.0; v[38].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	//triangle 3 4 5
	v[39].position = D3DXVECTOR3(-3.0, 1.0, -7.0); v[39].color = D3DCOLOR_XRGB(255, 255, 255); v[39].tu = 0.0; v[39].tv = 0.0; v[39].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[40].position = D3DXVECTOR3(-1.0, 1.0, -7.0); v[40].color = D3DCOLOR_XRGB(255, 255, 255); v[40].tu = 1.0; v[40].tv = 0.0; v[40].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[41].position = D3DXVECTOR3(-1.0, -1.0, -7.0); v[41].color = D3DCOLOR_XRGB(255, 255, 255); v[41].tu = 1.0; v[41].tv = 1.0; v[41].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	//triangle 5 4 9
	v[42].position = D3DXVECTOR3(-1.0, -1.0, -7.0); v[42].color = D3DCOLOR_XRGB(255, 255, 255); v[42].tu = 1.0; v[42].tv = 0.0; v[42].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[43].position = D3DXVECTOR3(-1.0, 1.0, -7.0); v[43].color = D3DCOLOR_XRGB(255, 255, 255); v[43].tu = 0.0; v[43].tv = 0.0; v[43].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[44].position = D3DXVECTOR3(1.0, -1.0, -7.0); v[44].color = D3DCOLOR_XRGB(255, 255, 255); v[44].tu = 1.0; v[44].tv = 1.0; v[44].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	//triangle 4 6 9
	v[45].position = D3DXVECTOR3(-1.0, 1.0, -7.0); v[45].color = D3DCOLOR_XRGB(255, 255, 255); v[45].tu = 0.0; v[45].tv = 0.0; v[45].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[46].position = D3DXVECTOR3(0.0, 2.0, -7.0); v[46].color = D3DCOLOR_XRGB(255, 255, 255); v[46].tu = 1.0; v[46].tv = 0.0; v[46].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[47].position = D3DXVECTOR3(1.0, -1.0, -7.0); v[47].color = D3DCOLOR_XRGB(255, 255, 255); v[47].tu = 1.0; v[47].tv = 1.0; v[47].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	//triangle 6 7 9
	v[48].position = D3DXVECTOR3(0.0, 2.0, -7.0); v[48].color = D3DCOLOR_XRGB(255, 255, 255); v[48].tu = 0.0; v[48].tv = 0.0; v[48].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[49].position = D3DXVECTOR3(2.0, 2.0, -7.0); v[49].color = D3DCOLOR_XRGB(255, 255, 255); v[49].tu = 1.0; v[49].tv = 1.0; v[49].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[50].position = D3DXVECTOR3(1.0, -1.0, -7.0); v[50].color = D3DCOLOR_XRGB(255, 255, 255); v[50].tu = 0.0; v[50].tv = 1.0; v[50].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	//triangle 7 8 9
	v[51].position = D3DXVECTOR3(2.0, 2.0, -7.0); v[51].color = D3DCOLOR_XRGB(255, 255, 255); v[51].tu = 0.0; v[51].tv = 0.0; v[51].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[52].position = D3DXVECTOR3(3.0, 1.0, -7.0); v[52].color = D3DCOLOR_XRGB(255, 255, 255); v[52].tu = 1.0; v[52].tv = 0.0; v[52].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[53].position = D3DXVECTOR3(1.0, -1.0, -7.0); v[53].color = D3DCOLOR_XRGB(255, 255, 255); v[53].tu = 1.0; v[53].tv = 1.0; v[53].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	//triangle 9 8 10
	v[54].position = D3DXVECTOR3(1.0, -1.0, -7.0); v[54].color = D3DCOLOR_XRGB(255, 255, 255); v[54].tu = 0.0; v[54].tv = 0.0; v[54].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[55].position = D3DXVECTOR3(3.0, 1.0, -7.0); v[55].color = D3DCOLOR_XRGB(255, 255, 255); v[55].tu = 1.0; v[55].tv = 1.0; v[55].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[56].position = D3DXVECTOR3(3.0, -1.0, -7.0); v[56].color = D3DCOLOR_XRGB(255, 255, 255); v[56].tu = 0.0; v[56].tv = 1.0; v[56].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	//triangle 9 8 11
	v[57].position = D3DXVECTOR3(3.0, -1.0, -7.0); v[57].color = D3DCOLOR_XRGB(255, 255, 255); v[57].tu = 0.0; v[57].tv = 0.0; v[57].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[58].position = D3DXVECTOR3(5.0, 1.0, -7.0); v[58].color = D3DCOLOR_XRGB(255, 255, 255); v[58].tu = 1.0; v[58].tv = 1.0; v[58].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[59].position = D3DXVECTOR3(3.0, 1.0, -7.0); v[59].color = D3DCOLOR_XRGB(255, 255, 255); v[59].tu = 0.0; v[59].tv = 1.0; v[59].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	//triangle 9 11 10
	v[60].position = D3DXVECTOR3(3.0, -1.0, -7.0); v[60].color = D3DCOLOR_XRGB(255, 255, 255); v[60].tu = 0.0; v[60].tv = 0.0; v[60].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[61].position = D3DXVECTOR3(5.0, 1.0, -7.0); v[61].color = D3DCOLOR_XRGB(255, 255, 255); v[61].tu = 1.0; v[61].tv = 1.0; v[61].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[62].position = D3DXVECTOR3(5.0, -1.0, -7.0); v[62].color = D3DCOLOR_XRGB(255, 255, 255); v[62].tu = 0.0; v[62].tv = 1.0; v[62].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	//triangle 10 11 12
	v[63].position = D3DXVECTOR3(5.0, -1.0, -7.0); v[63].color = D3DCOLOR_XRGB(255, 255, 255); v[63].tu = 0.0; v[63].tv = 0.0; v[63].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[64].position = D3DXVECTOR3(5.0, 1.0, -7.0); v[64].color = D3DCOLOR_XRGB(255, 255, 255); v[64].tu = 1.0; v[64].tv = 1.0; v[64].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[65].position = D3DXVECTOR3(6.0, 0.0, -7.0); v[65].color = D3DCOLOR_XRGB(255, 255, 255); v[65].tu = 0.0; v[65].tv = 1.0; v[65].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);

	//bottom of body and side pyramids
	//triangle 1 2 3 
	v[66].position = D3DXVECTOR3(-3.0, -1.0, 0.0); v[66].color = D3DCOLOR_XRGB(255, 255, 255); v[66].tu = 0.0; v[66].tv = 0.0; v[66].NORMAL = D3DXVECTOR3(0.0, 1.0, 0.0);
	v[67].position = D3DXVECTOR3(-3.0, -1.0, -7.0); v[67].color = D3DCOLOR_XRGB(255, 255, 255); v[67].tu = 1.0; v[67].tv = 0.0; v[67].NORMAL = D3DXVECTOR3(0.0, 1.0, 0.0);
	v[68].position = D3DXVECTOR3(5.0, -1.0, -7.0); v[68].color = D3DCOLOR_XRGB(255, 255, 255); v[68].tu = 1.0; v[68].tv = 1.0; v[68].NORMAL = D3DXVECTOR3(0.0, 1.0, 0.0);
	// triangle 
	v[69].position = D3DXVECTOR3(5.0, -1.0, -7.0); v[69].color = D3DCOLOR_XRGB(255, 255, 255); v[69].tu = 1.0; v[69].tv = 1.0; v[69].NORMAL = D3DXVECTOR3(0.0, 1.0, 0.0);
	v[70].position = D3DXVECTOR3(-3.0, -1.0, 0.0); v[70].color = D3DCOLOR_XRGB(255, 255, 255); v[70].tu = 0.0; v[70].tv = 0.0; v[70].NORMAL = D3DXVECTOR3(0.0, 1.0, 0.0);
	v[71].position = D3DXVECTOR3(5.0, -1.0, 0.0); v[71].color = D3DCOLOR_XRGB(255, 255, 255); v[71].tu = 0.0; v[71].tv = 1.0; v[71].NORMAL = D3DXVECTOR3(0.0, 1.0, 0.0);
	// triangle 
	v[72].position = D3DXVECTOR3(-4.0, 0.0, -7.0); v[72].color = D3DCOLOR_XRGB(255, 255, 255); v[72].tu = 1.0; v[72].tv = 1.0; v[72].NORMAL = D3DXVECTOR3(-1.0, 1.0, 0.0);
	v[73].position = D3DXVECTOR3(-4.0, 0.0, 0.0); v[73].color = D3DCOLOR_XRGB(255, 255, 255); v[73].tu = 1.0; v[73].tv = 0.0; v[73].NORMAL = D3DXVECTOR3(-1.0, 1.0, 0.0);
	v[74].position = D3DXVECTOR3(-3.0, -1.0, 0.0); v[74].color = D3DCOLOR_XRGB(255, 255, 255); v[74].tu = 0.8; v[74].tv = 0.0; v[74].NORMAL = D3DXVECTOR3(-1.0, 1.0, 0.0);
	// triangle 
	v[75].position = D3DXVECTOR3(-3.0, -1.0, -7.0); v[75].color = D3DCOLOR_XRGB(255, 255, 255); v[75].tu = 0.8; v[75].tv = 1.0; v[75].NORMAL = D3DXVECTOR3(-1.0, 1.0, 0.0);
	v[76].position = D3DXVECTOR3(-4.0, 0.0, -7.0); v[76].color = D3DCOLOR_XRGB(255, 255, 255); v[76].tu = 1.0; v[76].tv = 1.0; v[76].NORMAL = D3DXVECTOR3(-1.0, 1.0, 0.0);
	v[77].position = D3DXVECTOR3(-3.0, -1.0, 0.0); v[77].color = D3DCOLOR_XRGB(255, 255, 255); v[77].tu = 0.8; v[77].tv = 0.0; v[77].NORMAL = D3DXVECTOR3(-1.0, 1.0, 0.0);
	// triangle 
	v[78].position = D3DXVECTOR3(-4.0, 0.0, -7.0); v[78].color = D3DCOLOR_XRGB(255, 255, 255); v[78].tu = 1.0; v[78].tv = 1.0; v[78].NORMAL = D3DXVECTOR3(1.0, -1.0, 0.0);
	v[79].position = D3DXVECTOR3(-4.0, 0.0, 0.0); v[79].color = D3DCOLOR_XRGB(255, 255, 255); v[79].tu = 1.0; v[79].tv = 0.0; v[79].NORMAL = D3DXVECTOR3(1.0, -1.0, 0.0);
	v[80].position = D3DXVECTOR3(-3.0, 1.0, 0.0); v[80].color = D3DCOLOR_XRGB(255, 255, 255); v[80].tu = 0.8; v[80].tv = 0.0; v[80].NORMAL = D3DXVECTOR3(1.0, -1.0, 0.0);
	// triangle 
	v[81].position = D3DXVECTOR3(-3.0, 1.0, -7.0); v[81].color = D3DCOLOR_XRGB(255, 255, 255); v[81].tu = 0.8; v[81].tv = 1.0; v[81].NORMAL = D3DXVECTOR3(1.0, -1.0, 0.0);
	v[82].position = D3DXVECTOR3(-4.0, 0.0, -7.0); v[82].color = D3DCOLOR_XRGB(255, 255, 255); v[82].tu = 1.0; v[82].tv = 1.0; v[82].NORMAL = D3DXVECTOR3(1.0, -1.0, 0.0);
	v[83].position = D3DXVECTOR3(-3.0, 1.0, 0.0); v[83].color = D3DCOLOR_XRGB(255, 255, 255); v[83].tu = 0.8; v[83].tv = 0.0; v[83].NORMAL = D3DXVECTOR3(1.0, -1.0, 0.0);
	// triangle 
	v[84].position = D3DXVECTOR3(6.0, 0.0, -7.0); v[84].color = D3DCOLOR_XRGB(255, 255, 255); v[84].tu = 1.0; v[84].tv = 1.0; v[84].NORMAL = D3DXVECTOR3(-1.0, 1.0, 0.0);
	v[85].position = D3DXVECTOR3(6.0, 0.0, 0.0); v[85].color = D3DCOLOR_XRGB(255, 255, 255); v[85].tu = 1.0; v[85].tv = 0.0; v[85].NORMAL = D3DXVECTOR3(-1.0, 1.0, 0.0);
	v[86].position = D3DXVECTOR3(5.0, 1.0, 0.0); v[86].color = D3DCOLOR_XRGB(255, 255, 255); v[86].tu = 0.8; v[86].tv = 0.0; v[86].NORMAL = D3DXVECTOR3(-1.0, 1.0, 0.0);
	// triangle 
	v[87].position = D3DXVECTOR3(5.0, 1.0, -7.0); v[87].color = D3DCOLOR_XRGB(255, 255, 255); v[87].tu = 0.8; v[87].tv = 1.0;  v[87].NORMAL = D3DXVECTOR3(-1.0, 1.0, 0.0);
	v[88].position = D3DXVECTOR3(6.0, 0.0, -7.0); v[88].color = D3DCOLOR_XRGB(255, 255, 255); v[88].tu = 1.0; v[88].tv = 1.0;  v[88].NORMAL = D3DXVECTOR3(-1.0, 1.0, 0.0);
	v[89].position = D3DXVECTOR3(5.0, 1.0, 0.0); v[89].color = D3DCOLOR_XRGB(255, 255, 255); v[89].tu = 0.8; v[89].tv = 0.0;  v[89].NORMAL = D3DXVECTOR3(-1.0, 1.0, 0.0);
	// triangle 
	v[90].position = D3DXVECTOR3(6.0, 0.0, -7.0); v[90].color = D3DCOLOR_XRGB(255, 255, 255); v[90].tu = 1.0; v[90].tv = 1.0; v[90].NORMAL = D3DXVECTOR3(1.0, -1.0, 0.0);
	v[91].position = D3DXVECTOR3(6.0, 0.0, 0.0); v[91].color = D3DCOLOR_XRGB(255, 255, 255); v[91].tu = 1.0; v[91].tv = 0.0; v[91].NORMAL = D3DXVECTOR3(1.0, -1.0, 0.0);
	v[92].position = D3DXVECTOR3(5.0, -1.0, 0.0); v[92].color = D3DCOLOR_XRGB(255, 255, 255); v[92].tu = 0.8; v[92].tv = 0.0; v[92].NORMAL = D3DXVECTOR3(1.0, -1.0, 0.0);
	// triangle 
	v[93].position = D3DXVECTOR3(5.0, -1.0, -7.0); v[93].color = D3DCOLOR_XRGB(255, 255, 255); v[93].tu = 0.8; v[93].tv = 1.0; v[93].NORMAL = D3DXVECTOR3(1.0, -1.0, 0.0);
	v[94].position = D3DXVECTOR3(6.0, 0.0, -7.0); v[94].color = D3DCOLOR_XRGB(255, 255, 255); v[94].tu = 1.0; v[94].tv = 1.0; v[94].NORMAL = D3DXVECTOR3(1.0, -1.0, 0.0);
	v[95].position = D3DXVECTOR3(5.0, -1.0, 0.0); v[95].color = D3DCOLOR_XRGB(255, 255, 255); v[95].tu = 0.8; v[95].tv = 0.0; v[95].NORMAL = D3DXVECTOR3(1.0, -1.0, 0.0);

	//top of body
	//triangle 3 4 5
	v[96].position = D3DXVECTOR3(-3.0, 1.0, 0.0); v[96].color = D3DCOLOR_XRGB(255, 255, 255); v[96].tu = 0.0; v[96].tv = 0.0; v[96].NORMAL = D3DXVECTOR3(0.0, -1.0, 0.0);
	v[97].position = D3DXVECTOR3(-1.0, 1.0, 0.0); v[97].color = D3DCOLOR_XRGB(255, 255, 255); v[97].tu = 1.0; v[97].tv = 0.0; v[97].NORMAL = D3DXVECTOR3(0.0, -1.0, 0.0);
	v[98].position = D3DXVECTOR3(-1.0, 1.0, -7.0); v[98].color = D3DCOLOR_XRGB(255, 255, 255); v[98].tu = 1.0; v[98].tv = 1.0; v[98].NORMAL = D3DXVECTOR3(0.0, -1.0, 0.0);
	//triangle 3 4 5
	v[99].position = D3DXVECTOR3(-3.0, 1.0, 0.0); v[99].color = D3DCOLOR_XRGB(255, 255, 255); v[99].tu = 0.0; v[99].tv = 0.0; v[99].NORMAL = D3DXVECTOR3(0.0, -1.0, 0.0);
	v[100].position = D3DXVECTOR3(-3.0, 1.0, -7.0); v[100].color = D3DCOLOR_XRGB(255, 255, 255); v[100].tu = 1.0; v[100].tv = 0.0; v[100].NORMAL = D3DXVECTOR3(0.0, -1.0, 0.0);
	v[101].position = D3DXVECTOR3(-1.0, 1.0, -7.0); v[101].color = D3DCOLOR_XRGB(255, 255, 255); v[101].tu = 1.0; v[101].tv = 1.0; v[101].NORMAL = D3DXVECTOR3(0.0, -1.0, 0.0);
	//triangle 3 4 5
	v[102].position = D3DXVECTOR3(3.0, 1.0, 0.0); v[102].color = D3DCOLOR_XRGB(255, 255, 255); v[102].tu = 0.0; v[102].tv = 0.0; v[102].NORMAL = D3DXVECTOR3(0.0, -1.0, 0.0);
	v[103].position = D3DXVECTOR3(5.0, 1.0, 0.0); v[103].color = D3DCOLOR_XRGB(255, 255, 255); v[103].tu = 1.0; v[103].tv = 0.0; v[103].NORMAL = D3DXVECTOR3(0.0, -1.0, 0.0);
	v[104].position = D3DXVECTOR3(5.0, 1.0, -7.0); v[104].color = D3DCOLOR_XRGB(255, 255, 255); v[104].tu = 1.0; v[104].tv = 1.0; v[104].NORMAL = D3DXVECTOR3(0.0, -1.0, 0.0);
	//triangle 3 4 5
	v[105].position = D3DXVECTOR3(3.0, 1.0, 0.0); v[105].color = D3DCOLOR_XRGB(255, 255, 255); v[105].tu = 0.0; v[105].tv = 0.0; v[105].NORMAL = D3DXVECTOR3(0.0, -1.0, 0.0);
	v[106].position = D3DXVECTOR3(3.0, 1.0, -7.0); v[106].color = D3DCOLOR_XRGB(255, 255, 255); v[106].tu = 1.0; v[106].tv = 0.0; v[106].NORMAL = D3DXVECTOR3(0.0, -1.0, 0.0);
	v[107].position = D3DXVECTOR3(5.0, 1.0, -7.0); v[107].color = D3DCOLOR_XRGB(255, 255, 255); v[107].tu = 1.0; v[107].tv = 1.0; v[107].NORMAL = D3DXVECTOR3(0.0, -1.0, 0.0);
	//triangle 3 4 5
	v[108].position = D3DXVECTOR3(0.0, 2.0, 0.0); v[108].color = D3DCOLOR_XRGB(255, 255, 255); v[108].tu = 0.0; v[108].tv = 0.0; v[108].NORMAL = D3DXVECTOR3(0.0, -1.0, 0.0);
	v[109].position = D3DXVECTOR3(2.0, 2.0, 0.0); v[109].color = D3DCOLOR_XRGB(255, 255, 255); v[109].tu = 1.0; v[109].tv = 0.0; v[109].NORMAL = D3DXVECTOR3(0.0, -1.0, 0.0);
	v[110].position = D3DXVECTOR3(2.0, 2.0, -7.0); v[110].color = D3DCOLOR_XRGB(255, 255, 255); v[110].tu = 1.0; v[110].tv = 1.0; v[110].NORMAL = D3DXVECTOR3(0.0, -1.0, 0.0);
	//triangle 3 4 5
	v[111].position = D3DXVECTOR3(0.0, 2.0, 0.0); v[111].color = D3DCOLOR_XRGB(255, 255, 255); v[111].tu = 0.0; v[111].tv = 0.0; v[111].NORMAL = D3DXVECTOR3(0.0, -1.0, 0.0);
	v[112].position = D3DXVECTOR3(0.0, 2.0, -7.0); v[112].color = D3DCOLOR_XRGB(255, 255, 255); v[112].tu = 1.0; v[112].tv = 0.0; v[112].NORMAL = D3DXVECTOR3(0.0, -1.0, 0.0);
	v[113].position = D3DXVECTOR3(2.0, 2.0, -7.0); v[113].color = D3DCOLOR_XRGB(255, 255, 255); v[113].tu = 1.0; v[113].tv = 1.0; v[113].NORMAL = D3DXVECTOR3(0.0, -1.0, 0.0);
	//triangle 3 4 5
	v[114].position = D3DXVECTOR3(3.0, 1.0, 0.0); v[114].color = D3DCOLOR_XRGB(255, 255, 255); v[114].tu = 0.0; v[114].tv = 0.0; v[114].NORMAL = D3DXVECTOR3(-1.0, -1.0, 0.0);
	v[115].position = D3DXVECTOR3(2.0, 2.0, 0.0); v[115].color = D3DCOLOR_XRGB(255, 255, 255); v[115].tu = 1.0; v[115].tv = 0.0; v[115].NORMAL = D3DXVECTOR3(-1.0, -1.0, 0.0);
	v[116].position = D3DXVECTOR3(2.0, 2.0, -7.0); v[116].color = D3DCOLOR_XRGB(255, 255, 255); v[116].tu = 1.0; v[116].tv = 1.0; v[116].NORMAL = D3DXVECTOR3(-1.0, -1.0, 0.0);
	//triangle 3 4 5
	v[117].position = D3DXVECTOR3(3.0, 1.0, 0.0); v[117].color = D3DCOLOR_XRGB(255, 255, 255); v[117].tu = 0.0; v[117].tv = 0.0; v[117].NORMAL = D3DXVECTOR3(-1.0, -1.0, 0.0);
	v[118].position = D3DXVECTOR3(3.0, 1.0, -7.0); v[118].color = D3DCOLOR_XRGB(255, 255, 255); v[118].tu = 1.0; v[118].tv = 0.0; v[118].NORMAL = D3DXVECTOR3(-1.0, -1.0, 0.0);
	v[119].position = D3DXVECTOR3(2.0, 2.0, -7.0); v[119].color = D3DCOLOR_XRGB(255, 255, 255); v[119].tu = 1.0; v[119].tv = 1.0; v[119].NORMAL = D3DXVECTOR3(-1.0, -1.0, 0.0);
	//triangle 3 4 5
	v[120].position = D3DXVECTOR3(0.0, 2.0, 0.0); v[120].color = D3DCOLOR_XRGB(255, 255, 255); v[120].tu = 0.0; v[120].tv = 0.0; v[120].NORMAL = D3DXVECTOR3(1.0, -1.0, 0.0);
	v[121].position = D3DXVECTOR3(0.0, 2.0, -7.0); v[121].color = D3DCOLOR_XRGB(255, 255, 255); v[121].tu = 1.0; v[121].tv = 0.0; v[121].NORMAL = D3DXVECTOR3(1.0, -1.0, 0.0);
	v[122].position = D3DXVECTOR3(-1.0, 1.0, -7.0); v[122].color = D3DCOLOR_XRGB(255, 255, 255); v[122].tu = 1.0; v[122].tv = 1.0; v[122].NORMAL = D3DXVECTOR3(1.0, -1.0, 0.0);
	//triangle 3 4 5
	v[123].position = D3DXVECTOR3(0.0, 2.0, 0.0); v[123].color = D3DCOLOR_XRGB(255, 255, 255); v[123].tu = 0.0; v[123].tv = 0.0; v[123].NORMAL = D3DXVECTOR3(1.0, -1.0, 0.0);
	v[124].position = D3DXVECTOR3(-1.0, 1.0, 0.0); v[124].color = D3DCOLOR_XRGB(255, 255, 255); v[124].tu = 1.0; v[124].tv = 0.0; v[124].NORMAL = D3DXVECTOR3(1.0, -1.0, 0.0);
	v[125].position = D3DXVECTOR3(-1.0, 1.0, -7.0); v[125].color = D3DCOLOR_XRGB(255, 255, 255); v[125].tu = 1.0; v[125].tv = 1.0; v[125].NORMAL = D3DXVECTOR3(1.0, -1.0, 0.0);

	//top wing #################################################################################################################
	//left side
	//triangle main
	v[126].position = D3DXVECTOR3(1.1, 2.0, 0.0); v[126].color = D3DCOLOR_XRGB(255, 255, 255); v[126].tu = 0.1; v[126].tv = 1.0; v[126].NORMAL = D3DXVECTOR3(-1.0, 0.0, 0.0);
	v[127].position = D3DXVECTOR3(1.1, 14.0, -7.0); v[127].color = D3DCOLOR_XRGB(255, 255, 255); v[127].tu = 1.0; v[127].tv = 0.0; v[127].NORMAL = D3DXVECTOR3(-1.0, 0.0, 0.0);
	v[128].position = D3DXVECTOR3(1.1, 2.0, -7.0); v[128].color = D3DCOLOR_XRGB(255, 255, 255); v[128].tu = 0.9; v[128].tv = 1.0; v[128].NORMAL = D3DXVECTOR3(-1.0, 0.0, 0.0);
	//triangle rear
	v[129].position = D3DXVECTOR3(1.1, 2.0, -7.5); v[129].color = D3DCOLOR_XRGB(255, 255, 255); v[129].tu = 0.9; v[129].tv = 1.0; v[129].NORMAL = D3DXVECTOR3(-1.0, 0.0, 0.0);
	v[130].position = D3DXVECTOR3(1.1, 14.0, -7.0); v[130].color = D3DCOLOR_XRGB(255, 255, 255); v[130].tu = 1.0; v[130].tv = 0.0; v[130].NORMAL = D3DXVECTOR3(-1.0, 0.0, 0.0);
	v[131].position = D3DXVECTOR3(1.1, 2.0, -7.0); v[131].color = D3DCOLOR_XRGB(255, 255, 255); v[131].tu = 1.0; v[131].tv = 1.0; v[131].NORMAL = D3DXVECTOR3(-1.0, 0.0, 0.0);
	//triangle front
	v[132].position = D3DXVECTOR3(1.1, 2.0, 0.0); v[132].color = D3DCOLOR_XRGB(255, 255, 255); v[132].tu = 0.1; v[132].tv = 1.0; v[132].NORMAL = D3DXVECTOR3(-1.0, 0.0, 0.0);
	v[133].position = D3DXVECTOR3(1.1, 14.0, -7.0); v[133].color = D3DCOLOR_XRGB(255, 255, 255); v[133].tu = 1.0; v[133].tv = 0.0; v[133].NORMAL = D3DXVECTOR3(-1.0, 0.0, 0.0);
	v[134].position = D3DXVECTOR3(1.1, 4.0, 1.0); v[134].color = D3DCOLOR_XRGB(255, 255, 255); v[134].tu = 0.0; v[134].tv = 0.8; v[134].NORMAL = D3DXVECTOR3(-1.0, 0.0, 0.0);
	//triangle front top
	v[135].position = D3DXVECTOR3(1.1, 14.0, -4.0); v[135].color = D3DCOLOR_XRGB(255, 255, 255); v[135].tu = 0.6; v[135].tv = 0.0; v[135].NORMAL = D3DXVECTOR3(-1.0, 0.0, 0.0);
	v[136].position = D3DXVECTOR3(1.1, 14.0, -7.0); v[136].color = D3DCOLOR_XRGB(255, 255, 255); v[136].tu = 1.0; v[136].tv = 0.0; v[136].NORMAL = D3DXVECTOR3(-1.0, 0.0, 0.0);
	v[137].position = D3DXVECTOR3(1.1, 4.0, 1.0); v[137].color = D3DCOLOR_XRGB(255, 255, 255); v[137].tu = 0.0; v[137].tv = 0.8; v[137].NORMAL = D3DXVECTOR3(-1.0, 0.0, 0.0);
	//right side
	//triangle main
	v[138].position = D3DXVECTOR3(0.9, 2.0, 0.0); v[138].color = D3DCOLOR_XRGB(255, 255, 255); v[138].tu = 0.1; v[138].tv = 1.0; v[138].NORMAL = D3DXVECTOR3(1.0, 0.0, 0.0);
	v[139].position = D3DXVECTOR3(0.9, 14.0, -7.0); v[139].color = D3DCOLOR_XRGB(255, 255, 255); v[139].tu = 1.0; v[139].tv = 0.0; v[139].NORMAL = D3DXVECTOR3(1.0, 0.0, 0.0);
	v[140].position = D3DXVECTOR3(0.9, 2.0, -7.0); v[140].color = D3DCOLOR_XRGB(255, 255, 255); v[140].tu = 0.9; v[140].tv = 1.0; v[140].NORMAL = D3DXVECTOR3(1.0, 0.0, 0.0);
	//triangle rear
	v[141].position = D3DXVECTOR3(0.9, 2.0, -7.5); v[141].color = D3DCOLOR_XRGB(255, 255, 255); v[141].tu = 0.9; v[141].tv = 1.0; v[141].NORMAL = D3DXVECTOR3(1.0, 0.0, 0.0);
	v[142].position = D3DXVECTOR3(0.9, 14.0, -7.0); v[142].color = D3DCOLOR_XRGB(255, 255, 255); v[142].tu = 1.0; v[142].tv = 0.0; v[142].NORMAL = D3DXVECTOR3(1.0, 0.0, 0.0);
	v[143].position = D3DXVECTOR3(0.9, 2.0, -7.0); v[143].color = D3DCOLOR_XRGB(255, 255, 255); v[143].tu = 1.0; v[143].tv = 1.0; v[143].NORMAL = D3DXVECTOR3(1.0, 0.0, 0.0);
	//triangle front
	v[144].position = D3DXVECTOR3(0.9, 2.0, 0.0); v[144].color = D3DCOLOR_XRGB(255, 255, 255); v[144].tu = 0.1; v[144].tv = 1.0; v[144].NORMAL = D3DXVECTOR3(1.0, 0.0, 0.0);
	v[145].position = D3DXVECTOR3(0.9, 14.0, -7.0); v[145].color = D3DCOLOR_XRGB(255, 255, 255); v[145].tu = 1.0; v[145].tv = 0.0; v[145].NORMAL = D3DXVECTOR3(1.0, 0.0, 0.0);
	v[146].position = D3DXVECTOR3(0.9, 4.0, 1.0); v[146].color = D3DCOLOR_XRGB(255, 255, 255); v[146].tu = 0.0; v[146].tv = 0.8; v[146].NORMAL = D3DXVECTOR3(1.0, 0.0, 0.0);
	//triangle front top
	v[147].position = D3DXVECTOR3(0.9, 14.0, -4.0); v[147].color = D3DCOLOR_XRGB(255, 255, 255); v[147].tu = 0.6; v[147].tv = 0.0; v[147].NORMAL = D3DXVECTOR3(1.0, 0.0, 0.0);
	v[148].position = D3DXVECTOR3(0.9, 14.0, -7.0); v[148].color = D3DCOLOR_XRGB(255, 255, 255); v[148].tu = 1.0; v[148].tv = 0.0; v[148].NORMAL = D3DXVECTOR3(1.0, 0.0, 0.0);
	v[149].position = D3DXVECTOR3(0.9, 4.0, 1.0); v[149].color = D3DCOLOR_XRGB(255, 255, 255); v[149].tu = 0.0; v[149].tv = 0.8; v[149].NORMAL = D3DXVECTOR3(1.0, 0.0, 0.0);
	//wing edge
	//triangle front edge 1
	v[150].position = D3DXVECTOR3(0.9, 2.0, 0.0); v[150].color = D3DCOLOR_XRGB(255, 255, 255); v[150].tu = 0.8; v[150].tv = 0.0; v[150].NORMAL = D3DXVECTOR3(0.0, 1.0, -1.0);
	v[151].position = D3DXVECTOR3(1.1, 4.0, 1.0); v[151].color = D3DCOLOR_XRGB(255, 255, 255); v[151].tu = 1.0; v[151].tv = 0.0; v[151].NORMAL = D3DXVECTOR3(0.0, 1.0, -1.0);
	v[152].position = D3DXVECTOR3(0.9, 4.0, 1.0); v[152].color = D3DCOLOR_XRGB(255, 255, 255); v[152].tu = 1.0; v[152].tv = 1.0; v[152].NORMAL = D3DXVECTOR3(0.0, 1.0, -1.0);
	//triangle front edge 2
	v[153].position = D3DXVECTOR3(0.9, 2.0, 0.0); v[153].color = D3DCOLOR_XRGB(255, 255, 255); v[153].tu = 0.8; v[153].tv = 0.0; v[153].NORMAL = D3DXVECTOR3(0.0, 1.0, -1.0);
	v[154].position = D3DXVECTOR3(1.1, 4.0, 1.0); v[154].color = D3DCOLOR_XRGB(255, 255, 255); v[154].tu = 1.0; v[154].tv = 0.0; v[154].NORMAL = D3DXVECTOR3(0.0, 1.0, -1.0);
	v[155].position = D3DXVECTOR3(1.1, 2.0, 0.0); v[155].color = D3DCOLOR_XRGB(255, 255, 255); v[155].tu = 1.0; v[155].tv = 1.0; v[155].NORMAL = D3DXVECTOR3(0.0, 1.0, -1.0);
	//triangle front top edge 1
	v[156].position = D3DXVECTOR3(0.9, 14.0, -4.0); v[156].color = D3DCOLOR_XRGB(255, 255, 255); v[156].tu = 0.8; v[156].tv = 0.0; v[156].NORMAL = D3DXVECTOR3(0.0, -1.0, -1.0);
	v[157].position = D3DXVECTOR3(1.1, 14.0, -4.0); v[157].color = D3DCOLOR_XRGB(255, 255, 255); v[157].tu = 1.0; v[157].tv = 0.0; v[157].NORMAL = D3DXVECTOR3(0.0, -1.0, -1.0);
	v[158].position = D3DXVECTOR3(0.9, 4.0, 1.0); v[158].color = D3DCOLOR_XRGB(255, 255, 255); v[158].tu = 1.0; v[158].tv = 1.0; v[158].NORMAL = D3DXVECTOR3(0.0, -1.0, -1.0);
	//triangle front top edge 2
	v[159].position = D3DXVECTOR3(1.1, 4.0, 1.0); v[159].color = D3DCOLOR_XRGB(255, 255, 255); v[159].tu = 0.8; v[159].tv = 0.0; v[159].NORMAL = D3DXVECTOR3(0.0, -1.0, -1.0);
	v[160].position = D3DXVECTOR3(1.1, 14.0, -4.0); v[160].color = D3DCOLOR_XRGB(255, 255, 255); v[160].tu = 1.0; v[160].tv = 0.0; v[160].NORMAL = D3DXVECTOR3(0.0, -1.0, -1.0);
	v[161].position = D3DXVECTOR3(0.9, 4.0, 1.0); v[161].color = D3DCOLOR_XRGB(255, 255, 255); v[161].tu = 1.0; v[161].tv = 1.0; v[161].NORMAL = D3DXVECTOR3(0.0, -1.0, -1.0);
	//triangle top edge 1
	v[162].position = D3DXVECTOR3(0.9, 14.0, -4.0); v[162].color = D3DCOLOR_XRGB(255, 255, 255); v[162].tu = 0.8; v[162].tv = 0.0; v[162].NORMAL = D3DXVECTOR3(0.0, -1.0, 0.0);
	v[163].position = D3DXVECTOR3(0.9, 14.0, -7.0); v[163].color = D3DCOLOR_XRGB(255, 255, 255); v[163].tu = 1.0; v[163].tv = 0.0; v[163].NORMAL = D3DXVECTOR3(0.0, -1.0, 0.0);
	v[164].position = D3DXVECTOR3(1.1, 14.0, -4.0); v[164].color = D3DCOLOR_XRGB(255, 255, 255); v[164].tu = 1.0; v[164].tv = 1.0; v[164].NORMAL = D3DXVECTOR3(0.0, -1.0, 0.0);
	//triangle top edge 2
	v[165].position = D3DXVECTOR3(1.1, 14.0, -7.0); v[165].color = D3DCOLOR_XRGB(255, 255, 255); v[165].tu = 0.8; v[165].tv = 0.0; v[165].NORMAL = D3DXVECTOR3(0.0, -1.0, 0.0);
	v[166].position = D3DXVECTOR3(0.9, 14.0, -7.0); v[166].color = D3DCOLOR_XRGB(255, 255, 255); v[166].tu = 1.0; v[166].tv = 0.0; v[166].NORMAL = D3DXVECTOR3(0.0, -1.0, 0.0);
	v[167].position = D3DXVECTOR3(1.1, 14.0, -4.0); v[167].color = D3DCOLOR_XRGB(255, 255, 255); v[167].tu = 1.0; v[167].tv = 1.0; v[167].NORMAL = D3DXVECTOR3(0.0, -1.0, 0.0);
	//triangle rear edge 1
	v[168].position = D3DXVECTOR3(0.9, 2.0, -7.5); v[168].color = D3DCOLOR_XRGB(255, 255, 255); v[168].tu = 0.8; v[168].tv = 0.0; v[168].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[169].position = D3DXVECTOR3(0.9, 14.0, -7.0); v[169].color = D3DCOLOR_XRGB(255, 255, 255); v[169].tu = 1.0; v[169].tv = 0.0; v[169].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[170].position = D3DXVECTOR3(1.1, 14.0, -7.0); v[170].color = D3DCOLOR_XRGB(255, 255, 255); v[170].tu = 1.0; v[170].tv = 1.0; v[170].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	//triangle rear edge 2
	v[171].position = D3DXVECTOR3(0.9, 2.0, -7.5); v[171].color = D3DCOLOR_XRGB(255, 255, 255); v[171].tu = 0.8; v[171].tv = 0.0; v[171].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[172].position = D3DXVECTOR3(1.1, 2.0, -7.5); v[172].color = D3DCOLOR_XRGB(255, 255, 255); v[172].tu = 1.0; v[172].tv = 0.0; v[172].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[173].position = D3DXVECTOR3(1.1, 14.0, -7.0); v[173].color = D3DCOLOR_XRGB(255, 255, 255); v[173].tu = 1.0; v[173].tv = 1.0; v[173].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);

	//bottom left wing #################################################################################################################
	//outter wing
	//triangle join 1
	v[174].position = D3DXVECTOR3(-3.0, -1.0, -7.0); v[174].color = D3DCOLOR_XRGB(255, 255, 255); v[174].tu = 1.0; v[174].tv = 0.0; v[174].NORMAL = D3DXVECTOR3(1.0, 0.0, 0.0);
	v[175].position = D3DXVECTOR3(-3.0, -2.5, -7.0); v[175].color = D3DCOLOR_XRGB(255, 255, 255); v[175].tu = 1.0; v[175].tv = 0.2; v[175].NORMAL = D3DXVECTOR3(1.0, 0.0, 0.0);
	v[176].position = D3DXVECTOR3(-3.0, -1.0, -1.0); v[176].color = D3DCOLOR_XRGB(255, 255, 255); v[176].tu = 0.0; v[176].tv = 0.0; v[176].NORMAL = D3DXVECTOR3(1.0, 0.0, 0.0);
	//triangle join 2
	v[177].position = D3DXVECTOR3(-3.0, -2.5, -1.0); v[177].color = D3DCOLOR_XRGB(255, 255, 255); v[177].tu = 0.0; v[177].tv = 0.2; v[177].NORMAL = D3DXVECTOR3(1.0, 0.0, 0.0);
	v[178].position = D3DXVECTOR3(-3.0, -2.5, -7.0); v[178].color = D3DCOLOR_XRGB(255, 255, 255); v[178].tu = 1.0; v[178].tv = 0.2; v[178].NORMAL = D3DXVECTOR3(1.0, 0.0, 0.0);
	v[179].position = D3DXVECTOR3(-3.0, -1.0, -1.0); v[179].color = D3DCOLOR_XRGB(255, 255, 255); v[179].tu = 0.0; v[179].tv = 0.0; v[179].NORMAL = D3DXVECTOR3(1.0, 0.0, 0.0);
	//triangle top main
	v[180].position = D3DXVECTOR3(-3.0, -2.5, -1.0); v[180].color = D3DCOLOR_XRGB(255, 255, 255); v[180].tu = 0.0; v[180].tv = 0.0; v[180].NORMAL = D3DXVECTOR3(1.0, -1.0, 0.0);
	v[181].position = D3DXVECTOR3(-3.0, -2.5, -7.0); v[181].color = D3DCOLOR_XRGB(255, 255, 255); v[181].tu = 1.0; v[181].tv = 0.0; v[181].NORMAL = D3DXVECTOR3(1.0, -1.0, 0.0);
	v[182].position = D3DXVECTOR3(-10.0, -12.5, -7.0); v[182].color = D3DCOLOR_XRGB(255, 255, 255); v[182].tu = 1.0; v[182].tv = 1.0; v[182].NORMAL = D3DXVECTOR3(1.0, -1.0, 0.0);
	//triangle bottom main
	v[183].position = D3DXVECTOR3(-3.0, -2.5, -1.0); v[183].color = D3DCOLOR_XRGB(255, 255, 255); v[183].tu = 0.0; v[183].tv = 0.0; v[183].NORMAL = D3DXVECTOR3(1.0, -1.0, 0.0);
	v[184].position = D3DXVECTOR3(-10.0, -12.5, -3.0); v[184].color = D3DCOLOR_XRGB(255, 255, 255); v[184].tu = 1.0; v[184].tv = 0.0; v[184].NORMAL = D3DXVECTOR3(1.0, -1.0, 0.0);
	v[185].position = D3DXVECTOR3(-10.0, -12.5, -7.0); v[185].color = D3DCOLOR_XRGB(255, 255, 255); v[185].tu = 1.0; v[185].tv = 1.0; v[185].NORMAL = D3DXVECTOR3(1.0, -1.0, 0.0);
	//inner wing
	//triangle join 1
	v[186].position = D3DXVECTOR3(-2.8, -1.0, -7.0); v[186].color = D3DCOLOR_XRGB(255, 255, 255); v[186].tu = 1.0; v[186].tv = 0.0; v[186].NORMAL = D3DXVECTOR3(-1.0, 0.0, 0.0);
	v[187].position = D3DXVECTOR3(-2.8, -2.5, -7.0); v[187].color = D3DCOLOR_XRGB(255, 255, 255); v[187].tu = 1.0; v[187].tv = 0.2; v[187].NORMAL = D3DXVECTOR3(-1.0, 0.0, 0.0);
	v[188].position = D3DXVECTOR3(-2.8, -1.0, -1.0); v[188].color = D3DCOLOR_XRGB(255, 255, 255); v[188].tu = 0.0; v[188].tv = 0.0; v[188].NORMAL = D3DXVECTOR3(-1.0, 0.0, 0.0);
	//triangle join 2
	v[189].position = D3DXVECTOR3(-2.8, -2.5, -1.0); v[189].color = D3DCOLOR_XRGB(255, 255, 255); v[189].tu = 0.0; v[189].tv = 0.2; v[189].NORMAL = D3DXVECTOR3(-1.0, 0.0, 0.0);
	v[190].position = D3DXVECTOR3(-2.8, -2.5, -7.0); v[190].color = D3DCOLOR_XRGB(255, 255, 255); v[190].tu = 1.0; v[190].tv = 0.2; v[190].NORMAL = D3DXVECTOR3(-1.0, 0.0, 0.0);
	v[191].position = D3DXVECTOR3(-2.8, -1.0, -1.0); v[191].color = D3DCOLOR_XRGB(255, 255, 255); v[191].tu = 0.0; v[191].tv = 0.0; v[191].NORMAL = D3DXVECTOR3(-1.0, 0.0, 0.0);
	//triangle top main
	v[192].position = D3DXVECTOR3(-2.8, -2.5, -1.0); v[192].color = D3DCOLOR_XRGB(255, 255, 255); v[192].tu = 0.0; v[192].tv = 0.0; v[192].NORMAL = D3DXVECTOR3(-1.0, 1.0, 0.0);
	v[193].position = D3DXVECTOR3(-2.8, -2.5, -7.0); v[193].color = D3DCOLOR_XRGB(255, 255, 255); v[193].tu = 1.0; v[193].tv = 0.0; v[193].NORMAL = D3DXVECTOR3(-1.0, 1.0, 0.0);
	v[194].position = D3DXVECTOR3(-9.8, -12.5, -7.0); v[194].color = D3DCOLOR_XRGB(255, 255, 255); v[194].tu = 1.0; v[194].tv = 1.0; v[194].NORMAL = D3DXVECTOR3(-1.0, 1.0, 0.0);
	//triangle bottom main
	v[195].position = D3DXVECTOR3(-2.8, -2.5, -1.0); v[195].color = D3DCOLOR_XRGB(255, 255, 255); v[195].tu = 0.0; v[195].tv = 0.0; v[195].NORMAL = D3DXVECTOR3(-1.0, 1.0, 0.0);
	v[196].position = D3DXVECTOR3(-9.8, -12.5, -3.0); v[196].color = D3DCOLOR_XRGB(255, 255, 255); v[196].tu = 1.0; v[196].tv = 0.0; v[196].NORMAL = D3DXVECTOR3(-1.0, 1.0, 0.0);
	v[197].position = D3DXVECTOR3(-9.8, -12.5, -7.0); v[197].color = D3DCOLOR_XRGB(255, 255, 255); v[197].tu = 1.0; v[197].tv = 1.0; v[197].NORMAL = D3DXVECTOR3(-1.0, 1.0, 0.0);
	//wing sides
	//triangle join front 1
	v[198].position = D3DXVECTOR3(-3.0, -2.5, -1.0); v[198].color = D3DCOLOR_XRGB(255, 255, 255); v[198].tu = 0.8; v[198].tv = 0.0; v[198].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[199].position = D3DXVECTOR3(-2.8, -1.0, -1.0); v[199].color = D3DCOLOR_XRGB(255, 255, 255); v[199].tu = 1.0; v[199].tv = 0.0; v[199].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[200].position = D3DXVECTOR3(-3.0, -1.0, -1.0); v[200].color = D3DCOLOR_XRGB(255, 255, 255); v[200].tu = 1.0; v[200].tv = 1.0; v[200].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	//triangle join front 2
	v[201].position = D3DXVECTOR3(-3.0, -2.5, -1.0); v[201].color = D3DCOLOR_XRGB(255, 255, 255); v[201].tu = 0.8; v[201].tv = 0.0; v[201].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[202].position = D3DXVECTOR3(-2.8, -1.0, -1.0); v[202].color = D3DCOLOR_XRGB(255, 255, 255); v[202].tu = 1.0; v[202].tv = 0.0; v[202].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[203].position = D3DXVECTOR3(-2.8, -2.5, -1.0); v[203].color = D3DCOLOR_XRGB(255, 255, 255); v[203].tu = 1.0; v[203].tv = 1.0; v[203].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	//triangle front 1
	v[204].position = D3DXVECTOR3(-2.8, -2.5, -1.0); v[204].color = D3DCOLOR_XRGB(255, 255, 255); v[204].tu = 0.8; v[204].tv = 0.0; v[204].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[205].position = D3DXVECTOR3(-10.0, -12.5, -3.0); v[205].color = D3DCOLOR_XRGB(255, 255, 255); v[205].tu = 1.0; v[205].tv = 0.0; v[205].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[206].position = D3DXVECTOR3(-3.0, -2.5, -1.0); v[206].color = D3DCOLOR_XRGB(255, 255, 255); v[206].tu = 1.0; v[206].tv = 1.0; v[206].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	//triangle front 1
	v[207].position = D3DXVECTOR3(-2.8, -2.5, -1.0); v[207].color = D3DCOLOR_XRGB(255, 255, 255); v[207].tu = 0.8; v[207].tv = 0.0; v[207].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[208].position = D3DXVECTOR3(-10.0, -12.5, -3.0); v[208].color = D3DCOLOR_XRGB(255, 255, 255); v[208].tu = 1.0; v[208].tv = 0.0; v[208].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[209].position = D3DXVECTOR3(-9.8, -12.5, -3.0); v[209].color = D3DCOLOR_XRGB(255, 255, 255); v[209].tu = 1.0; v[209].tv = 1.0; v[209].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	//triangle bottom 1
	v[210].position = D3DXVECTOR3(-10.0, -12.5, -3.0); v[210].color = D3DCOLOR_XRGB(255, 255, 255); v[210].tu = 0.8; v[210].tv = 0.0; v[210].NORMAL = D3DXVECTOR3(0.0, 1.0, 0.0);
	v[211].position = D3DXVECTOR3(-9.8, -12.5, -3.0); v[211].color = D3DCOLOR_XRGB(255, 255, 255); v[211].tu = 1.0; v[211].tv = 0.0; v[211].NORMAL = D3DXVECTOR3(0.0, 1.0, 0.0);
	v[212].position = D3DXVECTOR3(-9.8, -12.5, -7.0); v[212].color = D3DCOLOR_XRGB(255, 255, 255); v[212].tu = 1.0; v[212].tv = 1.0; v[212].NORMAL = D3DXVECTOR3(0.0, 1.0, 0.0);
	//triangle bottom 2
	v[213].position = D3DXVECTOR3(-10.0, -12.5, -3.0); v[213].color = D3DCOLOR_XRGB(255, 255, 255); v[213].tu = 0.8; v[213].tv = 0.0; v[213].NORMAL = D3DXVECTOR3(0.0, 1.0, 0.0);
	v[214].position = D3DXVECTOR3(-10.0, -12.5, -7.0); v[214].color = D3DCOLOR_XRGB(255, 255, 255); v[214].tu = 1.0; v[214].tv = 0.0; v[214].NORMAL = D3DXVECTOR3(0.0, 1.0, 0.0);
	v[215].position = D3DXVECTOR3(-9.8, -12.5, -7.0); v[215].color = D3DCOLOR_XRGB(255, 255, 255); v[215].tu = 1.0; v[215].tv = 1.0; v[215].NORMAL = D3DXVECTOR3(0.0, 1.0, 0.0);
	//triangle back 1
	v[216].position = D3DXVECTOR3(-3.0, -2.5, -7.0); v[216].color = D3DCOLOR_XRGB(255, 255, 255); v[216].tu = 0.8; v[216].tv = 0.0; v[216].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[217].position = D3DXVECTOR3(-2.8, -2.5, -7.0); v[217].color = D3DCOLOR_XRGB(255, 255, 255); v[217].tu = 1.0; v[217].tv = 0.0; v[217].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[218].position = D3DXVECTOR3(-9.8, -12.5, -7.0); v[218].color = D3DCOLOR_XRGB(255, 255, 255); v[218].tu = 1.0; v[218].tv = 1.0; v[218].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	//triangle back 2
	v[219].position = D3DXVECTOR3(-3.0, -2.5, -7.0); v[219].color = D3DCOLOR_XRGB(255, 255, 255); v[219].tu = 0.8; v[219].tv = 0.0; v[219].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[220].position = D3DXVECTOR3(-10.0, -12.5, -7.0); v[220].color = D3DCOLOR_XRGB(255, 255, 255); v[220].tu = 1.0; v[220].tv = 0.0; v[220].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[221].position = D3DXVECTOR3(-9.8, -12.5, -7.0); v[221].color = D3DCOLOR_XRGB(255, 255, 255); v[221].tu = 1.0; v[221].tv = 1.0; v[221].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	//triangle join back 1
	v[222].position = D3DXVECTOR3(-2.8, -1.0, -7.0); v[222].color = D3DCOLOR_XRGB(255, 255, 255); v[222].tu = 0.8; v[222].tv = 0.0; v[222].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[223].position = D3DXVECTOR3(-2.8, -2.5, -7.0); v[223].color = D3DCOLOR_XRGB(255, 255, 255); v[223].tu = 1.0; v[223].tv = 0.0; v[223].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[224].position = D3DXVECTOR3(-3.0, -1.0, -7.0); v[224].color = D3DCOLOR_XRGB(255, 255, 255); v[224].tu = 1.0; v[224].tv = 1.0; v[224].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	//triangle join back 1
	v[225].position = D3DXVECTOR3(-3.0, -2.5, -7.0); v[225].color = D3DCOLOR_XRGB(255, 255, 255); v[225].tu = 0.8; v[225].tv = 0.0; v[225].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[226].position = D3DXVECTOR3(-2.8, -2.5, -7.0); v[226].color = D3DCOLOR_XRGB(255, 255, 255); v[226].tu = 1.0; v[226].tv = 0.0; v[226].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[227].position = D3DXVECTOR3(-3.0, -1.0, -7.0); v[227].color = D3DCOLOR_XRGB(255, 255, 255); v[227].tu = 1.0; v[227].tv = 1.0; v[227].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);

	//bottom right wing #################################################################################################################
	//outter wing
	//triangle join 1
	v[228].position = D3DXVECTOR3(5.0, -1.0, -7.0); v[228].color = D3DCOLOR_XRGB(255, 255, 255); v[228].tu = 1.0; v[228].tv = 0.0; v[228].NORMAL = D3DXVECTOR3(-1.0, 0.0, 0.0);
	v[229].position = D3DXVECTOR3(5.0, -2.5, -7.0); v[229].color = D3DCOLOR_XRGB(255, 255, 255); v[229].tu = 1.0; v[229].tv = 0.2; v[229].NORMAL = D3DXVECTOR3(-1.0, 0.0, 0.0);
	v[230].position = D3DXVECTOR3(5.0, -1.0, -1.0); v[230].color = D3DCOLOR_XRGB(255, 255, 255); v[230].tu = 0.0; v[230].tv = 0.0; v[230].NORMAL = D3DXVECTOR3(-1.0, 0.0, 0.0);
	//triangle join 2
	v[231].position = D3DXVECTOR3(5.0, -2.5, -1.0); v[231].color = D3DCOLOR_XRGB(255, 255, 255); v[231].tu = 0.0; v[231].tv = 0.2; v[231].NORMAL = D3DXVECTOR3(-1.0, 0.0, 0.0);
	v[232].position = D3DXVECTOR3(5.0, -2.5, -7.0); v[232].color = D3DCOLOR_XRGB(255, 255, 255); v[232].tu = 1.0; v[232].tv = 0.2; v[232].NORMAL = D3DXVECTOR3(-1.0, 0.0, 0.0);
	v[233].position = D3DXVECTOR3(5.0, -1.0, -1.0); v[233].color = D3DCOLOR_XRGB(255, 255, 255); v[233].tu = 0.0; v[233].tv = 0.0; v[233].NORMAL = D3DXVECTOR3(-1.0, 0.0, 0.0);
	//triangle top main
	v[234].position = D3DXVECTOR3(5.0, -2.5, -1.0); v[234].color = D3DCOLOR_XRGB(255, 255, 255); v[234].tu = 0.0; v[234].tv = 0.0; v[234].NORMAL = D3DXVECTOR3(-1.0, -1.0, 0.0);
	v[235].position = D3DXVECTOR3(5.0, -2.5, -7.0); v[235].color = D3DCOLOR_XRGB(255, 255, 255); v[235].tu = 1.0; v[235].tv = 0.0; v[235].NORMAL = D3DXVECTOR3(-1.0, -1.0, 0.0);
	v[236].position = D3DXVECTOR3(13.0, -12.5, -7.0); v[236].color = D3DCOLOR_XRGB(255, 255, 255); v[236].tu = 1.0; v[236].tv = 1.0; v[236].NORMAL = D3DXVECTOR3(-1.0, -1.0, 0.0);
	//triangle bottom main
	v[237].position = D3DXVECTOR3(5.0, -2.5, -1.0); v[237].color = D3DCOLOR_XRGB(255, 255, 255); v[237].tu = 0.0; v[237].tv = 0.0; v[237].NORMAL = D3DXVECTOR3(-1.0, -1.0, 0.0);
	v[238].position = D3DXVECTOR3(13.0, -12.5, -3.0); v[238].color = D3DCOLOR_XRGB(255, 255, 255); v[238].tu = 1.0; v[238].tv = 0.0; v[238].NORMAL = D3DXVECTOR3(-1.0, -1.0, 0.0);
	v[239].position = D3DXVECTOR3(13.0, -12.5, -7.0); v[239].color = D3DCOLOR_XRGB(255, 255, 255); v[239].tu = 1.0; v[239].tv = 1.0; v[239].NORMAL = D3DXVECTOR3(-1.0, -1.0, 0.0);
	//inner wing
	//triangle join 1
	v[240].position = D3DXVECTOR3(4.8, -1.0, -7.0); v[240].color = D3DCOLOR_XRGB(255, 255, 255); v[240].tu = 1.0; v[240].tv = 0.0; v[240].NORMAL = D3DXVECTOR3(1.0, 0.0, 0.0);
	v[241].position = D3DXVECTOR3(4.8, -2.5, -7.0); v[241].color = D3DCOLOR_XRGB(255, 255, 255); v[241].tu = 1.0; v[241].tv = 0.2; v[241].NORMAL = D3DXVECTOR3(1.0, 0.0, 0.0);
	v[242].position = D3DXVECTOR3(4.8, -1.0, -1.0); v[242].color = D3DCOLOR_XRGB(255, 255, 255); v[242].tu = 0.0; v[242].tv = 0.0; v[242].NORMAL = D3DXVECTOR3(1.0, 0.0, 0.0);
	//triangle join 2
	v[243].position = D3DXVECTOR3(4.8, -2.5, -1.0); v[243].color = D3DCOLOR_XRGB(255, 255, 255); v[243].tu = 0.0; v[243].tv = 0.2; v[243].NORMAL = D3DXVECTOR3(1.0, 0.0, 0.0);
	v[244].position = D3DXVECTOR3(4.8, -2.5, -7.0); v[244].color = D3DCOLOR_XRGB(255, 255, 255); v[244].tu = 1.0; v[244].tv = 0.2; v[244].NORMAL = D3DXVECTOR3(1.0, 0.0, 0.0);
	v[245].position = D3DXVECTOR3(4.8, -1.0, -1.0); v[245].color = D3DCOLOR_XRGB(255, 255, 255); v[245].tu = 0.0; v[245].tv = 0.0; v[245].NORMAL = D3DXVECTOR3(1.0, 0.0, 0.0);
	//triangle top main
	v[246].position = D3DXVECTOR3(4.8, -2.5, -1.0); v[246].color = D3DCOLOR_XRGB(255, 255, 255); v[246].tu = 0.0; v[246].tv = 0.0; v[246].NORMAL = D3DXVECTOR3(1.0, 1.0, 0.0);
	v[247].position = D3DXVECTOR3(4.8, -2.5, -7.0); v[247].color = D3DCOLOR_XRGB(255, 255, 255); v[247].tu = 1.0; v[247].tv = 0.0; v[247].NORMAL = D3DXVECTOR3(1.0, 1.0, 0.0);
	v[248].position = D3DXVECTOR3(12.8, -12.5, -7.0); v[248].color = D3DCOLOR_XRGB(255, 255, 255); v[248].tu = 1.0; v[248].tv = 1.0; v[248].NORMAL = D3DXVECTOR3(1.0, 1.0, 0.0);
	//triangle bottom main
	v[249].position = D3DXVECTOR3(4.8, -2.5, -1.0); v[249].color = D3DCOLOR_XRGB(255, 255, 255); v[249].tu = 0.0; v[249].tv = 0.0; v[249].NORMAL = D3DXVECTOR3(1.0, 1.0, 0.0);
	v[250].position = D3DXVECTOR3(12.8, -12.5, -3.0); v[250].color = D3DCOLOR_XRGB(255, 255, 255); v[250].tu = 1.0; v[250].tv = 0.0; v[250].NORMAL = D3DXVECTOR3(1.0, 1.0, 0.0);
	v[251].position = D3DXVECTOR3(12.8, -12.5, -7.0); v[251].color = D3DCOLOR_XRGB(255, 255, 255); v[251].tu = 1.0; v[251].tv = 1.0; v[251].NORMAL = D3DXVECTOR3(1.0, 1.0, 0.0);
	//wing sides
	//triangle join front 1
	v[252].position = D3DXVECTOR3(5.0, -2.5, -1.0); v[252].color = D3DCOLOR_XRGB(255, 255, 255); v[252].tu = 0.8; v[252].tv = 0.0; v[252].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[253].position = D3DXVECTOR3(4.8, -1.0, -1.0); v[253].color = D3DCOLOR_XRGB(255, 255, 255); v[253].tu = 1.0; v[253].tv = 0.0; v[253].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[254].position = D3DXVECTOR3(5.0, -1.0, -1.0); v[254].color = D3DCOLOR_XRGB(255, 255, 255); v[254].tu = 1.0; v[254].tv = 1.0; v[254].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	//triangle join front 2
	v[255].position = D3DXVECTOR3(5.0, -2.5, -1.0); v[255].color = D3DCOLOR_XRGB(255, 255, 255); v[255].tu = 0.8; v[255].tv = 0.0; v[255].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[256].position = D3DXVECTOR3(4.8, -1.0, -1.0); v[256].color = D3DCOLOR_XRGB(255, 255, 255); v[256].tu = 1.0; v[256].tv = 0.0; v[256].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[257].position = D3DXVECTOR3(4.8, -2.5, -1.0); v[257].color = D3DCOLOR_XRGB(255, 255, 255); v[257].tu = 1.0; v[257].tv = 1.0; v[257].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	//triangle front 1
	v[258].position = D3DXVECTOR3(4.8, -2.5, -1.0); v[258].color = D3DCOLOR_XRGB(255, 255, 255); v[258].tu = 0.8; v[258].tv = 0.0; v[258].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[259].position = D3DXVECTOR3(13.0, -12.5, -3.0); v[259].color = D3DCOLOR_XRGB(255, 255, 255); v[259].tu = 1.0; v[259].tv = 0.0; v[259].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[260].position = D3DXVECTOR3(5.0, -2.5, -1.0); v[260].color = D3DCOLOR_XRGB(255, 255, 255); v[260].tu = 1.0; v[260].tv = 1.0; v[260].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	//triangle front 1
	v[261].position = D3DXVECTOR3(4.8, -2.5, -1.0); v[261].color = D3DCOLOR_XRGB(255, 255, 255); v[261].tu = 0.8; v[261].tv = 0.0; v[261].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[262].position = D3DXVECTOR3(13.0, -12.5, -3.0); v[262].color = D3DCOLOR_XRGB(255, 255, 255); v[262].tu = 1.0; v[262].tv = 0.0; v[262].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	v[263].position = D3DXVECTOR3(12.8, -12.5, -3.0); v[263].color = D3DCOLOR_XRGB(255, 255, 255); v[263].tu = 1.0; v[263].tv = 1.0; v[263].NORMAL = D3DXVECTOR3(0.0, 0.0, -1.0);
	//triangle bottom 1
	v[264].position = D3DXVECTOR3(13.0, -12.5, -3.0); v[264].color = D3DCOLOR_XRGB(255, 255, 255); v[264].tu = 0.8; v[264].tv = 0.0; v[264].NORMAL = D3DXVECTOR3(0.0, 1.0, 0.0);
	v[265].position = D3DXVECTOR3(12.8, -12.5, -3.0); v[265].color = D3DCOLOR_XRGB(255, 255, 255); v[265].tu = 1.0; v[265].tv = 0.0; v[265].NORMAL = D3DXVECTOR3(0.0, 1.0, 0.0);
	v[266].position = D3DXVECTOR3(12.8, -12.5, -7.0); v[266].color = D3DCOLOR_XRGB(255, 255, 255); v[266].tu = 1.0; v[266].tv = 1.0; v[266].NORMAL = D3DXVECTOR3(0.0, 1.0, 0.0);
	//triangle bottom 2
	v[267].position = D3DXVECTOR3(13.0, -12.5, -3.0); v[267].color = D3DCOLOR_XRGB(255, 255, 255); v[267].tu = 0.8; v[267].tv = 0.0; v[267].NORMAL = D3DXVECTOR3(0.0, 1.0, 0.0);
	v[268].position = D3DXVECTOR3(13.0, -12.5, -7.0); v[268].color = D3DCOLOR_XRGB(255, 255, 255); v[268].tu = 1.0; v[268].tv = 0.0; v[268].NORMAL = D3DXVECTOR3(0.0, 1.0, 0.0);
	v[269].position = D3DXVECTOR3(12.8, -12.5, -7.0); v[269].color = D3DCOLOR_XRGB(255, 255, 255); v[269].tu = 1.0; v[269].tv = 1.0; v[269].NORMAL = D3DXVECTOR3(0.0, 1.0, 0.0);
	//triangle back 1
	v[270].position = D3DXVECTOR3(5.0, -2.5, -7.0); v[270].color = D3DCOLOR_XRGB(255, 255, 255); v[270].tu = 0.8; v[270].tv = 0.0; v[270].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[271].position = D3DXVECTOR3(4.8, -2.5, -7.0); v[271].color = D3DCOLOR_XRGB(255, 255, 255); v[271].tu = 1.0; v[271].tv = 0.0; v[271].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[272].position = D3DXVECTOR3(12.8, -12.5, -7.0); v[272].color = D3DCOLOR_XRGB(255, 255, 255); v[272].tu = 1.0; v[272].tv = 1.0; v[272].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	//triangle back 2
	v[273].position = D3DXVECTOR3(5.0, -2.5, -7.0); v[273].color = D3DCOLOR_XRGB(255, 255, 255); v[273].tu = 0.8; v[273].tv = 0.0; v[273].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[274].position = D3DXVECTOR3(13.0, -12.5, -7.0); v[274].color = D3DCOLOR_XRGB(255, 255, 255); v[274].tu = 1.0; v[274].tv = 0.0; v[274].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[275].position = D3DXVECTOR3(12.8, -12.5, -7.0); v[275].color = D3DCOLOR_XRGB(255, 255, 255); v[275].tu = 1.0; v[275].tv = 1.0; v[275].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	//triangle join back 1
	v[276].position = D3DXVECTOR3(4.8, -1.0, -7.0); v[276].color = D3DCOLOR_XRGB(255, 255, 255); v[276].tu = 0.8; v[276].tv = 0.0; v[276].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[277].position = D3DXVECTOR3(4.8, -2.5, -7.0); v[277].color = D3DCOLOR_XRGB(255, 255, 255); v[277].tu = 1.0; v[277].tv = 0.0; v[277].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[278].position = D3DXVECTOR3(5.0, -1.0, -7.0); v[278].color = D3DCOLOR_XRGB(255, 255, 255); v[278].tu = 1.0; v[278].tv = 1.0; v[278].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	//triangle join back 1
	v[279].position = D3DXVECTOR3(5.0, -2.5, -7.0); v[279].color = D3DCOLOR_XRGB(255, 255, 255); v[279].tu = 0.8; v[279].tv = 0.0; v[279].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[280].position = D3DXVECTOR3(4.8, -2.5, -7.0); v[280].color = D3DCOLOR_XRGB(255, 255, 255); v[280].tu = 1.0; v[280].tv = 0.0; v[280].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);
	v[281].position = D3DXVECTOR3(5.0, -1.0, -7.0); v[281].color = D3DCOLOR_XRGB(255, 255, 255); v[281].tu = 1.0; v[281].tv = 1.0; v[281].NORMAL = D3DXVECTOR3(0.0, 0.0, 1.0);

	//cockpit ######################################################################################################################
	//triangle top back 1
	v[282].position = D3DXVECTOR3(0.25, 1.9, 0.0); v[282].color = D3DCOLOR_XRGB(255, 255, 255); v[282].tu = 0.0; v[282].tv = 0.0; v[282].NORMAL = D3DXVECTOR3(0.0, -1.0, -1.0);
	v[283].position = D3DXVECTOR3(1.75, 1.9, 0.0); v[283].color = D3DCOLOR_XRGB(255, 255, 255); v[283].tu = 0.0; v[283].tv = 0.5; v[283].NORMAL = D3DXVECTOR3(0.0, -1.0, -1.0);
	v[284].position = D3DXVECTOR3(2.0, 1.0, 1.0); v[284].color = D3DCOLOR_XRGB(255, 255, 255); v[284].tu = 0.3; v[284].tv = 0.5; v[284].NORMAL = D3DXVECTOR3(0.0, -1.0, -1.0);
	//triangle top back 2
	v[285].position = D3DXVECTOR3(0.25, 1.9, 0.0); v[285].color = D3DCOLOR_XRGB(255, 255, 255); v[285].tu = 0.0; v[285].tv = 0.0; v[285].NORMAL = D3DXVECTOR3(0.0, -1.0, -1.0);
	v[286].position = D3DXVECTOR3(0.0, 1.0, 1.0); v[286].color = D3DCOLOR_XRGB(255, 255, 255); v[286].tu = 0.3; v[286].tv = 0.0; v[286].NORMAL = D3DXVECTOR3(0.0, -1.0, -1.0);
	v[287].position = D3DXVECTOR3(2.0, 1.0, 1.0); v[287].color = D3DCOLOR_XRGB(255, 255, 255); v[287].tu = 0.3; v[287].tv = 0.5; v[287].NORMAL = D3DXVECTOR3(0.0, -1.0, -1.0);
	//triangle top front 1
	v[288].position = D3DXVECTOR3(1.75, -2.0, 4.0); v[288].color = D3DCOLOR_XRGB(255, 255, 255); v[288].tu = 0.5; v[288].tv = 0.45; v[288].NORMAL = D3DXVECTOR3(0.0, -1.0, -1.0);
	v[289].position = D3DXVECTOR3(0.0, 1.0, 1.0); v[289].color = D3DCOLOR_XRGB(255, 255, 255); v[289].tu = 1.0; v[289].tv = 0.0; v[289].NORMAL = D3DXVECTOR3(0.0, -1.0, -1.0);
	v[290].position = D3DXVECTOR3(2.0, 1.0, 1.0); v[290].color = D3DCOLOR_XRGB(255, 255, 255); v[290].tu = 1.0; v[290].tv = 0.45; v[290].NORMAL = D3DXVECTOR3(0.0, -1.0, -1.0);
	//triangle top front 2
	v[291].position = D3DXVECTOR3(1.75, -2.0, 4.0); v[291].color = D3DCOLOR_XRGB(255, 255, 255); v[291].tu = 0.5; v[291].tv = 0.45; v[291].NORMAL = D3DXVECTOR3(0.0, -1.0, -1.0);
	v[292].position = D3DXVECTOR3(0.0, 1.0, 1.0); v[292].color = D3DCOLOR_XRGB(255, 255, 255); v[292].tu = 1.0; v[292].tv = 0.0; v[292].NORMAL = D3DXVECTOR3(0.0, -1.0, -1.0);
	v[293].position = D3DXVECTOR3(0.25, -2.0, 4.0); v[293].color = D3DCOLOR_XRGB(255, 255, 255); v[293].tu = 0.5; v[293].tv = 0.0; v[293].NORMAL = D3DXVECTOR3(0.0, -1.0, -1.0);
	//sides
	//triangle right top back 
	v[294].position = D3DXVECTOR3(2.5, -0.25, 0.0); v[294].color = D3DCOLOR_XRGB(255, 255, 255); v[294].tu = 0.6; v[294].tv = 1.0; v[294].NORMAL = D3DXVECTOR3(-1.0, -1.0, 0.2);
	v[295].position = D3DXVECTOR3(1.75, 1.9, 0.0); v[295].color = D3DCOLOR_XRGB(255, 255, 255); v[295].tu = 1.0; v[295].tv = 0.6; v[295].NORMAL = D3DXVECTOR3(-1.0, -1.0, 0.2);
	v[296].position = D3DXVECTOR3(2.0, 1.0, 1.0); v[296].color = D3DCOLOR_XRGB(255, 255, 255); v[296].tu = 1.0; v[296].tv = 1.0; v[296].NORMAL = D3DXVECTOR3(-1.0, -1.0, 0.2);
	//triangle right middle front
	v[297].position = D3DXVECTOR3(2.5, -0.25, 0.0); v[297].color = D3DCOLOR_XRGB(255, 255, 255); v[297].tu = 0.0; v[297].tv = 0.0; v[297].NORMAL = D3DXVECTOR3(-1.0, -1.0, -0.2);
	v[298].position = D3DXVECTOR3(1.75, -2.0, 4.0); v[298].color = D3DCOLOR_XRGB(255, 255, 255); v[298].tu = 1.0; v[298].tv = 1.0; v[298].NORMAL = D3DXVECTOR3(-1.0, -1.0, -0.2);
	v[299].position = D3DXVECTOR3(2.0, 1.0, 1.0); v[299].color = D3DCOLOR_XRGB(255, 255, 255); v[299].tu = 0.0; v[299].tv = 1.0; v[299].NORMAL = D3DXVECTOR3(-1.0, -1.0, -0.2);
	//triangle left top back 
	v[300].position = D3DXVECTOR3(0.25, 1.9, 0.0); v[300].color = D3DCOLOR_XRGB(255, 255, 255); v[300].tu = 1.0; v[300].tv = 0.6; v[300].NORMAL = D3DXVECTOR3(1.0, -1.0, -0.2);
	v[301].position = D3DXVECTOR3(0.0, 1.0, 1.0); v[301].color = D3DCOLOR_XRGB(255, 255, 255); v[301].tu = 1.0; v[301].tv = 1.0; v[301].NORMAL = D3DXVECTOR3(1.0, -1.0, -0.2);
	v[302].position = D3DXVECTOR3(-0.5, -0.25, 0.0); v[302].color = D3DCOLOR_XRGB(255, 255, 255); v[302].tu = 0.6; v[302].tv = 1.0; v[302].NORMAL = D3DXVECTOR3(1.0, -1.0, -0.2);
	//triangle left middle front
	v[303].position = D3DXVECTOR3(0.25, -2.0, 4.0); v[303].color = D3DCOLOR_XRGB(255, 255, 255); v[303].tu = 0.0; v[303].tv = 0.0; v[303].NORMAL = D3DXVECTOR3(1.0, -1.0, -0.2);
	v[304].position = D3DXVECTOR3(0.0, 1.0, 1.0); v[304].color = D3DCOLOR_XRGB(255, 255, 255); v[304].tu = 1.0; v[304].tv = 1.0; v[304].NORMAL = D3DXVECTOR3(1.0, -1.0, -0.2);
	v[305].position = D3DXVECTOR3(-0.5, -0.25, 0.0); v[305].color = D3DCOLOR_XRGB(255, 255, 255); v[305].tu = 0.0; v[305].tv = 1.0; v[305].NORMAL = D3DXVECTOR3(1.0, -1.0, -0.2);
	//underside
	//triangle bottom 1
	v[306].position = D3DXVECTOR3(0.25, -2.0, 4.0); v[306].color = D3DCOLOR_XRGB(255, 255, 255); v[306].tu = 0.0; v[306].tv = 1.0; v[306].NORMAL = D3DXVECTOR3(0.0, 1.0, 0.5);
	v[307].position = D3DXVECTOR3(1.75, -2.0, 4.0); v[307].color = D3DCOLOR_XRGB(255, 255, 255); v[307].tu = 0.5; v[307].tv = 1.0; v[307].NORMAL = D3DXVECTOR3(0.0, 1.0, 0.5);
	v[308].position = D3DXVECTOR3(-0.5, -0.25, 0.0); v[308].color = D3DCOLOR_XRGB(255, 255, 255); v[308].tu = 0.0; v[308].tv = 0.7; v[308].NORMAL = D3DXVECTOR3(0.0, 1.0, 0.5);
	//triangle bottom 2
	v[309].position = D3DXVECTOR3(2.5, -0.25, 0.0); v[309].color = D3DCOLOR_XRGB(255, 255, 255); v[309].tu = 0.5; v[309].tv = 0.7; v[309].NORMAL = D3DXVECTOR3(0.0, 1.0, 0.5);
	v[310].position = D3DXVECTOR3(1.75, -2.0, 4.0); v[310].color = D3DCOLOR_XRGB(255, 255, 255); v[310].tu = 0.5; v[310].tv = 1.0; v[310].NORMAL = D3DXVECTOR3(0.0, 1.0, 0.5);
	v[311].position = D3DXVECTOR3(-0.5, -0.25, 0.0); v[311].color = D3DCOLOR_XRGB(255, 255, 255); v[311].tu = 0.0; v[311].tv = 0.7; v[311].NORMAL = D3DXVECTOR3(0.0, 1.0, 0.5);

g_pVB->Unlock();
return S_OK;
}

 //-----------------------------------------------------------------------------
 // Name: InitLight()
 // Desc: Define the light sources and default material
 //-----------------------------------------------------------------------------
 void InitLight(void)
 {
	D3DLIGHT9 light; //Light object (holds different light models)
	D3DMATERIAL9 material; //Default material (determines how surfaces diffuse light)
	ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_DIRECTIONAL;   //Directional light is of a constant strength with a linear direction
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  //The colour of the diffused light (RGBA 0-1 rather than 0-255)
	light.Direction = D3DXVECTOR3(-1.0f, -0.3f, -1.0f); //The direction of the light
	
	g_pd3dDevice->SetLight(0, &light); //Set the described light to index 0 (reference address of the light object)
	g_pd3dDevice->LightEnable(0, TRUE); //Turn the light on

	light.Type = D3DLIGHT_POINT;  //Point lights emit in all directions from one point, the light has a range and fades over distance
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); //The colour of the diffused light (RGBA 0-1 rather than 0-255)
	light.Position = D3DXVECTOR3(5.0f, -5.0f, 5.0f); //The Vector3 position of the light source
	light.Range = 200.0f; //The range of the light
	light.Attenuation0 = 0.0f;  //Linear attenuation (light stays at this constant regardless of range)
	light.Attenuation1 = 0.125f;  //Inverse attenuation (light dissipates more slowly as range increases)
	light.Attenuation2 = 0.0f;    //Square Inverse attenuation (very quick fading at low range, slowing as range increases)

	g_pd3dDevice->SetLight(1, &light); //Set the described light to index 1 (reference address of the light object)
	g_pd3dDevice->LightEnable(1, FALSE); //Set this light to be off initially

	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); //The colour of the diffused light that will relect off the surface
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  //The colour of the diffused ambient light that will relect of the surface
	g_pd3dDevice->SetMaterial(&material); //Set this material as the default 
  }

 //-----------------------------------------------------------------------------
 // Name: Cleanup()
 // Desc: Handle memory management within the message loop
 //-----------------------------------------------------------------------------
void Cleanup(void)
{
	if(g_pTexture != NULL )  g_pTexture->Release();
	if(g_pVB != NULL )       g_pVB->Release();
	if(g_pd3dDevice!=NULL)   g_pd3dDevice->Release();
	if(g_pD3D != NULL )      g_pD3D->Release();
}

//-----------------------------------------------------------------------------
// Name: SetupMatrices()
// Desc: Define the world, view and projection matricies
//-----------------------------------------------------------------------------
void SetupMatrices()
{
	// Set up world matrix
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixRotationYawPitchRoll(&matWorld,xrot, yrot, 0);
	g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we allow the
	// the user to zoom on the z-axis and look up three units, look at the
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt( 0.0f, 3.0f, zoom);
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewpoint space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj,D3DX_PI/4,1.0f,1.0f,100.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Main rendering loop, is called once per frame. Handles user input,
//		 and rendering the image to the screen
//-----------------------------------------------------------------------------
void Render()
{
	// Handle user input
	if (KEY_DOWN(VK_F4)) { g_pd3dDevice->LightEnable(0, TRUE);  g_pd3dDevice->LightEnable(1, FALSE); } //Enable Directional Light, Disable Point Light
	if (KEY_DOWN(VK_F3)) { g_pd3dDevice->LightEnable(0, FALSE);  g_pd3dDevice->LightEnable(1, TRUE); } //Enable Point Light, Disable Directional Light
	if (KEY_DOWN(VK_F2)) g_pd3dDevice->SetRenderState(D3DRS_FILLMODE,0); //Textured Fill
	if (KEY_DOWN(VK_F1)) g_pd3dDevice->SetRenderState(D3DRS_FILLMODE,2); //Wireframe

	if (KEY_DOWN(VK_LEFT))   xrot = xrot + 0.05; //Rotate the camera left
	if (KEY_DOWN(VK_RIGHT))  xrot = xrot - 0.05; //Rotate the camera right
	if (KEY_DOWN(VK_UP))     yrot = yrot + 0.05; //Rotate the camera up
	if (KEY_DOWN(VK_DOWN))   yrot = yrot - 0.05; //Rotate the camera down

	if (KEY_DOWN(VK_SHIFT) && zoom < -5) zoom += 1;		//Zoom in closer to the object
	if (KEY_DOWN(VK_CONTROL) && zoom > -80) zoom -= 1;	//Zoom away from the object
  

	//set background colour  
	g_pd3dDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,0),1.0f,0);

	//Begin the scene
	if (SUCCEEDED(g_pd3dDevice->BeginScene())) {

		SetupMatrices();//Setup the world, view, and projection matrices
		// Setup our texture. Using textures introduces the texture stage states,
		// which govern how textures get blended together (in the case of multiple
		// textures) and lighting information. In this case, we are modulating
		// (blending) our texture with the diffuse color of the vertices.
		g_pd3dDevice->SetTexture( 0, g_pTexture );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
		// Render the vertex buffer contents
		g_pd3dDevice->SetStreamSource(0,g_pVB,0,sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0,104);
		// End the scene
		g_pd3dDevice->EndScene();
	}

	// Present the backbuffer contents to the display
	g_pd3dDevice->Present(NULL,NULL,NULL,NULL);
}

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: Windows Message Handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg) {
		case WM_DESTROY:
		Cleanup();
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

//-----------------------------------------------------------------------------
// Name: wWinMain()
// Desc: Creates the window and initialises the enviroment, lighting and geometry.
//-----------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT ) {

	// Register the window class
	WNDCLASSEX wc =
	{
		sizeof( WNDCLASSEX ), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle( NULL ), NULL, NULL, NULL, NULL,
		L"Star Wars Lambda Shuttle", NULL
	};
	RegisterClassEx( &wc );

	// Create the application's window to the given screen width and height variables
	HWND hWnd=CreateWindow(L"Star Wars Lambda Shuttle",L"3D Assignment 2 - Star Wars Lambda Shuttle",WS_OVERLAPPEDWINDOW,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,NULL,NULL,wc.hInstance,NULL);

	// Initialize Direct3D
	if(SUCCEEDED(InitD3D(hWnd))) {

		// Create the lighting
		InitLight();

		// Create the scene geometry
		if(SUCCEEDED( InitGeometry())) {

			// Show the window
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);

			// Enter the message loop
			MSG msg;
			ZeroMemory(&msg, sizeof(msg));
			
			while (msg.message != WM_QUIT) {

				if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {

					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
					Render();		
				}
			}
		}
	UnregisterClass( L"Star Wars Lambda Shuttle", wc.hInstance );
	return 0;
}



