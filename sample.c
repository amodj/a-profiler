/* 
 * Just a sample program to try out the profiler.
 * Does not do anything useful.
 */

#include <stdio.h>

int rec_func(int i)
{
        while(i < 10)
                rec_func(++ii);
                
}

void just_for_fun()
{
        int i;
        for(i = 0; i < 100; i++);
        return;
}

void just_for_fun_2()
{
        int i;
        for(i = 0; i < 1000; i++);
        return;
}

int main()
{
        int i;

        for(i = 0; i < 1000; i++);
        
        just_for_fun();

        for(i = 0; i < 1000; i++);
        
        just_for_fun_2();

        for(i = 0; i < 1000; i++);

        return 0;
}
