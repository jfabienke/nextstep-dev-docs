# 4. Mach Functions

**Release 3.3** — Copyright ©1995 by NeXT Computer, Inc. All Rights Reserved.

## Overview

This chapter gives detailed descriptions of the C functions provided by the NeXT Mach operating system. It also describes some macros that behave like functions. For this chapter, the functions and macros are divided into five groups:

- **C-thread functions** — Use these to implement multiple threads in an application.
- **Mach kernel functions** — Use these to get access to the Mach operating system.
- **Bootstrap Server functions** — Use these to set up communication between the task that provides a local service and the tasks that use the service.
- **Network Name Server functions** — Use these to set up communication between tasks that might not be on the same machine.
- **Kernel-server loader functions** — Use these to load and unload loadable kernel servers, to add and delete servers to and from the kernel-server loader, and to get information about servers.

Within each section, functions are subgrouped with other functions that perform related tasks. These subgroups are described in alphabetical order by the name of the first function listed in the subgroup. Functions within subgroups are also listed alphabetically, with a pointer to the subgroup description.

For convenience, these functions are summarized in the NEXTSTEP Programming Interface Summary. The summary lists functions by the same subgroups used in this chapter and combines several related subgroups under a heading such as "Basic C-Thread Functions" or "Task Functions." For each function, the summary shows the calling sequence.

---

## C-Thread Functions

These functions provide a C language interface to the low-level, language-independent primitives for manipulating threads of control.

In a multithreaded application, you should use the C-thread functions whenever possible, rather than Mach kernel functions. If you need to call a Mach kernel function that requires a `thread_t` argument, you can find the Mach thread that corresponds to a particular C thread by calling `cthread_thread()`.

### condition_alloc(), mutex_alloc()

**SUMMARY**

Create a condition or mutex object

**SYNOPSIS**

```c
#import <mach/cthreads.h>

condition_t condition_alloc(void)
mutex_t mutex_alloc(void)
```

**DESCRIPTION**

The macros `condition_alloc()` and `mutex_alloc()` provide dynamic allocation of condition and mutex objects. When you're finished using these objects, you can deallocate them using `condition_free()` and `mutex_free()`.

**EXAMPLE**

```c
my_condition = condition_alloc();
my_mutex = mutex_alloc();
```

**SEE ALSO**

`condition_init()`, `mutex_init()`, `condition_free()`, `mutex_free()`

---

### condition_broadcast()

**SUMMARY**

Broadcast a condition

**SYNOPSIS**

```c
#import <mach/cthreads.h>

void condition_broadcast(condition_t c)
```

**DESCRIPTION**

The macro `condition_broadcast()` wakes up all threads that are waiting (with `condition_wait()`) for the condition `c`. This macro is similar to `condition_signal()`, except that `condition_signal()` doesn't wake up every waiting thread.

**EXAMPLE**

```c
any_t listen(any_t arg)
{
    mutex_lock(my_mutex);
    while(!data)
        condition_wait(my_condition, my_mutex);
    /* ... */
    mutex_unlock(my_mutex);

    mutex_lock(printing);
    printf("Condition has been met\n");
    mutex_unlock(printing);
}

main()
{
    my_condition = condition_alloc();
    my_mutex = mutex_alloc();
    printing = mutex_alloc();

    cthread_detach(cthread_fork((cthread_fn_t)listen, (any_t)0));

    mutex_lock(my_mutex);
    data = 1;
    mutex_unlock(my_mutex);
    condition_broadcast(my_condition);
    /* ... */
}
```

**SEE ALSO**

`condition_signal()`, `condition_wait()`

---

### condition_clear(), mutex_clear()

**SUMMARY**

Clear a condition or mutex object

**SYNOPSIS**

```c
#import <mach/cthreads.h>

void condition_clear(struct condition *c)
void mutex_clear(struct mutex *m)
```

**DESCRIPTION**

You must call one of these macros before freeing an object of type `struct condition` or `struct mutex`. See the discussion of `condition_init()` and `mutex_init()` for information on why you might want to use these types instead of `condition_t` and `mutex_t`.

**EXAMPLE**

```c
struct mystruct {
    my_data_t     data;
    struct mutex  m;
};
struct mystruct  *mydata;
mydata = (struct mystruct *)malloc(sizeof(struct mystruct));

mutex_init(&mydata->m);
/* ... */
mutex_lock(&mydata->m);
/* Do something to mydata that only one thread can do. */
mutex_unlock(&mydata->m);
/* ... */
mutex_clear(&mydata->m);
free(mydata);
```

