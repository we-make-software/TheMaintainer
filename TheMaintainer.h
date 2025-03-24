#ifndef TheMaintainer_H
#define TheMaintainer_H
#include "../TheMailConditioner/TheMailConditioner.h"
extern bool IsTheMaintainerUsed(u8*value);
extern struct TheMailConditioner*GetTheMaintainer(u8*value);
extern void RegisterRecoverTheMaintainer(u8*ID,void(*Start)(void));
extern void UnregisterRecoverTheMaintainer(u8*ID);
extern void TriggerRecoverTheMaintainer(u8*ID);
#define TM(name,...) \
    struct TheMailConditioner*tmc##name=GetTheMaintainer((u8[]){__VA_ARGS__});\
    struct name*name##TM=(struct name*)GetTheMailConditionerData(tmc##name);\
    if(!name##TM){\
        TriggerRecoverTheMaintainer((u8[]){__VA_ARGS__});\
        tmc##name=GetTheMaintainer((u8[]){__VA_ARGS__});\
        name##TM=(struct name*)GetTheMailConditionerData(tmc##name);\
        if(!name##TM){\
            printk(KERN_ERR #name ": Can't get the " #name " TM.\n");\
            return;\
        }\
    }
#define TMFailback(name,failback,...) \
    struct TheMailConditioner*tmc##name=GetTheMaintainer((u8[]){__VA_ARGS__});\
    struct name*name##TM=(struct name*)GetTheMailConditionerData(tmc##name);\
    if(!name##TM){\
        TriggerRecoverTheMaintainer((u8[]){__VA_ARGS__});\
        tmc##name=GetTheMaintainer((u8[]){__VA_ARGS__});\
        name##TM=(struct name*)GetTheMailConditionerData(tmc##name);\
        if(!name##TM){\
            printk(KERN_ERR #name ": Can't get the " #name " TM.\n");\
            return failback;\
        }\
    }




#define SetupTM(description,version,build,...) \
    static void TMStart(void);\
    static void TMEnd(void*);\
    static struct TheMailConditioner*tmcTM; \
    static bool TMEndCalled=false; \
    static void AutoDeleteTM(void* data,struct ExpiryWorkBaseBenchmark){ \
        if(!TMEndCalled)\
            TMEndCalled=true; \
        if(data)\
            TMEnd(data); \
        if(data) \
            kfree(data); \
        data=NULL; \
    } \
    static void End(void){ \
        UnregisterRecoverTheMaintainer((u8[]){__VA_ARGS__}); \
        if(TMEndCalled) \
            return; \
        void*data=GetTheMailConditionerData(tmcTM); \
        if(data)\
            TMEnd(data); \
        if(data) \
            kfree(data); \
        data=NULL;\
    } \
    static void Start(void){ \
        u8 TMKey[17]={__VA_ARGS__}; \
        if(IsTheMaintainerUsed(TMKey)){ \
            TMStart(); \
            printk(KERN_ERR #description ": The " #description " TM is already in use.\n"); \
            return; \
        } \
        tmcTM=GetTheMaintainer(TMKey); \
        if (!tmcTM){ \
            printk(KERN_ERR #description ": Can't get the " #description " TM.\n"); \
            return; \
        } \
        if (!GetTheMailConditionerData(tmcTM)){ \
            struct description*tmObject=kmalloc(sizeof(struct description),GFP_KERNEL); \
            if(!tmObject){ \
                printk(KERN_ERR #description ": Can't allocate memory for the " #description " TM.\n"); \
                End(); \
                return; \
            } \
            if(!SetTheMailConditionerData(tmcTM,tmObject)){ \
                printk(KERN_ERR #description ": Can't set the " #description " TM data.\n"); \
                End(); \
                return; \
            } \
            SetupExpiryWorkBase(&tmObject->ewb,GetTheMailConditionerEWB(tmcTM), tmObject, AutoDeleteTM); \
            TMStart(); \
            RegisterRecoverTheMaintainer(TMKey,Start); \
        } \
    } \
    Setup(description,version,build)
#define SetTM(name,method)name->method=method
#endif

