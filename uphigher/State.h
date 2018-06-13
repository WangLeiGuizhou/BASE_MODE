// ***********************************************************************************
//
//Copy Right (c) 2018,广州威拓电子科技有限公司研发部
//All Rights reserved.
//
//文件名称：State.c
//广州威拓电子科技有限公司 7/230
//当前版本:1.1
//作 者:wll
//完成日期:2018 年 6 月 5 日
//**********************************************************************************
#ifndef AT_STATE
#define AT_STATE
#include "StateMode.h"//继承状态模式
#include <stdio.h>
#define MAXATSTATENUM  20
typedef int (*StateWaitDataResPonse)(void);
//对外接口 发送成功返回0 失败返回1 dataptr 是发送数据的地址使用 len是发送长度
typedef int (*StateDataSend)(void);
typedef int (*StateDataDeal)(unsigned char* dataptr,int len);
typedef int (*StateDataRec)(void);

typedef struct AtState_strc{
	CONTEXT AtStateContext;
	StateDataRec statedatarec;//对外接口，查询接受到的数据，返回接受数据量
	unsigned char * rec_data_ptr;//是数据指针，
	// StateWaitDataResPonse state_wait_data_response[MAXATSTATENUM];
	// int RegCountDataResPonse;

	StateDataSend state_send_data[MAXATSTATENUM];
	int RegCountSendData;//每次新加入一个发送函数加一
	int RunCountSendData;//每次运行一个发送函数加一

	StateDataDeal state_data_deal[MAXATSTATENUM];
	int RegCountDataDeal;//每次新加入一个处理函数加一
	int RunCountDataDeal;//每次运行一个处理函数加一

	int TimeOutMsUpper;//对外接口 超时用上限时间单位ms
	int TimeCount100Ms;//对外接口 超时用的计时器单位ms 每100ms减去1 直到0
	unsigned char flag_start_timeout;

	unsigned char flag_end_deal;
	unsigned char flag_enter_deal;
}AtState;
void AtStateContextRequest(AtState *atstate);//
void ATStateRegSendData_DataDeal(AtState *atstate,StateDataSend statedatasendptr,StateDataDeal statedatadealptr);
void InitAtState(AtState *atstate,StateDataRec statedatarec,unsigned char *ptr,int timeoutms);
void EnterAtState(AtState *atstate);

void TimeerAtState_100MS(AtState *atstate);

#endif
// StateDataSend
// 例子
// int SENDat(void){
// 	USART("FASDFA");
// 	return 0
// }