**SEE ALSO**

`condition_init()`, `mutex_init()`, `condition_free()`, `mutex_free()`

---

### condition_free(), mutex_free()

**SUMMARY**

Deallocate a condition or mutex object

**SYNOPSIS**

```c
#import <mach/cthreads.h>

void condition_free(condition_t c)
void mutex_free(mutex_t m)
```

**DESCRIPTION**

The macros `condition_free()` and `mutex_free()` let you deallocate condition and mutex objects that were allocated dynamically. Before deallocating such an object, you must guarantee that no other thread will reference it. In particular, a thread blocked in `mutex_lock()` or `condition_wait()` should be viewed as referencing the object continually; freeing the object out from under such a thread is erroneous, and can result in bugs that are extremely difficult to track down.

**SEE ALSO**

`condition_alloc()`, `mutex_alloc()`, `condition_clear()`, `mutex_clear()`

---

### condition_init(), mutex_init()

**SUMMARY**

Initialize a condition variable or mutex

**SYNOPSIS**

```c
#import <mach/cthreads.h>

void condition_init(struct condition *c)
void mutex_init(struct mutex *m)
```

**DESCRIPTION**

The macros `condition_init()` and `mutex_init()` initialize an object of the `struct condition` or `struct mutex` referent type, so that its address can be used wherever an object of type `condition_t` or `mutex_t` is expected. Initialization of the referent type is most often used when you have included the referent type itself (rather than a pointer) in a larger structure, for more efficient storage allocation.

For instance, a data structure might contain a component of type `struct mutex` to allow each instance of that structure to be locked independently. During initialization of the instance, you would call `mutex_init()` on the `struct mutex` component. The alternative of using a `mutex_t` component and initializing it using `mutex_alloc()` would be less efficient.

If you're going to free a condition or mutex object of type `struct condition` or `struct mutex`, you should first clear it using `condition_clear()` or `mutex_clear()`.

**EXAMPLE**

```c
struct mystruct {
    my_data_t     data;
    struct mutex  m;
};
struct mystruct  *mydata;
mydata = (struct mystruct *)malloc(sizeof(struct mystruct));

mutex_init(&mydata->m);
/* ... */
mutex_lock(&mydata->m);
/* Do something to mydata that only one thread can do. */
mutex_unlock(&mydata->m);
/* ... */
mutex_clear(&mydata->m);
free(mydata);
```

**SEE ALSO**

`condition_alloc()`, `mutex_alloc()`, `condition_clear()`, `mutex_clear()`

---

### condition_name(), condition_set_name(), mutex_name(), mutex_set_name()

**SUMMARY**

Associate a string with a condition or mutex variable

**SYNOPSIS**

```c
#import <mach/cthreads.h>

char *condition_name(condition_t c)
void condition_set_name(condition_t c, char *name)
char *mutex_name(mutex_t m)
void mutex_set_name(mutex_t m, char *name)
```

**DESCRIPTION**

These macros let you associate a name with a condition or a mutex object. The name is used when trace information is displayed. You can also use this name for your own application-dependent purposes.

**EXAMPLE**

```c
/* Do something if this is a "TYPE 1" condition. */
if (strcmp(condition_name(c), "TYPE 1") == 0)
    /* Do something. */;
```

---

### condition_signal()

**SUMMARY**

Signal a condition

**SYNOPSIS**

```c
#import <mach/cthreads.h>

void condition_signal(condition_t c)
```

**DESCRIPTION**

The macro `condition_signal()` should be called when one thread needs to indicate that the condition represented by the condition variable is now true. If any other threads are waiting (using `condition_wait()`), at least one of them will be awakened. If no threads are waiting, nothing happens. The macro `condition_broadcast()` is similar to this one, except that it wakes up all threads that are waiting.

**EXAMPLE**

```c
any_t listen(any_t arg)
{
    mutex_lock(my_mutex);
    while(!data)
        condition_wait(my_condition, my_mutex);
    /* ... */
    mutex_unlock(my_mutex);

    mutex_lock(printing);
    printf("Condition has been met\n");
    mutex_unlock(printing);
}

main()
{
    my_condition = condition_alloc();
    my_mutex = mutex_alloc();
    printing = mutex_alloc();

    cthread_detach(cthread_fork((cthread_fn_t)listen, (any_t)0));

    mutex_lock(my_mutex);
    data = 1;
    mutex_unlock(my_mutex);
    condition_signal(my_condition);
    /* ... */
}
```

