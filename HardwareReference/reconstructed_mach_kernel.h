#ifndef RECONSTRUCTED_MACH_KERNEL_H
#define RECONSTRUCTED_MACH_KERNEL_H

/* ==========================================================================
   Basic Types & Constants (Inferred from Mach/02_Messages.md & usage)
   ========================================================================== */

typedef int             kern_return_t;
typedef int             boolean_t;
typedef unsigned int    port_t;
typedef unsigned int    port_name_t;
typedef unsigned int    port_all_t;
typedef port_t          port_set_name_t;

typedef unsigned int    vm_address_t;
typedef unsigned int    vm_size_t;
typedef unsigned int    vm_offset_t;
typedef char *          pointer_t;

#define TRUE  1
#define FALSE 0
#define KERN_SUCCESS 0
#define PORT_NULL 0

/* ==========================================================================
   Mach Message Structures (from Mach/02_Messages.md)
   ========================================================================== */

typedef struct {
    unsigned int msg_unused : 24,
                 msg_simple : 8;
    unsigned int msg_size;
    int          msg_type;
    port_t       msg_local_port;
    port_t       msg_remote_port;
    int          msg_id;
} msg_header_t;

#define MSG_TYPE_NORMAL 0
#define MSG_TYPE_RPC    1

typedef struct  {
    unsigned int
        msg_type_name : 8,
        msg_type_size : 8,
        msg_type_number : 12,
        msg_type_inline : 1,
        msg_type_longform : 1,
        msg_type_deallocate : 1,
        msg_type_unused : 1;
} msg_type_t;

typedef struct {
    msg_type_t  msg_type_header;
    short       msg_type_long_name;
    short       msg_type_long_size;
    int         msg_type_long_number;
} msg_type_long_t;

/* ==========================================================================
   Driver Kit Types (from DriverKit/05_Reference/Types.md)
   ========================================================================== */

typedef struct IOAddressRange {
    unsigned    start;
    unsigned    length;
} IOAddressRange;

typedef enum {
    IO_CacheOff,
    IO_WriteThrough,
    IO_CopyBack
} IOCache;

typedef struct {
    msg_header_t header;
} IOInterruptMsg;

typedef int IOReturn;

typedef struct range {
    unsigned int start;
    unsigned int size;
} IORange;

typedef struct {
    int width;
    int height;
    int totalWidth;
    int rowBytes;
    int refreshRate;
    void *frameBuffer;
    int bitsPerPixel; /* Typedef IOBitsPerPixel not fully defined in docs, assumed int */
    int colorSpace;   /* Typedef IOColorSpace not fully defined in docs, assumed int */
    unsigned int flags;
    void *parameters;
} IODisplayInfo;

/* ==========================================================================
   Opaque Kernel Handles
   ========================================================================== */

typedef struct task *task_t;
typedef struct thread *thread_t;
typedef struct mutex *mutex_t;
typedef struct condition *condition_t;

/* Loadable Kernel Server Instance Handle */
/* Structure contents unknown, used as handle */
typedef struct kern_server {
    int _reserved; 
} kern_server_t; 

/* ==========================================================================
   Kernel Support Functions (from Mach/10_KernelSupportFunctions.md)
   ========================================================================== */

/* Synchronization */
void mutex_init(mutex_t m);
void mutex_lock(mutex_t m);
void mutex_unlock(mutex_t m);
void condition_init(condition_t c);
void condition_signal(condition_t c);
void condition_wait(condition_t c, mutex_t m);

/* Memory */
void *kalloc(int size);
void kfree(void *address, int size);
void bcopy(void *from, void *to, int length);
void bzero(void *address, int length);

/* Task & Thread */
task_t current_task(void);
thread_t kernel_thread(task_t task, void (*start)(void));

/* Loadable Kernel Server Support */
port_t kern_serv_bootstrap_port(kern_server_t *ksp);
void kern_serv_log(kern_server_t *ksp, int log_level, char *format, ...);
kern_return_t kern_serv_notify(kern_server_t *ksp, port_t reply_port, port_t request_port);
port_t kern_serv_notify_port(kern_server_t *ksp);
void kern_serv_port_gone(kern_server_t *ksp, port_name_t port);

/* Interrupts */
int spl0(void);
int spl1(void);
int spl2(void);
int spl3(void);
int spl4(void);
int spl5(void);
int spl6(void);
int spl7(void);
void splx(int s);

#endif /* RECONSTRUCTED_MACH_KERNEL_H */
