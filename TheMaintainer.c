#include "../TheMaintainer/TheMaintainer.h"
struct Recover{
    struct list_head list;
    u8 ID[17];
    void(*Start)(void);
};
static LIST_HEAD(RecoverList);
static DEFINE_MUTEX(RecoverMutex);
void RegisterRecoverTheMaintainer(u8*ID,void(*Start)(void)){
    if(!ID||!Start)return;
    struct Recover*recover=kmalloc(sizeof(struct Recover),GFP_KERNEL);
    if(!recover)return;
    memcpy(recover->ID,ID,17);
    recover->Start=Start;
    mutex_lock(&RecoverMutex);
    list_add(&recover->list,&RecoverList);
    mutex_unlock(&RecoverMutex);
} 
EXPORT_SYMBOL(RegisterRecoverTheMaintainer);
void UnregisterRecoverTheMaintainer(u8*ID){
    if(!ID)return;
    struct Recover*recover=NULL,*tmp=NULL;
    mutex_lock(&RecoverMutex);
    list_for_each_entry_safe(recover,tmp,&RecoverList,list)
        if(memcmp(recover->ID,ID,17)==0){
            list_del(&recover->list);
            kfree(recover);
            recover=NULL;
            break;
        }
    mutex_unlock(&RecoverMutex);
}
EXPORT_SYMBOL(UnregisterRecoverTheMaintainer);
void TriggerRecoverTheMaintainer(u8*ID){
    if(!ID)return;
    struct Recover*recover=NULL;
    mutex_lock(&RecoverMutex);
    list_for_each_entry(recover,&RecoverList,list)
        if(memcmp(recover->ID,ID,17)==0){
            recover->Start();
            break;
        }
    mutex_unlock(&RecoverMutex);
}
EXPORT_SYMBOL(TriggerRecoverTheMaintainer);
bool IsTheMaintainerUsed(u8*value){
     return GetTheMailConditioner(value,17,false)!=NULL;
}
EXPORT_SYMBOL(IsTheMaintainerUsed);
struct TheMailConditioner*GetTheMaintainer(u8*value){
    return GetTheMailConditioner(value,17,true);
}
EXPORT_SYMBOL(GetTheMaintainer);
static void End(void){}
static void Start(void){}
Setup(TheMaintainer,0,0)