**SEE ALSO**

`condition_broadcast()`, `condition_wait()`

---

### condition_wait()

**SUMMARY**

Wait on a condition

**SYNOPSIS**

```c
#import <mach/cthreads.h>

void condition_wait(condition_t c, mutex_t m)
```

**DESCRIPTION**

The function `condition_wait()` unlocks the mutex it takes as a argument, suspends the calling thread until the specified condition is likely to be true, and locks the mutex again when the thread resumes. There's no guarantee that the condition will be true when the thread resumes, so this function should always be used as follows:

```c
mutex_t m;
condition_t c;

mutex_lock(m);
/* ... */
while (/* condition isn't true */)
    condition_wait(c, m);
/* ... */
mutex_unlock(m);
```

**SEE ALSO**

`condition_broadcast()`, `condition_signal()`

---

### cthread_abort()

**SUMMARY**

Interrupt a C thread

**SYNOPSIS**

```c
#import <mach/cthreads.h>

kern_return_t cthread_abort(cthread_t t)
```

**DESCRIPTION**

This function provides the functionality of `thread_abort()` to C threads. The `cthread_abort()` function interrupts system calls; it's usually used along with `thread_suspend()`, which stops a thread from executing any more user code. Calling `cthread_abort()` on a thread that isn't suspended is risky, since it's difficult to know exactly what system trap, if any, the thread might be executing and whether an interrupt return would cause the thread to do something useful.

See `thread_abort()` for a full description of the use of this function.

---

### cthread_count()

**SUMMARY**

Get the number of threads in this task

**SYNOPSIS**

```c
#import <mach/cthreads.h>

int cthread_count()
```

**DESCRIPTION**

This function returns the number of threads that exist in the current task. You can use this function to help make sure that your task doesn't create too many threads (over 200 or so). See `cthread_set_limit()` for information on restricting the number of threads in a task.

**EXAMPLE**

```c
printf("C thread count should be 1, is %d\n", cthread_count());
cthread_detach(cthread_fork((cthread_fn_t)listen, (any_t)0));
printf("C thread count should be 2, is %d\n", cthread_count());
```

**SEE ALSO**

`cthread_limit()`, `cthread_set_limit()`

---

### cthread_data(), cthread_set_data()

**SUMMARY**

Associate data with a thread

**SYNOPSIS**

```c
#import <mach/cthreads.h>

any_t cthread_data(cthread_t t)
void cthread_set_data(cthread_t t, any_t data)
```

**DESCRIPTION**

The macros `cthread_data()` and `cthread_set_data()` let you associate arbitrary data with a thread, providing a simple form of thread-specific "global" variable. More elaborate mechanisms, such as per-thread property lists or hash tables, can then be built with these macros.

**EXAMPLE**

```c
int listen(any_t arg)
{
    mutex_lock(printing);
    printf("This thread's data is: %d\n",
        (int)cthread_data(cthread_self()));
    mutex_unlock(printing);
    /* ... */
}

main()
{
    cthread_t lthread;

    printing = mutex_alloc();

    lthread = cthread_fork((cthread_fn_t)listen, (any_t)0);
    cthread_set_data(lthread, (any_t)100);
    cthread_detach(lthread);
    /* ... */
}
```

**SEE ALSO**

`cthread_name()`, `cthread_set_name()`

---

### cthread_detach()

**SUMMARY**

Detach a thread

**SYNOPSIS**

```c
#import <mach/cthreads.h>

void cthread_detach(cthread_t t)
```

**DESCRIPTION**

The function `cthread_detach()` is used to indicate that `cthread_join()` will never be called on the given thread. This is usually known at the time the thread is forked, so the most efficient usage is the following:

```c
cthread_detach(cthread_fork(function, argument));
```

A thread may, however, be detached at any time after it's forked, as long as no other attempt is made to join it or detach it.

**EXAMPLE**

```c
cthread_detach(cthread_fork((cthread_fn_t)listen, (any_t)reply_port));
```

**SEE ALSO**

`cthread_fork()`, `cthread_join()`

---

### cthread_errno()

