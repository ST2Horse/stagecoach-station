#pragma once
struct user//用户链表
{
	int telnum;//电话号码
	char name[20];//姓名
	int credit;//信用分
	struct user* optUser[3];//三位授权人的指针数组
	struct ownpackage* pownPack;//自己的包裹链表
	struct optpackage* poptPack;//授权可取的包裹链表
	struct user* next;
};
struct ownPackage//自己的包裹链表
{
	struct packageProp eProp;//包裹信息
	struct ownPackage* next;
};
struct optPackage//授权可取的包裹链表
{
	struct packageProp eProp;//包裹信息
	struct optPackage* next;
};
struct packageProp//包裹信息
{
	int prop;//属性（0表示普通，1表示小袋装，2表示中袋装，3表示大袋装，4表示冷藏，5表示异常）
	char shore;//货架区域（A B C D E X)
	int nomLevel;//货架编号及层数(两个数字，第一个数字1~9表示货架编号，第二个数字1~4表示层数）
	int code;//包裹四位编号（随机生成的不重复编号）
	/*三个属性最后合成包裹编码（如A1-4-0000）*/
	float length;//最长边
	float height;//高
	float width;//最短边 
	/*这三项在入库时填写后需要动态调整一次避免乱填，若prop=1~3则都赋值为固定数值*/
	float weight;//重量
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
