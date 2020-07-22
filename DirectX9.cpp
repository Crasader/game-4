#include	"stdafx.h"
#include	"DirectX9.h"

#include <mmsystem.h>

namespace ProjectH
{

using namespace std;
using namespace stdext;

typedef hash_map< int, LPDIRECT3DTEXTURE9 >	HASHMAP;
typedef pair	< int, LPDIRECT3DTEXTURE9 >	HASHMAP_PAIR;

DirectX9*	DirectX9::Instance()
{
	static DirectX9 singleton;

	return &singleton;
}

DirectX9::DirectX9()
{
	d3d			=	NULL;
	d3dDevice	=	NULL;
	sprite		=	NULL;
	vb = NULL;

	backBufferWidth = 0;
	backBufferHeight = 0;
}

DirectX9::~DirectX9()
{
	Release();
}

bool DirectX9::Init( HWND _hWnd, int screenWidth, int screenHeight, int miniWidth, int miniHeight, float r )
{
	backBufferWidth = screenWidth;
	backBufferHeight = screenHeight;

	ratio = r;

	miniMapRect.left = 0;
	miniMapRect.top = 0;
	miniMapRect.right = miniWidth;
	miniMapRect.bottom = miniHeight;

	// Driect3D ������Ʈ�� �ۼ�
    if ( FAILED( d3d = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return false;

	D3DDISPLAYMODE mode;
    if( FAILED( d3d->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &mode ) ) )
        return false;

	D3DCAPS9 d3dCaps;
	d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps);

	// D3DPRESENT_PARAMETERS ����ü �ʱ�ȭ
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
	
	d3dpp.Windowed					= true;
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil	= false;
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;
	d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.BackBufferCount			= 1;
	d3dpp.BackBufferWidth			= screenWidth;
	d3dpp.BackBufferHeight			= screenHeight;
	d3dpp.BackBufferFormat			= D3DFMT_X8R8G8B8;

	HRESULT	hr	=	d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, _hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3dDevice );

	d3dDevice->CreateAdditionalSwapChain(&d3dpp, &chain1);

	d3dpp.BackBufferWidth			= miniMapWidth;
	d3dpp.BackBufferHeight			= miniMapHeight;

	d3dDevice->GetSwapChain(0, &chain2);

	d3dpp.BackBufferWidth			= screenWidth;
	d3dpp.BackBufferHeight			= screenHeight;

	d3dDevice->CreateAdditionalSwapChain(&d3dpp, &chain3);

	// �̹��� ���ҽ� �ε�
	Load();

	return true;
}

void DirectX9::Release()
{
	if( sprite )
		sprite->Release();

    if( d3dDevice )
        d3dDevice->Release();

    if( d3d )
        d3d->Release();

	d3d			=	NULL;
	d3dDevice	=	NULL;
	sprite		=	NULL;

	backBufferWidth = 0;
	backBufferHeight = 0;
}

void DirectX9::Load()
{
	LoadTexture( 0,	512,332,0.0f, 0.0f, 1.0f, TEXT( "Image/Grass.bmp" ) );
	LoadTexture( 1,	512,332,0.0f, 0.0f, 1.0f, TEXT( "Image/Ston.bmp" ) );
	LoadTexture( 2,	512,332,0.0f, 0.0f, 1.0f, TEXT( "Image/Castle.bmp" ) );

	LoadTexture( 101,	64,288, 0.0f, 0.0f, 1.0f, TEXT( "image/StartPoint.bmp" ) );
	LoadTexture( 102,	64,	32,	1.0f, 0.0f, 0.0f, TEXT( "Image/TileSelectTrue.bmp" ) );
	LoadTexture( 103,	64,	32,	0.0f, 0.0f, 1.0f, TEXT( "Image/TileSelectFalse.bmp" ) );
	LoadTexture( 104,	200,100,1.0f, 0.0f, 1.0f, TEXT( "Image/Monster.bmp" ) );

	D3DXCreateSprite( d3dDevice, &sprite );		// �ε�� �ؽ�ó ����

}

