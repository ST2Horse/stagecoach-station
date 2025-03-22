#include<stdio.h>
#include"gui_simple.h"

char username[32] = { 0 };//电话号码
char password[32] = { 0 };//密码
char repassword[32] = { 0 };//密码
char controlcode[32] = { 0 };//管理码
IMAGE img_user_login;
int login_register = 0;
//页面
enum Page
{
	page_firstpage,
	page_admin_Login,
	page_employee_Login,
	page_employee_Register,
	page_user_Login,
	page_user_Register,
	page_login_wrongpassword,
	page_register_repeatedname,
	page_register_notsamepassword,
	page_employee_register_wrongcontrolcode,
	page_admin_home
};
Page currentPage = page_firstpage;


void setPage(Page page)
{
	currentPage = page;
	GUI_ClearFocus();
	username[0] = '\0';
	password[0] = '\0';
	repassword[0] = '\0';
	controlcode[0] = '\0';
}//刷新页面同时清空输入框
void init(void* arg)//背景图片
{
	loadimage(&img_user_login, "res/pic/img.jpg",getwidth(),getheight());
}
void render(void* arg)//渲染
{
	putimage(0, 0, &img_user_login);
	//登录界面
	Rect rect = { (getwidth() - 340) / 2,128,340,370 };
	//1
	if (currentPage == page_firstpage)
	{
		login_register = 1;
		DK_SetFont(50, "楷体");//设置字体
		GUI_Label(R(rect.x, rect.y - 220, rect.w, rect.h), "驿站管理系统");//绘制标题
		DK_SetFont(30, "楷体");//设置字体
		GUI_Label(R(rect.x, rect.y - 170, rect.w, rect.h), "请选择你的身份");//绘制标题
		settextcolor(WHITE);
		saveStyle();//保存当前样式  主要是为了避免多条作用框相互影响
		ButtonStyle.background.color = RGB(24, 144, 255);//背景颜色
		ButtonStyle.background.hoverColor = RGB(31, 199, 253);//鼠标悬停时的颜色
		if (GUI_TextButton(GUID, R(rect.x, rect.y + 50, rect.w, 80), "管理员"))
		{
			currentPage = page_admin_Login;
		}
		else if (GUI_TextButton(GUID, R(rect.x, rect.y + 150, rect.w, 80), "员工"))
		{
			currentPage = page_employee_Login;
		}
		if (GUI_TextButton(GUID, R(rect.x, rect.y + 250, rect.w, 80), "用户"))
		{
			currentPage = page_user_Login;
		}
		restoreStyle();//恢复样式 

	}
	//2
	else if (currentPage == page_admin_Login)
	{
		login_register = 2;
		DK_SetFont(16, "楷体");//设置字体
		settextcolor(BLACK);
		if (GUI_TextButton(GUID, R(0.0, 10, 50), "返回", true))
		{
			setPage(page_firstpage);
		}
		DK_SetFont(20, "楷体");//设置字体
		GUI_Label(R(rect.x, rect.y + 80, rect.w, 20), "管理员登录");//绘制用户名

		//输入框
		GUI_Edit(GUID, R(rect.x, rect.y + 120, rect.w, 35), username, "请输入管理员名");//绘制用户名
		GUI_Edit(GUID, R(rect.x, rect.y + 170, rect.w, 35), password, "请输入密码", EchoMode::Password);//绘制密码


		//透明的文本按钮
		DK_SetFont(16, "楷体");//设置字体
		settextcolor(BLACK);
		//绘制登录按钮 
		settextcolor(WHITE);
		saveStyle();//保存当前样式  主要是为了避免多条作用框相互影响
		ButtonStyle.background.color = RGB(24, 144, 255);//背景颜色
		ButtonStyle.background.hoverColor = RGB(31, 199, 253);//鼠标悬停时的颜色
		if (GUI_TextButton(GUID, R(rect.x, rect.y + 250, rect.w, 35), "登录"))
		{
			setPage(page_login_wrongpassword);
		}
		restoreStyle();//恢复样式 
	}
	//3
	else if (currentPage == page_employee_Login)
	{
		login_register = 3;
		DK_SetFont(16, "楷体");//设置字体
		settextcolor(BLACK);
		if (GUI_TextButton(GUID, R(0.0, 10, 50), "返回", true))
		{
			setPage(page_firstpage);
		}
		DK_SetFont(20, "楷体");//设置字体
		GUI_Label(R(rect.x, rect.y + 80, rect.w, 20), "员工登录");//绘制用户名

		//输入框
		GUI_Edit(GUID, R(rect.x, rect.y + 120, rect.w, 35), username, "请输入电话号码");//绘制用户名
		GUI_Edit(GUID, R(rect.x, rect.y + 170, rect.w, 35), password, "请输入密码", EchoMode::Password);//绘制密码


		//透明的文本按钮
		DK_SetFont(16, "楷体");//设置字体
		settextcolor(BLACK);
		if (GUI_TextButton(GUID, R(rect.x + 200, rect.y + 200, 150, 35), "注册员工账户", true))
		{
			setPage(page_employee_Register);
		}
		//绘制登录按钮 
		settextcolor(WHITE);
		saveStyle();//保存当前样式  主要是为了避免多条作用框相互影响
		ButtonStyle.background.color = RGB(24, 144, 255);//背景颜色
		ButtonStyle.background.hoverColor = RGB(31, 199, 253);//鼠标悬停时的颜色
		if(GUI_TextButton(GUID, R(rect.x, rect.y + 250, rect.w, 35), "登录"))
		{
			setPage(page_login_wrongpassword);
		}
		restoreStyle();//恢复样式 
	}
	//4
	else if (currentPage == page_employee_Register)
	{
		login_register = 4;
		DK_SetFont(16, "楷体");//设置字体
		settextcolor(BLACK);
		if (GUI_TextButton(GUID, R(0.0, 10, 50), "返回", true))
		{
			setPage(page_firstpage);
		}
		DK_SetFont(20, "楷体");//设置字体
		GUI_Label(R(rect.x, rect.y + 80, rect.w, 20), "员工注册");//绘制用户名
		//输入框
		GUI_Edit(GUID, R(rect.x, rect.y + 120, rect.w, 35), username, "请输入电话号码");//绘制用户名
		GUI_Edit(GUID, R(rect.x, rect.y + 170, rect.w, 35), password, "请输入密码", EchoMode::Password);//绘制密码
		GUI_Edit(GUID, R(rect.x, rect.y + 220, rect.w, 35), password, "请再次输入密码", EchoMode::Password);//绘制密码
		GUI_Edit(GUID, R(rect.x, rect.y + 270, rect.w, 35), controlcode, "请输入管理码", EchoMode::Password);//绘制密码
		//透明的文本按钮
		DK_SetFont(16, "楷体");//设置字体
		settextcolor(BLACK);
		if (GUI_TextButton(GUID, R(rect.x + 200, rect.y + 300, 150, 35), "已有帐户？去登录", true))
		{
			setPage(page_employee_Login);
		}
		//绘制注册按钮 
		settextcolor(WHITE);
		saveStyle();//保存当前样式  主要是为了避免多条作用框相互影响
		ButtonStyle.background.color = RGB(24, 144, 255);//背景颜色
		ButtonStyle.background.hoverColor = RGB(31, 199, 253);//鼠标悬停时的颜色
		GUI_TextButton(GUID, R(rect.x, rect.y + 350, rect.w, 35), "注册");
		restoreStyle();//恢复样式 
	}
	//5
    else if (currentPage == page_user_Login)
	{
		login_register = 5;
		DK_SetFont(16, "楷体");//设置字体
		settextcolor(BLACK);
		if (GUI_TextButton(GUID, R(0.0, 10, 50), "返回", true))
		{
			setPage(page_firstpage);
		}
		DK_SetFont(20, "楷体");//设置字体
		GUI_Label(R(rect.x, rect.y + 80, rect.w, 20), "用户名登录");//绘制用户名

		//输入框
		GUI_Edit(GUID, R(rect.x, rect.y + 120, rect.w, 35), username, "请输入电话号码");//绘制用户名
		GUI_Edit(GUID, R(rect.x, rect.y + 170, rect.w, 35), password, "请输入密码", EchoMode::Password);//绘制密码


		//透明的文本按钮
		DK_SetFont(16, "楷体");//设置字体
		settextcolor(BLACK);
		if(GUI_TextButton(GUID, R(rect.x + 200, rect.y + 200, 150, 35), "尚无账户？去注册", true))
		{
			setPage(page_user_Register); 
		}
		//绘制登录按钮 
		settextcolor(WHITE);
		saveStyle();//保存当前样式  主要是为了避免多条作用框相互影响
		ButtonStyle.background.color = RGB(24, 144, 255);//背景颜色
		ButtonStyle.background.hoverColor = RGB(31, 199, 253);//鼠标悬停时的颜色
		GUI_TextButton(GUID, R(rect.x, rect.y + 250, rect.w, 35), "登录");
		restoreStyle();//恢复样式 
	}
	//6
	else if (currentPage == page_user_Register)
	{
		login_register = 6;
		DK_SetFont(16, "楷体");//设置字体
		settextcolor(BLACK);
		if (GUI_TextButton(GUID, R(0.0, 10, 50), "返回", true))
		{
			setPage(page_firstpage);
		}
		DK_SetFont(20, "楷体");//设置字体
		GUI_Label(R(rect.x, rect.y + 80, rect.w, 20), "用户名注册");//绘制用户名

		//输入框
		GUI_Edit(GUID, R(rect.x, rect.y + 120, rect.w, 35), username, "请输入电话号码");//绘制用户名
		GUI_Edit(GUID, R(rect.x, rect.y + 170, rect.w, 35), password, "请输入密码", EchoMode::Password);//绘制密码
		GUI_Edit(GUID, R(rect.x, rect.y + 220, rect.w, 35), password, "请再次输入密码", EchoMode::Password);//绘制密码

		//透明的文本按钮
		DK_SetFont(16, "楷体");//设置字体
		settextcolor(BLACK);
		if(GUI_TextButton(GUID, R(rect.x + 200, rect.y + 250, 150, 35), "已有帐户？去登录", true))
		{
			setPage(page_user_Login); 
		}
		//绘制注册按钮 
		settextcolor(WHITE);
		saveStyle();//保存当前样式  主要是为了避免多条作用框相互影响
		ButtonStyle.background.color = RGB(24, 144, 255);//背景颜色
		ButtonStyle.background.hoverColor = RGB(31, 199, 253);//鼠标悬停时的颜色
		GUI_TextButton(GUID, R(rect.x, rect.y + 300, rect.w, 35), "注册");
		restoreStyle();//恢复样式 
	}


	else if (currentPage == page_login_wrongpassword)
	{
		DK_SetFont(80, "楷体");//设置字体
		GUI_Label(R(rect.x, rect.y - 80, rect.w, rect.h), "密码错误");//绘制标题
		settextcolor(WHITE);
		saveStyle();//保存当前样式  主要是为了避免多条作用框相互影响
		ButtonStyle.background.color = RGB(24, 144, 255);//背景颜色
		ButtonStyle.background.hoverColor = RGB(31, 199, 253);//鼠标悬停时的颜色
		DK_SetFont(40, "楷体");//设置字体
		if (GUI_TextButton(GUID, R(rect.x, rect.y + 250, rect.w, 50), "返回"))
		{
			if (login_register == 2)
			{
				currentPage = page_admin_Login;
			}
			else if (login_register == 3)
			{
				currentPage = page_employee_Login;
			}
			else if (login_register == 5)
			{
				currentPage = page_user_Login;
			}
		}
		restoreStyle();//恢复样式 
	}
	else if (currentPage == page_register_notsamepassword)
	{
		DK_SetFont(80, "楷体");//设置字体
		GUI_Label(R(rect.x, rect.y - 80, rect.w, rect.h), "两次输入密码不相同");//绘制标题
		settextcolor(WHITE);
		saveStyle();//保存当前样式  主要是为了避免多条作用框相互影响
		ButtonStyle.background.color = RGB(24, 144, 255);//背景颜色
		ButtonStyle.background.hoverColor = RGB(31, 199, 253);//鼠标悬停时的颜色
		DK_SetFont(40, "楷体");//设置字体
		if (GUI_TextButton(GUID, R(rect.x, rect.y + 250, rect.w, 50), "返回"))
		{
			if (login_register == 2)
			{
				currentPage = page_user_Register;
			}
			else if (login_register == 3)
			{
				currentPage = page_employee_Register;
			}
		}
		restoreStyle();//恢复样式 
	}
	else if (currentPage == page_employee_register_wrongcontrolcode)
	{
		DK_SetFont(80, "楷体");//设置字体
		GUI_Label(R(rect.x, rect.y - 80, rect.w, rect.h), "管理码错误");//绘制标题
		settextcolor(WHITE);
		saveStyle();//保存当前样式  主要是为了避免多条作用框相互影响
		ButtonStyle.background.color = RGB(24, 144, 255);//背景颜色
		ButtonStyle.background.hoverColor = RGB(31, 199, 253);//鼠标悬停时的颜色
		DK_SetFont(40, "楷体");//设置字体
		if (GUI_TextButton(GUID, R(rect.x, rect.y + 250, rect.w, 50), "返回"))
		{
			currentPage = page_employee_Register;
		}
		restoreStyle();//恢复样式 
	}
	else if (currentPage == page_register_repeatedname)
	{
		DK_SetFont(80, "楷体");//设置字体
		GUI_Label(R(rect.x, rect.y - 80, rect.w, rect.h), "用户名已存在");//绘制标题
		settextcolor(WHITE);
		saveStyle();//保存当前样式  主要是为了避免多条作用框相互影响
		ButtonStyle.background.color = RGB(24, 144, 255);//背景颜色
		ButtonStyle.background.hoverColor = RGB(31, 199, 253);//鼠标悬停时的颜色
		DK_SetFont(40, "楷体");//设置字体
		if (GUI_TextButton(GUID, R(rect.x, rect.y + 250, rect.w, 50), "返回"))
		{
			if (login_register == 4)
			{
				currentPage = page_employee_Register;
			}
			else if (login_register == 6)
			{
				currentPage = page_user_Register;
			}
		}
		restoreStyle();//恢复样式 
		}
	//主页
	else if (currentPage == page_admin_home)
	{

	}


}


int main()
{
	createWindow(800, 600);//创建窗口
	setOnInit(init);//设置初始化回调函数

	setOnRender(render);//设置渲染回调函数

	messageLoop();//消息循环	
	return 0;
}