#include <windows.h>
#include <stdio.h>
#include <GL/gl.h>

#include "ivec2.h"
#include "vec2.h"
#include "vec3.h"

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"winmm.lib")

unsigned int (*glCreateProgram)();
unsigned int (*glCreateShader)(unsigned int shader);
unsigned int (*wglSwapIntervalEXT)(unsigned int satus);

int (*glGetUniformLocation)(unsigned int program,const char *name);

void (*glShaderSource)(unsigned int shader,int count,const char **string,int *length);
void (*glCompileShader)(unsigned int shader);
void (*glAttachShader)(unsigned int program,unsigned int shader);
void (*glLinkProgram)(unsigned int program);
void (*glUseProgram)(unsigned int program);
void (*glEnableVertexAttribArray)(unsigned int index);
void (*glVertexAttribPointer)(unsigned int index,int size,unsigned int type,unsigned char normalized,unsigned int stride,const void *pointer);
void (*glBufferData)(unsigned int target,unsigned int size,const void *data,unsigned int usage);
void (*glCreateBuffers)(unsigned int n,unsigned int *buffers);
void (*glBindBuffer)(unsigned int target,unsigned int buffer);
void (*glGetShaderInfoLog)(unsigned int shader,unsigned int maxlength,unsigned int *length,unsigned char *infolog);
void (*glActiveTexture)(unsigned int texture);
void (*glGenerateMipmap)(unsigned int target);
void (*glUniform1f)(int loc,float v1);
void (*glUniform2f)(int loc,float v1,float v2);
void (*glUniform3f)(int loc,float v1,float v2,float v3);
void (*glUniform2i)(int loc,int v1,int v2);
void (*glUniform1i)(int loc,int v1);

#define PI_2   6.28318530718f
#define PI     3.14159265358f
#define PI_05  1.57079632679f
#define PI_025 0.78539816339f

#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_ARRAY_BUFFER 0x8892
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE3 0x84C3
#define GL_TEXTURE4 0x84C4
#define GL_TEXTURE5 0x84C5
#define GL_TEXTURE6 0x84C6
#define GL_TEXTURE7 0x84C7
#define GL_TEXTURE8 0x84C8

#pragma pack(push,2)
typedef struct{
	unsigned short headerField;
	unsigned int size;
	unsigned short reserved1;
	unsigned short reserved2;
	unsigned int startPointer;
	unsigned int headerSize;
	unsigned int width;
	unsigned int height;
	unsigned short cplanes;
	unsigned short bpp;
	unsigned int compression;
	unsigned int imageSize;
	unsigned int hres;
	unsigned int vres;
	unsigned int cpallete;
	unsigned int importantc;
}BMP;
#pragma pack(pop)

typedef struct{
	unsigned char r;
	unsigned char g;
	unsigned char b;
}RGB;

typedef struct{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
}RGBA;

typedef struct{
	unsigned int c;
	unsigned int id[256];
	IVEC2 pos[256];
}OPENGLMES;

typedef struct{
	VEC2 pos;
	float zoom;
}CAMERA;

typedef struct{
	IVEC2 pos;
	IVEC2 size;
	unsigned int id;
}HITBOX;

unsigned char hitboxC;
HITBOX hitbox[128];

POINT cursor;

IVEC2 canvasSize;

CAMERA camera = {0.0f,0.0f,100.0f};

OPENGLMES openglmes;

unsigned int canvasText;
unsigned int fontText;

unsigned int shaderProgram;

unsigned int vertexShader;
unsigned int fragmentShader;

unsigned int VBO;

unsigned char toolSize;	
unsigned char toolSel;

char *VERTsource;
char *FRAGsource;

int *heap;
HWND window;
HDC dc;
MSG Msg;

RGBA *canvas;
RGBA *canvasb;
RGBA colorSel;

float *quad; 
unsigned char *font;

unsigned int quadPointer = 0;

VEC2 pointSel;
VEC2 pointSel2;

