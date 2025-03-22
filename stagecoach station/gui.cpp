#include "gui.h"
#include <malloc.h>
#include <stdio.h>
#include <map>
#include <vector>
#include <string>
#include <assert.h>
#include <time.h>
#pragma comment(lib,"msimg32.lib")
/*style*/
GUIStyle ButtonStyle = { {RGB(240,240,240),RGB(166,166,166)},{BLACK ,BLACK ,16,"Arial"},{RGB(179,202,234),RGB(179,202,234),1,10},{RGB(196,213,255)} };
GUIStyle EditStyle = { {RGB(242,245,252),RGB(252,252,252)},{BLACK ,BLACK,16,"Arial"},{RGB(133,145,162),RGB(32,61,119),1,10},{RGB(252,252,252)} };

static GUIStyle SaveButtonStyle;
static GUIStyle SaveEditStyle;

void drawFillRoundRect(Rect* rect, COLORREF fill, COLORREF border);
static void drawSolidRoundRect(const Rect& rect, COLORREF border);
void drawRoundRect(const Rect& rect, COLORREF border);
void drawkeyboardFocus(const Rect& rect);

/*UI状态*/
struct GUIState
{
	//Mouse
	int mx;
	int my;
	bool mouseDown;
	int mouseButton;

	int hotItem;	//鼠标当前悬停的item
	int activeItem;	//拥有鼠标焦点的item
	int lastHotItem;	//最后获得热点的item

	//key
	int kbdItem;		//拥有键盘焦点的item
	int keyEntered;		//当前按键
	int keyModifier;	//修饰符按键

	//char
	char ch;

	int lastWidget;
};

static GUIState uiState = { 0,0,false };
static GroupID  gGroupId = {-1,-1,NULL};		/*!> 如果没有给radioButton 指定组，则使用这个*/

/*状态函数*/
bool regionHit(Rect* rect)
{
	Point pos = { uiState.mx,uiState.my };
	return pointInRect(&pos, rect);
}
void GUI_Init()
{
	setbkcolor(RGB(232, 232, 236));
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	DK_SetFont(18, "楷体");

	BeginBatchDraw();

	//uiState.kbdItem = -1;	/*!> 默认情况下，没有任何item获得焦点。通过按tab键可以调出焦点*/
	GUI_ClearFocus();
}

void GUI_Clean()
{
	EndBatchDraw();
	closegraph();
}

void GUI_Update(ExMessage* msg)
{
	if (msg->message == WM_MOUSEMOVE)
	{
		uiState.mx = msg->x;
		uiState.my = msg->y;
	}
	else if (msg->message == WM_LBUTTONDOWN)
	{
		uiState.mouseDown = true;
		uiState.mouseButton = WM_LBUTTONDOWN;
	}
	else if (msg->message == WM_LBUTTONUP)
	{
		uiState.mouseDown = false;
		uiState.mouseButton = 0;
	}
	else if (msg->message == WM_RBUTTONDOWN)
	{
		uiState.mouseButton = WM_RBUTTONDOWN;
	}
	else if (msg->message == WM_RBUTTONUP)
	{
		uiState.mouseButton = 0;
	}
	else if (msg->message == WM_CHAR)
	{
		uiState.ch = msg->ch;
	}
	else if (msg->message == WM_KEYDOWN)
	{
		uiState.keyEntered = msg->vkcode;
		uiState.keyModifier |= msg->ctrl ? 2 : (msg->shift ? 4 : 0);
	}
	else if (msg->message == WM_KEYUP)
	{
	}
}

void GUI_Prepare()
{
	cleardevice();

	//每帧开始时，重置鼠标悬停
	uiState.hotItem = 0;
}

void GUI_Finish()
{
	FlushBatchDraw();

	//如果鼠标没有按下
	if (!uiState.mouseDown)
	{
		//取消焦点
		uiState.activeItem = 0;
	}
	//如果鼠标按下了
	else
	{
		if (uiState.activeItem == 0)
			uiState.activeItem = -1;

	}

	if (uiState.keyEntered == VK_TAB)
		uiState.kbdItem = 0;
	uiState.keyEntered = 0;
	uiState.ch = 0;
}

void GUI_ClearFocus()
{
	uiState.kbdItem = -1;	/*!> 默认情况下，没有任何item获得焦点。通过按tab键可以调出焦点*/
}

