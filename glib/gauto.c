/*
 * #define G_LOG_USE_STRUCTURED 1
 */
#include <glib-2.0/glib.h>
#include <glib-2.0/glib-object.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

GVariant *
my_func(void)
{
  g_auto(GQueue) queue = G_QUEUE_INIT;
  g_auto(GVariantBuilder) builder;
  g_auto(GStrv) strv;

  g_variant_builder_init (&builder, G_VARIANT_TYPE_VARDICT);
  strv = g_strsplit("a:b:c", ":", -1);

  // ...

  if (FALSE)
    return NULL;

  // ...

  return g_variant_builder_end (&builder);
}


int main(int argc, char **argv)
{
    g_autoptr(GVariant) ptr = NULL;
    // must declare as g_autoptr and init as NULL
    ptr = my_func();

    return 0;
}
/* 
///////////////////////////////////////////////////////////////
if don't use g_autofree, test result is like this:
///////////////////////////////////////////////////////////////

*/
