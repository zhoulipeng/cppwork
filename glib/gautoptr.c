/*
 * gcc gauto.c -o gauto -lglib-2.0 -lgobject-2.0 -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -L/usr/lib64/glib-2.0/lib
 */
#include <glib-2.0/glib.h>
#include <glib-2.0/glib/gstdio.h>
#include <glib-2.0/glib-object.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

gboolean
check_exists(GVariant *dict)
{
  g_autoptr(GVariant) dirname, basename = NULL;
  g_autofree gchar *path = NULL;

  dirname = g_variant_lookup_value (dict, "dirname", G_VARIANT_TYPE_STRING);

  if (dirname == NULL)
    return FALSE;

  basename = g_variant_lookup_value (dict, "basename", G_VARIANT_TYPE_STRING);

  if (basename == NULL)
    return FALSE;

  path = g_build_filename (g_variant_get_string (dirname, NULL),
                           g_variant_get_string (basename, NULL),
                           NULL);

  return g_access (path, R_OK) == 0;
}


int main(int argc, char **argv)
{
    g_autoptr(GVariantBuilder) b = NULL;
    g_autoptr(GVariant) dict = NULL;
    b = g_variant_builder_new (G_VARIANT_TYPE ("a{sv}"));
    g_variant_builder_add (b, "{sv}", "basename", g_variant_new_string ("pip.log"));
    g_variant_builder_add (b, "{sv}", "dirname", g_variant_new_string ("/home/yb/"));
    dict = g_variant_builder_end (b);

    printf("check_exists: %d\n", check_exists(dict)); 

    return 0;
}

