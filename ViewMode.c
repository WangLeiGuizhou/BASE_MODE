#include "ViewMode.h"

// typedef struct ClassBase{
//     void *me;//ClassBase
//     // ClassBase *me
// }ClassBase;


// typedef struct FUNCDATA{
//     ClassBase *csbase;//暂时不用
//     unsigned char datalen_u8;
//     int data1_32;
//     int data2_32;
//     int data3_32;
// }FUNCDATA;
// typedef void (*ViewerHandle)(FUNCDATA *funcdata);
// typedef struct ViewerClient{
//     ViewerHandle viewer_func
//由客户端实现这个结构体，对于server来说就是一个通用的接口,必须实现！！！！
// }ViewerClient;
// typedef struct ViewerServer{
//     ViewerClient ViewerClient[4];
//     ViewerHandle viewer_func;//服务器函数指针，不执行，作为存在标志
//     FUNCDATA     FuncData;
// }ViewerServer;
//data = 1 datra = 2
// OledShowPowerShake(da)
// OledShowPowerShake(dra)

/*对数据*/
void SetFuncData1_u32(FUNCDATA *data,int a_32){
    if(data == 0)return;
    data->data1_32 = a_32;
}
int GetFuncData1_u32(FUNCDATA *data){
    if(data == 0)while(1) ;
    return  data->data1_32;
}
void SetFuncData3(FUNCDATA *data,int a_32){
    if(data == 0)return;
    data->data3_32 = a_32;
}
int GetFuncData3(FUNCDATA *data){
    if(data == 0)while(1);
    return  data->data3_32;
}
void SetFuncOther_void(FUNCDATA *data,void* a_VOID){
    if(data == 0)return;
    data->other_void = a_VOID;

}
void * GetFuncOther_void(FUNCDATA *data){
    if(data == 0)while(1);
    return data->other_void;
}
static void ViewerModeNotify(ViewerServer *base);
// 
void ViewerServerCreate(ViewerServer *base){
    unsigned char i_u8 = 0,len_u8 = 0,data_len_u8 = 0;
    if(0 == base) return;
    base->viewer_func = 0;//作为服务器的id
    base->FuncData.csbase = 0;//暂时不用
    base->FuncData.datalen_u8 = 0;
    base->FuncData.data1_32 = 0;
    base->FuncData.data2_32 = 0;
    base->FuncData.data3_32 = 0;
    len_u8 = sizeof(base->ViewerClient)/sizeof(base->ViewerClient[0]);
    for(i_u8 = 0;i_u8 < len_u8;i_u8 ++){
        base->ViewerClient[i_u8].viewer_func = 0;
    }   
}
// 参数中的数据咋得到呢？为什么里面不存在notify！因为考虑到客户端会做一些堵塞操作，所以触发和执行是分开的
void ViewerServerTriggerSC(ViewerServer *base,int data1_32,int data2_32,int data3_32){
    if(0 == base) return;
    // base->FuncData.datalen_u8 = datalen_u8;
    base->FuncData.data1_32 = data1_32;
    base->FuncData.data2_32 = data2_32;
    base->FuncData.data3_32 = data3_32;
}
// 该函数配合ViewerServerTrigger使用
// 客户端的注册需要一个指定服务器，一般来说，服务器给客户的数据至少是部分匹配，最好是全部匹配，万一根本不要匹配么？
void ViewerModeScanTR(ViewerServer *base){//所有的客户端函数在注册后只能通过指定的方法去掉。
    ViewerModeNotify(base);
}
void ViewerServerTrigger(ViewerServer *base){
    if(0 == base) return;
    // base->FuncData.datalen_u8 = datalen_u8;
    ViewerModeNotify(base);
}
// 认为添加一次第二次就添加失败
unsigned char ViewerServerSubscribeClient(ViewerServer *base,void *cli_fptr){
    unsigned char i_u8 = 0,len_u8 = 0,data_len_u8 = 0;
    if(0 == cli_fptr) return 'n';
    if(0 == base) return 'n';   
    len_u8 = sizeof(base->ViewerClient)/sizeof(base->ViewerClient[0]);
    for(i_u8 = 0;i_u8 < len_u8;i_u8 ++){
        if(cli_fptr == base->ViewerClient[i_u8].viewer_func){//注册过了
            // printf("add sub%d\n",i_u8);
            return 'k';
        }
        if(0 == base->ViewerClient[i_u8].viewer_func){
            base->ViewerClient[i_u8].viewer_func = cli_fptr;
            // printf("add sub%d\n",i_u8);
            return 'k';
        }
    }
  //  printf("add sub%d\n",i_u8);
//    if(len_u8 == i_u8){while(1);}
    return 'n';///没位置了
}
// 解除注册
void ViewerServerUnsubscribeClient(ViewerServer *base,void *cli_fptr){
    unsigned char i_u8 = 0,len_u8 = 0;
    if(0 == cli_fptr) return ;
    if(0 == base) return ;
	len_u8 = sizeof(base->ViewerClient)/sizeof(base->ViewerClient[0]);
    for(i_u8 = 0;i_u8 < len_u8;i_u8 ++){
        if(cli_fptr == base->ViewerClient[i_u8].viewer_func){
            base->ViewerClient[i_u8].viewer_func = 0;
        }
    }   
}
// 按照观察者模式的还要一个服务器打印出客户的方法
void ViewerServerDumpList(ViewerServer *base){
    if(0 == base) return ;
    // ?????????????
}

// 服务器用来执行一个函数的，轮询执行
static void ViewerModeNotify(ViewerServer *base){
    unsigned char i_u8 = 0,len_u8 = 0;
    if(0 == base) return;
    len_u8 = sizeof(base->ViewerClient)/sizeof(base->ViewerClient[0]);
    for(i_u8 = 0;i_u8 < len_u8;i_u8 ++){
        if(0 != base->ViewerClient[i_u8].viewer_func){
            base->ViewerClient[i_u8].viewer_func(&base->FuncData);
        }
    }
}
//2018-04-20 12:02:44 设置观察者环境,在原来的观察者上增加环境参数
void ViewerServerEnvirementSet(ViewerServer *base,void * void_ptr,int i_32){
    if(0 == base) return;
    SetFuncOther_void(&base->FuncData,void_ptr);
    SET_FUNCDATA1_U32(&base->FuncData,i_32);
    // SET_FUNCDATA2_U32(&base->funcdata,GET_FUNCDATA2_U32(FuncDataPtr));
    // SET_FUNCDATA3_U32(&base->funcdata,GET_FUNCDATA3_U32(FuncDataPtr));
}