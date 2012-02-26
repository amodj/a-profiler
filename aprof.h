#ifndef __A_PROF_H__

#define __A_PROF_H__

/* The max depth that can be tracked - 2000, for now */
#define MAX_FRAMES 2000

/* Don't profile the profiler */
#define DONT_PROF __attribute__((no_instrument_function))


typedef struct prof_func_node
{
        void *next_node;
        void *eip;
        unsigned long long call_tick;
        unsigned long long self_time;
        unsigned long long child_time;
}prof_func_node_t;

/* Maintain a stack of function nodes */
static prof_func_node_t* _call_stack[MAX_FRAMES];
static int _current_call;

/* A list to maintain all the functions that we have seen */
static prof_func_node_t *func_node_head;

/* Get the current ticks */
DONT_PROF static __inline__ unsigned long long rdtsc()
{
        unsigned long long int x;
        __asm__ volatile ("rdtsc" : "=A" (x));
        return x;
}

/* 
 * When the -finstrument-functions option is used with gcc, the following functions are
 * invoked. 
 * The __cyg_profile_func_enter function is called just after the function call,
 * and the __cyg_profile_func_exit function is called just before rerturn.
 */

DONT_PROF void __cyg_profile_func_enter(void *function, void* call_site);
DONT_PROF void __cyg_profile_func_exit(void *function, void* call_site);


#else
#endif
