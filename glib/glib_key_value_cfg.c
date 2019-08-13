#include <glib.h>
#include <glib/gprintf.h>


/*
 * ---------------- config.ini content ------
 * [config]
 * raw=@ByteArray(|(>16<y)
 * remember=2
 * user=leon
 */
void readconfig(void)
{
    g_autoptr(GError) error = NULL;
    g_autoptr(GKeyFile) key_file = g_key_file_new();
    printf("111");

    if (!g_key_file_load_from_file(key_file, "/home/yb/.eacloudfuse.ini", G_KEY_FILE_NONE, &error)) {
        printf("info 11");
        if (!g_error_matches(error, G_FILE_ERROR, G_FILE_ERROR_NOENT))
            g_warning("Error loading key file: %s", error->message);
        return;
    }

    g_autofree gchar* val = g_key_file_get_string(key_file, "config", "user", &error);
    if (val == NULL && !g_error_matches(error, G_KEY_FILE_ERROR, G_KEY_FILE_ERROR_KEY_NOT_FOUND)) {
        g_warning("Error finding key in key file: %s", error->message);
        return;
    } else if (val == NULL) {
        // Fall back to a default value.
        val = g_strdup("default-value");
    }
    printf("info: %s", val);

}

int main(void)
{
    readconfig();
}

