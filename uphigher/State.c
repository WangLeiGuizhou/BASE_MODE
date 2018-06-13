// context->data1_32是计时器用
// context->data2_32是接受数据长度用
// context->other_void 是接受数据的地址使用
#include "State.h"
static void TimeerAtStateStart(AtState *atstate);
static void SendSomeData(CONTEXT *context,FUNCDATA *funcdata)
{
	if(0 == context) 
	{
		return ;
	}
	//ContextSetSta(context,DataDeal);
}
static void DataDeal(CONTEXT *context,FUNCDATA *funcdata)
{
	if(0 == context) 
	{
		return ;
	}
	if(context->funcdata.data1_32 <= 0)//超时
	{
		ContextSetSta(context,SendSomeData);
	}else
	{
		ContextSetSta(context,DataDeal);//下一个状态		
	}
}



	void AtStateContextRequest(AtState *atstate)
	{
		if(atstate == NULL)
		{
			return;
		}
		if((atstate->flag_enter_deal == 1)&&(0 != (atstate->RunCountDataDeal | atstate->RunCountSendData)))
		{
			while(1);//表示没有处理完毕又一次状态机处理开始！
		}else if(atstate->flag_enter_deal == 1)
		{
			atstate->flag_enter_deal = 0;
		}
		if((atstate->RunCountSendData | atstate->RunCountDataDeal) == 0)
		{
			return;//没有注册过任何处理函数，发送函数
		}
		if(0 == (atstate->RunCountSendData - atstate->RunCountDataDeal))//发送先于处理执行
		{
			if((atstate->RunCountSendData < MAXATSTATENUM) && (atstate->RegCountSendData > atstate->RunCountSendData))
			{
				if(NULL == atstate->state_send_data[atstate->RunCountSendData])
				{
					while(1);
				}
				// 发送数据
				atstate->state_send_data[atstate->RunCountSendData++]();
				// 设置超时时间
				TimeerAtStateStart(atstate);
			}					
		}else if((atstate->RunCountSendData - atstate->RunCountDataDeal) == 1){//到了处理接受数据阶段
			if(0 != (atstate->AtStateContext.funcdata.data2_32 = atstate->statedatarec()))
			{
				if((atstate->RunCountDataDeal < MAXATSTATENUM) && (atstate->RegCountDataDeal > atstate->RunCountDataDeal))
				{//处理函数仍然还有
					if(NULL == atstate->state_data_deal[atstate->RunCountDataDeal])
					{
						while(1);
					} 
					// atstate->state_data_deal[atstate->RunCountDataDeal++]((unsigned char *)atstate->AtStateContext.funcdata.other_void,atstate->AtStateContext.funcdata.data2_32);
					atstate->state_data_deal[atstate->RunCountDataDeal++](atstate->rec_data_ptr,atstate->AtStateContext.funcdata.data2_32);
					if(atstate->RunCountDataDeal == atstate->RegCountDataDeal)
					{
						atstate->flag_end_deal = 1;//c
					}
				}else
				{
					while(1);
				}
			}else
			{
				// 没接受到数据 做超时处理
				if(0 == CheckAtStateTimeOut(atstate))
				{
					atstate->RunCountSendData--;// 回退
				}else
				{
					// 继续等待超时
				}
			}			
		}
		else{
			while(1);
		}
	}
	void ATStateRegSendData_DataDeal(AtState *atstate,StateDataSend statedatasendptr,StateDataDeal statedatadealptr)
	{
		if(atstate->RegCountSendData < MAXATSTATENUM)
		{
			atstate->state_send_data[atstate->RegCountSendData++] = statedatasendptr;
		}
		else
		{
			while(1);
		}
		if(atstate->RegCountDataDeal < MAXATSTATENUM)
		{
			atstate->state_data_deal[atstate->RegCountDataDeal++] = statedatadealptr;
		}
		else
		{
			while(1);
		}		
	}
	void InitAtState(AtState *atstate,StateDataRec statedatarec,unsigned char *ptr,int timeoutms)
	{
		int count = 0;
		if(atstate == NULL)
		{
			return;
		}
		// ContextInit(&atstate->AtStateContext,0);
		atstate->statedatarec = statedatarec;
		atstate->rec_data_ptr = ptr;
		if (ptr == NULL)
		{
			while(1);		
		}

		atstate->TimeOutMsUpper = timeoutms;
		atstate->flag_start_timeout = 0;
		atstate->TimeCount100Ms = 0;

		atstate->RegCountSendData = 0;
		atstate->RegCountDataDeal = 0;

		atstate->RunCountSendData = 0;
		atstate->RunCountDataDeal = 0;

		atstate->flag_end_deal = 0;    
		atstate->flag_enter_deal = 0;    
		for(count = 0;count < MAXATSTATENUM;count++){
			atstate->state_send_data[count] = NULL;

			atstate->state_data_deal[count] = NULL;
		}
	}
	void TimeerAtState_100MS(AtState *atstate)
	{
		if((atstate == NULL) && (atstate->flag_start_timeout == 0))
		{
			return;
		}else
		{
			if(atstate->TimeCount100Ms <= 0)
			{
				return;
			}else
			{
				atstate->TimeCount100Ms--;
			}
		}
	}
	void EnterAtState(AtState *atstate)
	{
		atstate->flag_enter_deal = 1;
		atstate->flag_end_deal = 0;
	}
	static void TimeerAtStateStart(AtState *atstate)
	{
		atstate->TimeCount100Ms = atstate->TimeOutMsUpper;
		atstate->flag_start_timeout = 1;
	}
	static inline int CheckAtStateTimeOut(AtState *atstate)
	{
		if(atstate->flag_start_timeout == 1)
		{
			if(atstate->TimeCount100Ms <= 0)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}else
		{
			while(1);
		}
	}