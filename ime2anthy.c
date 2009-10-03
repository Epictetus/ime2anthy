#include <glib-2.0/glib.h>
#include <glib/gprintf.h>
#include <glib/gstdio.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>

static void usage(char *prog);
static void read_file(char *name);

static inline void test(const gchar *p)
{
	GString *str = g_string_new(p);
	g_printf("%s\n", str->str);
	g_string_free(str, TRUE);
}

static void read_file(char *name)
{
	FILE *fp;
	gchar str[1024];

	fp = g_fopen(name, "r");
	assert(fp != NULL);

	memset(str, 0, sizeof(str));

	while (fgets(str, sizeof(str), fp) != NULL) {
		if (str[0] != '!' && str[0] != ' ') {
			gchar **array = g_strsplit(str, "\t", 3);
			test(array[0]);
			test(array[1]);
			test(array[2]);
			g_strfreev(array); 
				
#if 0
			GString *p = g_string_new(str);
			g_printf("%s", p->str);
			g_string_free(p, TRUE);
#endif
		}
		memset(str, 0, sizeof(str));
	}

	fclose(fp);
}

static void usage(char *prog)
{
	fprintf(stderr, "usage %s <ime dictionary>\n\tNote: IME dictionary's charactor code should be UTF-8\n", prog);
	exit(-1);
}

int main(int argc, char **argv)
{
	if (argc != 2)
		usage(argv[0]);

	read_file(argv[1]);

	return 0;
}
