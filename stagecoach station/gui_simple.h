#pragma once

#include "gui.h"
#include <time.h>
#include <stdint.h>

/** 获取从DK_lib库初始化来所经历过的时间，单位为毫秒 */
uint32_t DK_GetTicks();

/** 等待指定的毫秒数(休眠) */
void DK_Delay(uint32_t ms);

/** 随机生成[0~1]之间的浮点数 */
float DK_Rand0_1();

/** 随机生成[min~max)之间的整数 */
int	  DK_RandRange(int min, int max);

/**
 * 帧率控制.
 */
struct FrameRate
{
	float fps;					/*!> 帧率*/
	uint32_t startTicks;		/*!> 每帧率开始的毫秒*/
	uint32_t frameTicks;		/*!> 每帧实际小号的毫秒*/
};
void DK_StartFrame(FrameRate* pthis);		/*>! 每帧开始前调用 */
void DK_EndFrame(FrameRate* pthis);			/*>! 每帧结束时调用 */
void DK_SetFPS(FrameRate* pthis,float fps);	/*>! 设置帧率 如:1000/60 */

void setOnInit(void (*on_render)(void*arg));
void setOnClean(void (*on_render)(void*arg));
/**
 * 设置渲染回调，所有绘图操作都必须在此函数中进行.
 * 回调函数的参数是用户通过setUserData函数传入的.
 */
void setOnRender(void (*on_render)(void*arg));

/**
 * 设置更新回调.
 */
void setOnUpdate(void (*on_update)(void*arg));

/**
 * 设置消息处理回调.
 */
void setOnHandleMsg(void (*on_handleMsg)(void* arg, ExMessage* msg));

/**
 * 设置回调函数用户参数.
 */
void setUserData(void* arg);

/**
 * 初始化gui，并创建窗口.
 */
HWND createWindow(int w,int h,int flags = EX_SHOWCONSOLE);
/**
 * 退出gui，并关闭窗口.
 */
void quit();
/**
 * 开启消息循环.
 */
void messageLoop();




