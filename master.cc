#include <cstdio>
#include <cstdlib>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

static const size_t kNumCPUs = sysconf(_SC_NPROCESSORS_ONLN);

int main(int argc, char *argv[]) {
	dprintf(STDOUT_FILENO, "num CPUs: %zu\n", kNumCPUs);

	for (size_t i = 0; i < kNumCPUs; i++) {
		int pid = fork();
		if (pid < 0) {
			printf("error in fork\n");
			exit(0);
		}

		if (pid == 0) {
			system("./piggybank");
			exit(0);
		}
		waitpid(pid, NULL, 0);
	}
}