//鼠标是否在某个区域悬停或者按下了
static bool mouseHotActiveHandle(ItemID id, Rect* rect)
{
	//鼠标是否在这个区域里面
	if (regionHit(rect))
	{
		//当前拥有鼠标热点（鼠标悬停）的item
		uiState.hotItem = id;

		//如果鼠标没有按下，那么就将此控件设置最后一个获取热点的控件（如果鼠标没有按下，那么lastHotItem和hotItem是一样的）
		//这是为了记录鼠标点击的是哪个控件，因为当点击slider并拖动时，如果鼠标hot移动到了别的控件，那么此时slier将失去hot和active，让拖动失效
		//如果保存了点击时，所在的控件，那么可以对此进行判断(下面有)
		if (!uiState.mouseDown)
			uiState.lastHotItem = id;

		//如果最后获取热点的控件不是自己，那么表示有其他控件正在处理，自己将放弃热点（比如点击按钮不松开，再把鼠标移动到其他控件上时，其他控件不能获取热点）
		if (uiState.lastHotItem != id)
			uiState.hotItem = 0;

		//如果鼠标按下了，并且最后获得鼠标hot的是本控件，则设置为激活
		if (uiState.mouseDown && uiState.lastHotItem == id)
		{
			uiState.activeItem = id;
			return true;
		}
	}
	return false;
}
//键盘焦点
static bool keyboardHot(ItemID id, Rect* rect)
{
	//键盘焦点
	if (uiState.kbdItem == 0)
		uiState.kbdItem = id;

	if (uiState.kbdItem == id)
	{
		Rect drect = { rect->x - 1,rect->y - 1,rect->w + 2,rect->h + 2 };
		drawkeyboardFocus(drect);
	}

	if (uiState.kbdItem == id)
	{
		switch (uiState.keyEntered)
		{
		case VK_TAB:
			uiState.kbdItem = 0;

			//如果按下了shift
			if (uiState.keyModifier & 4)
				uiState.kbdItem = uiState.lastWidget;
			
			uiState.keyEntered = 0;
			break;
		case VK_RETURN:		//回车
			return true;
			break;
		}
	}
	uiState.lastWidget = id;
	return false;
}


/*控件*/
/* Button*/
bool GUI_TextButton(ItemID id, Rect* rect, const char* text,bool lucency)
{
	if (mouseHotActiveHandle(id,rect))
	{
		uiState.hotItem = id;
		//如果没有获取焦点的item，并且鼠标按下了，就获取焦点
		if (uiState.activeItem == 0 && uiState.mouseDown)
			uiState.activeItem = id;
	}

	if (!lucency)
	{
		//如果鼠标在当前按钮上悬停
		if (uiState.hotItem == id)
		{
			//并且有焦点
			if (uiState.activeItem == id)
			{
				drawFillRoundRect(rect, ButtonStyle.focus.color, ButtonStyle.focus.color);
			}
			else
			{
				drawFillRoundRect(rect, ButtonStyle.background.hoverColor, ButtonStyle.border.hoverColor);
			}
		}
		else
		{
			drawFillRoundRect(rect, ButtonStyle.background.color, ButtonStyle.border.color);
		}
	}
	//绘制文本
	outtextxy(rect->x + (rect->w - textwidth(text)) / 2,
		rect->y + (rect->h - textheight(text)) / 2, text);

	if (keyboardHot(id, rect))
		return true;

	//鼠标弹起时触发点击
	if (uiState.mouseDown == false && uiState.hotItem == id && uiState.activeItem == id)
		return true;

	return false;
}

bool GUI_TextureButton(ItemID id,Point* pos, IMAGE* normalImg, IMAGE* hoverImg)
{
	if (!normalImg)
		return false;

	Rect rect = {pos->x, pos->y, normalImg->getwidth(), normalImg->getheight()};
	if (mouseHotActiveHandle(id, &rect))
	{
		uiState.hotItem = id;
		//如果没有获取焦点的item，并且鼠标按下了，就获取焦点
		if (uiState.activeItem == 0 && uiState.mouseDown)
			uiState.activeItem = id;		
	}

	//如果鼠标在当前按钮上悬停
	if (uiState.hotItem == id && hoverImg)
	{
		DK_PutImage(pos->x - 1, pos->y - 1, hoverImg);
	}
	else
	{
		DK_PutImage(pos->x, pos->y, normalImg);
	}

	if (keyboardHot(id, &rect))
		return true;

	//鼠标弹起时触发点击
	if (uiState.mouseDown == false && uiState.hotItem == id && uiState.activeItem == id)
		return true;

	return false;
}