void DirectX9::LoadTexture( int key, int _imgWidth, int _imgHeight, float _r, float _g, float _b, TCHAR* _fileAdr )
{
	D3DXIMAGE_INFO		d3dxImageInfo;
	LPDIRECT3DTEXTURE9	pTexture;

	HRESULT	hr	=	D3DXCreateTextureFromFileEx( d3dDevice,
												 _fileAdr,
												 _imgWidth,
												 _imgHeight,
												 1,
												 D3DPOOL_DEFAULT,
												 D3DFMT_UNKNOWN,
												 D3DPOOL_DEFAULT,
												 D3DX_DEFAULT,
												 D3DX_DEFAULT,
												 D3DCOLOR_COLORVALUE( _r, _g, _b, 1.0f ),
												 &d3dxImageInfo,
												 NULL,
												 &pTexture );						

	m_arrTex.insert(  HASHMAP_PAIR( key, pTexture ) );
}

void DirectX9::Draw( int _nTex,  int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, float spriteIndex /* = 0 */,
					int _flag /* = 0 */, float _r /* = 1.0f */, float _g /* = 1.0f */, float _b /* = 1.0f */, float _a /* = 1.0f */ )
{
	D3DXMATRIX	mat, mat_old;										// ������ �� �ʿ��Ѱ�.. -_-

	D3DXPLANE	plane		(	0.0f,			0.0f,			0.0f,		0.0f);	// ���� �¿� ����
	D3DXVECTOR3	vCenter		(	0.0f,			0.0f,			0.0f );				// �̹��� ������ (����)
	D3DXVECTOR3 vPosition	(	(float) xSrc,	(float) ySrc,	spriteIndex );		// ȭ�� ��ǥ

	if ( _flag == SCAILE )
	{
		D3DXMatrixIdentity( &mat );
		D3DXMatrixScaling( &mat, ratio, ratio, 0.0f );
		sprite->GetTransform( &mat_old );
		sprite->SetTransform( &mat );
	}
	// �¿� ������ ��
	else if ( _flag == LR_REVERSE )
	{
		plane.a	=	1.0f;									// ������ x ���� 1.0f �� �־��ش�... -_-
		D3DXMatrixReflect( &mat, &plane );					// ������ ������ ��İ���� �ؼ� �����´�... -_-

		sprite->GetTransform( &mat_old );					// ���� ��Ʈ���� ��İ��� �����Ѵ�.
		sprite->SetTransform( &mat );						// ���ο� ��Ʈ���� ��İ��� �Է��Ѵ�.

		vCenter.x	=	(FLOAT) ( nDestWidth - xDest );		// ��������Ʈ�� ������Ű�� ������ǥ�� �ٲ�� ������ �̹����� ���̸�ŭ ������ǥ�� �缳���Ѵ�.
		vPosition.x	*=	-1;									// - �� �ٲ� ���� + �� ����� ���� -1 �� �����ش�.
	}
	// ���� ������ ��
	else if ( _flag == TB_REVERSE )
	{
		plane.b	=	1.0f;									// ������ x ���� 1.0f �� �־��ش�... -_-
		D3DXMatrixReflect( &mat, &plane );					// ������ ������ ��İ���� �ؼ� �����´�... -_-

		sprite->GetTransform( &mat_old );					// ���� ��Ʈ���� ��İ��� �����Ѵ�.
		sprite->SetTransform( &mat );						// ���ο� ��Ʈ���� ��İ��� �Է��Ѵ�.
		
		vCenter.y	=	(FLOAT) ( nDestHeight - yDest );	// ��������Ʈ�� ������Ű�� ������ǥ�� �ٲ�� ������ �̹����� ���̸�ŭ ������ǥ�� �缳���Ѵ�.
		vPosition.y	*=	-1;									// - �� �ٲ� ���� + �� ����� ���� -1 �� �����ش�.
	}
	
	RECT destRect = { xDest, yDest, nDestWidth, nDestHeight };

	// �׸���	
	sprite->Draw(	m_arrTex[ _nTex ],							// �ؽ�ó ������
					&destRect,									// �ؽ�ó �̹��� ��ǥ
					NULL,										// �̹��� ������ (ȸ��)
					&vPosition,									// ������ ����� ��ǥ
					D3DCOLOR_COLORVALUE( _r, _g, _b, _a ) );	// �̹����� �÷� ��������

	if ( _flag != 0 )
		sprite->SetTransform( &mat_old );
}

void DirectX9::BeginDraw()
{
	LPDIRECT3DSURFACE9 pBackBuffer = NULL;
	chain1->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	d3dDevice->SetRenderTarget(0, pBackBuffer);
	d3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
	d3dDevice->BeginScene();
	sprite->Begin( D3DXSPRITE_ALPHABLEND );
}

void DirectX9::EndDraw( RECT* drawRt, HWND hWnd )
{
	sprite->End();
	d3dDevice->EndScene();
	chain1->Present(drawRt, drawRt, hWnd, NULL, 0);
}

