#include "TimManange.h"


// 另外一种类，管理类
static unsigned char TimeManagerAdd( TimeManager* base, TimeRegObj *obj);
void TimeManagerCreate( TimeManager* base){
	unsigned char i_u8 = 0,len_u8 = 0;
	if(base != 0){
		len_u8 = sizeof(base->TimeRegObjHandle)/sizeof(base->TimeRegObjHandle[0]);
		for(i_u8 = 0;i_u8 < len_u8;i_u8 ++){
			base->TimeRegObjCountMs_32[i_u8] = 0;//客房时间调整
			base->TimeRegObjCountMsTemp_32[i_u8] = 0;//客房时间调整
			base->TimeRegObjHandle[i_u8].TimeReg_32 = 0;//客房打扫
			base->TimeRegObjHandle[i_u8].funcdoCount = 0;//客房打扫
			base->TimeRegObjHandle[i_u8].LoopCan = 0;//客房打扫
			base->TimeRegObjHandle[i_u8].funptr = 0;//客人不再入住			
		}
	}	
}
void TimeManagerScanTimeMs( TimeManager* base,int  freq_ms_32){//由中断触发执行
	unsigned char i_u8 = 0,len_u8 = 0;
	if(base != 0){
		len_u8 = sizeof(base->TimeRegObjHandle)/sizeof(base->TimeRegObjHandle[0]);
		for(i_u8 = 0;i_u8 < len_u8;i_u8 ++){
			if(0 == base->TimeRegObjHandle[i_u8].funptr) continue;//管理会员区域没有被注册过
			if(base->TimeRegObjCountMs_32[i_u8] < 0) continue;
			base->TimeRegObjCountMs_32[i_u8]-=freq_ms_32;
			
		}
	}	
}
void TimeManagerScan( TimeManager* base){
	unsigned char i_u8 = 0,len_u8 = 0;
	if(base == 0)return;
	len_u8 = sizeof(base->TimeRegObjHandle)/sizeof(base->TimeRegObjHandle[0]);
	for(i_u8 = 0;i_u8 < len_u8;i_u8 ++){
		if(0 != (base->TimeRegObjHandle[i_u8].funptr)){
			//重新装值，从会员类的值里得到数据
			if(base->TimeRegObjCountMs_32[i_u8] > 0) continue;
			if('k' == base->TimeRegObjHandle[i_u8].LoopCan){
				((TimeRegHandleFptr)(base->TimeRegObjHandle[i_u8].funptr))();//执行函数
				base->TimeRegObjCountMs_32[i_u8] = base->TimeRegObjHandle[i_u8].TimeReg_32;
			}else if('n' == base->TimeRegObjHandle[i_u8].LoopCan){
				if(0 == base->TimeRegObjHandle[i_u8].funcdoCount) continue;//会员执行次数为0
				if(0 != (TimeRegHandleFptr*)(base->TimeRegObjHandle[i_u8].funptr)){//会员的执行函数存在
					((TimeRegHandleFptr)(base->TimeRegObjHandle[i_u8].funptr))();
					base->TimeRegObjHandle[i_u8].funcdoCount--;
					base->TimeRegObjCountMs_32[i_u8] = base->TimeRegObjHandle[i_u8].TimeReg_32;
				}
				if(0 == base->TimeRegObjHandle[i_u8].funcdoCount){
					TimeManagerDelete(base,(TimeRegObj *)&(base->TimeRegObjHandle[i_u8]));//属于多次性的会员
				}
			}
		} 
	}
}
TimeRegObj *TimeManagerCheck( TimeManager* base, TimeRegObj *obj){//检测一个会员是不是注册过，否则返回0
	unsigned char i_u8 = 0,len_u8 = 0;
	if((base != 0) && (0 != obj)){
		len_u8 = sizeof(base->TimeRegObjHandle)/sizeof(base->TimeRegObjHandle[0]);
		for(i_u8 = 0;i_u8 < len_u8;i_u8 ++){
			if(base->TimeRegObjHandle[i_u8].funptr == obj->funptr){//该会员已经注册过了
				return &base->TimeRegObjHandle[i_u8];//r是已经注册好了
			}
		}
		return 0;
	}
	return 0;
}
unsigned char TimeManagerCheckTimeIsOver( TimeManager* base, TimeRegObj *obj){
	unsigned char i_u8 = 0,len_u8 = 0;
	if(base == 0)return 'n';
	if(obj == 0)return 'n';
	len_u8 = sizeof(base->TimeRegObjHandle)/sizeof(base->TimeRegObjHandle[0]);
	if(0 != TimeManagerCheck(base,obj)){//该会员已经注册过了
		for(i_u8 = 0;i_u8 < len_u8;i_u8 ++){
			if(base->TimeRegObjHandle[i_u8].funptr == obj->funptr){//该会员已经注册过了
				if('k' == base->TimeRegObjHandle[i_u8].LoopCan){//永久会员
					if(base->TimeRegObjCountMs_32[i_u8] > 0){
						return 'n';
					}else{
						return 'k';
					}
				}else if('n' == base->TimeRegObjHandle[i_u8].LoopCan){
					if(0 == base->TimeRegObjHandle[i_u8].funcdoCount) return 'n';//会员执行次数为0
					return 'k';
				}
			}
		}
	}
	return 'n';
}
static unsigned char TimeManagerAdd( TimeManager* base, TimeRegObj *obj){//增加一个会员
	unsigned char i_u8 = 0,len_u8 = 0;
	if((base != 0) && (0 != obj)){
		len_u8 = sizeof(base->TimeRegObjHandle)/sizeof(base->TimeRegObjHandle[0]);
		for(i_u8 = 0;i_u8 < len_u8;i_u8 ++){
			if(base->TimeRegObjHandle[i_u8].funptr == obj->funptr){//该会员已经注册过了
				return 'r';//r是已经注册好了
			}
			if(0 == base->TimeRegObjHandle[i_u8].funptr){
				base->TimeRegObjHandle[i_u8].funptr = obj->funptr;
				base->TimeRegObjHandle[i_u8].LoopCan = obj->LoopCan;
				base->TimeRegObjHandle[i_u8].TimeReg_32 = obj->TimeReg_32;
				base->TimeRegObjCountMs_32[i_u8] = obj->TimeReg_32;
				return 'k';
			}
		}
		printf("bad !!! in time\n");while(1);
		return 'n';
	}
	return 'n';
}
void TimeManagerDelete( TimeManager* base, TimeRegObj *obj){//从管理员的会员列表中删除删除一个会员
	unsigned char i_u8 = 0,len_u8 = 0;
	if(0 == base){return;}
	if(0 == obj){return;}
	len_u8 = sizeof(base->TimeRegObjHandle)/sizeof(base->TimeRegObjHandle[0]);
	for(i_u8 = 0;i_u8 < len_u8;i_u8 ++){
		if(&base->TimeRegObjHandle[i_u8] == obj){//该会员已经注册过了
			base->TimeRegObjHandle[i_u8].funptr = 0;
			base->TimeRegObjHandle[i_u8].LoopCan = 0;
			base->TimeRegObjCountMs_32[i_u8] = 0;
			base->TimeRegObjCountMsTemp_32[i_u8] = 0;
		}
	}
}
// 删除旗下所有的会员
void TimeManagerDelAll(TimeManager* base){
	unsigned char i_u8 = 0,len_u8 = 0;
	if(base == 0)return;
	len_u8 = sizeof(base->TimeRegObjHandle)/sizeof(base->TimeRegObjHandle[0]);
	for(i_u8 = 0;i_u8 < len_u8;i_u8 ++){
		if(0 != base->TimeRegObjHandle[i_u8].funptr){//该会员已经注册过了
			TimeRegObjDestory(base,&base->TimeRegObjHandle[i_u8]);
			base->TimeRegObjHandle[i_u8].funptr = 0;
			base->TimeRegObjCountMs_32[i_u8] = 0;
		}
	}
}
void TimeManagerUpdataTime( TimeManager* base, TimeRegObj *obj,int time_32){
	unsigned char i_u8 = 0,len_u8 = 0;
	if(0 == base){return;}
	if(0 == obj){return;}
	len_u8 = sizeof(base->TimeRegObjHandle)/sizeof(base->TimeRegObjHandle[0]);
	for(i_u8 = 0;i_u8 < len_u8;i_u8 ++){
		if(&base->TimeRegObjHandle[i_u8] == obj){//该会员已经注册过了	
			base->TimeRegObjCountMs_32[i_u8] = time_32;//客房时间调整
			base->TimeRegObjCountMsTemp_32[i_u8] = 0;//客房时间调整
		}
	}	
}
static int mallocok_count = 0;
static int mallocer_count = 0;
static int free_count = 0;
// 会员类
//添加一个会员，如果这个会员已经存在，返回的是原先的值，默认会员是永久类型
TimeRegObj * TimeRegObjCreate(void (*Handle)(),int time_32, TimeManager *Manager){//Manager是会员的管理员
	TimeRegObj obj;
	TimeRegObj * ret_obj;
	unsigned char res = 0;
	obj.funptr = (TimeRegHandleFptr)Handle;
	obj.TimeReg_32 = time_32;
	obj.LoopCan = 'k';
	if((0 == Handle) || (0 == Manager)){
		return 0;
	}
	res = TimeManagerAdd(Manager,&obj);
	if(('r' == res) || ('k' == res)){//添加成功，会员注册过,或者注册成功
		ret_obj = TimeManagerCheck(Manager,&obj);
		return ret_obj;
	}else{
		//添加失败
		// while(1){;}
		return 0;
	}
}
//解除会员关系会员的消失
void TimeRegObjDestory( TimeManager* base, TimeRegObj *obj){
	if(0 != obj){
		if(base == 0)return;
		TimeManagerDelete(base,obj);//记账本清理
		obj = 0;//房卡归还
	}
}
// 更新会员的时间，包括管理员的时间
void TimeRegObjUpdataTime(TimeManager *Manager,TimeRegObj *obj,int time_32){
	if(0 == obj) return ;
	if(0 == Manager) return;
	obj->TimeReg_32 = time_32;
	TimeManagerUpdataTime(Manager,obj,time_32);
}
// 设置一个会员是普通会员，只能运行count_32次
void TimeRegObjRunCount( TimeRegObj *obj,int count_32){
	if(0 == obj) return ;
	obj->funcdoCount = count_32;
	obj->LoopCan = 'n';
}
// 会员