void quadAddEx2(VEC2 pos,VEC2 size,VEC3 data,float id){
	quad[quadPointer + 0 ] = pos.x + size.x;
	quad[quadPointer + 1 ] = pos.y;
	quad[quadPointer + 2 ] = 1.0;
	quad[quadPointer + 3 ] = 0.0;
	quad[quadPointer + 4 ] = id;
	quad[quadPointer + 5 ] = data.x;
	quad[quadPointer + 6 ] = data.y;
	quad[quadPointer + 7 ] = data.z;
	quad[quadPointer + 8 ] = pos.x;
	quad[quadPointer + 9 ] = pos.y;
	quad[quadPointer + 10] = 0.0;
	quad[quadPointer + 11] = 0.0;
	quad[quadPointer + 12] = id;
	quad[quadPointer + 13] = data.x;
	quad[quadPointer + 14] = data.y;
	quad[quadPointer + 15] = data.z;
	quad[quadPointer + 16] = pos.x + size.x;
	quad[quadPointer + 17] = pos.y + size.y;
	quad[quadPointer + 18] = 1.0;
	quad[quadPointer + 19] = 1.0;
	quad[quadPointer + 20] = id;
	quad[quadPointer + 21] = data.x;
	quad[quadPointer + 22] = data.y;
	quad[quadPointer + 23] = data.z;
	quad[quadPointer + 24] = pos.x;
	quad[quadPointer + 25] = pos.y + size.y;
	quad[quadPointer + 26] = 0.0;
	quad[quadPointer + 27] = 1.0;
	quad[quadPointer + 28] = id;
	quad[quadPointer + 29] = data.x;
	quad[quadPointer + 30] = data.y;
	quad[quadPointer + 31] = data.z;
	quad[quadPointer + 32] = pos.x;
	quad[quadPointer + 33] = pos.y;
	quad[quadPointer + 34] = 0.0;
	quad[quadPointer + 35] = 0.0;
	quad[quadPointer + 36] = id;
	quad[quadPointer + 37] = data.x;
	quad[quadPointer + 38] = data.y;
	quad[quadPointer + 39] = data.z;
	quad[quadPointer + 40] = pos.x + size.x;
	quad[quadPointer + 41] = pos.y + size.y;
	quad[quadPointer + 42] = 1.0;
	quad[quadPointer + 43] = 1.0;
	quad[quadPointer + 44] = id;
	quad[quadPointer + 45] = data.x;
	quad[quadPointer + 46] = data.y;
	quad[quadPointer + 47] = data.z;
	quadPointer+=48;
	glBufferData(GL_ARRAY_BUFFER,(quadPointer) * sizeof(float),quad,GL_DYNAMIC_DRAW);
}

void quadAddEx(VEC2 pos,VEC2 size,VEC2 tpos,VEC2 tsize,float id){
	quad[quadPointer + 0 ] = pos.x + size.x;
	quad[quadPointer + 1 ] = pos.y;
	quad[quadPointer + 2 ] = tpos.x + tsize.x;
	quad[quadPointer + 3 ] = tpos.y;
	quad[quadPointer + 4 ] = id;
	quad[quadPointer + 8 ] = pos.x;
	quad[quadPointer + 9 ] = pos.y;
	quad[quadPointer + 10] = tpos.x;
	quad[quadPointer + 11] = tpos.y;
	quad[quadPointer + 12] = id;
	quad[quadPointer + 16] = pos.x + size.x;
	quad[quadPointer + 17] = pos.y + size.y;
	quad[quadPointer + 18] = tpos.x + tsize.x;
	quad[quadPointer + 19] = tpos.y + tsize.y;
	quad[quadPointer + 20] = id;
	quad[quadPointer + 24] = pos.x;
	quad[quadPointer + 25] = pos.y + size.y;
	quad[quadPointer + 26] = tpos.x;
	quad[quadPointer + 27] = tpos.y + tsize.y;
	quad[quadPointer + 28] = id;
	quad[quadPointer + 32] = pos.x;
	quad[quadPointer + 33] = pos.y;
	quad[quadPointer + 34] = tpos.x;
	quad[quadPointer + 35] = tpos.y;
	quad[quadPointer + 36] = id;
	quad[quadPointer + 40] = pos.x + size.x;
	quad[quadPointer + 41] = pos.y + size.y;
	quad[quadPointer + 42] = tpos.x + tsize.x;
	quad[quadPointer + 43] = tpos.y + tsize.y;
	quad[quadPointer + 44] = id;
	quadPointer+=48;
	glBufferData(GL_ARRAY_BUFFER,(quadPointer) * sizeof(float),quad,GL_DYNAMIC_DRAW);
}

void quadAdd(VEC2 pos,VEC2 size,float id){
	quad[quadPointer + 0 ] = pos.x + size.x;
	quad[quadPointer + 1 ] = pos.y;
	quad[quadPointer + 2 ] = 1.0;
	quad[quadPointer + 3 ] = 0.0;
	quad[quadPointer + 4 ] = id;
	quad[quadPointer + 8 ] = pos.x;
	quad[quadPointer + 9 ] = pos.y;
	quad[quadPointer + 10] = 0.0;
	quad[quadPointer + 11] = 0.0;
	quad[quadPointer + 12] = id;
	quad[quadPointer + 16] = pos.x + size.x;
	quad[quadPointer + 17] = pos.y + size.y;
	quad[quadPointer + 18] = 1.0;
	quad[quadPointer + 19] = 1.0;
	quad[quadPointer + 20] = id;
	quad[quadPointer + 24] = pos.x;
	quad[quadPointer + 25] = pos.y + size.y;
	quad[quadPointer + 26] = 0.0;
	quad[quadPointer + 27] = 1.0;
	quad[quadPointer + 28] = id;
	quad[quadPointer + 32] = pos.x;
	quad[quadPointer + 33] = pos.y;
	quad[quadPointer + 34] = 0.0;
	quad[quadPointer + 35] = 0.0;
	quad[quadPointer + 36] = id;
	quad[quadPointer + 40] = pos.x + size.x;
	quad[quadPointer + 41] = pos.y + size.y;
	quad[quadPointer + 42] = 1.0;
	quad[quadPointer + 43] = 1.0;
	quad[quadPointer + 44] = id;
	quadPointer+=48;
	glBufferData(GL_ARRAY_BUFFER,quadPointer * sizeof(float),quad,GL_DYNAMIC_DRAW);
}

