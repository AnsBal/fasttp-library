#include "dyntrace/fasttp/common.hpp"
#include "dyntrace/fasttp/fasttp.hpp"

#include <thread>
#include <unistd.h>
#include <sys/syscall.h>
#include <cstdlib>	
#define gettid() syscall(SYS_gettid)

using namespace dyntrace;

extern "C" void __attribute__((noinline)) bar(std::string s)
{
	printf("RET   from: BAR to: %s tid: %li CPU:%i \n", s.c_str(), gettid(), sched_getcpu());
}

extern "C" void __attribute__((noinline)) foo1(std::string s)
{
	std::string dummy ="FOO1";
 	bar(dummy);
	printf("RET   from: FOO1 to: %s tid: %li CPU:%i \n", s.c_str(), gettid(), sched_getcpu());
}
extern "C" void __attribute__((noinline)) foo2(std::string s)
{
	std::string dummy ="FOO2";
 	bar(dummy);
	printf("RET   from: FOO2 to: %s tid: %li CPU:%i \n", s.c_str(), gettid(), sched_getcpu());
}

void worker1()
{
	std::string dummy ="WO1";
	for(;;) foo1(dummy);
}

void worker2()
{
	std::string dummy ="WO2";
	for(;;) foo2(dummy);
}

#define N_THREADS_WORKER1 1
#define N_THREADS_WORKER2 1
void do_run()
{
	std::vector<std::thread> threads;
	for(int i = 0; i < N_THREADS_WORKER1; ++i)
	{
		threads.push_back(std::thread{worker1});
	}
	for(int i = 0; i < N_THREADS_WORKER2; ++i)
	{
		threads.push_back(std::thread{worker2});
	}
	for (std::thread& th : threads)
	{
		if (th.joinable())
			th.join();
	}
}

void enter_handler(const void *caller, const arch::regs& r, const void *ret)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1 + std::rand()/((RAND_MAX + 1u)/1000)));
	printf("Enter %p     tid: %li CPU:%i\n", caller,gettid(),sched_getcpu());
};

void exit_handler(const void* caller, const arch::regs& r, const void *ret)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1 + std::rand()/((RAND_MAX + 1u)/1000)));
        printf("Exit  %p     tid: %li CPU:%i\n", caller,gettid(),sched_getcpu());
};

std::vector<fasttp::tracepoint*> tracepoints;
void setup_fasttp()
{
	tracepoints.push_back(new fasttp::tracepoint{fasttp::resolve(bar), fasttp::entry_exit_handler{enter_handler, exit_handler}});
	tracepoints.push_back(new fasttp::tracepoint{fasttp::resolve(foo1), fasttp::entry_exit_handler{enter_handler, exit_handler}});
	tracepoints.push_back(new fasttp::tracepoint{fasttp::resolve(foo2), fasttp::entry_exit_handler{enter_handler, exit_handler}});
}

void cleanup_fasttp()
{
	for (fasttp::tracepoint* tp : tracepoints)
	{
		delete(tp);
	}
}

int main()
{
	setup_fasttp();
			
	do_run();
	
	cleanup_fasttp();

    	return 0;
}
