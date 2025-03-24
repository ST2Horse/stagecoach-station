#pragma once

struct user* creatUser(void);//创建用户链表哨兵(系统初始化时调用)

void addUser(struct user* head, char* telnum, char* name);//用户入链

void delUser(struct user* head, char* telnum); // 删除用户

struct user* findUser(struct user* head, char* telnum);//查找用户

struct shelf* creatShelf(void);//创建货架链表(系统初始化时调用)

int sumTime(char* t,struct report* rep); // 生成时间

char* addPack(struct user* head, struct shelf* shelf, struct report* rep, struct packageProp eProp,
				char* telnum, char* name);//  包裹入库

char* popPack(struct user* head, struct report* rep, char* Code, char* telnum); // 包裹出库

struct report* creatReport(void); // 日报链表系统初始化时使用

void dayPlus(struct report* head); // 天数加一