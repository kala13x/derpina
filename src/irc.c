/*
 *  derpina/irc.c
 *
 *  Copyleft (C) 2015  Sun Dro
 *
 * Operations for Internet Realy Chat bot.
 * Licensed by MIT license. See LICENSE file.
 */


#include "../slog/slog.h"
#include "stdinc.h"
#include "conf.h"
#include "irc.h"

#define MAXMSG 4098

/*
 * search_str - Search string in another string. If string 
 * is found, function returns 1, else 0 or -1. Argument str 
 * is string to search and srch is string we want search for.
 */
int search_str(char *str, char *srch)
{
    int lenstr = strlen(str);
    int lensrch = strlen(srch);
    int found = -1;
 
    /* Search through each char in str */
    for (int i = 0; i < lenstr; i++)
    {
        /* If first search item is valid then search str */
        if (srch[0] == str[i])
        {
            found = 1;

            /* Search the char array for search field */
            for (int x = 1; x < lensrch; x++) 
                if (str[i+x] != srch[x]) found = 0;
 
            /* Return if found */
            if (found) return found;
        }
    }
 
    return found;
}


/*
 * send_data - Send data via socket. Function sends buffer with send 
 * function and returns 1 on success. Otherwise it returns 0. Argument 
 * sock is socket descriptor and buf is data which we want to send.
 */
int send_data(int sock, char *buf)
{
    int len = strlen(buf);
    int bytes_sent = send(sock, buf, len, 0);
 
    /* Check status */
    if (!bytes_sent) return 0;
    
    return 1;
}


/*
 * create_socket - Create socket for irc server. Function creates and returns 
 * connected socket for irc server. If error is occured function returns -1. 
 * Argument addr is address for socket and port is port for socket creation.
 */
int create_socket(char *addr, char *port) 
{
    struct addrinfo hints, *sinfo;
    int sock;

    /* Make sure that sinfo is clear */
    memset(&hints, 0, sizeof hints);
 
    /* Setup hints */
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
 
    /* Setup the structs */
    if (getaddrinfo(addr, port, &hints, &sinfo)) 
    {
        slog(0, SLOG_ERROR, "Can not get addres info: %s", addr);
        exit(-1);
    }
 
    /* Setup the socket */
    if ((sock = socket(sinfo->ai_family, sinfo->ai_socktype, sinfo->ai_protocol)) == -1)
    {
        slog(0, SLOG_ERROR, "Can not create socket");
        exit(-1);
    }
 
    /* Connect to the socker */
    if (connect(sock,sinfo->ai_addr, sinfo->ai_addrlen) == -1)
    {
        slog(0, SLOG_ERROR, "Can not connect to socket: %d", sock);
        close (sock);
        exit(-1);
    }
 
    /* We dont need this anymore */
    freeaddrinfo(sinfo);

    return sock;
}


/* 
 * authorise_user - Get authorisatin to the irc server, join to 
 * channel and anser ping requests. Argument usr is pointer of 
 * IRCUser structure and inf is pointer of IRCInfo structure.
 */
int authorise_user(IRCUser *usr, IRCInfo *inf) 
{
    int sock, bytes;
    int count = 0;
    int joined = 0;
    char buf[MAXMSG];
    char cmd[128];

    /* Create socket */
    sock = create_socket(inf->server, inf->port);
    if (sock < 0) return -1;
 
    while (1)
    {
        if (!joined) 
        {
            /* Move on */
            count++;
     
            switch (count) {
                case 3:
                    /* After 3 recives send data to server */
                    send_data(sock, usr->nick);
                    send_data(sock, usr->name);
                    break;
                case 4:
                    /* Join in channel */
                    bzero(cmd, sizeof(cmd));
                    sprintf(cmd, "JOIN #%s\r\n", inf->channel);

                    send_data(sock, cmd);
                    joined = 1;
                default:
                    break;
            }
        }
 
        /* Recieve data from socket */
        bytes = recv(sock, buf, MAXMSG-1, 0);
        buf[bytes] = '\0';

        /* Print recieved buffer */
        if (strlen(buf) > 0) slog(0, SLOG_LIVE, "Recieved from IRC: %s", buf);
 
        /* Check if ping request and send pong */
        //if (search_str(buf, "PING")) ret = send_keepalive(buf);

        /* Check if joined to channel */
        //if (joined) return sock;
    }
}