void DirectX9::BeginDraw2()
{
	LPDIRECT3DSURFACE9 pBackBuffer = NULL;
	chain2->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &back);
	d3dDevice->SetRenderTarget(0, back);
	d3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
	d3dDevice->BeginScene();
	sprite->Begin( D3DXSPRITE_ALPHABLEND );
}

void DirectX9::EndDraw2( RECT* rectangle, HWND hWnd )
{
	sprite->End();
	d3dDevice->EndScene();

	// �׺���̼� �� ���翵�� �簢�� ��ġ ���
	CUSTOMVERTEX Vertices[] =
	{
		{ (float)rectangle->left,  (float)rectangle->top, 0.0f, 1.0f, 0xffffffff, }, // x, y, z, rhw, color
		{ (float)rectangle->right-2, (float)rectangle->top, 0.0f, 1.0f, 0xffffffff, },
		{ (float)rectangle->right-2, (float)rectangle->bottom-2, 0.0f, 1.0f, 0xffffffff, },
		{ (float)rectangle->left, (float)rectangle->bottom-2, 0.0f, 1.0f, 0xffffffff, },
		{ (float)rectangle->left,  (float)rectangle->top, 0.0f, 1.0f, 0xffffffff, }
	};

	d3dDevice->CreateVertexBuffer( 5 * sizeof( CUSTOMVERTEX ), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &vb, NULL );

	VOID* pVertices;
	vb->Lock( 0, sizeof( Vertices ), ( void** )&pVertices, 0 );
	memcpy( pVertices, Vertices, sizeof( Vertices ) );
	vb->Unlock();

	// �׺���̼� �� ���翵�� �簢�� �׵θ� ���
	d3dDevice->BeginScene();
		d3dDevice->SetStreamSource( 0, vb, 0, sizeof( CUSTOMVERTEX ) );
		d3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
		d3dDevice->DrawPrimitive( D3DPT_LINESTRIP, 0, 4 );
	d3dDevice->EndScene();
	d3dDevice->Present( &miniMapRect, &miniMapRect, hWnd, NULL );
	vb->Release();
}

void DirectX9::BeginDraw3()
{
	LPDIRECT3DSURFACE9 pBackBuffer = NULL;
	chain3->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	d3dDevice->SetRenderTarget(0, pBackBuffer);
	d3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
	d3dDevice->BeginScene();
	sprite->Begin( D3DXSPRITE_ALPHABLEND );
}

void DirectX9::EndDraw3( RECT* drawRt, RECT* drawRt2, HWND hWnd )
{
	sprite->End();
	d3dDevice->EndScene();
	chain3->Present(drawRt, drawRt2, hWnd, NULL, 0);
}

void DirectX9::Reset( int _width, int _height )
{
	if( d3dDevice )
	{
		backBufferWidth = ( _width ? _width : backBufferWidth );
		backBufferHeight = ( _height ? _height : backBufferHeight );

		// �ؽ�ó �ʱ�ȭ
		HASHMAP::iterator iter;
		for( iter = m_arrTex.begin(); iter != m_arrTex.end(); ++iter )
		{
			(iter->second)->Release();
			(iter->second) = NULL;
		}
		m_arrTex.clear();

		sprite->Release();
		sprite = NULL;

		// D3DPRESENT_PARAMETERS ����ü �ʱ�ȭ
		D3DPRESENT_PARAMETERS	d3dpp;
		ZeroMemory( &d3dpp, sizeof(d3dpp) );
		
		//d3dpp.Windowed					= true;
		//d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
		//d3dpp.BackBufferWidth			= backBufferWidth;
		//d3dpp.BackBufferHeight			= backBufferHeight;

		d3dpp.Windowed					= true;
		d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
		d3dpp.EnableAutoDepthStencil	= false;
		d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;
		d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;
		d3dpp.BackBufferCount			= 1;     //����� ���� ���� 
		d3dpp.BackBufferWidth			= backBufferWidth;
		d3dpp.BackBufferHeight			= backBufferHeight;
		d3dpp.BackBufferFormat			= D3DFMT_UNKNOWN;  //����ȭ�� �������� �ĸ���� ����
		d3dpp.Flags						= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

		// D3D����̽� ����
		d3dDevice->Reset( &d3dpp );

		// �̹��� ���ҽ� �ε�
		Load();
	}
}

} /* ProjectH NameSpace */
