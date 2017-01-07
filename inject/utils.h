#include <stdio.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
//#include <linux/user.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <elf.h>
#include <link.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

// #define __ELF_NATIVE_CLASS 32

// #define ElfW(type)	_ElfW (Elf, __ELF_NATIVE_CLASS, type)
// #define _ElfW(e,w,t)	_ElfW_1 (e, w, _##t)
// #define _ElfW_1(e,w,t)	e##w##t

// eglibc-2.19/sysdeps/generic/ldsodefs.h
// #define D_PTR(map, i) ((map)->i->d_un.d_ptr + (map)->l_addr)
// #define LOOKUP_VALUE_ADDRESS(map) ((map) ? (map)->l_addr : 0)
// #define DL_SYMBOL_ADDRESS(map, ref) \
//  (void *) (LOOKUP_VALUE_ADDRESS (map) + ref->st_value)

#define PROGRAM_LOAD_ADDRESS 0x08048000


struct link_map_more {
    // section
	ElfW(Addr) dynsym_addr;
	ElfW(Addr) dynstr_addr;
	ElfW(Addr) gnuhash_addr;

	// .gnu.hash
	Elf_Symndx nbuckets;
	Elf_Symndx symndx;
    Elf_Symndx nmaskwords;
    ElfW(Word) shift2;
	ElfW(Addr) bitmask_addr;
	ElfW(Addr) hash_buckets_addr;
	ElfW(Addr) hash_values_addr;

	// so name;
	char soname[256];
};

void ptrace_attach(int pid);

ElfW(Addr) find_sym_in_lib(int pid, char *sym_name, char *lib);