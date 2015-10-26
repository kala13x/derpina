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
 * strsrc - Search string in another string. If string 
 * is found, function returns 1, else 0 or -1. Argument str 
 * is string to search and srch is string we want search for.
 */
int strsrc(char *str, char *srch);


/*
 * send_data - Send data via socket. Function sends buffer with send 
 * function and returns 1 on success. Otherwise it returns 0. Argument 
 * sock is socket descriptor and buf is data which we want to send.
 */
int send_data(int sock, char *buf);


/* 
 * send_keepalive - Function searchs ping request in recieved 
 * buffer and send pong answer to the socket. Argument sock 
 * is socket descriptor and buf is buffer recieved from socket. 
 */
void send_keepalive(int sock, char *buf);


/* 
 * send_message - Send message in chat room. Function sends buffer with socket 
 * in channel room. Argument sock is socket and  buf is buffer we want to send.
 */
void send_message(int sock, char *chan, char *buf);


/* 
 * handle_msg - Handle messages from recieved buffer and make 
 * correct response. Argument sock is socket descriptor, buf 
 * is recieved buffer and usr is bot nickname on irc server.
 */
void handle_msg(int sock, char *buf, char *usr, char *chan, int agent);


/* 
 * authorize_user - Get authorizatin to the irc server, join to 
 * channel and anser ping requests. Argument usr is pointer of 
 * IRCUser structure and inf is pointer of IRCInfo structure.
 */
int authorize_user(IRCUser *usr, IRCInfo *inf);


/* For include header in CPP code */
#ifdef __cplusplus
}
#endif