#define _GNU_SOURCE
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include "nsprocess.h"


#define STACK_SIZE (1024 * 1024)
static char container_stack[STACK_SIZE];


int container_main(void* args)
{
	DEBUG_LOG("container: container started ...\n");

	const char *hostname = "container";
	sethostname(hostname, sizeof(hostname) + 1);

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

	int flags = CLONE_NEWPID | CLONE_NEWIPC | CLONE_NEWUTS | SIGCHLD;

	int pid = clone(container_main, container_stack + STACK_SIZE, flags, NULL);
	if (pid == -1) {
		ERROR_LOG("clone error: %s.\n", strerror(errno));
	}

	waitpid(pid, NULL, 0);
	DEBUG_LOG("main: container %d stoped.\n", pid);
	return 0;
}


