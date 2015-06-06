/*
 *  derpina/derp.c
 *
 *  Copyleft (C) 2015  Sun Dro
 *
 * Concept of Internet Realy Chat bot.
 * Licensed by MIT license. See LICENSE file.
 */

#include "stdinc.h"
#include "../slog/slog.h"
#include "conf.h"
#include "info.h"
#include "irc.h"

#define MAXMSG 4098
#define CONFIG_FILE "conf.cfg"


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
 * print_irc_info - Print basic irc information such as username, 
 * nickname, server and channel. Argument usr is pointer of IRCUser 
 * structure and inf is pointer of IRCInfo structure.
 */
void print_irc_info(IRCUser *usr, IRCInfo *inf) 
{
    slog(0, SLOG_INFO, "Username: %s", usr->name);
    slog(0, SLOG_INFO, "Nickname: %s", usr->nick);
    slog(0, SLOG_INFO, "Server: %s", inf->server);
    slog(0, SLOG_INFO, "Channel: %s", inf->channel);
}


/* 
 * parse_arguments - Parse user inputed arguments and store 
 * data in structure variables. Argument usr is pointer of 
 * IRCUser structure and inf is pointer of IRCInfo structure.
 */
static int parse_arguments(int argc, char *argv[], IRCUser *usr, IRCInfo *inf)
{
    int c;
    while ( (c = getopt(argc, argv, "u:n:s:c:h1")) != -1) {
        switch (c) {
        case 'u':
            usr->name = strdup(optarg);
            break;
        case 'n':
            usr->nick = strdup(optarg);
            break;
        case 's':
            inf->server = strdup(optarg);
            break;
        case 'c':
            inf->channel = strdup(optarg);
            break;
        case 'h':
        default:
            usage();
            return -1;
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
    /* Irc info */
    IRCUser usr;
    IRCInfo inf;

    /* Greet users */
    greet("IRC Bot Derpina");
    slog(0, SLOG_INFO, "Logger Version: %s", slog_version(1));

    /* Catch ilegal signal */
    signal(SIGINT, clean_prog);
    signal(SIGSEGV, clean_prog);
    signal(SIGILL , clean_prog);

    /* Initialize logger */
    init_slog("derpina", "conf.cfg", 2);

    /* Initialize irc config */
    if (!parse_config(CONFIG_FILE, &usr, &inf)) 
    {
        slog(0, SLOG_ERROR, "Can not parse config file: %s", CONFIG_FILE);
        exit(-1);
    }
    else slog(0, SLOG_INFO, "Config Loaded from: %s", CONFIG_FILE);

    /* Print irc info */
    print_irc_info(&usr, &inf);

    /* Parse cli arguments */
    parse_arguments(argc, argv, &usr, &inf);

    /* Some debug line */
    slog(0, SLOG_DEBUG, "We run!");

    return 0;
}