**SUMMARY**

Get a thread's errno value

**SYNOPSIS**

```c
#import <mach/cthreads.h>

int cthread_errno(void)
```

**DESCRIPTION**

Use the `cthread_errno()` function to get the errno value for the current thread. In the UNIX operating system, errno is a process-wide global variable that's set to an error number when a UNIX system call fails. However, because Mach has multiple threads per process, Mach keeps errno information on a per-thread basis as well as in errno.

Like the value of errno, the value returned by `cthread_errno()` is valid only if the last UNIX system call returned -1. Errno values are defined in the header file `bsd/sys/errno.h`.

**EXAMPLE**

```c
int ret;

ret = chown(FILEPATH, newOwner, newGroup);
if (ret == -1) {
    if (cthread_errno() == ENAMETOOLONG)
        /* ... */
}
```

**SEE ALSO**

`cthread_set_errno()`, `intro(2)` UNIX manual page

---

### cthread_exit()

**SUMMARY**

Exit a thread

**SYNOPSIS**

```c
#import <mach/cthreads.h>

void cthread_exit(any_t result)
```

**DESCRIPTION**

The function `cthread_exit()` terminates the calling thread. The result is passed to the thread that joins the caller, or is discarded if the caller is detached.

An implicit `cthread_exit()` occurs when the top-level function of a thread returns, but it may also be called explicitly.

**EXAMPLE**

```c
cthread_exit(0);
```

**SEE ALSO**

`cthread_detach()`, `cthread_fork()`, `cthread_join()`

---

### cthread_fork()

**SUMMARY**

Fork a thread

**SYNOPSIS**

```c
#import <mach/cthreads.h>

cthread_t cthread_fork(any_t (*function)(), any_t arg)
```

**DESCRIPTION**

The function `cthread_fork()` takes two arguments: a function for the new thread to execute, and an argument to this function. The `cthread_fork()` function creates a new thread of control in which the specified function is executed concurrently with the caller's thread. This is the sole means of creating new threads.

The `any_t` type represents a pointer to any C type. The `cthread_t` type is an integer-size handle that uniquely identifies a thread of control. Values of type `cthread_t` will be referred to as thread identifiers. Arguments larger than a pointer must be passed by reference. Similarly, multiple arguments must be simulated by passing a pointer to a structure containing several components. The call to `cthread_fork()` returns a thread identifier that can be passed to `cthread_join()` or `cthread_detach()`. Every thread must be either joined or detached exactly once.

**EXAMPLE**

```c
cthread_detach(cthread_fork((cthread_fn_t)listen, (any_t)reply_port));
```

**SEE ALSO**

`cthread_detach()`, `cthread_exit()`, `cthread_join()`

---

### cthread_join()

**SUMMARY**

Join threads

**SYNOPSIS**

```c
#import <mach/cthreads.h>

any_t cthread_join(cthread_t t)
```

**DESCRIPTION**

The function `cthread_join()` suspends the caller until the specified thread `t` terminates. The caller receives either the result of `t`'s top-level function or the argument with which `t` explicitly called `cthread_exit()`.

Attempting to join one's own thread results in deadlock.

**EXAMPLE**

```c
cthread_t t;

t = cthread_fork((any_t (*)())listen, (any_t)reply_port);
/* ... (Do some work, perhaps forking other threads.) */
result = cthread_join(t);  /* Wait for the thread to finish executing. */
/* ... (Continue doing work) */
```

**SEE ALSO**

`cthread_detach()`, `cthread_exit()`, `cthread_fork()`

---

### cthread_limit(), cthread_set_limit()

**SUMMARY**

Get or set the maximum number of threads in this task

**SYNOPSIS**

```c
#import <mach/cthreads.h>

int cthread_limit(void)
void cthread_set_limit(int limit)
```

**ARGUMENTS**

- `limit`: The new maximum number of C threads per task. Specify zero if you want no limit.

**DESCRIPTION**

These functions can help you to avoid creating too many threads. The danger in creating a large number of threads is that the kernel might run out of resources and panic. Usually, a task should avoid creating more than about 200 threads.

Use `cthread_set_limit()` to set a limit on the number of threads in the current task. When the limit is reached, new C threads will appear to fork successfully. However, they will have no associated Mach thread, so they won't do anything.

Use `cthread_limit()` to find out how many threads can exist in the current task. If the returned value is zero (the default), then no limit is currently being enforced.

