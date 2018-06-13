AtState testatstate;
unsigned char data[100];
int rec_testatstate(void)
{
	return 100;
}

int send_testatstate_1(void)
{
	// usart("123")
}
int deal_testatstate_1(unsigned char* dataptr,int len)
{
	dataptr[0] = 10;
}
int send_testatstate_2(void)
{
	// usart("123")
}
int deal_testatstate_2(unsigned char* dataptr,int len)
{
	dataptr[1] = 20;
}
int send_testatstate_3(void)
{
	// usart("123")
}
int deal_testatstate_3(unsigned char* dataptr,int len)
{
	dataptr[2] = 30;
}

do{
	InitAtState(&testatstate,rec_testatstate,data,1000);
	ATStateRegSendData_DataDeal(&testatstate,send_testatstate_1,deal_testatstate_1);
	ATStateRegSendData_DataDeal(&testatstate,send_testatstate_2,deal_testatstate_2);
	ATStateRegSendData_DataDeal(&testatstate,send_testatstate_3,deal_testatstate_3);
	AtStateContextRequest(&testatstate);
}while(0);
