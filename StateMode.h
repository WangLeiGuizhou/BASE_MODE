#ifndef __StateMode
#define __StateMode
#include "ViewMode.h"
typedef struct CONTEXT CONTEXT;
typedef struct State State;
typedef void (*StaHandle)(CONTEXT *Context,FUNCDATA *funcdata);//该，State
typedef struct State{
    void *me_forclass;//为了类继承和接口
    StaHandle handle;
    // CONTEXT *context;
}State;
// 不知道原先的状态模式是如何传递数据的，我这里改进使用观察模式里的数据结构体
// 似乎状态模式是事先知道了状态的变化，客户端只是执行context
struct CONTEXT{
    FUNCDATA funcdata;
    State state;
    ViewerServer StaChangeView;
};

void ContextSetSta(CONTEXT *context,StaHandle handle);
void ContextRequest(CONTEXT *context);
void ContextInit(CONTEXT *context,StaHandle handle);
StaHandle ContextGetSta(CONTEXT *context);
void ContextAddView(CONTEXT *context,void*func);
#endif