bool GUI_LucencyButton(ItemID id, Rect* rect,bool test)
{
	if (mouseHotActiveHandle(id, rect))
	{
		uiState.hotItem = id;
		//如果没有获取焦点的item，并且鼠标按下了，就获取焦点
		if (uiState.activeItem == 0 && uiState.mouseDown)
			uiState.activeItem = id;
	}

	if (test)
	{
		COLORREF tmp = getlinecolor();
		//如果鼠标在当前按钮上悬停
		if (uiState.hotItem == id)
		{
			setlinecolor(ButtonStyle.border.color);
			rectangle(rect->x, rect->y, rect->x + rect->w, rect->y + rect->h);
		}
		else
		{
			setlinecolor(ButtonStyle.border.hoverColor);
			rectangle(rect->x, rect->y, rect->x + rect->w, rect->y + rect->h);
		}
		setlinecolor(tmp);
	}

	if (keyboardHot(id, rect))
		return true;

	//鼠标弹起时触发点击
	if (uiState.mouseDown == false && uiState.hotItem == id && uiState.activeItem == id)
		return true;

	return false;
}

bool GUI_SwitchButtonEx(ItemID id, Rect* rect, bool* state, const char* offText, const char* onText)
{
	if (!state) return false;

	//鼠标是否在这个区域里面
	mouseHotActiveHandle(id, rect);

	//绘制按钮
	if (uiState.hotItem == id || uiState.activeItem == id)
	{
		drawFillRoundRect(rect, RGB(171, 239, 250), RGB(16, 124, 16));
	}
	else
	{
		drawFillRoundRect(rect, RGB(230, 231, 232), RGB(16, 124, 16));
	}

	//绘制内部的小指示器
	int interval = 5;	//指示器距离外边框的距离
	Rect leftRect = { rect->x + interval,rect->y + interval,rect->w / 2 - interval,rect->h - 2 * interval };
	Rect rightRect = { rect->x + rect->w / 2 + interval,rect->y + interval,rect->w / 2 - 2*interval,rect->h - 2 * interval };

	if (!*state)
	{
		drawFillRoundRect(&leftRect, RGB(129, 217, 233), RGB(69, 199, 222));
	}
	else
	{
		drawFillRoundRect(&rightRect, RGB(129, 217, 233), RGB(69, 199, 222));
	}
	GUI_Label(&leftRect, offText, BLACK, AlignCenter);
	GUI_Label(&rightRect, onText, BLACK, AlignCenter);


	if (keyboardHot(id, rect))
	{
		*state = !*state;
		return true;
	}

	//是否点击了按钮
	if (!uiState.mouseDown && uiState.hotItem == id && uiState.activeItem == id)
	{
		*state = !*state;
		return true;
	}

	return false;
}

bool GUI_SwitchButton(ItemID id, Rect* rect, bool* state)
{
	return GUI_SwitchButtonEx(id, rect, state, "关", "开");
}

GroupID GUI_CreateGroupID()
{
	static int lastid = 0;
	return GroupID{ lastid++ ,0,NULL};
}

bool GUI_RadioButton(ItemID id, Rect* rect, const char* label, bool* checked, GroupID* group)
{
	if (!checked) 
		return false;
	if (!group)
		group = &gGroupId;

	mouseHotActiveHandle(id, rect);


	int ir = rect->h / 2 - 10;
	int ix = rect->x + ir + 5;
	int iy = rect->y + rect->h / 2;

	//if (uiState.hotItem == id)
	//{
	//	if (uiState.activeItem == id)
	//	{
	//		//drawFillRoundRect(rect, RGB(196, 213, 255), RGB(16, 124, 16));
	//	}
	//	else
	//	{
	//		//drawFillRoundRect(rect, RGB(104, 104, 104), RGB(16, 124, 16));
	//	}
	//}
	//else
	{
		setlinecolor(BLACK);
		circle(ix, iy, ir);
	}



	if (*checked)
	{
		setfillcolor(BLACK);
		solidcircle(ix, iy, ir - 2);
	}

	Rect drect = { ix + ir + 5,rect->y,rect->w - (ix + iy + 5 - rect->x),rect->h };
	GUI_Label(&drect, label, BLACK, AlignLeft | AlignVCenter);

	//如果选中的不是自己，取消选择
	if (group->checkedItem !=-1 && //如果都没有选中，直接忽略
		group->checkedItem != id && *checked == true)
	{
		*checked = false;
		group->itemText = NULL;
	}
	//有键盘焦点，并按下了回车，就选中
	if (keyboardHot(id, rect))
	{
		*checked = !*checked;
		group->checkedItem = id;
		group->itemText = label;
		return true;
	}
	//鼠标点击了，就选中
	if (!uiState.mouseDown && uiState.hotItem == id && uiState.activeItem == id)
	{
		//如果自己没有选中，就选中（必须选中一个）
		if (group->checkedItem != id && *checked == false)
			*checked = true;
		//*checked = !*checked;
		group->checkedItem = id;
		group->itemText = label;
		return true;
	}
	return false;
}

