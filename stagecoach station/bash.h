void chainPack(struct package* head, struct packageProp eProp);//包裹入链
struct user* creatuser(void);//创建用户链表哨兵(系统初始化时调用)
struct user* creatuser(void);//创建用户链表哨兵(系统初始化时调用)
void adduser(struct user* head, int telnum, char* name);//用户入链
struct user* findUser(struct user* head, int telnum);//查找用户
void orgnizePack(struct packageProp& eProp);//整理包裹信息
void addPack(struct user* head, struct packageProp eProp, int telnum, char* name);//包裹入库