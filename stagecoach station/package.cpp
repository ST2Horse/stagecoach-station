#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

struct package* creatPack(void)//�������������ڱ�
{
	struct package* head = (struct package*)malloc(sizeof(struct package));
	head->next = NULL;
	return head;
};

void chainPack(struct package* head, struct packageProp eProp)//��������
{
	struct package* p = head;
	struct package* q = (struct package*)malloc(sizeof(struct package));
	q->eProp = eProp;
	q->next = p->next;
	p->next = q;
};

struct user* creatUser(void)//�����û������ڱ�(ϵͳ��ʼ��ʱ����)
{
	struct user* head = (struct user*)malloc(sizeof(struct user));
	head->next = NULL;
	return head;
};

void addUser(struct user* head, int telnum, char* name)//�û�����
{
	struct user* p = head;
	struct user* q = (struct user*)malloc(sizeof(struct user));
	q->telnum = telnum;
	strcpy(q->name, name);
	q->password[0] = NULL;
	q->credit = 100;
	q->level = 0;
	q->optUser[0] = NULL;
	q->optUser[1] = NULL;
	q->optUser[2] = NULL;
	q->pPack = creatPack();
	q->next = p->next;
	p->next = q;
};

struct user* findUser(struct user* head, int telnum)//�����û�
{
	struct user* p = head->next;
	while (p)
	{
		if (p->telnum == telnum)
			return p;
		p = p->next;
	}
	return NULL;
};

struct shelf* creatShelf(void)//������������ϵͳ��ʼ��ʱ����)
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
}
void allocate(struct packageProp& eProp,struct shelf* head)//�������
{
	struct shelf* p = head->next;
	if(eProp.length>40||eProp.length>p->space)
	{
		eProp.level10 = 0��
		return;
	}
	int a=p->level5
	eProp->level10=p->
	arrShelf(head);
};

void orgnizePack(struct packageProp& eProp)//���������Ϣ
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
		allocate(eProp);
	}
};

void addPack(struct user* head, struct packageProp eProp, int telnum, char* name)//�������
{
	struct user* p = findUser(head, telnum);
	if (!p)
	{
		addUser(head, telnum, name);
		p = head->next;
	}
	struct package* q = p->pPack;
	chainPack(q, eProp);
};