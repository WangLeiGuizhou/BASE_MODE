#ifndef __TimeServer
#define __TimeServer
#include <stdio.h>
// typedef void (*counttoendhandle)(void);
typedef struct TimeServer
{	
	unsigned char sta_;
	int count_32sz[20];//
	unsigned char RegIndex_u16[20];
	// counttoendhandle CountToEndHandle[4]
}TimeServer;
// void TimeServerCreate( TimeServer *base);
// unsigned char TimeServerAdd( TimeServer *base,unsigned int TimeServerEndTime_ms_u32);
// void TimeServerDestroy( TimeServer *base,unsigned char RegIndex_u8);
// // void TimeServerScan( TimeServer *base,unsigned int ScanTimems_u32);
// unsigned char TimeServerEnd( TimeServer *base,unsigned char RegIndex_u8);
// 另外一种类
// 会员类 有多个
typedef void (*TimeRegHandleFptr)(void);//会员要执行的函数指针
typedef struct TimeRegObj{  
    TimeRegHandleFptr funptr;
    int TimeReg_32;
    int funcdoCount;
    char LoopCan;
}TimeRegObj;


// 另外一种类，管理类非常强大的类
typedef void (*TimeHandle)(void);
typedef struct TimeManager TimeManager;
typedef struct TimeRegObj TimeRegObj;
typedef struct TimeManager{
    TimeRegObj TimeRegObjHandle[15];//不再使用门牌号码，原因是处理器环境没有MMU设备支持
    int  TimeRegObjCountMs_32[15];
    int  TimeRegObjCountMsTemp_32[15];
    // unsigned char TimeMangerRegObjSta[15];//执行状态
} TimeManager;

void TimeManagerCreate( TimeManager* base);
void TimeManagerScanTimeMs( TimeManager* base,int  freq_ms_32);//放入中断中
void TimeManagerScan( TimeManager* base);//放在主循环里
TimeRegObj *TimeManagerCheck( TimeManager* base, TimeRegObj *obj);//返回找到的地址 没有找到是0
void TimeManagerDelAll( TimeManager* base);
void TimeManagerUpdataTime( TimeManager* base, TimeRegObj *obj,int time_32);
unsigned char TimeManagerCheckTimeIsOver( TimeManager* base, TimeRegObj *obj);

TimeRegObj * TimeRegObjCreate(void (*Handle)(),int time_32, TimeManager *Manager);
void TimeRegObjDestory( TimeManager* base, TimeRegObj *obj);
void TimeRegObjUpdataTime(TimeManager *Manager,TimeRegObj *obj,int time_32);
void TimeRegObjRunCount( TimeRegObj *obj,int count_32);
void TimeManagerDelete( TimeManager* base, TimeRegObj *obj);
#endif