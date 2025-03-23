#ifndef TheMaintainer_H
#define TheMaintainer_H
#include "../TheMailConditioner/TheMailConditioner.h"
extern bool IsTheMaintainerUsed(u8*value);
extern struct TheMailConditioner*GetTheMaintainer(u8*value);
#define TM(name,...) \
    struct TheMailConditioner*tmc##name = GetTheMaintainer((u8[]){__VA_ARGS__}); \
    struct name*name##TM=NULL; \
    if(!GetTheMailConditionerData(tmc##name)) { \
        name##TM=(struct name*)GetTheMailConditionerData(tmc##name); \
        printk(KERN_ERR #name ": Can't get the " #name " TM.\n"); \
    }
#define SetupTM(description,version,build,...) \
    static void TMStart(void);\
    static void TMEnd(void*);\
    static struct TheMailConditioner*tmcTM; \
    static bool TMEndCalled=false; \
    static void AutoDeleteTM(void* data,struct ExpiryWorkBaseBenchmark){ \
        if(!TMEndCalled)\
            TMEndCalled=true; \
        if(data) { \
            TMEnd(data); \
            if(data) \
            kfree(data); \
            data=NULL;\
        } \
    } \
    static void End(void){ \
        if(TMEndCalled) \
            return; \
        void*data=GetTheMailConditionerData(tmcTM); \
        if(data) { \
            TMEnd(data); \
            if(data) \
            kfree(data); \
            data=NULL;\
        } \
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
            void*tmObject=kmalloc(sizeof(struct description),GFP_KERNEL); \
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
            SetAutoDeleteTheMailConditioner(tmcTM,AutoDeleteTM); \
            TMStart(); \
        } \
    } \
    Setup(description,version,build)
#define SetTM(name,method)name->method=method 
#endif

