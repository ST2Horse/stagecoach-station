#pragma once
#include <time.h>
struct user // 用户链表
{
    char telnum[12]; // 电话号码
    char password[20]; // 密码
    /* 注册时只需要这两个信息 */
    char name[20]; // 姓名
    int credit; // 信用分
    int level; // 用户等级（0表示普通用户，1表示VIP用户）
    struct user* optUser[3]; // 三位授权人的指针数组（用栈维护,可用于访问授权包裹）
    struct package* pPack; // 自己的包裹链表
    struct user* next;
};

struct packageProp // 包裹信息
{
    int prop; // 属性（0表示普通，1表示小袋装，2表示中袋装，3表示大袋装，4表示异常）
    char time[13];// 入库时间
    int level10; /* 货架编号及层数(三个数字，第一个数字1~5表示货架区域A B C D E (值为0表示X，是放外面），
                    第二个数字1~5表示货架编号，第三个数字表示层数（共五层，一层40cm） */
    int code; // 包裹三位编号（顺序生成的在不同层不重复编号）
    /* 两个属性最后合成包裹编码（如A1-4000） */
    int length; // 最长边
    int height; // 高
    int width; // 最短边 
    /* 这三项在入库时填写后需要动态调整一次避免乱填 单位cm，若prop=1~3则都赋值为固定数值 */
};

struct package // 自己的包裹链表
{
    struct packageProp eProp; // 包裹信息
    char telephone[12];
    struct package* next;
};

struct employee // 员工链表
{
    char adminCode[20]; // 管理码
    char password[20]; // 密码
    int telephone;
    struct employee* next;
};

struct shelf // 货架链表
{
    int space; // 已占用空间
    int level5; // 货架编号及层数(1~125)
    int pack[1000] = { 0 };
    struct shelf* next;
};