inline IVEC2 getPosp(){
	IVEC2 posp = {(unsigned int)((float)cursor.y/1080.0f*camera.zoom+camera.pos.y),(unsigned int)((float)cursor.x/1080.0f*camera.zoom+camera.pos.x)};
	return posp;
}

void bucket(RGBA color,volatile IVEC2 pos){
	if(pos.x < 0 || pos.x > canvasSize.x || pos.y < 0 || pos.y > canvasSize.y){
		return;
	}
	canvas[pos.x*canvasSize.x+pos.y] = colorSel;
	pos.x++;
	if(canvas[pos.x*canvasSize.x+pos.y].r == color.r && canvas[pos.x*canvasSize.x+pos.y].g == color.g && canvas[pos.x*canvasSize.x+pos.y].b == color.b){
		bucket(color,pos);
	}
	pos.x-=2;
	if(canvas[pos.x*canvasSize.x+pos.y].r == color.r && canvas[pos.x*canvasSize.x+pos.y].g == color.g && canvas[pos.x*canvasSize.x+pos.y].b == color.b){
		bucket(color,pos);
	}
	pos.x++;
	pos.y++;
	if(canvas[pos.x*canvasSize.x+pos.y].r == color.r && canvas[pos.x*canvasSize.x+pos.y].g == color.g && canvas[pos.x*canvasSize.x+pos.y].b == color.b){
		bucket(color,pos);	
	}
	pos.y-=2;
	if(canvas[pos.x*canvasSize.x+pos.y].r == color.r && canvas[pos.x*canvasSize.x+pos.y].g == color.g && canvas[pos.x*canvasSize.x+pos.y].b == color.b){
		bucket(color,pos);
	}
}

