#pragma once

struct user* creatUser(void);//�����û������ڱ�(ϵͳ��ʼ��ʱ����)

void addUser(struct user* head, char* telnum, char* name);//�û�����

void delUser(struct user* head, char* telnum); // ɾ���û�

struct user* findUser(struct user* head, char* telnum);//�����û�

struct shelf* creatShelf(void);//������������(ϵͳ��ʼ��ʱ����)

int sumTime(char* t,struct report* rep); // ����ʱ��

char* addPack(struct user* head, struct shelf* shelf, struct report* rep, struct packageProp eProp,
				char* telnum, char* name);//  �������

char* popPack(struct user* head, struct report* rep, char* Code, char* telnum); // ��������

struct report* creatReport(void); // �ձ�����ϵͳ��ʼ��ʱʹ��

void dayPlus(struct report* head); // ������һ