bool GUI_CheckBox(ItemID id, Rect* rect, const char* label, bool* checked)
{
	if (!checked) return false;
	mouseHotActiveHandle(id, rect);

	Rect indRect = { rect->x + 5,rect->y + (rect->h - rect->h * 0.5) / 2,rect->h * 0.5,rect->h * 0.5 };


	//if (uiState.hotItem == id)
	//{
	//	if (uiState.activeItem == id)
	//	{
	//		drawFillRoundRect(rect, RGB(196, 213, 255), RGB(16, 124, 16));
	//	}
	//	else
	//	{
	//		drawFillRoundRect(rect, RGB(104, 104, 104), RGB(16, 124, 16));
	//	}
	//}
	//else
	{
		drawRoundRect(indRect, RGB(125, 125, 125));
	}

	if (*checked)
	{
		Rect trect = indRect;
		trect.x += 2;
		trect.y += 2;
		trect.w -= 4;
		trect.h -= 4;
		drawSolidRoundRect(trect, RGB(125, 125, 125));
	}

	Rect drect = { indRect.x + indRect.w + 5,rect->y, rect->w - (indRect.x + indRect.w + 5),rect->h };
	GUI_Label(&drect, label, BLACK, AlignLeft | AlignVCenter);

	if (keyboardHot(id, rect))
	{
		*checked = !*checked;
		return true;
	}

	if (!uiState.mouseDown && uiState.hotItem == id && uiState.activeItem == id)
	{
		*checked = !*checked;
		return true;
	}

	return false;
}



/* edit */
EditSignal GUI_Edit(ItemID id, Rect* rect, char* buffer, EchoMode mode)
{
	return GUI_Edit(id, rect, buffer, "", mode);
}

EditSignal GUI_Edit(ItemID id, Rect* rect, char* buffer, const char* placeholder_text, EchoMode mode)
{
	if (mouseHotActiveHandle(id, rect))
	{
		uiState.hotItem = id;
		if (uiState.activeItem == 0 && uiState.mouseDown)
		{
			uiState.activeItem = id;
		}
	}


	if (uiState.hotItem == id)
	{
		if (uiState.activeItem == id)
		{
			drawFillRoundRect(rect, RGB(166, 166, 166), RGB(43, 145, 175));
		}
		else
		{
			drawFillRoundRect(rect, RGB(231, 231, 231), RGB(43, 145, 175));
		}
	}
	else
	{
		drawFillRoundRect(rect, RGB(255, 255, 255), RGB(115, 198, 221));
	}


	int len = strlen(buffer);
	//bool changed = false;
	EditSignal sig = Edit_Normal;

	//绘制文本
	if (mode == Normal)
	{
		outtextxy(rect->x + 5,rect->y + (rect->h - textheight(buffer)) / 2, buffer);

		if (uiState.kbdItem == id && (clock() >> 9) & 1)
			outtextxy(rect->x + 5 + textwidth(buffer), rect->y + (rect->h - textheight("|")) / 2, "|");
	}
	else if(mode == Password)
	{
		for (int i = 0; i < len; i++)
		{
			outtextxy(rect->x + 5 + i * textwidth('*'), rect->y + (rect->h - textheight('*')) / 2, '*');
		}

		if (uiState.kbdItem == id && (clock() >> 9) & 1)
			outtextxy(rect->x + 5 + textwidth(buffer), rect->y + (rect->h - textheight("|")) / 2, "|");	
	}
	else
	{
		if (uiState.kbdItem == id && (clock() >> 9) & 1)
			outtextxy(rect->x + 5, rect->y + (rect->h - textheight("|")) / 2, "|");
	}


	//绘制占位文本
	if (buffer[0] == '\0' && uiState.kbdItem != id)
	{
		COLORREF c = gettextcolor();
		settextcolor(RGB(127, 127, 127));
		outtextxy(rect->x + 5, rect->y + (rect->h - textheight(placeholder_text)) / 2, placeholder_text);
		settextcolor(c);
	}

	//如果没有控件有键盘焦点，获取它
	if (uiState.kbdItem == 0)
		uiState.kbdItem = id;

	if (uiState.kbdItem == id)
	{
		drawkeyboardFocus(*rect);
	}

	if (uiState.kbdItem == id)
	{
		switch (uiState.keyEntered)
		{
		case VK_TAB:
			uiState.kbdItem = 0;
			//按tab键，让焦点切换到下一个输入框
			if (uiState.keyModifier & 4)
				uiState.kbdItem = uiState.lastWidget;

			uiState.keyEntered = 0;
		
			break;
		case '\b':
			//起码还有两个字符，而且这两个字符组成一个中文，就删除最后两个（中文每个字节都是负数）
			if (len > 1 && buffer[len - 1] < 0 && buffer[len - 2] < 0)
			{
				buffer[len - 2] = '\0';
				//changed = true;
				sig = Edit_TextChanged;
			}
			//删除一个字符
			else if (len > 0)
			{
				buffer[len - 1] = '\0';
				//changed = true;
				sig = Edit_TextChanged;
			}
			
			break;
		case '\r':
		case '\n':
			uiState.kbdItem = -1;	//清除焦点
			sig = Edit_Finish;
			//return changed;
			break;
		default:
			//最多只能输入size-1长度
			//if (len < size)
			{
				//只接受可打印字符和中文输入
				if ((uiState.ch >= 32 && uiState.ch < 127) || uiState.ch < 0)
				{
					buffer[len++] = uiState.ch;
					buffer[len] = '\0';
					//changed = true;
					sig = Edit_TextChanged;
				}
			}

			break;
		}
	}

	if (uiState.mouseDown == false &&
		uiState.hotItem == id &&
		uiState.activeItem == id)
	{
		uiState.kbdItem = id;
	}
	uiState.lastWidget = id;
	return sig;
}

