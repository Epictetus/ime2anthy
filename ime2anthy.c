#include <glib-2.0/glib.h>
#include <glib/gprintf.h>
#include <glib/gstdio.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define NKF_OUTPUT_FILE_NAME "ime_std_utf.txt"
#define ANTHY_DIC_FILE_NAME "/.anthy/2ch_kaomoji.txt"

static void usage(char *prog);
static void do_command(char *com);
static void create_anthy_dic_template(char *file);
static void do_nkf(char *in, char *in_code, char *out, char *out_code);
static void convert_imedic2anthydic(char *in, char *out);
static void load_dictionary(char *file);

static void load_dictionary(char *file)
{
	char cmd[512] = { 0 };

	snprintf(cmd, sizeof(cmd) - 1,
		 "cat %s | anthy-dic-tool --load --utf8", file);

	do_command(cmd);

}

static void convert_imedic2anthydic(char *in, char *out)
{
	FILE *rfp;
	FILE *wfp;
	gchar str[1024] = { 0 };

	rfp = g_fopen(in, "r");
	assert(rfp != NULL);

	wfp = g_fopen(out, "a+");
	assert(wfp != NULL);

	g_fprintf(wfp, "\n");

	while (fgets(str, sizeof(str), rfp) != NULL) {
		if (str[0] != '!' && str[0] != ' ') {
			gchar word[1024] = { 0 };
			gchar **array = g_strsplit(str, "\t", 3);

			if (array[2]) {
				g_snprintf(word, sizeof(word) - 1, "%s #T35 %s\n",
					   array[0],
					   array[1]);
				
				g_strfreev(array); 
				g_fprintf(wfp, "%s", word);
			}
		}
		memset(str, 0, sizeof(str));
	}

	fclose(rfp);
	fclose(wfp);
}

static void do_command(char *com)
{
	assert(system(com) != -1);
}

static void do_nkf(char *in, char *in_code, char *out, char *out_code)
{
	char com[512] = { 0 };

	snprintf(com, sizeof(com) - 1, "nkf -%s -%s %s > %s", out_code, in_code, in, out);
	do_command(com);
}

static void usage(char *prog)
{
	fprintf(stderr, "usage %s <ime dictionary>\n", prog);
	exit(-1);
}

int main(int argc, char **argv)
{
	char anthy_dic[512] = { 0 };

	if (argc != 2)
		usage(argv[0]);

	snprintf(anthy_dic, sizeof(anthy_dic) - 1, "%s/%s", getenv("HOME"), ANTHY_DIC_FILE_NAME);

	create_anthy_dic_template(anthy_dic);
	do_nkf(argv[1], "S", NKF_OUTPUT_FILE_NAME, "w8");

	convert_imedic2anthydic(NKF_OUTPUT_FILE_NAME, anthy_dic);

	do_nkf(ANTHY_DIC_TMP_FILE_NAME, "W8", ANTHY_DIC_FILE_NAME, "E");

	g_remove(NKF_OUTPUT_FILE_NAME);

	return 0;
}
