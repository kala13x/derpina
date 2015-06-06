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


/*
 * create_socket - Create socket for irc server. Function creates and returns 
 * connected socket for irc server. If error is occured function returns -1. 
 * Argument addr is address for socket and port is port for socket creation.
 */
int create_socket(char *addr, char *port);


/* 
 * authorise_user - Get authorisatin to the irc server, join to 
 * channel and anser ping requests. Argument usr is pointer of 
 * IRCUser structure and inf is pointer of IRCInfo structure.
 */
int authorise_user(IRCUser *usr, IRCInfo *inf);


/* For include header in CPP code */
#ifdef __cplusplus
}
#endif