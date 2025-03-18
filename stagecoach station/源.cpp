#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

struct package* creatpack(void)//创建包裹链表哨兵
{
	struct package* head = (struct package*)malloc(sizeof(struct package));
	head->next = NULL;
	return head;
};

void chainPack(struct package* head, struct packageProp eProp)//包裹入链
{
	struct package* p = head;
	struct package* q = (struct package*)malloc(sizeof(struct package));
	q->eProp = eProp;
	q->next = p->next;
	p->next = q;
};

struct user* creatuser(void)//创建用户链表哨兵(系统初始化时调用)
{
	struct user* head = (struct user*)malloc(sizeof(struct user));
	head->next = NULL;
	return head;
};

void adduser(struct user* head, int telnum, char* name)//用户入链
{
	struct user* p = head;
	struct user* q = (struct user*)malloc(sizeof(struct user));
	q->telnum = telnum;
	strcpy(q->name, name);
	q->credit = 100;
	q->optUser[0] = NULL;
	q->optUser[1] = NULL;
	q->optUser[2] = NULL;
	q->pPack = creatpack();
	q->next = p->next;
	p->next = q;
};

struct user* findUser(struct user* head, int telnum)//查找用户
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

void orgnizePack(struct packageProp &eProp)//整理包裹信息
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
		
	}
};

void addPack(struct user* head, struct packageProp eProp, int telnum, char* name)//包裹入库
{
	struct user* p = findUser(head, telnum);
	if (!p)
	{
		adduser(head, telnum, name);
		p = head->next;
	}
	struct package* q = p->pPack;
	chainPack(q, eProp);
};