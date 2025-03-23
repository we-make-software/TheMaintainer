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
#define SetupGetAddTMInData() \
        struct TMList*owner;  \
        struct list_head tm
#define SetupGetAddTMInStruct(name) \
        struct name*(*Get)(u8*value);\
        struct name*(*Add)(u8*value); \
        SetupGetAddTMInData()
#define SetupGetAddTMInMainTMStart(object) \
        INIT_LIST_HEAD(&object->tm); \
        SetTM(object,Add); \
        SetTM(object,Get)
#define SetupGetAddTMInCode(name,object,size,add) \
        static DEFINE_MUTEX(GetAddTMMutex);\
        struct TMList { \
            struct list_head list; \
            u8 value[size]; \
        }; \
        static struct object *Get(u8*value); \
        static struct object *Get(u8*value){ \
            Get##name##TM(); \
            struct TMList *entry = NULL, *match = NULL; \
            list_for_each_entry(entry, &name##TM->tm, list) \
                if (memcmp(entry->value, value, size) == 0) { \
                    match = entry; \
                    break; \
                } \
            if (!match) return NULL; \
            struct TheMailConditioner*tmcTM=GetTheMailConditioner(match->value, size, false); \
            if (!tmcTM) return NULL; \
            return (struct object*)GetTheMailConditionerData(tmcTM); \
        } \
        static void AutoDeleteTMData(void*data,struct ExpiryWorkBaseBenchmark); \
        static struct object*Add(u8*value);\
        static struct object*Add(u8*value){\
            struct object*data=Get(value);\
            if(data)return data;\
            mutex_loc(&GetAddTMMutex);\
            data=Get(value);\
            if(data){\
                mutex_unlock(&GetAddTMMutex);\
                return data;\
            }\
            struct TheMailConditioner*_tmcTM=GetTheMailConditioner(value,6,true);\
            if(!_tmcTM){\
                mutex_unlock(&GetAddTMMutex);\
                return NULL;\
            }\
            data=kmalloc(sizeof(struct object),GFP_KERNEL);\
            if(!SetTheMailConditionerData(_tmcTM,data)){\
                mutex_unlock(&GetAddTMMutex); \
                return NULL; \
            } \
            struct TMList*tmList=kmalloc(sizeof(struct TMList),GFP_KERNEL);\
            if(!tmList){\
                kfree(data);\
                mutex_unlock(&GetAddTMMutex);\
                return NULL;\
            }\
            memcpy(tmList->value, value, size);\
            INIT_LIST_HEAD(&tmList->list);\
            Get##name##TM();\
            list_add(&tmList->list, &name##TM->tm); \
            SetupExpiryWorkBase(&data->ewb,&name##TM->ewb,data,AutoDeleteTMData); \
            data->owner=tmList;\
            add \
            mutex_unlock(&GetAddTMMutex);\
            return data;\
        } \
        static void AutoDeleteTMData(void*vdata,struct ExpiryWorkBaseBenchmark){ \
            if(!vdata)return; \
            mutex_loc(&GetAddTMMutex);\
            struct name *data=(struct object*)vdata; \
            Get##name##TM();\
            list_del(&data->owner->list);\
            mutex_unlock(&GetAddTMMutex);\
            kfree(data->owner);\
            if(!vdata)\
            kfree(vdata); \
        } 
#define SetupGetAddTMInMainTMEnd(object)\
        struct TMList *entry, *tmp; \
        list_for_each_entry_safe(entry, tmp,&object->tm,list){ \
            struct TheMailConditioner*tmc=GetTheMailConditioner(entry->value,6,false);\
            if (tmc)\
                CancelTheMailConditioner(tmc);\
        }
    
#endif

