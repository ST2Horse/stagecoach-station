#include<stdio.h>
#include"gui_simple.h"
#include"struct.h"
#include<time.h>
#include"bash.h"
#include"package.cpp"
char username[32] = { 0 };//�绰����
char password[32] = { 0 };//����
char repassword[32] = { 0 };//����
char controlcode[32] = { 0 };//������
IMAGE img_user_login;
int login_register = 0;
//ҳ��
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
	page_admin_home,
	page_employee_home,
	page_user_home,
};
Page currentPage = page_firstpage;

void user_load(const char* file)
{
	FILE* fp = fopen(file, "r");//���ļ�(ֻ��)
	if (!fp)//�ļ�������
	{
		perror("user.txt not found");//���������Ϣ
		return;
	}
	char buf[BUFSIZ];//������
	//��ȡ��ͷ
	fgets(buf, BUFSIZ, fp);
	//��ȡ����
	while (!feof(fp))
	{
		regUser* reguser = (regUser*)calloc(1, sizeof(regUser));
		fscanf(fp, "%d\t%s\n", &reguser->telnum, reguser->password);
		
	}

	fclose(fp);
}


void setPage(Page page)
{
	currentPage = page;
	GUI_ClearFocus();
	username[0] = '\0';
	password[0] = '\0';
	repassword[0] = '\0';
	controlcode[0] = '\0';
}//ˢ��ҳ��ͬʱ��������
void init(void* arg)//����ͼƬ
{
	loadimage(&img_user_login, "res/pic/img.jpg",getwidth(),getheight());
	//user_load("res/txt/user.txt");
}
void render(void* arg)//��Ⱦ
{
	putimage(0, 0, &img_user_login);
	//��¼����
	Rect rect = { (getwidth() - 340) / 2,128,340,370 };
	//1
	if (currentPage == page_firstpage)
	{
		login_register = 1;
		DK_SetFont(50, "����");//��������
		GUI_Label(R(rect.x, rect.y - 220, rect.w, rect.h), "��վ����ϵͳ");//���Ʊ���
		DK_SetFont(30, "����");//��������
		GUI_Label(R(rect.x, rect.y - 170, rect.w, rect.h), "��ѡ��������");//���Ʊ���
		settextcolor(WHITE);
		saveStyle();//���浱ǰ��ʽ  ��Ҫ��Ϊ�˱���������ÿ��໥Ӱ��
		ButtonStyle.background.color = RGB(24, 144, 255);//������ɫ
		ButtonStyle.background.hoverColor = RGB(31, 199, 253);//�����ͣʱ����ɫ
		if (GUI_TextButton(GUID, R(rect.x, rect.y + 50, rect.w, 80), "����Ա"))
		{
			currentPage = page_admin_Login;
		}
		else if (GUI_TextButton(GUID, R(rect.x, rect.y + 150, rect.w, 80), "Ա��"))
		{
			currentPage = page_employee_Login;
		}
		if (GUI_TextButton(GUID, R(rect.x, rect.y + 250, rect.w, 80), "�û�"))
		{
			currentPage = page_user_Login;
		}
		restoreStyle();//�ָ���ʽ 

	}
	//2
	else if (currentPage == page_admin_Login)
	{
		login_register = 2;
		DK_SetFont(16, "����");//��������
		settextcolor(BLACK);
		if (GUI_TextButton(GUID, R(0.0, 10, 50), "����", true))
		{
			setPage(page_firstpage);
		}
		DK_SetFont(20, "����");//��������
		GUI_Label(R(rect.x, rect.y + 80, rect.w, 20), "����Ա��¼");//�����û���

		//�����
		GUI_Edit(GUID, R(rect.x, rect.y + 120, rect.w, 35), username, "���������Ա��");//�����û���
		GUI_Edit(GUID, R(rect.x, rect.y + 170, rect.w, 35), password, "����������", EchoMode::Password);//��������


		//͸�����ı���ť
		DK_SetFont(16, "����");//��������
		settextcolor(BLACK);
		//���Ƶ�¼��ť 
		settextcolor(WHITE);
		saveStyle();//���浱ǰ��ʽ  ��Ҫ��Ϊ�˱���������ÿ��໥Ӱ��
		ButtonStyle.background.color = RGB(24, 144, 255);//������ɫ
		ButtonStyle.background.hoverColor = RGB(31, 199, 253);//�����ͣʱ����ɫ
		if (GUI_TextButton(GUID, R(rect.x, rect.y + 250, rect.w, 35), "��¼"))
		{
			setPage(page_login_wrongpassword);
		}
		restoreStyle();//�ָ���ʽ 
	}
	//3
	else if (currentPage == page_employee_Login)
	{
		login_register = 3;
		DK_SetFont(16, "����");//��������
		settextcolor(BLACK);
		if (GUI_TextButton(GUID, R(0.0, 10, 50), "����", true))
		{
			setPage(page_firstpage);
		}
		DK_SetFont(20, "����");//��������
		GUI_Label(R(rect.x, rect.y + 80, rect.w, 20), "Ա����¼");//�����û���

		//�����
		GUI_Edit(GUID, R(rect.x, rect.y + 120, rect.w, 35), username, "������绰����");//�����û���
		GUI_Edit(GUID, R(rect.x, rect.y + 170, rect.w, 35), password, "����������", EchoMode::Password);//��������


		//͸�����ı���ť
		DK_SetFont(16, "����");//��������
		settextcolor(BLACK);
		if (GUI_TextButton(GUID, R(rect.x + 200, rect.y + 200, 150, 35), "ע��Ա���˻�", true))
		{
			setPage(page_employee_Register);
		}
		//���Ƶ�¼��ť 
		settextcolor(WHITE);
		saveStyle();//���浱ǰ��ʽ  ��Ҫ��Ϊ�˱���������ÿ��໥Ӱ��
		ButtonStyle.background.color = RGB(24, 144, 255);//������ɫ
		ButtonStyle.background.hoverColor = RGB(31, 199, 253);//�����ͣʱ����ɫ
		if(GUI_TextButton(GUID, R(rect.x, rect.y + 250, rect.w, 35), "��¼"))
		{
			setPage(page_login_wrongpassword);
		}
		restoreStyle();//�ָ���ʽ 
	}
	//4
	else if (currentPage == page_employee_Register)
	{
		login_register = 4;
		DK_SetFont(16, "����");//��������
		settextcolor(BLACK);
		if (GUI_TextButton(GUID, R(0.0, 10, 50), "����", true))
		{
			setPage(page_firstpage);
		}
		DK_SetFont(20, "����");//��������
		GUI_Label(R(rect.x, rect.y + 80, rect.w, 20), "Ա��ע��");//�����û���
		//�����
		GUI_Edit(GUID, R(rect.x, rect.y + 120, rect.w, 35), username, "������绰����");//�����û���
		GUI_Edit(GUID, R(rect.x, rect.y + 170, rect.w, 35), password, "����������", EchoMode::Password);//��������
		GUI_Edit(GUID, R(rect.x, rect.y + 220, rect.w, 35), password, "���ٴ���������", EchoMode::Password);//��������
		GUI_Edit(GUID, R(rect.x, rect.y + 270, rect.w, 35), controlcode, "�����������", EchoMode::Password);//��������
		//͸�����ı���ť
		DK_SetFont(16, "����");//��������
		settextcolor(BLACK);
		if (GUI_TextButton(GUID, R(rect.x + 200, rect.y + 300, 150, 35), "�����ʻ���ȥ��¼", true))
		{
			setPage(page_employee_Login);
		}
		//����ע�ᰴť 
		settextcolor(WHITE);
		saveStyle();//���浱ǰ��ʽ  ��Ҫ��Ϊ�˱���������ÿ��໥Ӱ��
		ButtonStyle.background.color = RGB(24, 144, 255);//������ɫ
		ButtonStyle.background.hoverColor = RGB(31, 199, 253);//�����ͣʱ����ɫ
		GUI_TextButton(GUID, R(rect.x, rect.y + 350, rect.w, 35), "ע��");
		restoreStyle();//�ָ���ʽ 
	}
	//5
    else if (currentPage == page_user_Login)
	{
		login_register = 5;
		DK_SetFont(16, "����");//��������
		settextcolor(BLACK);
		if (GUI_TextButton(GUID, R(0.0, 10, 50), "����", true))
		{
			setPage(page_firstpage);
		}
		DK_SetFont(20, "����");//��������
		GUI_Label(R(rect.x, rect.y + 80, rect.w, 20), "�û�����¼");//�����û���

		//�����
		GUI_Edit(GUID, R(rect.x, rect.y + 120, rect.w, 35), username, "������绰����");//�����û���
		GUI_Edit(GUID, R(rect.x, rect.y + 170, rect.w, 35), password, "����������", EchoMode::Password);//��������


		//͸�����ı���ť
		DK_SetFont(16, "����");//��������
		settextcolor(BLACK);
		if(GUI_TextButton(GUID, R(rect.x + 200, rect.y + 200, 150, 35), "�����˻���ȥע��", true))
		{
			setPage(page_user_Register); 
		}
		//���Ƶ�¼��ť 
		settextcolor(WHITE);
		saveStyle();//���浱ǰ��ʽ  ��Ҫ��Ϊ�˱���������ÿ��໥Ӱ��
		ButtonStyle.background.color = RGB(24, 144, 255);//������ɫ
		ButtonStyle.background.hoverColor = RGB(31, 199, 253);//�����ͣʱ����ɫ
		GUI_TextButton(GUID, R(rect.x, rect.y + 250, rect.w, 35), "��¼");
		restoreStyle();//�ָ���ʽ 
	}
	//6
	else if (currentPage == page_user_Register)
	{
		login_register = 6;
		DK_SetFont(16, "����");//��������
		settextcolor(BLACK);
		if (GUI_TextButton(GUID, R(0.0, 10, 50), "����", true))
		{
			setPage(page_firstpage);
		}
		DK_SetFont(20, "����");//��������
		GUI_Label(R(rect.x, rect.y + 80, rect.w, 20), "�û���ע��");//�����û���

		//�����
		GUI_Edit(GUID, R(rect.x, rect.y + 120, rect.w, 35), username, "������绰����");//�����û���
		GUI_Edit(GUID, R(rect.x, rect.y + 170, rect.w, 35), password, "����������", EchoMode::Password);//��������
		GUI_Edit(GUID, R(rect.x, rect.y + 220, rect.w, 35), password, "���ٴ���������", EchoMode::Password);//��������

		//͸�����ı���ť
		DK_SetFont(16, "����");//��������
		settextcolor(BLACK);
		if(GUI_TextButton(GUID, R(rect.x + 200, rect.y + 250, 150, 35), "�����ʻ���ȥ��¼", true))
		{
			setPage(page_user_Login); 
		}
		//����ע�ᰴť 
		settextcolor(WHITE);
		saveStyle();//���浱ǰ��ʽ  ��Ҫ��Ϊ�˱���������ÿ��໥Ӱ��
		ButtonStyle.background.color = RGB(24, 144, 255);//������ɫ
		ButtonStyle.background.hoverColor = RGB(31, 199, 253);//�����ͣʱ����ɫ
		GUI_TextButton(GUID, R(rect.x, rect.y + 300, rect.w, 35), "ע��");
		restoreStyle();//�ָ���ʽ 
	}


	else if (currentPage == page_login_wrongpassword)
	{
		DK_SetFont(80, "����");//��������
		GUI_Label(R(rect.x, rect.y - 80, rect.w, rect.h), "�������");//���Ʊ���
		settextcolor(WHITE);
		saveStyle();//���浱ǰ��ʽ  ��Ҫ��Ϊ�˱���������ÿ��໥Ӱ��
		ButtonStyle.background.color = RGB(24, 144, 255);//������ɫ
		ButtonStyle.background.hoverColor = RGB(31, 199, 253);//�����ͣʱ����ɫ
		DK_SetFont(40, "����");//��������
		if (GUI_TextButton(GUID, R(rect.x, rect.y + 250, rect.w, 50), "����"))
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
		restoreStyle();//�ָ���ʽ 
	}
	else if (currentPage == page_register_notsamepassword)
	{
		DK_SetFont(80, "����");//��������
		GUI_Label(R(rect.x, rect.y - 80, rect.w, rect.h), "�����������벻��ͬ");//���Ʊ���
		settextcolor(WHITE);
		saveStyle();//���浱ǰ��ʽ  ��Ҫ��Ϊ�˱���������ÿ��໥Ӱ��
		ButtonStyle.background.color = RGB(24, 144, 255);//������ɫ
		ButtonStyle.background.hoverColor = RGB(31, 199, 253);//�����ͣʱ����ɫ
		DK_SetFont(40, "����");//��������
		if (GUI_TextButton(GUID, R(rect.x, rect.y + 250, rect.w, 50), "����"))
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
		restoreStyle();//�ָ���ʽ 
	}
	else if (currentPage == page_employee_register_wrongcontrolcode)
	{
		DK_SetFont(80, "����");//��������
		GUI_Label(R(rect.x, rect.y - 80, rect.w, rect.h), "���������");//���Ʊ���
		settextcolor(WHITE);
		saveStyle();//���浱ǰ��ʽ  ��Ҫ��Ϊ�˱���������ÿ��໥Ӱ��
		ButtonStyle.background.color = RGB(24, 144, 255);//������ɫ
		ButtonStyle.background.hoverColor = RGB(31, 199, 253);//�����ͣʱ����ɫ
		DK_SetFont(40, "����");//��������
		if (GUI_TextButton(GUID, R(rect.x, rect.y + 250, rect.w, 50), "����"))
		{
			currentPage = page_employee_Register;
		}
		restoreStyle();//�ָ���ʽ 
	}
	else if (currentPage == page_register_repeatedname)
	{
		DK_SetFont(80, "����");//��������
		GUI_Label(R(rect.x, rect.y - 80, rect.w, rect.h), "�û����Ѵ���");//���Ʊ���
		settextcolor(WHITE);
		saveStyle();//���浱ǰ��ʽ  ��Ҫ��Ϊ�˱���������ÿ��໥Ӱ��
		ButtonStyle.background.color = RGB(24, 144, 255);//������ɫ
		ButtonStyle.background.hoverColor = RGB(31, 199, 253);//�����ͣʱ����ɫ
		DK_SetFont(40, "����");//��������
		if (GUI_TextButton(GUID, R(rect.x, rect.y + 250, rect.w, 50), "����"))
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
		restoreStyle();//�ָ���ʽ 
		}
	//��ҳ
	else if (currentPage == page_admin_home)
	{

	}


}


int main()
{
	createWindow(800, 600);//��������
	setOnInit(init);//���ó�ʼ���ص�����

	setOnRender(render);//������Ⱦ�ص�����

	messageLoop();//��Ϣѭ��	
	return 0;
}