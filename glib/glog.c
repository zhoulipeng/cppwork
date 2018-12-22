/*
 * #define G_LOG_USE_STRUCTURED 1
 */
#include <glib-2.0/glib.h>
#include <stdio.h>
#include <string.h>

// gcc glog.c -o glog -lglib-2.0 -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -L/usr/lib64/glib-2.0/lib
static void _dummy(const gchar *log_domain,
                     GLogLevelFlags log_level,
                     const gchar *message,
                     gpointer user_data )

{
  /* Dummy does nothing */ 
  fprintf((FILE *) user_data, "%s\n", message);
  printf("%s\n", message);
  return ;      
}
int main(int argc, char **argv)
{
    /* Set dummy for all levels */
    FILE *file = fopen("glib.log", "a");
    //g_log_set_handler(NULL, G_LOG_LEVEL_MASK |G_LOG_FLAG_FATAL
    //               | G_LOG_FLAG_RECURSION , _dummy, file);
    g_log_set_handler (G_LOG_DOMAIN, G_LOG_LEVEL_MASK | G_LOG_FLAG_FATAL
                   | G_LOG_FLAG_RECURSION, _dummy, file);
    int i = 100;
    int *p = &i;
    //g_log_set_default_handler(_dummy, file);
    //g_log_structured (G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
    //              "MESSAGE_ID", "06d4df59e6c24647bfe69d2c27ef0b4e",
    //              "MY_APPLICATION_CUSTOM_FIELD", "some debug string",
    //              "MESSAGE", "This is a debug message about pointer %p and integer %u.",
    //              p, i);
    /* Set default handler based on argument for appropriate log level */
    if ( argc > 1)
    {
         /* If -vv passed set to ONLY debug */
         if(!strncmp("-vv", argv[1], 3))
         {
            g_log_set_handler(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,  g_log_default_handler, NULL);
         }
         /* If -v passed set to ONLY info */
         else if(!strncmp("-v", argv[1], 2))
         {
             g_log_set_handler(G_LOG_DOMAIN, G_LOG_LEVEL_INFO, g_log_default_handler, NULL);
         }
        /* For everything else, set to back to default*/
         else
         {
              g_log_set_handler(G_LOG_DOMAIN, G_LOG_LEVEL_MASK, g_log_default_handler, NULL);
         }

    }
    else /* If no arguments then set to ONLY warning & critical levels */
    {
        //g_log_set_handler(G_LOG_DOMAIN, G_LOG_LEVEL_WARNING| G_LOG_LEVEL_CRITICAL, g_log_default_handler, NULL);
    }

    g_log(NULL, G_LOG_LEVEL_WARNING, "This is warning1");
    g_warning("This is warning");
    g_message("This is message");
    g_debug("This is debug");
    //g_critical("This is critical");
    char str[1024] = {0};
    sprintf(str, "%s:%s", G_STRLOC, G_STRFUNC);
    g_log(NULL, G_LOG_LEVEL_INFO , "%s,This is info", str);
    
    g_info("%s", G_STRLOC);
    fclose(file);
    return 0;
}

