#include "gui_simple.h"
uint32_t DK_GetTicks()
{
	return clock();
}

void DK_Delay(uint32_t ms)
{
	Sleep(ms);
}

float DK_Rand0_1()
{
	return rand() / (float)RAND_MAX;
}

int DK_RandRange(int min, int max)
{
	return rand() % (max - min) + min;
}

/** 帧率 */
void DK_StartFrame(FrameRate* pthis)
{
	pthis->startTicks = DK_GetTicks();
}

void DK_EndFrame(FrameRate* pthis)
{
	pthis->frameTicks = DK_GetTicks() - pthis->startTicks;
	if (pthis->fps > pthis->frameTicks)
	{
		Sleep(pthis->fps - pthis->frameTicks);
	}
}

void DK_SetFPS(FrameRate* pthis,float fps)
{
	pthis->fps = fps;
}


/**
 * 默认渲染回调.
 * \param  arg 用户自定义数据
 */
void maye_render(void* arg)
{
	DK_SetFont(36, "楷体");
	static COLORREF c = RED;
	static int cnt = 0;
	if (cnt++ % 40 == 0)
		c = RGB(rand() % 256, rand() % 256, rand() % 256);
	GUI_Label(R(0, 0, getwidth(), getheight()), "欢迎使用DK_GUI",c);
	DK_SetFont(18, "楷体");
}

struct Game
{
	bool isRunning = true;

	void* arg = NULL;
	void (*onInit)(void* arg) = NULL;
	void (*onClean)(void* arg) = NULL;
	void (*onRender)(void* arg) = maye_render;
	void (*onUpdate)(void* arg) = NULL;
	void (*onHandleMsg)(void* arg, ExMessage* msg) = NULL;
}game;
#define fun_call(fun,...) if(fun){fun(__VA_ARGS__);}

void setOnInit(void(*on_init)(void* arg))
{
	game.onInit = on_init;
}

void setOnClean(void(*on_clean)(void* arg))
{
	game.onClean = on_clean;
}

void setOnRender(void (*on_render)(void* arg)) {
	game.onRender = on_render;
}

void setOnUpdate(void (*on_update)(void* arg)) {
	game.onUpdate = on_update;
}

void setOnHandleMsg(void (*on_handleMsg)(void*arg,ExMessage* msg)) {
	game.onHandleMsg = on_handleMsg;
}

void setUserData(void* arg)
{
	game.arg = arg;
}

HWND createWindow(int w, int h, int flags)
{
	HWND hnd = initgraph(w, h, flags);
	return hnd;
}

void quit()
{
	game.isRunning = false;
}

/** 方便的消息循环 */
void messageLoop()
{
	fun_call(game.onInit,game.arg);
	FrameRate fr = {0};
	DK_SetFPS(&fr,1000 / 60);

	GUI_Init();
	
	ExMessage msg = { 0 };
	while (game.isRunning)
	{
		DK_StartFrame(&fr);
		GUI_Prepare();
		while (peekmessage(&msg))
		{
			GUI_Update(&msg);

			fun_call(game.onHandleMsg, game.arg,&msg);
			if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE) {
				game.isRunning = false;
			}
		}
		fun_call(game.onRender, game.arg);
		GUI_Finish();
		fun_call(game.onUpdate, game.arg);
		DK_EndFrame(&fr);
	}
	fun_call(game.onClean, game.arg);
	GUI_Clean();
}

