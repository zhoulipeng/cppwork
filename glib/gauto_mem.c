/*
 * #define G_LOG_USE_STRUCTURED 1
 */
#include <glib-2.0/glib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>


gboolean
operate_on_malloc_buf (void)
{
  g_autofree guint8* membuf = NULL;
  //guint8* membuf = NULL;

  membuf = g_malloc (8192);

  // Some computation on membuf

  // membuf will be automatically freed here
  return TRUE;
}

int main(int argc, char **argv)
{
    /* Set dummy for all levels */
    operate_on_malloc_buf();

    return 0;
}
/* 
///////////////////////////////////////////////////////////////
if don't use g_autofree, test result is like this:
///////////////////////////////////////////////////////////////

yb@slakn:~/zhoulp/github/cppwork/glib$ valgrind ./gauto                                                                                               
==14699== Memcheck, a memory error detector
==14699== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==14699== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==14699== Command: ./gauto
==14699==
==14699==
==14699== HEAP SUMMARY:
==14699==     in use at exit: 26,796 bytes in 7 blocks
==14699==   total heap usage: 7 allocs, 0 frees, 26,796 bytes allocated
==14699==
==14699== LEAK SUMMARY:
==14699==    definitely lost: 8,192 bytes in 1 blocks
==14699==    indirectly lost: 0 bytes in 0 blocks
==14699==      possibly lost: 0 bytes in 0 blocks
==14699==    still reachable: 18,604 bytes in 6 blocks
==14699==         suppressed: 0 bytes in 0 blocks
==14699== Rerun with --leak-check=full to see details of leaked memory
==14699==
==14699== For counts of detected and suppressed errors, rerun with: -v
==14699== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

///////////////////////////////////////////////////////////////
if use g_autofree, test result is like this:
///////////////////////////////////////////////////////////////
==14119== Memcheck, a memory error detector
==14119== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==14119== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==14119== Command: ./gauto
==14119==
==14119==
==14119== HEAP SUMMARY:
==14119==     in use at exit: 18,604 bytes in 6 blocks
==14119==   total heap usage: 7 allocs, 1 frees, 26,796 bytes allocated
==14119==
==14119== LEAK SUMMARY:
==14119==    definitely lost: 0 bytes in 0 blocks
==14119==    indirectly lost: 0 bytes in 0 blocks
==14119==      possibly lost: 0 bytes in 0 blocks
==14119==    still reachable: 18,604 bytes in 6 blocks
==14119==         suppressed: 0 bytes in 0 blocks
==14119== Rerun with --leak-check=full to see details of leaked memory
==14119==
==14119== For counts of detected and suppressed errors, rerun with: -v
==14119== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

*/