bool GUI_ChineseEdit(char* buffer, const char* def_value, const char* title, const char* hitText)
{
	return InputBox(buffer, 128, hitText, title, def_value);
}

/* label */
void GUI_Label(Rect* rect, const char* text, COLORREF color, int aliginment)
{
	int tx = rect->x, ty = rect->y;
	if (rect->w != 0 && rect->h != 0)
	{
		int tw = textwidth(text);
		int th = textheight(text);

		int hspace = (rect->w - tw) / 2;
		int vspace = (rect->h - th) / 2;

		//对齐方式
		if(aliginment &  AlignTop)
			ty = rect->y;

		if(aliginment & AlignBottom)
			ty = rect->y + rect->h - th;

		if (aliginment& AlignLeft)
			tx = rect->x;

		if (aliginment& AlignRight)
			tx = rect->x + rect->w - tw;

		if (aliginment& AlignHCenter)
			tx = rect->x + hspace;

		if (aliginment& AlignVCenter)
			ty = rect->y + vspace;
	}
	settextcolor(color);
	outtextxy(tx,ty, text);
}
void GUI_LabelFormatEx(Rect* rect, COLORREF color, int aliginment, const char* fmt, ...)
{
	va_list alist;
	va_start(alist, fmt);

	char buf[256];
	vsnprintf(buf, 128, fmt, alist);
	GUI_Label(rect, buf, color, aliginment);

	va_end(alist);
}
void GUI_LabelFormat(Rect* rect, const char* fmt, ...)
{
	va_list alist;
	va_start(alist, fmt);

	char buf[256];
	vsnprintf(buf, 128, fmt, alist);
	GUI_Label(rect, buf, BLACK, AlignCenter);

	va_end(alist);
}

/* dialog */
bool GUI_DialogBase(ItemID id, const char* title, const Rect* rect, Rect* clientRect)
{
	DK_SetFont(26, "宋体");
	mouseHotActiveHandle(id,(Rect*)rect);
	//整个对话框 + 边框
	if (!regionHit((Rect*)rect) && uiState.mouseDown)
	{
		Rect* rr = (Rect*)rect;
		rr->x -= 2;
		rr->y -= 2;
		rr->w += 2 * 2;
		rr->h += 2 * 2;
	}
	//绘制对话框
	drawFillRoundRect((Rect*)rect, RGB(225, 225, 225), RGB(115, 198, 221));

	//绘制标题栏下面的线
	setlinecolor(ButtonStyle.border.color);
	Rect r = { rect->x + 1,rect->y + 38,rect->w - 2,rect->h - 39 };
	line(r.x, r.y - 3, r.x + r.w, r.y - 3);

	//传出客户区矩形
	if (clientRect) *clientRect = r;

	//绘制标题
	settextstyle(18, 0, "Arial"); 
	Rect titleR = { rect->x + 5,rect->y + 0,textwidth(title),39 };
	GUI_Label(&titleR, title, 0, AlignVCenter);

	//关闭按钮
	Rect closeR = { rect->x + rect->w - 33,rect->y + 1,32,32 };

	if (GUI_TextButton(id+10000, &closeR, "×"))
	{
		return true;
	}
	return false;
}

