/*

*/

#include <stdio.h>
#include "mountlist.h"


static struct mount_entry *mount_list = NULL;
//static struct fs_type_list *fs_select_list;
//static struct fs_type_list *fs_exclude_list;
//static bool print_type;
int main(int argc, char *argv[])
{
    printf("read_file_system_list \n");
    mount_list = read_file_system_list(false);
    if(mount_list)
    {
        printf("get ok\n");
        struct mount_entry *me = mount_list;
        for(; me;)
        {
        	if(me->me_mountdir )printf("read_file_system_list %s\n", me->me_mountdir);
        	//printf("read_file_system_list  \n" );
        	me=me->me_next;
        }

    }
     while (mount_list)
     {
        me = mount_list->me_next;
        free_mount_entry (mount_list);
        mount_list = me;
     }

    printf("return \n");
}
