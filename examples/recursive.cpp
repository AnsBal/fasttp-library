#include "dyntrace/fasttp/common.hpp"
#include "dyntrace/fasttp/fasttp.hpp"

using namespace dyntrace;

void enter_handler(const void *caller, const arch::regs& r, const void *return_address)
{
	printf("Enter %p Ret: %p \n", caller, return_address);
};

void exit_handler(const void* caller, const arch::regs& r, const void *return_address)
{
	printf("Exit  %p Ret: %p \n", caller, return_address);
};

int n_calls(int n) 
{
	if(n <= 1)
	{
		return 1;
	}
	else 
	{
		return n_calls(n-1) + 1;	
	}
}

std::vector<fasttp::tracepoint*> tracepoints;
void setup_fasttp()
{
	tracepoints.push_back(new fasttp::tracepoint{fasttp::resolve(n_calls), fasttp::entry_exit_handler{enter_handler, exit_handler}});
}

void cleanup_fasttp()
{
	for (fasttp::tracepoint* tp : tracepoints)
	{
		delete(tp);
	}
}

#define NUMBER_CALLS 2

int main()
{
	//setup_fasttp();
	
	printf("Called n_call() %i times\n", n_calls(NUMBER_CALLS));
	
	//cleanup_fasttp();

    	return 0;
}