/* slider */
bool GUI_SliderHorizontal(ItemID id, Rect* r, int maxVal, int* value)
{
	//计算滑块的滑块大小
	int handleSize = r->h / 2;

	//计算鼠标光标的相对y偏移量
	int xPos = ((r->w - handleSize * 1.5) * *value) / maxVal;

	//凹槽区域，鼠标移动到凹槽区域才能滑动滑块
	Rect rect = { r->x + handleSize / 2, r->y + handleSize / 2, r->w - handleSize / 2, handleSize };
	mouseHotActiveHandle(id, &rect);
	//画面板
	{
		Rect tRect = { r->x, r->y, r->w + handleSize / 2, r->h };
		drawFillRoundRect(&tRect, RGB(119, 119, 119), RGB(119, 119, 119));

	}
	//画凹槽
	{
		Rect tRect = { rect.x, rect.y, rect.w - handleSize / 2, rect.h };
		drawFillRoundRect(&rect, RGB(71, 71, 71), RGB(71, 71, 71));
	}

	//有鼠标焦点或者鼠标悬停
	if (uiState.activeItem == id || uiState.hotItem == id)
	{
		Rect tRect = { r->x + handleSize / 2 + xPos, r->y + handleSize / 2, handleSize, handleSize };
		drawFillRoundRect(&tRect, WHITE, WHITE);
	}
	else
	{
		Rect tRect = { r->x + handleSize / 2 + xPos, r->y + handleSize / 2, handleSize, handleSize };
		drawFillRoundRect(&tRect, RGB(170, 170, 170), RGB(170, 170, 170));
	}

	// If no widget has keyboard focus, take it
	if (uiState.kbdItem == 0)
		uiState.kbdItem = id;

	// 绘制键盘焦点
	if (uiState.kbdItem == id)
	{
		Rect tRect = { r->x, r->y, r->w + handleSize / 2, r->h };
		drawkeyboardFocus(tRect);
	}


	// If we have keyboard focus, we'll need to process the keys
	if (uiState.kbdItem == id)
	{
		switch (uiState.keyEntered)
		{
		case VK_TAB:
			// If tab is pressed, lose keyboard focus.
			// Next widget will grab the focus.
			uiState.kbdItem = 0;
			// If shift was also pressed, we want to move focus
			// to the previous widget instead.
			if (uiState.keyModifier & VK_SHIFT)
				uiState.kbdItem = uiState.lastWidget;
			// Also clear the key so that next widget
			// won't process it
			uiState.keyEntered = 0;
			break;
		case VK_LEFT:
			// Slide slider up (if not at zero)
			if (*value > 0)
			{
				(*value)--;
				return 1;
			}
			break;
		case VK_RIGHT:
			// Slide slider down (if not at max)
			if (*value < maxVal)
			{
				(*value)++;
				return 1;
			}
			break;
		}
	}

	uiState.lastWidget = id;

	// Update widget value
	if (uiState.activeItem == id)
	{
		int mousepos = uiState.mx - (r->x + handleSize / 2);
		if (mousepos < 0) mousepos = 0;
		if (mousepos > r->w) mousepos = r->w;
		int v = (mousepos * maxVal) / r->w;
		if (v != *value)
		{
			*value = v;
			return true;
		}
	}
	return false;
}

