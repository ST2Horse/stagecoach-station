#pragma once
struct user//用户链表
{
	int telnum;//电话号码
	char name[20];//姓名
	int credit;//信用分
	struct user* optUser[3];//三位授权人的指针数组（用栈维护,可用于访问授权包裹）
	struct package* pPack;//自己的包裹链表
	struct user* next;
};
struct packageProp//包裹信息
{
	int prop;//属性（0表示普通，1表示小袋装，2表示中袋装，3表示大袋装，4表示冷藏，5表示异常）
	int level;//货架编号及层数(三个数字，第一个数字1~6表示货架区域A B C D E X(E为冷藏区,X是放外面），
								//第二个数字1~5表示货架编号，第三个数字表示层数（共四层）
	int code;//包裹四位编号（随机生成的在不同区域不重复编号）
	/*两个属性最后合成包裹编码（如A1-4-0000）*/
	int length;//最长边
	int height;//高
	int width;//最短边 
	/*这三项在入库时填写后需要动态调整一次避免乱填 单位cm，若prop=1~3则都赋值为固定数值*/
};
struct package//自己的包裹链表
{
	struct packageProp eProp;//包裹信息
	struct package* next;
};

struct regUser//已注册用户链表
{
	int telnum;//电话号码
	char password[20];//密码
	/*注册时只需要这两个信息*/
	char name[20];//姓名（若未遍历到电话号码说明从未有包裹寄入驿站，需要填写姓名，若遍历到则同步）
	struct user* pUser;//指向用户具体信息（若从未有包裹寄入驿站则自动生成用户节点插入user链表，并同步姓名）
	int level;//用户等级（0表示普通用户，1表示VIP用户）
	struct regUser* next;
};
struct admin//管理员和员工链表
{
	char name[20];//姓名
	char password[20];//密码
	struct admin* next;
};
int shelf[5][5];//货架占用空间（第一个维度表示货架区域，第二个维度表示货架编号，总长3m）
