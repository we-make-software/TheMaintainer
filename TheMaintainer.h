#ifndef TheMaintainer_H
#define TheMaintainer_H
#include "../TheMailConditioner/TheMailConditioner.h"
extern struct TheMailConditioner*GetTheMaintainer(u8*value);
#define TM(name, ...) \
    struct TheMailConditione*tmc##name=GetTheMaintainer((u8[]){__VA_ARGS__}); \
    struct name*name##API=NULL; \
    if(!GetTheMailConditionerData(tmc##name)) { \
        name##API=(struct name*)GetTheMailConditionerData(GetTheMailConditionerUnsafeData(GetTheMaintainer((u8[]){__VA_ARGS__}))); \
        printk(KERN_ERR #name ": Can't get the " #name " API.\n"); \
    }
#define SetupTM(description,version,build,...) \
    static struct TheMailConditione*tmcTMAPI; \
    static void End(void){ \
        TMEnd(); \
        CancelTheMailConditioner(tmcTMAPI); \
    } \
    static void AutoDeleteTM(void* data,struct ExpiryWorkBaseBenchmark){ \
        kfree(data); \
    } \
    static void Start(void){ \
        tmcTMAPI=GetTheMaintainer((u8[]){__VA_ARGS__}); \
        if (!GetTheMailConditionerUnsafeData(tmcTMAPI)){ \
            void*tmObject=kmalloc(sizeof(struct description),GFP_KERNEL); \
            if(!tmObject){ \
                printk(KERN_ERR #description ": Can't allocate memory for the " #description " API.\n"); \
                End(); \
                return; \
            } \
            if(!SetTheMailConditionerData(tmcTMAPI,tmObject)){ \
                printk(KERN_ERR #description ": Can't set the " #description " API data.\n"); \
                End(); \
                return; \
            } \
            SetAutoDeleteTheMailConditioner(tmcTMAPI,AutoDeleteTM); \
            TMStart(); \
        } \
    } \
    Setup(description,version,build)
#endif