/*
 *  derpina/info.c
 *
 *  Copyleft (C) 2015  Sun Dro
 *
 * Get additional information about library
 */


#define VERSION_MAX     1
#define VERSION_MIN     2
#define BUILD_NUMBER    54


/* For include header in CPP code */
#ifdef __cplusplus
extern "C" {
#endif


/* 
 * derp_version - Get program version. Function returns 
 * version, build number and build date of application. 
 * Return value is char pointer.
 */
const char* derp_version();


/* 
 * derp_version_short - Get program version. Function returns only
 * version, build number of application without additional strings. 
 * Return value is char pointer. For example: 1.0.32
 */
const char* derp_version_short();


/* 
 * greet - Print greeting message. Function returns program 
 * name, version, build number and build date. Argument
 * p_name is program name from which we want to greet users. 
 */
void greet(char *p_name);


/*
 * usage - Print help and usage information.
 */
void usage(void);


/* For include header in CPP code */
#ifdef __cplusplus
}
#endif