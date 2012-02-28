#include "aprof.h"
#include <malloc.h>
#include <assert.h>

DONT_PROF static prof_func_node_t *get_node(void *function)
{
        prof_func_node_t *runner;
        
        runner = func_node_head;
        
        while(runner){
                if(function == runner->eip){
                        return runner;
                }
                runner = runner->next_node;
        }
        return runner;
}

DONT_PROF static void insert_into_node_list(prof_func_node_t *func_node)
{
        prof_func_node_t *runner = func_node_head;
        
        if(!runner){
                func_node_head = func_node;
                func_node_head->next_node = 0;
        }
        else{
                while(runner->next_node){
                        runner = runner->next_node;
                }
                runner->next_node = func_node;
                func_node->next_node = 0;
        }
}

DONT_PROF void __cyg_profile_func_enter(void *function, void *call_site)
{  
        int is_seen = 1;
        unsigned long long tick_now = rdtsc();
        prof_func_node_t *func_node;

        /* Update the self time for the previous frame */
        if(_current_call > 0){
                prof_func_node_t *prev = _call_stack[_current_call - 1];        
                prev->self_time += (tick_now - (prev->call_tick));
        }
        
        /* Get the node for this function */
        func_node = get_node(function);
        if(!func_node){
                is_seen = 0;
                func_node = calloc(1, sizeof(prof_func_node_t));
                if(!func_node) assert(0);
                func_node->eip = function;
                insert_into_node_list(func_node);
        }

        /* Push this on the stack, this is the active function now */
        _call_stack[_current_call] = func_node;
        _current_call++;
       
        /* Start counting the time now, since the processing done
         * before this is "extra" */
        func_node->call_tick = rdtsc();
}

DONT_PROF void __cyg_profile_func_exit(void *function, void *call_site)
{
        prof_func_node_t *top;

        /* Save the current ticks, we want this to be the ticks 
         * on which the accnting takes place.
         * The processing from here on is just "extra".
         */
        unsigned long long tick_now  = rdtsc();
        unsigned long long self_time;

        /* Currently, we're profiling all functions, so anything on the 
         * top of the stack is a legit candidate for popping 
         */       
        top = _call_stack[_current_call - 1];
        _current_call--;
       
        self_time = (tick_now - top->call_tick);
        top->self_time += self_time;

        if(_current_call > 0){
                /* update this time in the caller's child_time */
                _call_stack[_current_call - 1]->child_time += self_time;
                _call_stack[_current_call -1]->call_tick = rdtsc();
        }
        
} 

DONT_PROF static __attribute__((destructor))void print_profiler_op()
{
        prof_func_node_t *runner = func_node_head;
        
        printf("\n\t===============================================\n");
        printf("\t               Profiler results\n");
        printf("\t===============================================\n\n");
        
        printf("Function \t\t   Self Ticks\t\t    Total Ticks\n");
        printf("----------------------------------------------------------------------\n");

        while(runner){
                printf("%p                  %10llu                %10llu\n", 
                       runner->eip, runner->self_time, runner->self_time + runner->child_time);
                runner = runner->next_node;
        }
        printf("\n\n");
}
