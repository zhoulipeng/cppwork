/** 
 * @FileName    linux_process_fork1_7.c
 * @Describe    Linux C/C++多进程同时写一个文件（三）
 * @Author      vfhky 2017-10-29 22:23 https://typecodes.com/cseries/linuxmutilprocesswrite3.html
 * @Compile     gcc linux_process_fork1_7.c -o linux_process_fork1_7
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define FILE_NAME "LINUX_MUTIL_PROCESS_WRITE_2"
static const char *p_buf = "123456789";


int main( const int argc, const char * const *argv  )
{
    int i_fd = open( FILE_NAME, O_RDWR | O_CREAT );
    if( -1 == i_fd )
    {
        printf( "Open file=[%s] failed, errno=[%d] errmsg=[%s].\n", FILE_NAME, errno, strerror( errno ) );
        return -1;
    }

    static int i_write_len = 0;
    for( int i=0; i<10000; i++ )
    {
        //每次都偏移到文件最末尾
//        if( 0 > lseek( i_fd, 0L, SEEK_END ) )
//        {
//          PRINTNOLOG( "lseek error.\n" );
//          return 0;
//        }

        usleep( 1000 );

        i_write_len += write( i_fd, p_buf, strlen( p_buf ) );
    }
    close( i_fd );
    printf( "i_write_len=[%ld].\n", i_write_len );

    return 0;
}
