/*
 *  derpina/irc.h
 *
 *  Copyleft (C) 2015  Sun Dro
 *
 * Operations for Internet Realy Chat bot.
 * Licensed by MIT license. See LICENSE file.
 */


/* For include header in CPP code */
#ifdef __cplusplus
extern "C" {
#endif


/*
 * search_str - Search string in another string. If string 
 * is found, function returns 1, else 0 or -1. Argument str 
 * is string to search and srch is string we want search for.
 */
int search_str(char *str, char *srch);


/*
 * send_data - Send data via socket. Function sends buffer with send 
 * function and returns 1 on success. Otherwise it returns 0. Argument 
 * sock is socket descriptor and buf is data which we want to send.
 */
int send_data(int sock, char *buf);


/* For include header in CPP code */
#ifdef __cplusplus
}
#endif