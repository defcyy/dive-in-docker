#define _GNU_SOURCE
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include <sys/wait.h>
#include "nsprocess.h"


#define STACK_SIZE (1024 * 1024)
static char container_stack[STACK_SIZE];

int container_main(void* args)
{
	DEBUG_LOG("container: container started ...\n");

	char* const exec_args[] = {
		"/bin/bash",
		NULL
	};
	execv("/bin/bash", exec_args);

	DEBUG_LOG("container: container done.\n");

	return 0;
}

int main(int argc, char *argv)
{
	DEBUG_LOG("main: start container ...\n");

	int pid = clone(container_main, container_stack + STACK_SIZE, SIGCHLD, NULL);

	waitpid(pid, NULL, 0);
	DEBUG_LOG("main: container %d stoped.\n", pid);
	return 0;
}


