#include "dyntrace/fasttp/common.hpp"
#include "dyntrace/fasttp/fasttp.hpp"

#include <thread>

using namespace dyntrace;

extern "C" int __attribute__((noinline)) bar(int a)
{
   	 return a;
}

extern "C" void __attribute__((noinline)) foo(int a)
{
	std::string c ="b";
	/*int h=4;
	int d=h+2;
	int f=d+h;
	int g=f;
	g = dummy(g);*/
 	bar(a);
}



extern "C" void __attribute__((noinline)) do_run()
{
  	foo(1);
}

fasttp::tracepoint* tp1;
fasttp::tracepoint* tp2;
fasttp::options ops{};
fasttp::options ops2{};

void enter_handler(const void *caller, const arch::regs& r)
    {
        //using arch::arg;
       //printf("Enter %p\n", caller);
    };

void exit_handler(const void* caller, const arch::regs& r)
    {
        //using arch::ret;
       // printf("Exit  %p \n", caller);
    };

void setup_fasttp(){
    
    ops.x86.disable_thread_safe = true;
    tp1 = new fasttp::tracepoint{fasttp::resolve(bar), fasttp::entry_exit_handler{enter_handler, exit_handler}, ops};
    
    ops2.x86.disable_thread_safe = true;
    tp2 = new fasttp::tracepoint{fasttp::resolve(foo), fasttp::entry_exit_handler{enter_handler, exit_handler}, ops2};
}

int main()
{
    setup_fasttp();
    do_run();
    //delete tp1;
    //delete tp2;
    
    return 0;
}
