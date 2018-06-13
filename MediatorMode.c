#include "ViewMode.h"
#include "MediatorMode.h"
void MediatorCreate(MediatorSta *base,MediatorStaHandle okme){
    int a_32 = 0;
    if(0 == base) return ;
    for(a_32 = 0;a_32<MEDIATORSTAHANDLEMAX;a_32++){
        base->MediatorStaPtr[a_32] = 0;
    }
    base->MediatorOKPtr = okme;
}
void MediatorAdd(MediatorSta *base,MediatorStaHandle me){
    int a_32 = 0;
    if(base == 0)return;
    if(me == 0)return;
    for(a_32 = 0;a_32 < MEDIATORSTAHANDLEMAX;a_32 ++){
        if(me == base->MediatorStaPtr[a_32]){//已经注册了
            break;
        }
        if(0 == base->MediatorStaPtr[a_32]){
            base->MediatorStaPtr[a_32] = me;
            break;
        }
    }
    // while(a_32 == MEDIATORSTAHANDLEMAX);
}
void MediatorDel(MediatorSta *base,MediatorStaHandle me){
    
}
unsigned char MediatorGetSta_AndRun(MediatorSta *base){
    int a_32 = 0;
    unsigned char temp_u8sz[MEDIATORSTAHANDLEMAX] , temp_u8 = 0;
    if(base == 0) while(1);
     for(a_32 = 0;a_32 < MEDIATORSTAHANDLEMAX;a_32 ++){
        if(0 != base->MediatorStaPtr[a_32]){//已经注册了
            temp_u8 = base->MediatorStaPtr[a_32](&base->MediatorStaPtrFuncdata[a_32]);
            if('n' == temp_u8){ return 'n';}
        }else{
            if(0 != base->MediatorOKPtr){
                base->MediatorOKPtr(0);
            }
            return 'k';
        }
    }
    return 'k';
}
