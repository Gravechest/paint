#include <windows.h>
#include <glew.h>
#include <stdio.h>
#include <vecmath.h>

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

RGB *canvas;
RGB *canvasb;
RGB colorSel;

float *quad; 
unsigned char *font;

unsigned int quadPointer;

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
	glBufferData(GL_ARRAY_BUFFER,(quadPointer) * sizeof(float),quad,GL_DYNAMIC_DRAW);
}

inline IVEC2 getPosp(){
	IVEC2 posp = {(unsigned int)((float)cursor.y/1080.0f*camera.zoom+camera.pos.y),(unsigned int)((float)cursor.x/1080.0f*camera.zoom+camera.pos.x)};
	return posp;
}

void bucket(RGB color,volatile IVEC2 pos){
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
		break;
	case WM_LBUTTONDOWN:
		for(int i = 0;i < hitboxC;i++){
			if(cursor.x > hitbox[i].pos.x && cursor.x < hitbox[i].pos.x + hitbox[i].size.x && cursor.y > hitbox[i].pos.y && cursor.y < hitbox[i].pos.y + hitbox[i].size.y){
				switch(hitbox[i].id){
				case 0:{
					for(int i = 0;i < 100*100;i++){
						canvas[i].r ^= canvas[i].b;
						canvas[i].b ^= canvas[i].r;
						canvas[i].r ^= canvas[i].b;
					}
					HANDLE h = CreateFile("images/temp.bmp",GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
					BMP bmp;
					ZeroMemory(&bmp,sizeof(BMP));
					bmp.headerField  = 0x4d42;
					bmp.size         = 100*100*3+50;
					bmp.startPointer = 50;
					bmp.headerSize   = 40;
					bmp.width        = 100;
					bmp.height       = 100;
					bmp.bpp          = 24;
					bmp.cplanes      = 1;
					WriteFile(h,&bmp,sizeof(BMP),0,0);
					WriteFile(h,canvas,100*100*3+50,0,0);
					CloseHandle(h);
					ExitProcess(0);
					}
				case 1:{
					HANDLE h = CreateFile("images/temp.bmp",GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
					CloseHandle(h);
					}
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
					ZeroMemory(canvas,canvasSize.x*canvasSize.y*sizeof(RGB));
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
				memcpy(canvasb,canvas,canvasSize.x*canvasSize.y*sizeof(RGB));
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
			colorSel = canvas[posp.x*100+posp.y];
			glUniform3f(glGetUniformLocation(shaderProgram,"colorSel"),(float)colorSel.r/255.0f,(float)colorSel.g/255.0f,(float)colorSel.b/255.0f);
		}
		break;
		}
	case WM_MOUSEMOVE:{
		if(wParam&0x0002){
			POINT bcursor;
			GetCursorPos(&bcursor);
			bcursor.y = 1080 - bcursor.y;
			VEC2 rp = {bcursor.x-cursor.x,bcursor.y-cursor.y};
			vec2div(&rp,1080.0f/camera.zoom);
			vec2subvec2(&camera.pos,rp);
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
		quadPointer -= 48;
		quadAdd((VEC2){(float)cursor.x/960.0f-1.002f,(float)cursor.y/540.0f-1.0037f},(VEC2){0.004f,0.0074f},5.0f);
		break;
		}
	case WM_MOUSEWHEEL:{
		float z = (float)GET_WHEEL_DELTA_WPARAM(wParam)/1200.0f;
		camera.pos = vec2add(camera.pos,z*camera.zoom*0.5f);
		camera.zoom -= z*camera.zoom;
		VEC2 v = {(float)cursor.x,(float)cursor.y};
		openglmes.id[openglmes.c] = 1;
		openglmes.c++;
		}
	}
	return DefWindowProc(hwnd,msg,wParam,lParam);
}

WNDCLASS wndclass = {0,proc,0,0,0,0,0,0,"photoshop 2","photoshop 2"};

PIXELFORMATDESCRIPTOR pfd = {sizeof(PIXELFORMATDESCRIPTOR), 1,
PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,PFD_TYPE_RGBA,
24,0, 0, 0, 0, 0, 0,0,0,0,
0,0,0,0,32,0,0,PFD_MAIN_PLANE,
0,0,0,0	};

char *loadTexture(char *name){
	HANDLE h = CreateFile(name,GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
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

void main(){
	{
		HANDLE h = CreateFile("images/temp.bmp",GENERIC_READ,0,0,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
		BMP temp;
		ReadFile(h,&temp,sizeof(BMP),0,0);
		canvasSize.x = temp.height;
		canvasSize.y = temp.width;
		canvas  = HeapAlloc(GetProcessHeap(),8,canvasSize.x*canvasSize.y*3);
		canvasb = HeapAlloc(GetProcessHeap(),8,canvasSize.x*canvasSize.y*3);
		ReadFile(h,canvas,canvasSize.x*canvasSize.y*3,0,0);
		for(int i = 0;i < canvasSize.x*canvasSize.y;i++){
			canvas[i].r ^= canvas[i].b;
			canvas[i].b ^= canvas[i].r;
			canvas[i].r ^= canvas[i].b;
		}
		CloseHandle(h);
	}
	quad = HeapAlloc(GetProcessHeap(),8,sizeof(float)*4096);
	
	font = loadTexture("textures/font.bmp");

	GetCursorPos(&cursor);
	cursor.y = 1080 - cursor.y;

	wndclass.hInstance = GetModuleHandle(0);
	RegisterClass(&wndclass);
	window = CreateWindowEx(0,"photoshop 2","photoshop 2",0x90080000,0,0,1920,1080,0,0,wndclass.hInstance,0);
	dc = GetDC(window);
	ShowCursor(0);

	HANDLE h = CreateFile("shaders/vertex.vs",GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	int fsize = GetFileSize(h,0);
	VERTsource = HeapAlloc(GetProcessHeap(),8,fsize+1);
	ReadFile(h,VERTsource,fsize+1,0,0);
	CloseHandle(h);

	h = CreateFile("shaders/fragment.fs",GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	fsize = GetFileSize(h,0);
	FRAGsource = HeapAlloc(GetProcessHeap(),8,fsize+1);
	ReadFile(h,FRAGsource,fsize+1,0,0);
	CloseHandle(h);

	SetPixelFormat(dc, ChoosePixelFormat(dc, &pfd), &pfd);
	wglMakeCurrent(dc, wglCreateContext(dc));

	glewInit();

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
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,100,100,0,GL_RGB,GL_UNSIGNED_BYTE,canvas);
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

	glUniform2f(glGetUniformLocation(shaderProgram,"camOffset"),camera.pos.x,camera.pos.y);
	glUniform1f(glGetUniformLocation(shaderProgram,"camZoom"),camera.zoom);

	for(;;){
		if(GetKeyState(VK_LBUTTON)&0x80){
			if(cursor.x<1080){
				switch(toolSel){
				case 0:{
					float dst = vec2distance(pointSel,pointSel2);
					VEC2 rd = vec2divR(vec2subvec2R(pointSel2,pointSel),dst);
					VEC2 ro = pointSel;
					for(int i = 0;i < dst+1;i++){
						for(int i2 = ro.x;i2 < ro.x + toolSize;i2++){
							for(int i3 = ro.y;i3 < ro.y + toolSize;i3++){
								if(vec2distance((VEC2){i2-toolSize/2,i3-toolSize/2},ro)<toolSize/2){
									canvas[i2*canvasSize.x+i3] = canvasb[i2*canvasSize.x+i3];
								}
							}
						}
						vec2addvec2(&ro,rd);
					}
					IVEC2 posp = getPosp();
					pointSel2 = (VEC2){posp.x,posp.y}; 
					dst = vec2distance(pointSel,pointSel2);
					rd = vec2divR(vec2subvec2R(pointSel2,pointSel),dst);
					ro = pointSel;
					for(int i = 0;i < dst+1;i++){
						for(int i2 = ro.x;i2 < ro.x + toolSize;i2++){
							for(int i3 = ro.y;i3 < ro.y + toolSize;i3++){
								if(vec2distance((VEC2){i2-toolSize/2,i3-toolSize/2},ro)<toolSize/2){
									canvas[i2*canvasSize.x+i3] = colorSel;
								}
							}
						}
						vec2addvec2(&ro,rd);
					}
					openglmes.id[openglmes.c] = 3;
					openglmes.c++;
					break;
					}
				case 1:{
					IVEC2 posp = getPosp();
					for(int i = posp.x;i < posp.x + toolSize;i++){
						for(int i2 = posp.y;i2 < posp.y + toolSize;i2++){
							if(ivec2distance((IVEC2){toolSize/2-i,toolSize/2-i2},posp)<toolSize/2){
								canvas[i*canvasSize.x+i2] = colorSel;
							}
						}
					}
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
				glUniform3f(glGetUniformLocation(shaderProgram,"colorSel"),(float)colorSel.r/255.0f,(float)colorSel.g/255.0f,(float)colorSel.b/255.0f);
			}
			else if(cursor.x < 1336 && cursor.y > 27 && cursor.y < 54){
				colorSel.g = cursor.x-1080;
				glUniform3f(glGetUniformLocation(shaderProgram,"colorSel"),(float)colorSel.r/255.0f,(float)colorSel.g/255.0f,(float)colorSel.b/255.0f);
			}
			else if(cursor.x < 1336 && cursor.y > 54 && cursor.y < 81){
				colorSel.b = cursor.x-1080;
				glUniform3f(glGetUniformLocation(shaderProgram,"colorSel"),(float)colorSel.r/255.0f,(float)colorSel.g/255.0f,(float)colorSel.b/255.0f);
			}
			else if(cursor.x < 1366 && cursor.y > 81 && cursor.y < 108){
				toolSize = (cursor.x-1080)/22;
				glUniform1i(glGetUniformLocation(shaderProgram,"toolSize"),toolSize);
			}
		}
		while(openglmes.c>0){
			openglmes.c--;
			switch(openglmes.id[openglmes.c]){
			case 0:
				glTexSubImage2D(GL_TEXTURE_2D,0,openglmes.pos[openglmes.c].y,openglmes.pos[openglmes.c].x,1,1,GL_RGB,GL_UNSIGNED_BYTE,canvas+openglmes.pos[openglmes.c].x*100+openglmes.pos[openglmes.c].y);
				break;
			case 1:
				glUniform1f(glGetUniformLocation(shaderProgram,"camZoom"),camera.zoom);
				glUniform2f(glGetUniformLocation(shaderProgram,"camOffset"),camera.pos.x,camera.pos.y);
				break;
			case 2:
				glUniform1i(glGetUniformLocation(shaderProgram,"toolSel"),toolSel);
				break;
			case 3:
				glTexSubImage2D(GL_TEXTURE_2D,0,0,0,canvasSize.x,canvasSize.y,GL_RGB,GL_UNSIGNED_BYTE,canvas);
				break;
			}
		}
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.05f,0.10f,0.15f,1.0f);
		glDrawArrays(GL_TRIANGLES,0,quadPointer);
		SwapBuffers(dc);
		GetMessage(&Msg,window,0,0);
		TranslateMessage(&Msg);
		DispatchMessageW(&Msg);
	}
}