bool GUI_SliderVertical(ItemID id, Rect* r, int maxVal, int* value)
{
	//计算滑块的滑块大小
	int handleSize = r->w / 2;

	//计算鼠标光标的相对y偏移量
	int yPos = ((r->h - handleSize * 1.5) * *value) / maxVal;

	Rect rect = { r->x + handleSize / 2, r->y + handleSize / 2, handleSize, r->h - handleSize/2 };
	mouseHotActiveHandle(id, &rect);
	//画面板
	{
		Rect tRect = { r->x, r->y, r->w, r->h + handleSize / 2 };
		drawFillRoundRect(&tRect, RGB(119, 119, 119), RGB(119, 119, 119));
	}

	//画凹槽
	{
		drawFillRoundRect(&rect, RGB(71, 71, 71), RGB(71, 71, 71));
	}

	//有鼠标焦点或者鼠标悬停
	if (uiState.activeItem == id || uiState.hotItem == id)
	{
		Rect tRect = { r->x + handleSize / 2, r->y + handleSize / 2 + yPos, handleSize, handleSize };
		drawFillRoundRect(&tRect, WHITE, WHITE);
	}
	else
	{
		Rect tRect = { r->x + handleSize / 2, r->y + handleSize / 2 + yPos, handleSize, handleSize };
		drawFillRoundRect(&tRect, RGB(170, 170, 170), RGB(170, 170, 170));
	}


	// If no widget has keyboard focus, take it
	if (uiState.kbdItem == 0)
		uiState.kbdItem = id;

	// 绘制键盘焦点
	if (uiState.kbdItem == id)
	{
		drawkeyboardFocus({ r->x, r->y, r->w, r->h + handleSize / 2 });
	}
		

	// If we have keyboard focus, we'll need to process the keys
	if (uiState.kbdItem == id)
	{
		switch (uiState.keyEntered)
		{
		case VK_TAB:
			// If tab is pressed, lose keyboard focus.
			// Next widget will grab the focus.
			uiState.kbdItem = 0;
			// If shift was also pressed, we want to move focus
			// to the previous widget instead.
			if (uiState.keyModifier & VK_SHIFT)
				uiState.kbdItem = uiState.lastWidget;
			// Also clear the key so that next widget
			// won't process it
			uiState.keyEntered = 0;
			break;
		case VK_UP:
			// Slide slider up (if not at zero)
			if (*value > 0)
			{
				(*value)--;
				return 1;
			}
			break;
		case VK_DOWN:
			// Slide slider down (if not at max)
			if (*value < maxVal)
			{
				(*value)++;
				return 1;
			}
			break;
		}
	}

	uiState.lastWidget = id;

	// Update widget value
	if (uiState.activeItem == id)
	{
		int mousepos = uiState.my - (r->y + handleSize / 2);
		if (mousepos < 0) mousepos = 0;
		if (mousepos > r->h) mousepos = r->h;
		int v = (mousepos * maxVal) / r->h;
		if (v != *value)
		{
			*value = v;
			return true;
		}
	}
	return false;
}

bool GUI_Slider(ItemID id, Rect* rect, int maxVal, int* value)
{
	if (!value) return false;

	enum Orientation orien = rect->w > rect->h ? Horizontal : Vertical;
	bool sliderMoved = false;
	switch (orien)
	{
	case Horizontal:
		sliderMoved = GUI_SliderHorizontal(id, rect, maxVal, value);
		break;
	case Vertical:
		sliderMoved = GUI_SliderVertical(id, rect, maxVal, value);
		break;
	}
	return sliderMoved;
}

