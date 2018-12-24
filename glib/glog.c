/*
 * #define G_LOG_USE_STRUCTURED 1
 */
#include <glib-2.0/glib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define S3FS_LOW_LOGPRN(level, fmt, ...) \
       if(S3FS_LOG_CRIT == level || (S3FS_LOG_CRIT != debug_level && level == (debug_level & level))){ \
         if(foreground){ \
           fprintf(stdout, "%s%s:%s(%d): " fmt "%s\n", S3FS_LOG_LEVEL_STRING(level), __FILE__, __func__, __LINE__, __VA_ARGS__); \
         }else{ \
           syslog(S3FS_LOG_LEVEL_TO_SYSLOG(level), "%s%s:%s(%d): " fmt "%s", instance_name.c_str(), __FILE__, __func__, __LINE__, __VA_ARGS__); \
         } \
       }
#define S3FS_PRN_ERR(fmt, ...)    S3FS_LOW_LOGPRN(S3FS_LOG_ERR,  fmt, ##__VA_ARGS__, "")

//参考s3fs的定义如下
#define log_debug(fmt, ...) g_debug("%s:%s(%d)"     fmt "", __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define log_info(fmt, ...)  g_info("%s:%s(%d)"      fmt "", __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define log_msg(fmt, ...)   g_message("%s:%s(%d)"   fmt "", __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define log_warn(fmt, ...)  g_warning("%s:%s(%d)"   fmt "", __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define log_crit(fmt, ...)  g_critical("%s:%s(%d)"  fmt "", __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define log_error(fmt, ...) g_error("%s:%s(%d)"     fmt "", __FILE__, __func__, __LINE__, ##__VA_ARGS__)

// gcc glog.c -o glog -lglib-2.0 -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -L/usr/lib64/glib-2.0/lib
// 关闭不了error 日志
// gcc -DG_DISABLE_ASSERT glog.c -s -o glog -lglib-2.0 -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -L/usr/lib64/glib-2.0/lib
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
    // NULL or "" equal to G_LOG_DOMAIN
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

    log_warn("This is warning");
    log_msg("This is message");
    log_debug("This is debug");
    char str[1024] = {0};
    sprintf(str, "%s:%s", G_STRLOC, G_STRFUNC);
    //assert(1 == 0);
    log_crit("This is critical");
    log_error("%s,This is info", str);
    
    log_crit("%s", G_STRLOC);
    log_info("This is info");
    fclose(file);
    return 0;
}