long _stdcall proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
	case WM_QUIT:
	case WM_CLOSE:
	case WM_DESTROY:
		ExitProcess(0);
	case WM_KEYDOWN:
		switch(wParam){
		case 0x56:
			if(GetKeyState(VK_LCONTROL)&0x80){
				for(int i = 0;i < canvasSize.x*canvasSize.y;i++){
					canvas[i].r ^= canvas[i].b;
					canvas[i].b ^= canvas[i].r;
					canvas[i].r ^= canvas[i].b;
					canvas[i].a = 255;
				}
				BITMAPINFO bmp;
				bmp.bmiHeader.biClrImportant = 0;
				bmp.bmiHeader.biBitCount = 32;
				bmp.bmiHeader.biSize   = canvasSize.x*canvasSize.y*4+sizeof(BITMAPINFO);
				bmp.bmiHeader.biWidth  = canvasSize.x;
				bmp.bmiHeader.biHeight = canvasSize.y;
				bmp.bmiHeader.biCompression = 0;
				bmp.bmiHeader.biPlanes = 1;
				bmp.bmiHeader.biSizeImage = 0;
				bmp.bmiHeader.biClrUsed = 0;
				HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE,canvasSize.x*canvasSize.y*sizeof(RGBA)+sizeof(BITMAPINFO));
				char *p = GlobalLock(hMem);
				memcpy(p,&bmp,sizeof(BITMAPINFO));
				memcpy(p+sizeof(BITMAPINFO),canvas,canvasSize.x*canvasSize.y*sizeof(RGBA));
				GlobalUnlock(hMem);
				OpenClipboard(window);
				EmptyClipboard();
				SetClipboardData(CF_DIB,hMem);
				CloseClipboard();
				for(int i = 0;i < canvasSize.x*canvasSize.y;i++){
					canvas[i].r ^= canvas[i].b;
					canvas[i].b ^= canvas[i].r;
					canvas[i].r ^= canvas[i].b;
				}
			}
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		for(int i = 0;i < hitboxC;i++){
			if(cursor.x > hitbox[i].pos.x && cursor.x < hitbox[i].pos.x + hitbox[i].size.x && cursor.y > hitbox[i].pos.y && cursor.y < hitbox[i].pos.y + hitbox[i].size.y){
				switch(hitbox[i].id){
				case 0:{
					for(int i = 0;i < canvasSize.x*canvasSize.y;i++){
						canvas[i].r ^= canvas[i].b;
						canvas[i].b ^= canvas[i].r;
						canvas[i].r ^= canvas[i].b;
						canvas[i].a = 255;
					}
					HANDLE h = CreateFileA("images/temp.bmp",GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
					BMP bmp;
					ZeroMemory(&bmp,sizeof(BMP));
					bmp.headerField  = 0x4d42;
					bmp.size         = canvasSize.x*canvasSize.y*3+50;
					bmp.startPointer = 54;
					bmp.headerSize   = 40;
					bmp.width        = canvasSize.x;
					bmp.height       = canvasSize.y;
					bmp.bpp          = 32;
					bmp.cplanes      = 1;
					WriteFile(h,&bmp,sizeof(BMP),0,0);
					WriteFile(h,canvas,canvasSize.x*canvasSize.y*sizeof(RGBA)+50,0,0);
					CloseHandle(h);
					ExitProcess(0);
					}
				case 1:
					break;
				case 2:
					toolSel = 0;
					openglmes.id[openglmes.c] = 2;
					openglmes.c++;
					break;
				case 3:
					toolSel = 1;
					openglmes.id[openglmes.c] = 2;
					openglmes.c++;
					break;
				case 4:
					ZeroMemory(canvas,canvasSize.x*canvasSize.y*sizeof(RGBA));
					openglmes.id[openglmes.c] = 3;
					openglmes.c++;
					break;
				case 5:
					toolSel = 2;
					openglmes.id[openglmes.c] = 2;
					openglmes.c++;
					break;
				case 6:
					toolSel = 3;
					openglmes.id[openglmes.c] = 2;
					openglmes.c++;
					break;
				}
				break;
			}
		}
		if(cursor.x<1080){
			switch(toolSel){
			case 3:
			case 0:{
				IVEC2 posp = getPosp();
				pointSel.x = (float)posp.x;
				pointSel.y = (float)posp.y;
				memcpy(canvasb,canvas,canvasSize.x*canvasSize.y*sizeof(RGBA));
				break;
				}
			case 2:{
				IVEC2 posp = getPosp();
				if(colorSel.r == canvas[posp.x*canvasSize.x+posp.y].r && colorSel.g == canvas[posp.x*canvasSize.x+posp.y].g && colorSel.b == canvas[posp.x*canvasSize.x+posp.y].b){
					break;
				}
				bucket(canvas[posp.x*canvasSize.x+posp.y],getPosp());
				openglmes.id[openglmes.c] = 3;
				openglmes.c++;
				break;
				}
			}
		}
		break;
	case WM_MBUTTONDOWN:{
		if(cursor.x<1080){
			IVEC2 posp = {(unsigned int)((float)cursor.x/1080.0f*camera.zoom+camera.pos.x),(unsigned int)((float)cursor.y/1080.0f*camera.zoom+camera.pos.y)};
			colorSel = canvas[posp.y*canvasSize.x+posp.x];
			openglmes.id[openglmes.c] = 5;
			openglmes.c++;
		}
		break;
		}
	case WM_MOUSEMOVE:{
		if(wParam&0x0002){
			POINT bcursor;
			GetCursorPos(&bcursor);
			bcursor.y = 1080 - bcursor.y;
			VEC2 rp = {bcursor.x-cursor.x,bcursor.y-cursor.y};
			VEC2div(&rp,1080.0f/camera.zoom);
			VEC2subVEC2(&camera.pos,rp);
			cursor = bcursor;
			openglmes.id[openglmes.c] = 1;
			openglmes.c++;
		}
		else{
			GetCursorPos(&cursor);
			cursor.y = 1080 - cursor.y;
		}
		for(int i = 4;i < quadPointer;i+=48){
			if(quad[i]==5.0f){
				i-=4;
				for(int i2 = i;i2 < quadPointer;i2+=48){
					for(int i3 = 0;i3 < 48;i3++){
						quad[i2+i3] = quad[i2+i3+48];
					}
				}
				break;
			}
		}
		openglmes.id[openglmes.c] = 4;
		openglmes.c++;
		break;
		}
	case WM_MOUSEWHEEL:{
		float z = (float)GET_WHEEL_DELTA_WPARAM(wParam)/1200.0f;
		camera.pos = VEC2addR(camera.pos,z*camera.zoom*0.5f);
		camera.zoom -= z*camera.zoom;
		VEC2 v = {(float)cursor.x,(float)cursor.y};
		openglmes.id[openglmes.c] = 1;
		openglmes.c++;
		break;
		}
	}
	return DefWindowProcA(hwnd,msg,wParam,lParam);
}

WNDCLASS wndclass = {0,proc,0,0,0,0,0,0,"photoshop 2","photoshop 2"};

PIXELFORMATDESCRIPTOR pfd = {sizeof(PIXELFORMATDESCRIPTOR), 1,
PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,PFD_TYPE_RGBA,
24,0, 0, 0, 0, 0, 0,0,0,0,
0,0,0,0,32,0,0,PFD_MAIN_PLANE,
0,0,0,0	};

