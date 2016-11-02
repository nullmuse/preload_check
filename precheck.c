#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>



#define LIBC "/lib/libc.so.6" /* may need to change for yours  */

int main(int argc, char *argv[]) { 


FILE *fp = fopen("/usr/include/bits/syscall.h","r");
void *libc = dlopen(LIBC, RTLD_LAZY); 
char *data = malloc(50000);
char *strprint = malloc(10);
memset(strprint,0,10);
memset(data,0,50000);
char *strorig = strprint;
char *thing = NULL;
int i; 
int l = 1;
void *(*libc_func)();
void *(*next_func)();  
l = fread((void *)data, sizeof(char), 50000,fp);
while(l) {
thing = strstr(data, "SYS");
REDO:
if(thing != NULL) {
*thing = '_';
*(thing + sizeof(char)) = '_';
*(thing + sizeof(char) * 2) = '_';
while(*thing == '_')
thing++;
while(*thing != ' ' && *thing != 0x0a)
*strprint++ = *thing++;

printf("[+] Checking %s syscall\n", strorig); 
libc_func = dlsym(libc, strorig); 
next_func = dlsym(RTLD_NEXT, strorig); 
if (libc_func != next_func) { 
printf("[!] Preload hooks detected\n"); 
printf("Libc address: %p\n", libc_func); 
printf("Next address: %p\n", next_func); 

}
memset(strorig,0,10);
strprint = strorig;
thing = strstr(data, "SYS");
if(thing != NULL)
goto REDO;
}
memset(data,0,50000);
l = fread((void *)data, sizeof(char), 50000,fp);
}

fclose(fp); 
free(strorig); 
free(data); 

return 0; 

}
