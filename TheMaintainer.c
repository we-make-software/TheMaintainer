#include "../TheMaintainer/TheMaintainer.h"
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