**Important:** Use `cthread_count()` to determine when your task is approaching the maximum number of threads.

**EXAMPLE**

```c
cthread_set_limit(LIMIT);

/* ... */

/* Fork if we haven't reached the limit. */
if ((LIMIT == 0) || (LIMIT > cthread_count()))
    cthread_detach(cthread_fork((any_t (*)())a_thread, (any_t)0));
```

---

### cthread_name(), cthread_set_name()

**SUMMARY**

Associate a string with a thread

**SYNOPSIS**

```c
#import <mach/cthreads.h>

char *cthread_name(cthread_t t)
void cthread_set_name(cthread_t t, char *name)
```

**DESCRIPTION**

The functions `cthread_name()` and `cthread_set_name()` let you associate an arbitrary name with a thread. The name is used when trace information is displayed. The name may also be used for application-specific diagnostics.

**EXAMPLE**

```c
int listen(any_t arg)
{
    mutex_lock(printing);
    printf("This thread's name is: %s\n",
        cthread_name(cthread_self()));
    mutex_unlock(printing);
    /* ... */
}

main()
{
    cthread_t lthread;

    printing = mutex_alloc();

    lthread = cthread_fork((cthread_fn_t)listen, (any_t)0);
    cthread_set_name(lthread, "lthread");
    cthread_detach(lthread);
    /* ... */
}
```

**SEE ALSO**

`cthread_data()`, `cthread_set_data()`

---

### cthread_priority(), cthread_max_priority()

**SUMMARY**

Set the scheduling priority for a C thread

**SYNOPSIS**

```c
#import <mach/cthreads.h>

kern_return_t cthread_priority(cthread_t t, int priority, boolean_t set_max)
kern_return_t cthread_max_priority(cthread_t t, processor_set_t processor_set,
                                    int max_priority)
```

**ARGUMENTS**

- `t`: The C thread whose priority is to be changed.
- `priority`: The new priority to change it to.
- `set_max`: Also set `t`'s maximum priority if true.
- `processor_set`: The privileged port for the processor set to which thread is currently assigned.
- `max_priority`: The new maximum priority.

**DESCRIPTION**

These functions give C threads the functionality of `thread_priority()` and `thread_max_priority()`. See those functions for more details than are provided here.

The `cthread_priority()` function changes the base priority and (optionally) the maximum priority of `t`. If the new base priority is higher than the scheduled priority of the currently executing thread, this thread might be preempted. The maximum priority of the thread is also set if `set_max` is true. This call fails if `priority` is greater than the current maximum priority of the thread. As a result, `cthread_priority()` can lower--but never raise--the value of a thread's maximum priority.

The `cthread_max_priority()` function changes the maximum priority of the thread. Because it requires the privileged port for the processor set, this call can reset the maximum priority to any legal value. If the new maximum priority is less than the thread's base priority, then the thread's base priority is set to the new maximum priority.

**EXAMPLE**

```c
/* Get the privileged port for the default processor set. */
error = processor_set_default(host_self(), &default_set);
if (error != KERN_SUCCESS) {
    mach_error("Error calling processor_set_default()", error);
    exit(1);
}

error = host_processor_set_priv(host_priv_self(), default_set,
                                &default_set_priv);
if (error != KERN_SUCCESS) {
    mach_error("Call to host_processor_set_priv() failed", error);
    exit(1);
}

/* Set the max priority. */
error = cthread_max_priority(cthread_self(), default_set_priv, priority);
if (error != KERN_SUCCESS)
    mach_error("Call to cthread_max_priority() failed", error);

/* Set the thread's priority. */
error = cthread_priority(cthread_self(), priority, FALSE);
if (error != KERN_SUCCESS)
    mach_error("Call to cthread_priority() failed", error);
```

**RETURN**

- `KERN_SUCCESS`: Operation completed successfully
- `KERN_INVALID_ARGUMENT`: cthread is not a C thread, processor_set is not a privileged port for a processor set, or priority is out of range (not in 0-31).
- `KERN_FAILURE`: The requested operation would violate the thread's maximum priority (only for `cthread_priority()`) or the thread is not assigned to the processor set whose privileged port was presented.

**SEE ALSO**

`thread_priority()`, `thread_max_priority()`, `thread_policy()`, `task_priority()`, `processor_set_priority()`

---

### cthread_self()

