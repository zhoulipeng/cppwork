/*
 * #define G_LOG_USE_STRUCTURED 1
 */
#include <glib-2.0/glib.h>
#include <glib-2.0/glib-object.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>


GObject *
create_object (void)
{
  g_autoptr(GObject) obj = g_object_new (G_TYPE_OBJECT, NULL);
  printf("create_object %p\n", obj);

  if (FALSE)
    return NULL;

  return g_steal_pointer (&obj);
}

gboolean
get_object (GObject **obj_out)
{
  g_autoptr(GObject) obj = g_object_new (G_TYPE_OBJECT, NULL);
  printf("get_object %p\n", obj);

  if (FALSE)
    return FALSE;

  if (obj_out)
    *obj_out = g_steal_pointer (&obj);

  return TRUE;
}


int main(int argc, char **argv)
{
    /* Set dummy for all levels */
    g_autoptr(GObject) o1 = NULL;
    o1 = create_object();

    printf("create_object %p\n", o1);
    
    g_autoptr(GObject) o2 = NULL;
    get_object(&o2);
    printf("get_object %p\n", o2);

    return 0;

}
/*
yb@slakn:~/zhoulp/github/cppwork/glib$ ./gautoptr_steal
create_object 0x90da00
create_object 0x90da00
get_object 0x90da20
get_object 0x90da20
*/

