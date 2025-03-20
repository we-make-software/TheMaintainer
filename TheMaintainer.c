#include "../TheMailConditioner/TheMailConditioner.h"
bool IsTheMaintainerUsed(u8*value);
bool IsTheMaintainerUsed(u8*value){
  
     return GetTheMailConditioner((u8[]){value[0],value[1],value[2],value[3],value[4],value[5],value[6],value[7],value[8],value[9],value[10],value[11],value[12],value[13],value[14],value[15],value[16]},17,false)!=NULL;
}
EXPORT_SYMBOL(IsTheMaintainerUsed);
struct TheMailConditioner*GetTheMaintainer(u8*value);
struct TheMailConditioner*GetTheMaintainer(u8*value){
    return GetTheMailConditioner((u8[]){value[0],value[1],value[2],value[3],value[4],value[5],value[6],value[7],value[8],value[9],value[10],value[11],value[12],value[13],value[14],value[15],value[16]},17,true);
}
EXPORT_SYMBOL(GetTheMaintainer);


static void Layer1End(void){}
static void Layer0Start(void){}
Layer0_1Setup("TheMaintainer",0,0)