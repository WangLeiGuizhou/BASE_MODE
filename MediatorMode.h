#ifndef __MEDMODE
#define __MEDMODE 
#define MEDIATORSTAHANDLEMAX   6
// 我主要用来设置一些状态的通知，去掉直接得到状态，让中介者得到状态
typedef unsigned char (*MediatorStaHandle)(FUNCDATA *funcdata);//该，State
typedef struct {
    MediatorStaHandle MediatorStaPtr[MEDIATORSTAHANDLEMAX];
    MediatorStaHandle MediatorOKPtr;
    FUNCDATA MediatorStaPtrFuncdata[MEDIATORSTAHANDLEMAX];
}MediatorSta;
void MediatorCreate(MediatorSta *base,MediatorStaHandle okme);
void MediatorAdd(MediatorSta *base,MediatorStaHandle me);
void MediatorDel(MediatorSta *base,MediatorStaHandle me);
unsigned char MediatorGetSta_AndRun(MediatorSta *base);
#endif