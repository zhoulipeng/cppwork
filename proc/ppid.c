#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include  <proc/readproc.h>
// void printppid(pid_t pid) {
//     proc_t     process_info;
//     get_proc_stats(pid, &process_info);// 这个函数头文件中有，但是连接不上，估计已经废除了
//     printf("Parent of pid=%d is pid=%d\n", pid, process_info.ppid);
// }  

void read_pidinfo(pid_t pid) {
    proc_t     proc_info;
    memset(&proc_info, 0, sizeof(proc_info));
    PROCTAB *pt_ptr = openproc(PROC_FILLSTATUS | PROC_PID, &pid);
    if (readproc(pt_ptr, &proc_info) != 0) {
	    printf("%s\n", proc_info.cmd);
	    printf("%d", proc_info.ppid);
    }else{
	    printf("pid not found.");
    }
    closeproc(pt_ptr);
    printf("Parent of pid=%d is pid=%d\n", pid, proc_info.ppid);
}  

void get_exe_parentpid_bypid(pid_t pid, char* return_str, size_t bufsiz)
{
    char path[128] = { 0 };
    char buf[1024] = { 0 };
    int ppid_index = 4;
    sprintf(path, "/proc/%d/stat", pid);
    FILE *f = fopen(path, "r");
    if(f){
        fread(buf, 1, 1024, f);
        char *p,*str= " ", *value = NULL;
        p=buf;
        do{
            value = strsep(&p,str);
            printf("%s\n ", value);
            // printf("=== %s\n ",p);
        }while(--ppid_index);
        snprintf(return_str, bufsiz, "%s", value);
        fclose(f);
    }
}
// gcc ppid.c -o ppid.exe -lprocps
int main(int argc, char *argv[])
{
    char buf[8] = { 0 };
    read_pidinfo(1302);
    get_exe_parentpid_bypid(1302, buf, sizeof(buf));
    printf("buf=%s\n", buf);
    
    return 0;
}

