#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "struct.h"

struct package* creatPack(void)//创建包裹链表哨兵
{
	struct package* head = (struct package*)malloc(sizeof(struct package));
	head->next = nullptr;
	return head;
};

void chainPack(struct package* head, struct packageProp eProp,char* telnum)//包裹入链
{
	struct package* p = head;
	struct package* q = (struct package*)malloc(sizeof(struct package));
	q->eProp = eProp;
	strcpy(q->telephone,telnum);
	q->next = p->next;
	p->next = q;
	return;
};

struct user* creatUser(void)//创建用户链表哨兵(系统初始化时调用)
{
	struct user* head = (struct user*)malloc(sizeof(struct user));
	head->next = nullptr;
	return head;
};

void addUser(struct user* head, char* telnum, char* name)//用户入链
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

struct user* findUser(struct user* head, char* telnum)//查找用户
{
	struct user* p = head->next;
	while (p)
	{
		if (strcmp(p->telnum,telnum))
			return p;
		p = p->next;
	}
	return nullptr;
};

void delUser(struct user* head, char* telnum) // 删除用户
{
	struct user* p = head->next;
	while (p->next)
	{
		if (strcmp(p->next->telnum, telnum))
		{
			struct user* q = p->next;
			p->next = q->next;
			free(q);
		}
		p = p->next;
	}
}

struct shelf* creatShelf(void)//创建货架链表系统初始化时调用
{
	struct shelf* head = (struct shelf*)malloc(sizeof(struct shelf));
	head->next = nullptr;
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

void allocate(struct packageProp& eProp,struct shelf* head)//分配货架
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
	int a = p->level5/25+1,b=(p->level5/5)%5+1,c=p->level5%5+1;
	eProp.level10 = a*100+b*10+c;
	eProp.Code[0] = 'A'+a-1;
	eProp.Code[1] = '0' + b;
	eProp.Code[2] = '0' + c;
	eProp.Code[3] = '-';
	p->space += eProp.width;
	for (i = 0;p->pack[i];i++);
	p->pack[i] = 1;
	eProp.code = i;
	eProp.Code[4] = i / 100;
	eProp.Code[5] = (i / 10)%10;
	eProp.Code[6] = i % 10;
	eProp.Code[7] = '\0';
	arrShelf(head);
	return;
};

int sumTime(char* t,struct report* rep)// 生成时间
{
	std::time_t now = std::time(nullptr);
	std::tm local_time;  // 创建 tm 结构体存储时间
	// 参数顺序：先时间戳指针，再 tm 结构体指针
	int ca[6];
	if (localtime_s(&local_time, &now) == 0) { // 检查是否成功
		ca[0] = (local_time.tm_year + 1900) % 100;// 年
		ca[1] = local_time.tm_mon + 1;// 月
		ca[2] = local_time.tm_mday+rep->next->day;// 日
		ca[3] = local_time.tm_hour;// 时
		ca[4] = local_time.tm_min;// 分
		ca[5] = local_time.tm_sec;// 秒
	}
	else {
		printf("failed to load time");
		return 0;
	}
	t[2] = '-';t[5] = '-';t[8] = ' ';t[11] = ':';t[14] = ':';
	for (int i = 0;i < 17;i += 3)
	{
		t[i] = '0'+ca[i / 3] / 10;
		t[i + 1] = '0'+ca[i / 3] % 10;
	}
	t[17] = '\0';
	return now+rep->day*86400;
}

void orgnizePack(struct packageProp& eProp,struct shelf* head,struct report* rep)//整理包裹信息
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
	}
	allocate(eProp, head);
	eProp.sec=sumTime(eProp.time,rep);
	return;
}

char* addPack(struct user* head,struct shelf* shelf, struct report* rep , struct packageProp eProp, char* telnum, char* name)//包裹入库
{
	struct user* p = findUser(head, telnum);
	if (!p)
	{
		addUser(head, telnum, name);
		p = head->next;
	}
	orgnizePack(eProp, shelf,rep);
	struct package* q = p->pPack;
	chainPack(q, eProp,telnum);
	rep->pop++;
	return eProp.Code;
}

struct package* findPack(struct user* head, char* Code) // 通过编码找包裹前驱
{
	struct user* p = head->next;
	if (!p) return nullptr;
	struct package* q = p->pPack;
	while (p)
	{
		while (q->next)
		{
			if (strcmp(Code, q->next->eProp.Code)) return q;
			q = q->next;
		}
	}
	return nullptr;
}

void delPack(struct user* head,char* Code)
{
	struct package* p = findPack(head, Code);
	if (!p)
	{
		return;
	}
	struct package* q = p->next;
	p->next = q->next;
	free(q);
	return;
}

char* popPack(struct user* head,struct report* rep, char* Code, char* telnum)
{
	char ptime[13];
	int nptime=sumTime(ptime,rep);
	struct package* p = findPack(head, Code);
	if (!p)
	{
		printf("包裹不存在");
		return ptime;
	}
	if (p->next->eProp.prop == 4)
	{
		printf("包裹被误领");
		return ptime;
	}
	if (strcmp(telnum, p->next->telephone))
	{
		struct package* q = p->next;
		struct packageProp prop = q->eProp;
		struct user* u = findUser(head, telnum);
		if (nptime - prop.sec < 86400) u->credit += 5;
		if (nptime - prop.sec > 86400 * 5) u->credit -= 50;
		p->next = q->next;
		free(q);
		rep->next->pop++;
		return ptime;
	}
	else
	{
		p->next->eProp.prop = 4;
		rep->next->problem++;
		return ptime;
	}
}

struct report* creatReport(void) // 系统初始化时使用
{
	struct report* head = (struct report*)malloc(sizeof(struct report));
	head->next = nullptr;
	head->day = -1;
	return head;
}

void dayPlus(struct report* head) // 天数加一
{
	struct report* p = head;
	struct report* q = (struct report*)malloc(sizeof(struct report));
	q->pop = 0;
	q->push = 0;
	q->problem = 0;
	q->day = ++p->day;

	q->next = p->next;
	p->next = q;
	return;
}

