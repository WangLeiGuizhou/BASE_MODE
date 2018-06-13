#ifndef __VIEMODE
#define __VIEMODE 
// #include "config.h"
typedef struct ClassBase{
    void *me;//ClassBase
    // ClassBase *me
}ClassBase;

typedef struct FUNCDATA{
    ClassBase *csbase;//暂时不用
    unsigned char datalen_u8;
    int data1_32;
    int data2_32;
    int data3_32;
    void *other_void;//任何类型的指针
}FUNCDATA;
typedef void (*ViewerHandle)(FUNCDATA *funcdata);
typedef struct ViewerClient{
    // unsigned char RolaSignalStreng;
    ViewerHandle viewer_func;//由客户端实现这个结构体，对于server来说就是一个通用的接口,必须实现！！！！
}ViewerClient;
typedef struct ViewerServer{
    ViewerClient ViewerClient[12];
    ViewerHandle viewer_func;//服务器函数指针，不执行，作为存在标志
    FUNCDATA     FuncData;
}ViewerServer;//对程序员来说这就是你要用的

void ViewerServerTrigger(ViewerServer *base);
void ViewerModeScan(ViewerServer *base);//所有的客户端函数在注册后只能通过指定的方法去掉。该函数配合ViewerServerTrigger使用
void ViewerServerCreate(ViewerServer *base);
unsigned char ViewerServerSubscribeClient(ViewerServer *base,void *cli_fptr);
void ViewerServerUnsubscribeClient(ViewerServer *base,void *cli_fptr);
void ViewerServerDumpList(ViewerServer *base);
void ViewerServerEnvirementSet(ViewerServer *base,void * void_ptr,int i_32);

// 
void SetFuncData1_u32(FUNCDATA *data,int a_32);
int GetFuncData1_u32(FUNCDATA *data);
void SetFuncData3(FUNCDATA *data,int a_32);
int GetFuncData3(FUNCDATA *data);
void SetFuncOther_void(FUNCDATA *data,void* a_VOID);
void * GetFuncOther_void(FUNCDATA *data);

#define SET_FUNCDATA1_U32(FUdata,A_32)  ((FUdata)->data1_32 = (A_32))
#define SET_FUNCDATA2_U32(FUdata,A_32)  ((FUdata)->data2_32 = (A_32))
#define SET_FUNCDATA3_U32(FUdata,A_32)  ((FUdata)->data3_32 = (A_32))

#define GET_FUNCDATA1_U32(FUdata)  ((FUdata)->data1_32)
#define GET_FUNCDATA2_U32(FUdata)  ((FUdata)->data2_32)
#define GET_FUNCDATA3_U32(FUdata)  ((FUdata)->data3_32)

#define ADD_FUNCDATA1_U32(FUdata,A_32)  ((FUdata)->data1_32 += (A_32))
#define ADD_FUNCDATA2_U32(FUdata,A_32)  ((FUdata)->data2_32 += (A_32))
#define ADD_FUNCDATA3_U32(FUdata,A_32)  ((FUdata)->data3_32 += (A_32))

#define REDUC_FUNCDATA1_U32(FUdata,A_32)  ((FUdata)->data1_32 -= (A_32))
#define REDUC_FUNCDATA2_U32(FUdata,A_32)  ((FUdata)->data2_32 -= (A_32))
#define REDUC_FUNCDATA3_U32(FUdata,A_32)  ((FUdata)->data3_32 -= (A_32))

#endif