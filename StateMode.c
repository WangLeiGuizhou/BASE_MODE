#include "StateMode.h"
// static ViewerServer StaChangeView;
static unsigned char add_count = 0;
void ContextRequest(CONTEXT *context){
    if(0 == context)return ;
    if(0 == context->state.handle) return;
    context->state.handle(context,&context->funcdata);//这样实现参数的传递啊
}
void ContextInit(CONTEXT *context,StaHandle handle){
    if(0 == context)return ;
    context->state.handle = handle;
    ViewerServerCreate(&context->StaChangeView);
}
void ContextSetSta(CONTEXT *context,StaHandle handle){
    if(0 == context) return;
    if(0 == handle) return;
    context->state.handle = handle;
    // if(add_count == 0) return;
    ViewerServerTrigger(&context->StaChangeView);
}
StaHandle ContextGetSta(CONTEXT *context){
    if(0 == context) return 0;
    return context->state.handle;
}
// 为了不改变代码封装而做的工作，加入观察者！！
void ContextAddView(CONTEXT *context,void*func){
    if(func == 0)return;
    if(0 == context) return;
    ViewerServerSubscribeClient(&context->StaChangeView,func);
}
