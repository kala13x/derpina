/*
 *  derpina/derp.c
 *
 *  Copyleft (C) 2015  Sun Dro
 *
 * Concept of Internet Realy Chat bot.
 * Licensed by MIT license. See LICENSE file.
 */

#include "stdinc.h"
#include "slog/slog.h"
#include "info.h"
#include "irc.h"

#define MAXDATASIZE 4048

/* 
 * clean_prog - Cleanup on exit. Function handles exit signal
 * and terminates the program. Argument sig is signal value.
 */
void clean_prog(int sig)
{
    /* Handle signals */
    if (sig == SIGILL || sig == SIGSEGV) 
        slog(0, SLOG_NONE, "[%s] Incorrect data!\n", 
            strclr(2, "FATAL"));

    slog(0, SLOG_LIVE, "Cleaning up...");
    exit(-1);
}

void start()
{
    struct addrinfo hints, *servinfo;
    int s;
    char *port;
    char usr[64];
    char nick[64];
    strcpy(nick, "NICK Derpina\r\n");
    strcpy(usr, "USER Derpina Derpity :Derp derpity\r\n");
 
    //Setup run with no errors
 
    port = "6667";
 
    //Ensure that servinfo is clear
    memset(&hints, 0, sizeof hints); // make sure the struct is empty
 
    //setup hints
    hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
 
    //Setup the structs if error print why
    int res;
    if ((res = getaddrinfo("10.10.17.68",port,&hints,&servinfo)) != 0)
    {
        fprintf(stderr,"getaddrinfo: %s\n", gai_strerror(res));
    }
 
    //setup the socket
    if ((s = socket(servinfo->ai_family,servinfo->ai_socktype,servinfo->ai_protocol)) == -1)
    {
        perror("client: socket");
    }
 
    //Connect
    if (connect(s,servinfo->ai_addr, servinfo->ai_addrlen) == -1)
    {
        close (s);
        perror("Client Connect");
    }
 
    //We dont need this anymore
    freeaddrinfo(servinfo);
 
    //Recv some data
    int numbytes;
    char buf[MAXDATASIZE];
 
    int count = 0;
    while (1)
    {
        //declars
        count++;
 
        switch (count) {
            case 3:
                    //after 3 recives send data to server (as per IRC protacol)
                    send_data(s, nick);
                    send_data(s, usr);
                break;
            case 4:
                    //Join a channel after we connect, this time we choose beaker
                send_data(s, "JOIN #qarva\r\n");
            default:
                break;
        }
 
        //Recv & print Data
        numbytes = recv(s,buf,MAXDATASIZE-1,0);
        buf[numbytes]='\0';
        slog(0, SLOG_DEBUG, "%s", buf);
        //buf is the data that is recived
 
        //break if connection closed
        if (numbytes==0)
        {
            slog(0, SLOG_DEBUG, "Closed connection");
            break;
        }
    }
}

/* 
 * main - Main function initializes everything ans starts irc 
 * loop which is responsible for keep alive irc connection.
 */
int main ()
{
    /* Greet users */
    greet("IRC Bot Derpina");
    slog(0, SLOG_INFO, "Logger Version: %s", slog_version(1));

    /* Catch ilegal signal */
    signal(SIGINT, clean_prog);
    signal(SIGSEGV, clean_prog);
    signal(SIGILL , clean_prog);

    /* Initialize logger */
    init_slog("derpina", "conf.cfg", 2);
    slog(0, SLOG_DEBUG, "We run!");

    start();

    return 0;
}