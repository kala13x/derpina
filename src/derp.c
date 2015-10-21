/*
 *  derpina/derp.c
 *
 *  Copyleft (C) 2015  Sun Dro
 *
 * Concept of Internet Realy Chat bot.
 * Licensed by MIT license. See LICENSE file.
 */

#include "stdinc.h"
#include "send.h"
#include "conf.h"
#include "info.h"
#include "irc.h"

/* Lib includes */
#include "../slog/slog.h"
#include "../sms/magtisun.h"

#define CONFIG_FILE "conf.cfg"
#define MAXMSG 4098

static MutexSync lock;

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


/*
 * init_irc_info - Initialise irc connect information. Argument usr is 
 * pointer of IRCUser structure and inf is pointer of IRCInfo structure.
 */
void init_irc_info(IRCUser *usr, IRCInfo *inf) 
{
    bzero(usr->name, sizeof(usr->name));
    bzero(usr->nick, sizeof(usr->nick));
    bzero(inf->port, sizeof(inf->port));
    bzero(inf->server, sizeof(inf->server));
    bzero(inf->channel, sizeof(inf->channel));
    usr->agent = 0;
    usr->logout = 0;
}


/*
 * user_init_info - Initialise login variables from commandline input. 
 * Function initializes username and password (invisible password input) 
 * from commandline and saves values at MagtiSunLib structure as.
 */
 void user_init_info(MagtiSunLib* msl) 
{
    /* String variable */
    char *str;

    /* Get username */
    str = ret_slog("[INPUT] Enter Username: ");
    printf("%s", str);
    scanf("%s", msl->usr);

    /* Get password (invisible) */
    str = ret_slog("[INPUT] Enter Password: ");
    char* pwd = getpass(str);
    strcpy(msl->pwd, pwd);
}


/*
 * print_irc_info - Print basic irc information such as username, 
 * nickname, server and channel. Argument usr is pointer of IRCUser 
 * structure and inf is pointer of IRCInfo structure.
 */
void print_irc_info(IRCUser *usr, IRCInfo *inf) 
{
    slog(0, SLOG_INFO, "Username: %s", usr->name);
    slog(0, SLOG_INFO, "Nickname: %s", usr->nick);
    slog(0, SLOG_INFO, "Server: %s", inf->server);
    slog(0, SLOG_INFO, "Port: %s", inf->port);
    slog(0, SLOG_INFO, "Channel: %s", inf->channel);
}


/* 
 * fix_missing_input - Fix missing values from config file
 * or arguments inputed by user. Argument usr is pointer of 
 * IRCUser structure and inf is pointer of IRCInfo structure.
 */
void fix_missing_input(IRCUser *usr, IRCInfo *inf)
{
    if(strlen(usr->name) < 4) 
    {
        printf("%s", ret_slog("[INPUT] Username: "));
        scanf("%s", usr->name);
    }

    if(strlen(usr->nick) < 4) 
    {
        printf("%s", ret_slog("[INPUT] Nickname: "));
        scanf("%s", usr->nick);
    }

    if(strlen(inf->server) < 4) 
    {
        printf("%s", ret_slog("[INPUT] Server: "));
        scanf("%s", inf->server);
    }

    if(strlen(inf->port) < 4) 
    {
        printf("%s", ret_slog("[INPUT] Port: "));
        scanf("%s", inf->port);
    }

    if(strlen(inf->channel) < 4) 
    {
        printf("%s", ret_slog("[INPUT] Channel: "));
        scanf("%s", inf->channel);
    }
}


/* 
 * parse_arguments - Parse user inputed arguments and store 
 * data in structure variables. Argument usr is pointer of 
 * IRCUser structure and inf is pointer of IRCInfo structure.
 */
static int parse_arguments(int argc, char *argv[], IRCUser *usr, IRCInfo *inf)
{
    int c;
    while ( (c = getopt(argc, argv, "u:n:a1:o1:s:c:h1")) != -1) {
        switch (c) {
        case 'u':
            strcpy(usr->name, optarg);
            break;
        case 'n':
            strcpy(usr->nick, optarg);
            break;
        case 'a':
            usr->agent = 1;
            break;
        case 'o':
            usr->logout = 1;
            break;
        case 's':
            strcpy(inf->server, optarg);
            break;
        case 'p':
            strcpy(inf->port, optarg);
            break;
        case 'c':
            strcpy(inf->channel, optarg);
            break;
        case 'h':
        default:
            usage();
            exit(-1);
        }
    }

    return 0;
}


/* 
 * main - Main function initializes everything ans starts irc 
 * loop which is responsible for keep alive irc connection.
 */
int main(int argc, char *argv[])
{
    /* Used variables */
    int sock, bytes;
    char buf[MAXMSG];
    MagtiSunLib msl;

    /* Irc info */
    IRCUser usr;
    IRCInfo inf;

    /* Thread safe */
    sync_init(&lock);
    slog_set_mutex(&lock);

    /* Greet users */
    greet("IRC Bot Derpina");
    slog(0, SLOG_INFO, "Logger Version: %s", slog_version(1));

    /* Catch ilegal signal */
    signal(SIGINT, clean_prog);
    signal(SIGSEGV, clean_prog);
    signal(SIGILL , clean_prog);

    /* Initialize logger and irc info */
    init_slog("derpina", "conf.cfg", 2, NULL);
    init_irc_info(&usr, &inf);

    /* Initialize irc config */
    if (!parse_config(CONFIG_FILE, &usr, &inf)) 
    {
        slog(0, SLOG_ERROR, "Can not parse config file: %s", CONFIG_FILE);
        slog(0, SLOG_ERROR, "invalid config file or missing some value");
        exit(-1);
    }

    /* Parse cli arguments */
    parse_arguments(argc, argv, &usr, &inf);

    /* Fix missing user input */
    fix_missing_input(&usr, &inf);

    /* Check logout argument */
    if (usr.logout) 
    {
        slog(0, SLOG_LIVE, "Logging out");
        msl_logout();
    }

    /* Login for agent mode */
    if (usr.agent)
    {
        /* User input info */
        msl_init(&msl);

        if (!msl.logged) 
        {
            user_init_info(&msl);

            /* Do login */
            if(!msl_login(&msl)) 
            {
                slog(0, SLOG_ERROR, "Can not login as agent");
                usr.agent = 0;
            }

        }

        slog(0, SLOG_INFO, "SMS Agent Version: %s", msl_get_version(1));
        slog(0, SLOG_INFO, "SMS Agent logged in as: %s", msl.usr);
    }

    /* Print irc info */
    print_irc_info(&usr, &inf);

    /* Authorize user */
    sock = authorize_user(&usr, &inf);
    if (sock < 0) 
    {
        slog(0, SLOG_ERROR, "Disconnected from server: %s", inf.server);
        exit (-1);
    }

    /* Some debug line */
    slog(0, SLOG_DEBUG, "Here we go!");

    /* Main loop */
    while (1) 
    {
        /* Recieve data from socket */
        bytes = recv(sock, buf, MAXMSG-1, 0);
        buf[bytes] = '\0';

        /* Log recieved data */
        if (strlen(buf) > 0) 
        {
            sscanf(buf, "%512[^\n]\n", buf);
            slog(0, SLOG_LIVE, "%s", buf);
        }
 
        /* Handle messages */
        handle_msg(sock, buf, usr.nick, inf.channel, usr.agent);
    }

    return 0;
}