**SUMMARY**

Return the caller's C-thread identifier

**SYNOPSIS**

```c
#import <mach/cthreads.h>

cthread_t cthread_self(void)
```

**DESCRIPTION**

The function `cthread_self()` returns the caller's own C-thread identifier, which is the same value that was returned by `cthread_fork()` to the creator of the thread. The C-thread identifier uniquely identifies the thread, and hence may be used as a key in data structures that associate user data with individual threads. Since thread identifiers may be reused by the underlying implementation, you should be careful to clean up such associations when threads exit.

**EXAMPLE**

```c
printf("This thread's name is: %s\n", cthread_name(cthread_self()));
mutex_unlock(printing);
```

**SEE ALSO**

`cthread_fork()`, `cthread_thread()`, `thread_self()`

---

### cthread_set_errno_self()

**SUMMARY**

Set the current thread's errno value

**SYNOPSIS**

```c
#import <mach/cthreads.h>

void cthread_set_errno_self(int error)
```

**DESCRIPTION**

Use this function to set the errno value for the current thread to `error`. In the UNIX operating system, errno is a process-wide global variable that's set to an error number when a UNIX system call fails. However, because Mach has multiple threads per process, Mach keeps errno information on a per-thread basis as well as in errno. This function has no effect on the value of errno.

The current thread's errno value can be obtained by calling `cthread_errno()`. Errno values are defined in the header file `bsd/sys/errno.h`.

**EXAMPLE**

```c
cthread_set_errno_self(EPERM);
```

**SEE ALSO**

`cthread_errno()`, `intro(2)` UNIX manual page

---

### cthread_thread()

**SUMMARY**

Return the caller's Mach thread identifier

**SYNOPSIS**

```c
#import <mach/cthreads.h>

thread_t cthread_thread(cthread_t t)
```

**DESCRIPTION**

The macro `cthread_thread()` returns the Mach thread that corresponds to the specified C thread `t`.

**EXAMPLE**

```c
/* Save the cthread and thread values for the forked thread. */
l_cthread = cthread_fork((cthread_fn_t)listen, (any_t)0);
cthread_detach(l_cthread);
l_realthread = cthread_thread(l_cthread);
```

**SEE ALSO**

`cthread_fork()`, `cthread_self()`

---

### cthread_yield()

**SUMMARY**

Yield the processor to other threads

**SYNOPSIS**

```c
#import <mach/cthreads.h>

void cthread_yield(void)
```

**DESCRIPTION**

The function `cthread_yield()` is a hint to the scheduler, suggesting that this would be a convenient point to schedule another thread to run on the current processor.

**EXAMPLE**

```c
int i, n;

/* n is set previously */
for (i = 0; i < n; i += 1)
    cthread_yield();
```

**SEE ALSO**

`cthread_priority()`, `thread_switch()`

---

### mutex_lock()

**SUMMARY**

Lock a mutex variable

**SYNOPSIS**

```c
#import <mach/cthreads.h>

void mutex_lock(mutex_t m)
```

**DESCRIPTION**

The macro `mutex_lock()` attempts to lock the mutex `m` and blocks until it succeeds. If several threads attempt to lock the same mutex concurrently, one will succeed, and the others will block until `m` is unlocked. A deadlock occurs if a thread attempts to lock a mutex it has already locked.

**EXAMPLE**

```c
/* Only one thread at a time should call printf. */
mutex_lock(printing);
printf("Condition has been met\n");
mutex_unlock(printing);
```

**SEE ALSO**

`mutex_try_lock()`, `mutex_unlock()`

---

### mutex_try_lock()

**SUMMARY**

Try to lock a mutex variable

**SYNOPSIS**

```c
#import <mach/cthreads.h>

int mutex_try_lock(mutex_t m)
```

**DESCRIPTION**

The function `mutex_try_lock()` attempts to lock the mutex `m`, like `mutex_lock()`, and returns true if it succeeds. If `m` is already locked, however, `mutex_try_lock()` immediately returns false rather than blocking. For example, a busy-waiting version of `mutex_lock()` could be written using `mutex_try_lock()`:

```c
void mutex_lock(mutex_t m)
{
    for (;;)
        if (mutex_try_lock(m))
            return;
}
```

**SEE ALSO**

`mutex_lock()`, `mutex_unlock()`

---

### mutex_unlock()

**SUMMARY**

