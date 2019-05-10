/** 
 * @FileName    linux_process_fork1_5.c
 * @Describe    Linux C/C++多进程同时写一个文件（一）
 * @Author      vfhky 2017-10-28 13:23 https://typecodes.com/cseries/linuxmutilprocesswrite1.html
 * @Compile     gcc linux_process_fork1_5.c -o linux_process_fork1_5
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>


#define FILE_NAME "LINUX_MUTIL_PROCESS_WRITE"
static const char *p_buf = "123456789";


int main( const int argc, const char * const *argv )
{
    //这里使用ab，其中a表示追加，它能原子性地保证进程对应的文件表项中的当前文件偏移量每一次都等于v节点表中当前文件长度。
    FILE *fp = fopen( FILE_NAME, "ab" );
    if( fp == NULL )
    {
        printf( "Can not opent [%s].\n", FILE_NAME );
        return -1;
    }

    for( int i=0; i<10000000000; i++ )
    {
        usleep( 1000 );
        fwrite( p_buf, strlen( p_buf ), 1, fp );
    }
    fclose( fp );

    printf( "Write end.\n" );

    return 0;
}
