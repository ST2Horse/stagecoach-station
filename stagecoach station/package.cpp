#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "struct.h"

struct user* creatUser(void);//�����û������ڱ�(ϵͳ��ʼ��ʱ����)
void addUser(struct user* head, char* telnum, char* name);//�û�����
struct user* findUser(struct user* head, char* telnum);//�����û�
struct shelf* creatShelf(void);//������������(ϵͳ��ʼ��ʱ����)
void addPack(struct user* head, struct shelf* shelf, struct packageProp eProp, char* telnum, char* name);//�������

struct package* creatPack(void)//�������������ڱ�
{
	struct package* head = (struct package*)malloc(sizeof(struct package));
	head->next = NULL;
	return head;
};

void chainPack(struct package* head, struct packageProp eProp,char* telnum)//��������
{
	struct package* p = head;
	struct package* q = (struct package*)malloc(sizeof(struct package));
	q->eProp = eProp;
	strcpy(q->telephone,telnum);
	q->next = p->next;
	p->next = q;
	return;
};

struct user* creatUser(void)//�����û������ڱ�(ϵͳ��ʼ��ʱ����)
{
	struct user* head = (struct user*)malloc(sizeof(struct user));
	head->next = NULL;
	return head;
};

void addUser(struct user* head, char* telnum, char* name)//�û�����
{
	struct user* p = head;
	struct user* q = (struct user*)malloc(sizeof(struct user));
	strcpy(q->telnum, telnum);
	strcpy(q->name, name);
	q->password[0] = '\0';
	q->credit = 100;
	q->level = 0;
	q->optUser[0] = nullptr;
	q->optUser[1] = nullptr;
	q->optUser[2] = nullptr;
	q->pPack = creatPack();
	q->next = p->next;
	p->next = q;
	return;
};

struct user* findUser(struct user* head, char* telnum)//�����û�
{
	struct user* p = head->next;
	while (p)
	{
		if (strcmp(p->telnum,telnum))
			return p;
		p = p->next;
	}
	return NULL;
};

struct shelf* creatShelf(void)//������������ϵͳ��ʼ��ʱ����
{
	struct shelf* head = (struct shelf*)malloc(sizeof(struct shelf));
	head->next = NULL;
	for (int i = 1; i <= 125;i++)
	{
		struct shelf* p = (struct shelf*)malloc(sizeof(struct shelf));
		p->space = 0;
		p->level5 = i;
		p->next = head->next;
		head->next = p;
	}
	return head;
};

void arrShelf(struct shelf* head)
{
	struct shelf* p = head->next, * p0 = head, * q = p;
	while (q->next!=nullptr&&p->space < q->next->space)
	{
		q = q->next;
	}
	p0->next = p->next;
	p->next = q->next;
	q->next = p;
	return;
}

void allocate(struct packageProp& eProp,struct shelf* head)//�������
{
	struct shelf* p = head->next;int i;
	if(eProp.length>40||eProp.length>150-p->space)
	{
		eProp.level10 = 0;
		for (i = 0;head->pack[i];i++);
		head->pack[i] = 1;
		eProp.code = i;
		return;
	}
	int a = p->level5/25+1,b=p->level5/5+1,c=p->level5%5+1;
	eProp.level10 = a*100+b*10+c;
	p->space += eProp.width;
	for (i = 0;p->pack[i];i++);
	p->pack[i] = 1;
	eProp.code = i;
	arrShelf(head);
	return;
};

void sumTime(struct packageProp& eProp)// ����ʱ��
{
	std::time_t now = std::time(nullptr);
	std::tm local_time;  // ���� tm �ṹ��洢ʱ��
	// ʹ�� localtime_s ��� localtime������˳����ʱ���ָ�룬�� tm �ṹ��ָ�룩
	int ca[6];
	if (localtime_s(&local_time, &now) == 0) { // ����Ƿ�ɹ�
		ca[0] = (local_time.tm_year + 1900) % 100;// ��
		ca[1] = local_time.tm_mon + 1;// ��
		ca[2] = local_time.tm_mday;// ��
		ca[3] = local_time.tm_hour;// ʱ
		ca[4] = local_time.tm_min;// ��
		ca[5] = local_time.tm_sec;// ��
	}
	else {
		printf("failed to load time");
		return;
	}
	char* t = eProp.time;
	for (int i = 0;i < 12;i += 2)
	{
		t[i] = ca[i / 2] / 10;
		t[i + 1] = ca[i / 2] % 10;
	}
	t[12] = '\0';
	return;
}

void orgnizePack(struct packageProp& eProp,struct shelf* head)//���������Ϣ
{
	if (eProp.prop == 1)
	{
		eProp.length = 10;
		eProp.height = 10;
		eProp.width = 10;
	}
	else if (eProp.prop == 2)
	{
		eProp.length = 25;
		eProp.height = 25;
		eProp.width = 25;
	}
	else if (eProp.prop == 3)
	{
		eProp.length = 40;
		eProp.height = 40;
		eProp.width = 40;
	}
	else
	{
		int temp;
		if (eProp.length < eProp.width)
		{
			temp = eProp.length;
			eProp.length = eProp.width;
			eProp.width = temp;
		}
		if (eProp.length < eProp.height)
		{
			temp = eProp.length;
			eProp.length = eProp.height;
			eProp.height = temp;
		}
		if (eProp.width < eProp.height)
		{
			temp = eProp.width;
			eProp.width = eProp.height;
			eProp.height = temp;
		}
		allocate(eProp, head);
		sumTime(eProp);
	}
	return;
}

void addPack(struct user* head,struct shelf* shelf, struct packageProp eProp, char* telnum, char* name)//�������
{
	struct user* p = findUser(head, telnum);
	if (!p)
	{
		addUser(head, telnum, name);
		p = head->next;
	}
	orgnizePack(eProp, shelf);
	struct package* q = p->pPack;
	chainPack(q, eProp,telnum);
	return;
}
