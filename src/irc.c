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
#include "proto.h"
#include "conf.h"
#include "irc.h"

#define MAXMSG 4098


/*
 * create_socket - Create socket for irc server. Function creates and returns 
 * connected socket for irc server. If error is occured function returns -1. 
 * Argument addr is address for socket and port is port for socket creation.
 */
int create_socket(char *addr, char *port) 
{
    struct addrinfo hints, *sinfo;
    int sock, ret;

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
    sock = socket(sinfo->ai_family, sinfo->ai_socktype, sinfo->ai_protocol);
    if (sock < 0)
    {
        slog(0, SLOG_ERROR, "Can not create socket");
        exit(-1);
    }
 
    /* Connect to the socker */
    ret = connect(sock, sinfo->ai_addr, sinfo->ai_addrlen);
    if (ret < 0)
    {
        slog(0, SLOG_ERROR, "Can not connect to the irc socket: %d", sock);

        close(sock);
        return -1;
    }
 
    /* We dont need this anymore */
    freeaddrinfo(sinfo);

    return sock;
}


/* 
 * is_connected - Check if we are connected to the server.
 * Function searchs MOTD in recieved buffer and returns 1
 * if found, otherwise return value is 0. buf is a buffer.
 */
int is_connected(char *buf) 
{
    if (search_str(buf, "/MOTD") > 0)
        return 1;
    else if (search_str(buf, "MOTD") > 0)
        return 1;
    else
        return 0;
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
 * send_message - Send message in chat room. Function sends buffer with socket 
 * in channel room. Argument sock is socket and  buf is buffer we want to send.
 */
void send_message(int sock, char *chan, char *buf) 
{
    char msg[MAXMSG];

    /* Create response */
    bzero(msg, sizeof(msg));
    sprintf(msg, "PRIVMSG #%s :%s\r\n", chan, buf);
    send_data(sock, msg);

    /* Log response */
    sscanf(msg, "%512[^\n]\n", msg);
    slog(0, SLOG_LIVE, "%s", msg);

}


/* 
 * send_keepalive - Function searchs ping request in recieved 
 * buffer and send pong answer to the socket. Argument sock 
 * is socket descriptor and buf is buffer recieved from socket. 
 */
void send_keepalive(int sock, char *buf)
{ 
    char *str = "PING ";
    int found, i, x;
 
    for (i = 0; i < strlen(buf); i++)
    {
        //If the active char is equil to the first search item then search str
        if (buf[i] == str[0])
        {
            found = 1;

            /* Search the char array for search field */
            for (x = 1; x < 4; x++) 
                if (buf[i+x]!=str[x]) found = 0;
 
            /* If found */
            if (found)
            {
                int count = 0;

                /* Count the chars */
                for (x = (i+strlen(str)); x < strlen(buf);x++)
                    count++;

                /* Create the new char array */
                char returnHost[count + 5];
                bzero(returnHost, sizeof(returnHost));
                returnHost[0]='P';
                returnHost[1]='O';
                returnHost[2]='N';
                returnHost[3]='G';
                returnHost[4]=' ';
                count = 0;

                /* Set the hostname data */
                for (x = (i+strlen(str)); x < strlen(buf);x++)
                {
                    returnHost[count+5]=buf[x];
                    count++;
                }
 
                /* Send the pong */
                if (send_data(sock, returnHost)) 
                {
                    slog(0, SLOG_LIVE, "Answering with PONG");
                    return;
                }
            }
        }
    }
}


/* 
 * handle_msg - Handle messages from recieved buffer and make 
 * correct response. Argument sock is socket descriptor, buf 
 * is recieved buffer and usr is bot nickname on irc server.
 */
void handle_msg(int sock, char *buf, char *usr, char *chan, int agent) 
{
    char *answer;

    /* Check if they are talking to us */
    if(search_str(buf, usr) > 0) 
        answer = watch_private_chat(buf, agent);
    else 
        answer = watch_whole_chat(buf, agent);

    /* Make answer */
    if (answer != NULL) 
            send_message(sock, chan, answer);

    /* Check if ping request and send pong */
    if (search_str(buf, "PING") > 0) 
        send_keepalive(sock, buf);
}


/* 
 * authorize_user - Get authorizatin to the irc server, join to 
 * channel and anser ping requests. Argument usr is pointer of 
 * IRCUser structure and inf is pointer of IRCInfo structure.
 */
int authorize_user(IRCUser *usr, IRCInfo *inf) 
{
    int sock, bytes;
    char buf[MAXMSG];
    char cmd[128];

    /* Create socket */
    sock = create_socket(inf->server, inf->port);
    if (sock < 0) 
    {
        slog(0, SLOG_FATAL, "Connection failed to the socket");
        exit(-1);
    }
 
    /* Clear command */
    bzero(cmd, sizeof(cmd));

    /* Send nick to the server */
    sprintf(cmd, "NICK %s\r\n", usr->nick);
    send_data(sock, cmd);

    /* Send user to the server */
    sprintf(cmd, "USER %s 0 0 :%s\r\n", usr->name, usr->nick);
    send_data(sock, cmd);

    while (1)
    {
        /* Recieve data from socket */
        bytes = recv(sock, buf, MAXMSG-1, 0);
        buf[bytes] = '\0';

        /* Check if connected */
        if (is_connected(buf)) 
        {
            /* Join in channel */
            sprintf(cmd, "JOIN #%s\r\n", inf->channel);
            send_data(sock, cmd);
            return sock;
        }
    }
}