Unlock a mutex variable

**SYNOPSIS**

```c
#import <mach/cthreads.h>

void mutex_unlock(mutex_t m)
```

**DESCRIPTION**

The function `mutex_unlock()` unlocks `m`, giving other threads a chance to lock it.

**EXAMPLE**

```c
/* Only one thread at a time should call printf. */
mutex_lock(printing);
printf("Condition has been met\n");
mutex_unlock(printing);
```

**SEE ALSO**

`mutex_lock()`, `mutex_try_lock()`

---

## Mach Kernel Functions

### exc_server()

**SUMMARY**

Dispatch a message received on an exception port

**SYNOPSIS**

```c
#import <mach/mach.h>
#import <mach/exception.h>

boolean_t exc_server(msg_header_t *in, msg_header_t *out)
```

**ARGUMENTS**

- `in`: A message that was received on the exception port. This message structure should be at least 64 bytes long.
- `out`: An empty message to be filled by `exc_server()` and then sent. This message buffer should be at least 32 bytes long.

**DESCRIPTION**

This function calls the appropriate exception handler. You should call this function after you've received a message on an exception port that you set up previously. Usually, this function is used along with a user-defined exception handler, which must have the following protocol:

```c
kern_return_t catch_exception_raise(port_t exception_port, port_t thread,
                                     port_t task, int exception,
                                     int code, int subcode)
```

To receive a message on an exception port, you must first create a new port and make it the task or thread exception port. (You can't use the default task exception port because you can't get receive rights for it.) Before calling `msg_receive()`, you must set the `local_port` field of the header to the appropriate exception port and the `msg_size` field to the size of the structure for the incoming message.

If it accepted the incoming message, `exc_server()` returns true; otherwise it returns false.

You should keep a global value that indicates whether your exception handler successfully handled the exception. If it couldn't, then you should forward the exception message to the old exception port.

**EXAMPLE**

```c
typedef struct {
    port_t old_exc_port;
    port_t clear_port;
    port_t exc_port;
} ports_t;

volatile boolean_t  pass_on = FALSE;
mutex_t             printing;

/* Listen on the exception port. */
any_t exc_thread(ports_t *port_p)
{
    kern_return_t   r;
    char           *msg_data[2][64];
    msg_header_t   *imsg = (msg_header_t *)msg_data[0],
                   *omsg = (msg_header_t *)msg_data[1];

    /* Wait for exceptions. */
    while (1) {
        imsg->msg_size = 64;
        imsg->msg_local_port = port_p->exc_port;
        r = msg_receive(imsg, MSG_OPTION_NONE, 0);

        if (r == RCV_SUCCESS) {
            /* Give the message to the Mach exception server. */
            if (exc_server(imsg, omsg)) {
                /* Send the reply message that exc_server gave us. */
                r = msg_send(omsg, MSG_OPTION_NONE, 0);
                if (r != SEND_SUCCESS) {
                    mach_error("msg_send", r);
                    exit(1);
                }
            }
            else { /* exc_server refused to handle imsg. */
                mutex_lock(printing);
                printf("exc_server didn't like the message\n");
                mutex_unlock(printing);
                exit(2);
            }
        }
        else { /* msg_receive() returned an error. */
            mach_error("msg_receive", r);
            exit(3);
        }

        /* Pass the message to old exception handler, if necessary. */
        if (pass_on == TRUE) {
            imsg->msg_remote_port = port_p->old_exc_port;
            imsg->msg_local_port = port_p->clear_port;
            r = msg_send(imsg, MSG_OPTION_NONE, 0);
            if (r != SEND_SUCCESS) {
                mach_error("msg_send to old_exc_port", r);
                exit(4);
            }
        }
    }
}

/*
 * catch_exception_raise() is called by exc_server(). The only
 * exception it can handle is EXC_SOFTWARE.
 */
kern_return_t catch_exception_raise(port_t exception_port,
    port_t thread, port_t task, int exception, int code, int subcode)
{
    if ((exception == EXC_SOFTWARE) && (code == 0x20000)) {
        /* Handle the exception so that the program can continue. */
        mutex_lock(printing);
        printf("Handling the exception\n");
        mutex_unlock(printing);
        return KERN_SUCCESS;
    }
    else { /* Pass the exception on to the old port. */
        pass_on = TRUE;
        mach_NeXT_exception("Forwarding exception", exception, code, subcode);
        return KERN_FAILURE;  /* Couldn't handle this exception. */
    }
}

main()
{
    int             i;
    kern_return_t   r;
    ports_t         ports;

    printing = mutex_alloc();

    /* Save the old exception port for this task. */
    r = task_get_exception_port(task_self(), &(ports.old_exc_port));
    if (r != KERN_SUCCESS) {
        mach_error("task_get_exception_port", r);
        exit(1);
    }

    /* Create a new exception port for this task. */
    r = port_allocate(task_self(), &(ports.exc_port));
    if (r != KERN_SUCCESS) {
        mach_error("port_allocate 0", r);
        exit(1);
    }
    r = task_set_exception_port(task_self(), (ports.exc_port));
    if (r != KERN_SUCCESS) {
        mach_error("task_set_exception_port", r);
        exit(1);
    }

    /* Fork the thread that listens to the exception port. */
    cthread_detach(cthread_fork((cthread_fn_t)exc_thread, (any_t)&ports));

    /* Raise the exception. */
    ports.clear_port = thread_self();
    r = exception_raise(ports.exc_port, thread_reply(), ports.clear_port,
                        task_self(), EXC_SOFTWARE, 0x20000, 6);

    if (r != KERN_SUCCESS)
        mach_error("catch_exception_raise didn't handle exception", r);
    else {
        mutex_lock(printing);
        printf("Successfully called exception_raise\n");
        mutex_unlock(printing);
    }
}
```

