#ifndef __DECORATOR
#define __DECORATOR 
#include "ViewMode.h"//为了使用fundat
#define MAX_DECORATORHANDLEFUNC  6
typedef void(*DecoratorHandle)(FUNCDATA *funcdata);
typedef struct DECORATOR{
    DecoratorHandle DecoratorHandleFunc[MAX_DECORATORHANDLEFUNC];//最大支持六个装饰
    FUNCDATA funcdata[MAX_DECORATORHANDLEFUNC];
}DECORATOR;
void DecoratorAdd(DECORATOR *base,DecoratorHandle fptr,FUNCDATA *funcdata);
void DecoratorDoFunc(DECORATOR *base);
void DecoratorCreate(DECORATOR *base);
void testdc();
#endif