/*
 *  utils/info.c
 *
 *  Copyleft (C) 2015  Sun Dro (a.k.a. kala13x)
 *
 * Get additional information about library
 */


#include "stdinc.h"
#include "slog.h"
#include "info.h"


/* 
 * utils_version - Get library version. Function returns 
 * version, build number and build date of application. 
 * Return value is char pointer.
 */
const char* utils_version()
{
    static char str[128];
    sprintf(str, "%d.%d build %d (%s)", 
        VERSION_MAX, VERSION_MIN, BUILD_NUMBER, __DATE__);

    return str;
}


/* 
 * utils_version_short - Get library version. Function returns only
 * version, build number of application without additional strings. 
 * Return value is char pointer. For example: 1.0.32
 */
const char* utils_version_short()
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
    printf("============================================================\n");
    printf("%s. Utils Version: %s\n", p_name, utils_version());
    printf("============================================================\n");
}


/*
 * print_system_info - Get and print operating system information
 * such as system name, release, version, hardware identifier and etc.
 */
void print_system_info() 
{
    struct utsname udata;

    /* Get sys information */
    if (!uname(&udata))
    {
        slog(0, SLOG_INFO, "Sys Name: %s", udata.sysname);
        slog(0, SLOG_INFO, "Hostname: %s", udata.nodename);
        slog(0, SLOG_INFO, "Release: %s", udata.release);
        slog(0, SLOG_INFO, "Version: %s", udata.version);
        slog(0, SLOG_INFO, "Machine: %s", udata.machine);

        #ifdef _GNU_SOURCE
        slog(0, SLOG_INFO, "Domain Name: %s", udata.domainname);
        #endif
    }
}