**SEE ALSO**

`exception_raise()`, `mach_NeXT_exception()`

---

### exception_raise()

**SUMMARY**

Cause an exception to occur

**SYNOPSIS**

```c
#import <mach/mach.h>
#import <mach/exception.h>

kern_return_t exception_raise(port_t exception_port, port_t clear_port,
                               port_t thread, port_t task, int exception,
                               int code, int subcode)
```

**ARGUMENTS**

- `exception_port`: The exception port of the affected thread. (If the thread doesn't have its own exception port, then this should be the exception port of the task.)
- `clear_port`: The port to which a reply message should be sent from the exception handler. If you don't care to see the reply, you can use `thread_reply()`.
- `thread`: The thread in which the exception condition occurred. If the exception isn't thread-specific, then specify `THREAD_NULL`.
- `task`: The task in which the exception condition occurred.
- `exception`: The type of exception that occurred; for example, `EXC_SOFTWARE`. Values for this variable are defined in the header file `mach/exception.h`.
- `code`: The exception code. The meaning of this code depends on the value of exception.
- `subcode`: The exception subcode. The meaning of this subcode depends on the values of exception and code.

**DESCRIPTION**

This function causes an exception message to be sent to `exception_port`, which results in a call to the exception handler. Usually this function is used along with a user-defined exception handler. (See `exc_server()` and `mach_NeXT_exception()` for more information on user-defined exception handlers.)

You can obtain `exception_port` by calling `thread_get_exception_port()` or (if no thread exception port exists or the exception affects the whole task) `task_get_exception_port()`.

If you're defining your own type of exception, you must have `exception` equal to `EXC_SOFTWARE` and `code` equal to or greater than `0x20000`.

**EXAMPLE**

```c
/* Raise the exception. */
r = exception_raise(ports.exc_port, thread_reply(), thread_self(),
                    task_self(), EXC_SOFTWARE, 0x20000, 6);
if (r != KERN_SUCCESS)
    mach_error("catch_exception_raise didn't handle exception", r);
else {
    /* Use mutex so only one thread at a time can call printf. */
    mutex_lock(printing);
    printf("Successfully called exception_raise\n");
    mutex_unlock(printing);
}
```

**RETURN**

- `KERN_SUCCESS`: The call succeeded.
- `KERN_FAILURE`: The exception handler didn't successfully deal with the exception.
- `KERN_INVALID_ARGUMENT`: One of the arguments wasn't valid.

**SEE ALSO**

`exc_server()`, `mach_NeXT_exception()`, `task_get_exception_port()`, `thread_get_exception_port()`

---

*[Due to length constraints, I'll continue with the next major sections in a follow-up. This conversion maintains the exact formatting style of the previous Mach chapters, with proper code blocks, consistent heading hierarchy, and comprehensive function documentation.]*

**Copyright ©1995 by NeXT Computer, Inc. All Rights Reserved.**
