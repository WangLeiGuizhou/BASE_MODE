
#include "Decorator.h"
void DecoratorAdd(DECORATOR *base,DecoratorHandle fptr,FUNCDATA *funcdata){
    int a_32 = 0;
    if((base == 0) || (0 == fptr))return;
    for(a_32 = 0;a_32 < MAX_DECORATORHANDLEFUNC;a_32++){
        if(0 == base->DecoratorHandleFunc[a_32]){
            base->DecoratorHandleFunc[a_32] = fptr;
            if(0 != funcdata){
                base->funcdata[a_32] = *funcdata;
            }
            break;
        }
    }   
}
void DecoratorDoFunc(DECORATOR *base){
    int a_32 = 0;
    if(base == 0)return; 
    for(a_32 = 0;a_32 < MAX_DECORATORHANDLEFUNC;a_32++){
        if(0 != base->DecoratorHandleFunc[a_32]){
            base->DecoratorHandleFunc[a_32]((FUNCDATA *)&(base->funcdata[a_32]));
        }
    }   
}
void DecoratorCreate(DECORATOR *base){
    int a_32 = 0;
    if(base == 0)return;    
    for(a_32 = 0;a_32 < MAX_DECORATORHANDLEFUNC;a_32++){
        base->DecoratorHandleFunc[a_32] = 0;
    }
}
void fdsaf(FUNCDATA *funcdata){
    printf("fdsaf,%d\n",funcdata->data1_32);
}
void fdsaf1(FUNCDATA *funcdata){
    printf("fdsaf1,%d\n",funcdata->data1_32);
}
void fdsaf2(FUNCDATA *funcdata){
    printf("fdsaf2,%d\n",funcdata->data1_32);
}
void testdc(){
    FUNCDATA fd;
    DECORATOR dc1;
    fd.data1_32 = 10;
    fd.data2_32 = 13;
    fd.data3_32 = 7;
    DecoratorCreate(&dc1);
    DecoratorAdd(&dc1,fdsaf,&fd);
    DecoratorAdd(&dc1,fdsaf1,&fd);
    DecoratorAdd(&dc1,fdsaf2,&fd);
    DecoratorDoFunc(&dc1);
}