bool GUI_YearPicker(ItemID id, Point* pos, YearPickerData* currentYear)
{
	//准备数据
	static Size size = { 230, 250 };	//控件大小
	static const int	ROWS = 4;
	static const int	COLS = 3;

	static Point index = {-1,-1};

	if (!currentYear)
		return false;

	if (currentYear->year <= 0)
	{
		time_t tim;
		time(&tim);
		tm* t =  localtime(&tim);
		currentYear->year = 1900 +t->tm_year;
		currentYear->_startYear = currentYear->year;
	}

	int boundWidth = size.w / COLS;
	int boundHeight = (size.h - 35) / ROWS;
	int realWidth = 50;
	int realHeight = 30;
	int hSpace = (boundWidth - realWidth) / 2;
	int vSpace = (boundHeight - realHeight) / 2;
	/*if (uiState.mx >= pos->x && uiState.mx <= pos->x + size.w &&
		uiState.my >= pos->y + 35 && uiState.my <= pos->y + size.h)
	{
		int r = (uiState.my - pos->y - 35) / boundHeight;
		int c = (uiState.mx - pos->x) / boundWidth;
		index = { r,c };
		if (uiState.mouseDown)
		{
			currentYear->_isChecked = true;
		}
	}
	else
	{
		index = { -1,-1 };
	}
	*/

	//绘制整个控件区域
	setfillcolor(WHITE);
	fillroundrect(pos->x, pos->y, pos->x +  size.w, pos->y +  size.h, 10, 10);

	//绘制导航栏
	line(pos->x, pos->y + 35, pos->x +  size.w, pos->y + 35);

	Rect barRect = { pos->x, pos->y,  size.w,35 };
	char str[128];
	sprintf(str, "%d",  currentYear->year);
	GUI_Label(&barRect, str);

	if (GUI_LucencyButton(GUID + id, R( barRect.x + 95, barRect.y + 2 ,40,32 )))
	{
		currentYear->_startYear =  currentYear->year;
	}
	if (GUI_TextButton(GUID + id, R( barRect.x + 10, barRect.y  ,32,32),  "<",true))
	{
		currentYear->_startYear -=  ROWS *  COLS;
	}
	if (GUI_TextButton(GUID + id,R( barRect.x + barRect.w - 32, barRect.y ,32,32), ">",true))
	{
		currentYear->_startYear +=  ROWS *  COLS;
	}

	for (int r = 0; r < ROWS; r++)
	{
		for (int c = 0; c < COLS; c++)
		{
			int x = c * boundWidth +  pos->x;
			int y = r * boundHeight + pos->y + 35;

			COLORREF textColor;

			if (currentYear->year == currentYear->_startYear + r * COLS + c)
			{
				setfillcolor(RGB(9, 132, 216));
				textColor = WHITE;
			}
			else
			{
					Point mpos = { uiState.mx,uiState.my };
					Rect yearRect = { x + hSpace, y + vSpace, realWidth, realHeight };
					if (pointInRect(&mpos, &yearRect))
					{
						setfillcolor(RGB(194, 195, 201));
						textColor = BLACK;

						if (GUI_LucencyButton(id + r * COLS + c, &yearRect))
						{
							currentYear->year = currentYear->_startYear + r * COLS + c;
							return true;
						}
						//if (uiState.mouseDown)
						//{
						//	currentYear->year = currentYear->_startYear + r * COLS + c;
						//}
					}
					else
					{
						setfillcolor(WHITE);
						textColor = RGB(194, 195, 201);
					}
			}


			fillroundrect(x + hSpace, y + vSpace, x + hSpace +  realWidth, y + vSpace +  realHeight, 10, 10);

			Rect rect = { x + hSpace, y + vSpace,  realWidth , realHeight };
			GUI_LabelFormatEx(&rect, textColor, AlignCenter, "%d", currentYear->_startYear + r * COLS + c);
		}
	}

	return false;
}


/* helper */
void DK_SetFont(int size, const char* family)
{
	static LOGFONT font = { 0 };
	font.lfHeight = size;
	strcpy(font.lfFaceName, family);
	settextstyle(&font);
}

void DK_PutImage(int x, int y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

void DK_PutImage(int dstX, int dstY, int dstWidth, int dstHeight, IMAGE *img, int srcX, int srcY)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), dstX, dstY, dstWidth, dstHeight,
		GetImageHDC(img), srcX, srcY, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

bool pointInRect(Point* pos, Rect* rect)
{
	return pos->x >=rect->x && pos->x<=rect->x+rect->w && pos->y>=rect->y && pos->y <=rect->y + rect->h;
}

Rect* R(int x, int y, int w, int h)
{
	static Rect r = { 0 };
	if (w == -1 || h == -1) {
		w = 150;
		h = 35;
	}
	r = { x,y,w,h };
	return &r;
}
Point* P(int x, int y)
{
	static Point p = { 0 };
	p = { x,y };
	return &p;
}

static void drawFillRoundRect(Rect* rect, COLORREF fill,COLORREF border)
{
	setfillcolor(fill);
	setlinecolor(border);
	fillroundrect(rect->x, rect->y, rect->x + rect->w, rect->y + rect->h, 10, 10);
}

static void drawSolidRoundRect(const Rect& rect, COLORREF c)
{
	setfillcolor(c);
	solidroundrect(rect.x, rect.y, rect.x + rect.w, rect.y + rect.h, 10, 10);
}

static void drawRoundRect(const Rect& rect, COLORREF border)
{
	setlinecolor(border);
	roundrect(rect.x, rect.y, rect.x + rect.w, rect.y + rect.h, 10, 10);
}

static void drawRect(const Rect& rect, COLORREF border)
{
	setlinecolor(border);
	rectangle(rect.x, rect.y, rect.x + rect.w, rect.y + rect.h);
}

static void drawkeyboardFocus(const Rect& rect)
{
	LINESTYLE lineStyle;
	getlinestyle(&lineStyle);

	setlinecolor(RGB(165, 165, 165));
	setlinestyle(PS_DASH);
	roundrect(rect.x-1, rect.y-1, rect.x + rect.w +1, rect.y + rect.h+1, 10, 10);

	setlinestyle(&lineStyle);
}


void saveStyle()
{
	SaveButtonStyle = ButtonStyle;
	SaveEditStyle = EditStyle;
}

void restoreStyle()
{
	ButtonStyle = SaveButtonStyle;
	EditStyle = SaveEditStyle;
}




