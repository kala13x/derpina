/*
 *  utils/info.c
 *
 *  Copyleft (C) 2015  Sun Dro
 *
 * Get additional information.
 */


#include "stdinc.h"
#include "slog.h"
#include "info.h"


/* 
 * derp_version - Get program version. Function returns 
 * version, build number and build date of application. 
 * Return value is char pointer.
 */
const char* derp_version()
{
    static char str[128];
    sprintf(str, "%d.%d build %d (%s)", 
        VERSION_MAX, VERSION_MIN, BUILD_NUMBER, __DATE__);

    return str;
}


/* 
 * derp_version_short - Get program version. Function returns only
 * version, build number of application without additional strings. 
 * Return value is char pointer. For example: 1.0.32
 */
const char* derp_version_short()
{
    static char str[128];
    sprintf(str, "%d.%d.%d", 
        VERSION_MAX, VERSION_MIN, BUILD_NUMBER);

    return str;
}


/* 
 * greet - Print greeting message. Function returns program 
 * name, version, build number and build date. Argument 
 * p_name is program name from which we want to greet users. 
 */
void greet(char *p_name)
{
    printf("====================================================\n");
    printf("%s Version: %s\n", p_name, derp_version());
    printf("====================================================\n");
}


/*
 * usage - Print help and usage information.
 */
void usage(void) 
{
    printf("\nUsage: derp [-u <user>] [-n <nick>] [-a] [-o] [-s <server>] [-p <port>] [-c <room>] [-h]\n");
    printf("options are:\n");
    printf(" -u         # Username for IRC\n");
    printf(" -n         # Nickname for IRC\n");
    printf(" -a         # Agent mode\n");
    printf(" -o         # Logout from agent mode\n");
    printf(" -s         # Server address for IRC\n");
    printf(" -p         # Port for IRC\n");
    printf(" -c         # Channel room for IRC\n");
    printf("\n");
}
