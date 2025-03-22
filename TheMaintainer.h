#ifndef TheMaintainer_H
#define TheMaintainer_H
#include "../TheMailConditioner/TheMailConditioner.h"
extern struct TheMailConditioner*GetTheMaintainer(u8*value);
#define TM(name, ...) \
    struct TheMailConditioner*tmc##name = GetTheMaintainer((u8[]){__VA_ARGS__}); \
    struct name*name##TM=NULL; \
    if(!GetTheMailConditionerData(tmc##name)) { \
        name##TM=(struct name*)GetTheMailConditionerData(tmc##name); \
        printk(KERN_ERR #name ": Can't get the " #name " TM.\n"); \
    }
#define SetupTM(description,version,build,...) \
    static void TMStart(void);\
    static void TMEnd(void);\
    static struct TheMailConditioner*tmcTM; \
    static void End(void){ \
        TMEnd(); \
        CancelTheMailConditioner(tmcTM); \
    } \
    static void AutoDeleteTM(void* data,struct ExpiryWorkBaseBenchmark){ \
        kfree(data); \
    } \
    static void Start(void){ \
        tmcTM=GetTheMaintainer((u8[]){__VA_ARGS__}); \
        if (!GetTheMailConditionerUnsafeData(tmcTM)){ \
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
#endif