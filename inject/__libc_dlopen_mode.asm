nop
_start: jmp string
begin: pop eax ; char *file
mov edx, 0x1 ; int mode
push edx ; 
push eax ;
mov ebx, 0x12345678 ; addr   __libc_dlopen_mode()
call ebx ; call __libc_dlopen_mode()
add esp, 0x8 ; resotre stack
int3 ; breakpoint

string: call begin
db "/tmp/ourlibby.so",0x00
