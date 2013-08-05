#include "ASCDX9Common.h"

CASCDirect3D9Mode		G_D3D9Mode;
D3DDISPLAYMODEEX		G_D3D9DisplayMode	= {sizeof(G_D3D9DisplayMode)};
D3DPRESENT_PARAMETERS	G_D3D9PresentParams	= {0};
IDirect3D9*				G_pD3D9;
IDirect3DDevice9*		G_pD3D9Device;
D3DCAPS9				G_D3DCaps9			= {0};
ASCInt					G_nDX9ActiveDepthStencilLevel;