char *loadTexture(char *name){
	HANDLE h = CreateFileA(name,GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	int fsize = GetFileSize(h,0);
	char *text = HeapAlloc(GetProcessHeap(),8,fsize-137);
	SetFilePointer(h,138,0,0);
	ReadFile(h,text,fsize - 138,0,0);
	CloseHandle(h);
	return text;
}

void drawWord(char *str,VEC2 pos){
	for(int i = 0;i < strlen(str);i++){
		if(str[i] == ' '){
			continue;
		}
		VEC2 c;
		c.x = (float)((str[i]-0x61)%10)/10.0f;
		c.y = 0.5f-(float)((str[i]-0x61)/10)/4.0f;
		quadAddEx(pos,(VEC2){0.03f,0.05f},c,(VEC2){0.1f,0.25f},6.0f);
		pos.x += 0.03f;
	}
}

void hitboxAdd(IVEC2 pos,IVEC2 size,unsigned int id){
	hitbox[hitboxC].pos  = pos;
	hitbox[hitboxC].size = size;
	hitbox[hitboxC].id   = id;
	hitboxC++;
}

void redraw(){
	quadPointer = 0;
	quadAdd((VEC2){-1.0f,-1.0f},(VEC2){1.125f,2.0f},0.0f);
	quadAdd((VEC2){0.3917f,-1.00f},(VEC2){0.0300f,0.15f},1.0f);
	quadAdd((VEC2){0.1250f,-1.00f},(VEC2){0.2667f,0.05f},2.0f);
	quadAdd((VEC2){0.1250f,-0.95f},(VEC2){0.2667f,0.05f},3.0f);
	quadAdd((VEC2){0.1250f,-0.90f},(VEC2){0.2667f,0.05f},4.0f);
	quadAdd((VEC2){0.1250f,-0.85f},(VEC2){0.2967f,0.05f},13.0f);
	quadAdd((VEC2){(float)cursor.x/960.0f-1.002f,(float)cursor.y/540.0f-1.0037f},(VEC2){0.004f,0.0074f},5.0f);
	quadAdd((VEC2){0.95f,0.92f},(VEC2){0.05f,0.08f},7.0f);
	quadAdd((VEC2){0.95f,-1.0f},(VEC2){0.05f,0.08f},9.0f);
	quadAdd((VEC2){0.95f,-0.92f},(VEC2){0.05f,0.08f},10.0f);
	quadAdd((VEC2){0.95f,-0.84f},(VEC2){0.05f,0.08f},11.0f);
	quadAdd((VEC2){0.95f,-0.76f},(VEC2){0.05f,0.08f},12.0f);

	quadAdd((VEC2){0.1250f,0.95f},(VEC2){0.2667f,0.05f},14.0f);
	quadAdd((VEC2){0.1250f,0.90f},(VEC2){0.2667f,0.05f},15.0f);

	quadAddEx2((VEC2){0.82f,0.92f},(VEC2){0.13f,0.08f},(VEC3){0.2f,0.4f,0.6f},8.0f);
	quadAddEx2((VEC2){0.72f,0.92f},(VEC2){0.10f,0.08f},(VEC3){0.15f,0.3f,0.45f},8.0f);

	hitboxAdd((IVEC2){1872,1037},(IVEC2){43 ,43},0);
	hitboxAdd((IVEC2){1747,1037},(IVEC2){130,43},1);
	hitboxAdd((IVEC2){1872,0   },(IVEC2){43 ,43},2);
	hitboxAdd((IVEC2){1872,43  },(IVEC2){43 ,43},3);
	hitboxAdd((IVEC2){1653,1037},(IVEC2){100,43},4);
	hitboxAdd((IVEC2){1872,86  },(IVEC2){43 ,43},5);
	hitboxAdd((IVEC2){1872,129 },(IVEC2){43 ,43},6);

	drawWord("save",(VEC2){0.8255f,0.934f});
	drawWord("new",(VEC2){0.7255f,0.934f});
}

void openGL(){
	HANDLE h = CreateFileA("shaders/vertex.vs",GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	int fsize = GetFileSize(h,0);
	VERTsource = HeapAlloc(GetProcessHeap(),8,fsize+1);
	ReadFile(h,VERTsource,fsize+1,0,0);
	CloseHandle(h);

	h = CreateFileA("shaders/fragment.fs",GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	fsize = GetFileSize(h,0);
	FRAGsource = HeapAlloc(GetProcessHeap(),8,fsize+1);
	ReadFile(h,FRAGsource,fsize+1,0,0);
	CloseHandle(h);

	SetPixelFormat(dc, ChoosePixelFormat(dc, &pfd), &pfd);
	wglMakeCurrent(dc, wglCreateContext(dc));

	glCreateProgram			  = wglGetProcAddress("glCreateProgram");
	glCreateShader			  = wglGetProcAddress("glCreateShader");
	glShaderSource			  = wglGetProcAddress("glShaderSource");
	glCompileShader			  = wglGetProcAddress("glCompileShader");
	glAttachShader			  = wglGetProcAddress("glAttachShader");
	glLinkProgram			  = wglGetProcAddress("glLinkProgram");
	glUseProgram			  = wglGetProcAddress("glUseProgram");
	glEnableVertexAttribArray = wglGetProcAddress("glEnableVertexAttribArray");
	glVertexAttribPointer     = wglGetProcAddress("glVertexAttribPointer");
	glBufferData              = wglGetProcAddress("glBufferData");
	glCreateBuffers           = wglGetProcAddress("glCreateBuffers");
	glBindBuffer              = wglGetProcAddress("glBindBuffer");
	glGetShaderInfoLog        = wglGetProcAddress("glGetShaderInfoLog");
	glActiveTexture			  = wglGetProcAddress("glActiveTexture");
	glGenerateMipmap		  = wglGetProcAddress("glGenerateMipmap");
	glGetUniformLocation      = wglGetProcAddress("glGetUniformLocation");
	glUniform1f               = wglGetProcAddress("glUniform1f");
	glUniform2f               = wglGetProcAddress("glUniform2f");
	glUniform3f               = wglGetProcAddress("glUniform3f");
	glUniform1i				  = wglGetProcAddress("glUniform1i");
	glUniform2i				  = wglGetProcAddress("glUniform2i");

	shaderProgram = glCreateProgram();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader,1,(const char**)&VERTsource,0);
	glShaderSource(fragmentShader,1,(const char**)&FRAGsource,0);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
	glAttachShader(shaderProgram,vertexShader);
	glAttachShader(shaderProgram,fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	glGenTextures(1,&canvasText);
	glGenTextures(1,&fontText);

	glBindTexture(GL_TEXTURE_2D,fontText);
	
	glBindTexture(GL_TEXTURE_2D,canvasText);

	glActiveTexture(GL_TEXTURE1);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,320,144,0,GL_RGBA,GL_UNSIGNED_BYTE,font);
	glGenerateMipmap(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,canvasSize.x,canvasSize.y,0,GL_RGBA,GL_UNSIGNED_BYTE,canvas);
	glGenerateMipmap(GL_TEXTURE_2D);

	glUniform1i(glGetUniformLocation(shaderProgram,"canvas"),0);
	glUniform1i(glGetUniformLocation(shaderProgram,"font"),1);
	glUniform3f(glGetUniformLocation(shaderProgram,"colorSel"),colorSel.r,colorSel.g,colorSel.b);

	glCreateBuffers(1,&VBO);		
	glBindBuffer(GL_ARRAY_BUFFER,VBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,2,GL_FLOAT,0,8 * sizeof(float),(void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,2,GL_FLOAT,0,8 * sizeof(float),(void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,1,GL_FLOAT,0,8 * sizeof(float),(void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3,3,GL_FLOAT,0,8 * sizeof(float),(void*)(5 * sizeof(float)));

	redraw();

	glUniform2f(glGetUniformLocation(shaderProgram,"camOffset"),camera.pos.x,camera.pos.y);
	glUniform1f(glGetUniformLocation(shaderProgram,"camZoom"),camera.zoom);

	for(;;){
		while(openglmes.c>0){
			openglmes.c--;
			switch(openglmes.id[openglmes.c]){
			case 0:
				glTexSubImage2D(GL_TEXTURE_2D,0,openglmes.pos[openglmes.c].y,openglmes.pos[openglmes.c].x,1,1,GL_RGBA,GL_UNSIGNED_BYTE,canvas+openglmes.pos[openglmes.c].x*canvasSize.x+openglmes.pos[openglmes.c].y);
				break;
			case 1:
				glUniform1f(glGetUniformLocation(shaderProgram,"camZoom"),camera.zoom);
				glUniform2f(glGetUniformLocation(shaderProgram,"camOffset"),camera.pos.x,camera.pos.y);
				break;
			case 2:
				glUniform1i(glGetUniformLocation(shaderProgram,"toolSel"),toolSel);
				break;
			case 3:
				glTexSubImage2D(GL_TEXTURE_2D,0,0,0,canvasSize.x,canvasSize.y,GL_RGBA,GL_UNSIGNED_BYTE,canvas);
				break;
			case 4:
				if(quadPointer > 48){
					quadPointer -= 48;
				}
				quadAdd((VEC2){(float)cursor.x/960.0f-1.002f,(float)cursor.y/540.0f-1.0037f},(VEC2){0.004f,0.0074f},5.0f);
				break;
			case 5:
				glUniform3f(glGetUniformLocation(shaderProgram,"colorSel"),(float)colorSel.r/255.0f,(float)colorSel.g/255.0f,(float)colorSel.b/255.0f);
				break;
			case 6:
				HeapFree(GetProcessHeap(),8,canvas);
				canvas = HeapAlloc(GetProcessHeap(),8,canvasSize.x*canvasSize.y*sizeof(RGBA));
				glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,canvasSize.x,canvasSize.y,0,GL_RGBA,GL_UNSIGNED_BYTE,canvas);
				glGenerateMipmap(GL_TEXTURE_2D);
				break;
			}
		}
		glUniform2f(glGetUniformLocation(shaderProgram,"canvasSz"),(float)canvasSize.x/255.0f,(float)canvasSize.y/255.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.05f,0.10f,0.15f,1.0f);
		glDrawArrays(GL_TRIANGLES,0,quadPointer);
		SwapBuffers(dc);
	}
}

void tick(){
	for(;;){
		if(GetKeyState(VK_LBUTTON)&0x80){
			if(cursor.x<1080){
				switch(toolSel){
				case 0:{
					float dst = VEC2dist(pointSel,pointSel2);
					VEC2 rd = VEC2divR(VEC2subVEC2R(pointSel2,pointSel),dst);
					VEC2 ro = pointSel;
					for(int i = 0;i < dst+1;i++){
						canvas[(int)ro.x*canvasSize.x+(int)ro.y] = canvasb[(int)ro.x*canvasSize.x+(int)ro.y];
						VEC2addVEC2(&ro,rd);
					}
					IVEC2 posp = getPosp();
					pointSel2 = (VEC2){posp.x,posp.y}; 
					dst = VEC2dist(pointSel,pointSel2);
					rd = VEC2divR(VEC2subVEC2R(pointSel2,pointSel),dst);
					ro = pointSel;
					for(int i = 0;i < dst+1;i++){
						canvas[(int)ro.x*canvasSize.x+(int)ro.y] = colorSel;
						VEC2addVEC2(&ro,rd);
					}
					openglmes.id[openglmes.c] = 3;
					openglmes.c++;
					break;
					}
				case 1:{
					IVEC2 posp = getPosp();
					canvas[posp.x*canvasSize.x+posp.y] = colorSel;
					openglmes.id[openglmes.c] = 3;
					openglmes.c++;
					break;
					}
				case 3:{
					unsigned char c = 0;
					if(pointSel.y > pointSel2.y){
						float t = pointSel.y;
						pointSel.y = pointSel2.y;
						pointSel2.y = t;
						c |= 0x01;
					}
					if(pointSel.x > pointSel2.x){
						float t = pointSel.x;
						pointSel.x = pointSel2.x;
						pointSel2.x = t;
						c |= 0x02;
					}
					for(int i = pointSel.x;i < pointSel2.x;i++){
						for(int i2 = pointSel.y;i2 < pointSel2.y;i2++){
							canvas[i*canvasSize.x+i2] = canvasb[i*canvasSize.x+i2];
						}
					}
					if(c & 0x01){
						float t = pointSel.y;
						pointSel.y = pointSel2.y;
						pointSel2.y = t;
					}
					if(c & 0x02){
						float t = pointSel.x;
						pointSel.x = pointSel2.x;
						pointSel2.x = t;
					}
					IVEC2 posp = getPosp();
					pointSel2 = (VEC2){posp.x,posp.y};
					c = 0;
					if(pointSel.y > pointSel2.y){
						float t = pointSel.y;
						pointSel.y = pointSel2.y;
						pointSel2.y = t;
						c |= 0x01;
					}
					if(pointSel.x > pointSel2.x){
						float t = pointSel.x;
						pointSel.x = pointSel2.x;
						pointSel2.x = t;
						c |= 0x02;
					}
					for(int i = pointSel.x;i < pointSel2.x;i++){
						for(int i2 = pointSel.y;i2 < pointSel2.y;i2++){
							canvas[i*canvasSize.x+i2] = colorSel;
						}
					}
					if(c & 0x01){
						float t = pointSel.y;
						pointSel.y = pointSel2.y;
						pointSel2.y = t;
					}
					if(c & 0x02){
						float t = pointSel.x;
						pointSel.x = pointSel2.x;
						pointSel2.x = t;
					}
					openglmes.id[openglmes.c] = 3;
					openglmes.c++;
					break;
					}
				}
			}
			else if(cursor.x < 1336 && cursor.y > 0 && cursor.y < 27){
				colorSel.r = cursor.x-1080;
				openglmes.id[openglmes.c] = 5;
				openglmes.c++;
			}
			else if(cursor.x < 1336 && cursor.y > 27 && cursor.y < 54){
				colorSel.g = cursor.x-1080;
				openglmes.id[openglmes.c] = 5;
				openglmes.c++;
			}
			else if(cursor.x < 1336 && cursor.y > 54 && cursor.y < 81){
				colorSel.b = cursor.x-1080;
				openglmes.id[openglmes.c] = 5;
				openglmes.c++;
			}
			else if(cursor.x < 1366 && cursor.y > 81 && cursor.y < 108){
				toolSize = (cursor.x-1080)/22;
				glUniform1i(glGetUniformLocation(shaderProgram,"toolSize"),toolSize);
			}
			else if(cursor.x < 1336 && cursor.y > 1053 && cursor.y < 1080){
				canvasSize.x = cursor.x-1080;
				openglmes.id[openglmes.c] = 6;
				openglmes.c++;
			}
			else if(cursor.x < 1336 && cursor.y > 1026 && cursor.y < 1053){
				canvasSize.y = cursor.x-1080;
				openglmes.id[openglmes.c] = 6;
				openglmes.c++;
			}
		}
		Sleep(15);
	}
}

void main(){
	timeBeginPeriod(1);
	{
		CreateDirectoryA("images",0);
		HANDLE h = CreateFileA("images/temp.bmp",GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
		if(h==INVALID_HANDLE_VALUE){
			printf("yeet\n");
			canvasSize.x = 100;
			canvasSize.y = 100;
			canvas  = HeapAlloc(GetProcessHeap(),8,canvasSize.x*canvasSize.y*sizeof(RGBA));
			canvasb = HeapAlloc(GetProcessHeap(),8,canvasSize.x*canvasSize.y*sizeof(RGBA));
		}
		else{
			SetFilePointer(h,0x0a,0,FILE_BEGIN);
			unsigned int offset;
			ReadFile(h,&offset,4,0,0);
			SetFilePointer(h,0x0e,0,FILE_BEGIN);
			switch(offset){
			case 54:{
				BITMAPINFOHEADER bmp;
				ReadFile(h,&bmp,sizeof(BITMAPINFOHEADER),0,0);
				canvasSize.x = bmp.biWidth;
				canvasSize.y = bmp.biHeight;
				canvas  = HeapAlloc(GetProcessHeap(),8,canvasSize.x*canvasSize.y*sizeof(RGBA));
				canvasb = HeapAlloc(GetProcessHeap(),8,canvasSize.x*canvasSize.y*sizeof(RGBA));
				ReadFile(h,canvas,canvasSize.x*canvasSize.y*sizeof(RGBA),0,0);
				for(int i = 0;i < canvasSize.x*canvasSize.y;i++){
					canvas[i].r ^= canvas[i].b;
					canvas[i].b ^= canvas[i].r;
					canvas[i].r ^= canvas[i].b;
				}
				break;
			}
			case 122:{
				BITMAPV4HEADER bmp;
				ReadFile(h,&bmp,sizeof(BITMAPV4HEADER),0,0);
				canvasSize.x = bmp.bV4Width;
				canvasSize.y = -bmp.bV4Height;
				canvas  = HeapAlloc(GetProcessHeap(),8,canvasSize.x*canvasSize.y*sizeof(RGBA));
				canvasb = HeapAlloc(GetProcessHeap(),8,canvasSize.x*canvasSize.y*sizeof(RGBA));
				ReadFile(h,canvas,canvasSize.x*canvasSize.y*sizeof(RGBA),0,0);
				for(int i = 0;i < canvasSize.x*canvasSize.y;i++){
					canvas[i].r ^= canvas[i].b;
					canvas[i].b ^= canvas[i].r;
					canvas[i].r ^= canvas[i].b;
				}
				break;
			}
			case 138:{
				BITMAPV5HEADER bmp;
				ReadFile(h,&bmp,sizeof(BITMAPV5HEADER),0,0);
				canvasSize.x = bmp.bV5Width;
				canvasSize.y = -bmp.bV5Height;
				canvas  = HeapAlloc(GetProcessHeap(),8,canvasSize.x*canvasSize.y*sizeof(RGBA));
				canvasb = HeapAlloc(GetProcessHeap(),8,canvasSize.x*canvasSize.y*sizeof(RGBA));
				ReadFile(h,canvas,canvasSize.x*canvasSize.y*sizeof(RGBA),0,0);
				for(int i = 0;i < canvasSize.x*canvasSize.y;i++){
					canvas[i].r ^= canvas[i].b;
					canvas[i].b ^= canvas[i].r;
					canvas[i].r ^= canvas[i].b;
				}
				break;
			}

			}
		}
		CloseHandle(h);
	}
	quad = HeapAlloc(GetProcessHeap(),8,sizeof(float)*4096*16);

	font = loadTexture("textures/font.bmp");

	GetCursorPos(&cursor);
	cursor.y = 1080 - cursor.y;

	wndclass.hInstance = GetModuleHandleA(0);
	RegisterClassA(&wndclass);
	window = CreateWindowExA(0,"photoshop 2","photoshop 2",0x90080000,0,0,1920,1080,0,0,wndclass.hInstance,0);
	dc = GetDC(window);
	ShowCursor(0);

	CreateThread(0,0,tick,0,0,0);
	CreateThread(0,0,openGL,0,0,0);

	Sleep(10);

	for(;;){
		while(PeekMessageA(&Msg,window,0,0,0)){
			GetMessageA(&Msg,window,0,0);
			TranslateMessage(&Msg);
			DispatchMessageW(&Msg);
		}
		Sleep(1);
	}
}
