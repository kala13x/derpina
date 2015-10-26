/*
 *  derpina/proto.c
 *
 *  Copyleft (C) 2015  Sun Dro
 *
 * Message handling for Internet Realy Chat bot.
 * Add your message gandling and question answer protocol here.
 * Licensed by MIT license. See LICENSE file.
 */


#include "stdinc.h"
#include "send.h"
#include "agent.h"
#include "proto.h"
#include "../slog/slog.h"
#include "../sms/magtisun.h"

#define MAXMSG 4098

static int do_response = 1;

/*
 * strsrc - Search string in another string. If string 
 * is found, function returns 1, else 0 or -1. Argument str 
 * is string to search and srch is string we want search for.
 */
int strsrc(char *str, char *srch)
{
    int lenstr = strlen(str);
    int lensrch = strlen(srch);
    int found = -1;
    int i, x;
 
    /* Search through each char in str */
    for (i = 0; i < lenstr; i++)
    {
        /* If first search item is valid then search str */
        if (srch[0] == str[i])
        {
            found = 1;

            /* Search the char array for search field */
            for (x = 1; x < lensrch; x++) 
                if (str[i+x] != srch[x]) found = 0;
 
            /* Return if found */
            if (found) return found;
        }
    }
 
    return found;
}


/*
 * strcrypt - Crypt string with gnu c crypt library.
 * Argument str is string which we want to crypt and
 * return value is char pointer of the crypted string.
 * Crypted string with strcrypt, can not be decrypted.
 */
char* strcrypt(char* str)
{
    /* Used Variables */
    unsigned long seed[2];
    char *crypted;
    int i;

    /* Salt */
    char salt[] = "$1$..............";

    /* Seedchars */
    const char *const seedchars = 
    "./0123456789ABCDEFGHIJKLMNOPQRST"                                
    "UVWXYZabcdefghijklmnopqrstuvwxyz";

    /* Generate a (not very) random seed */
    seed[0] = 1920 * 69 * 13;
    seed[1] = 3321 ^ (seed[0] >> 14 & 0x30000);

    /* Turn it into printable characters from ‘seedchars’ */
    for (i = 0; i < 8; i++)
        salt[3+i] = seedchars[(seed[i/5] >> (i%5)*6) & 0x3f];

    /* Encrypt it */
    crypted = crypt(str, salt);

    /* Remove salt */
    sscanf(crypted, "$1$.SY4.tn.$%[^:]:", crypted);

    return crypted;
}


/* 
 * watch_whole_chat - Handle messages in public from recieved buffer 
 * and make correct response. buf is recieved buffer Return value is 
 * char pointer. P.S. Add your questions and answers protocol here.
 */
char* watch_whole_chat(char *buf, int agent) 
{
    static char output[256];
    bzero(output, sizeof(output));
        int i;

    /* Get alert number */
    int alerts_num = get_alerts_number("agent.cfg");
    if (!alerts_num) return NULL;

    /* Get alerts */
    char *alerts[alerts_num];
    parse_agent_cfg("agent.cfg", alerts);
    if (!alerts_num) return NULL;

    /* Check if they are talking about sundro */
    for(i = 0; i < alerts_num; i++) 
    {
        if(strsrc(buf, alerts[i]) > 0) 
        {
            /* Send sms to owner */
            if (agent) send_sms(buf);
        }
    }

    return NULL;
}

/*
 * check_valid_command - Function searchs and handles 
 * commands from private chat and checks key, if 
 * key is valid, function makes the response.
 */
char *check_valid_command(char *buf) 
{
    static char cmd[128];

    /* Get date */
    SlogDate date;
    get_slog_date(&date);

    if (strsrc(buf, "command") > 0) 
    {
        slog(0, SLOG_LIVE, "Received command. Checking key...");

        /* Get message */
        if (sscanf(buf, "%128[^,],", cmd))
        {
            /* Get command */
            int len = strlen(cmd);
            char *msg = strdup(buf+len+2);
            slog(2, SLOG_DEBUG, "Parsed message: %s", msg);

            /* Go through */
            if (sscanf(msg, "%32[^*]*", cmd)) 
            {
                char pre_krypt[80];
                sprintf(pre_krypt, "derp-%s:key-%d.%d.%d.%d.%d", 
                    cmd, date.year, date.mon, date.day, date.hour, date.min);
                slog(2, SLOG_DEBUG, "Parsed command: %s", cmd);

                /* Crypt */
                char *valid_key = strcrypt(pre_krypt);
                slog(2, SLOG_DEBUG, "Current valid key: %s", valid_key);

                /* Check valid key */
                if (strsrc(msg, valid_key) > 0)
                {
                    slog(0, SLOG_LIVE, "Key is valid.");
                    slog(0, SLOG_DEBUG, "Command is: %s", cmd);
                    return cmd;
                }
                else 
                {
                    slog(0, SLOG_WARN, "Incorrect key..");
                    strcpy(cmd, "incorrect-key");
                    return cmd;
                }
            }
        }
    }

    return NULL;
}


/*
 * handle_commands - Function searchs and handles 
 * commands from private chat and checks key, if 
 * key is valid, function makes the response.
 */
char *handle_commands(char *buf) 
{
    char *cmd = check_valid_command(buf);
    if (cmd == NULL) return NULL;

    if (strsrc(cmd, "incorrect-key") > 0)
    {
        cmd = strdup("Incorrect key bro.. :)");
        return cmd;
    }

    if ((strsrc(cmd, "die") > 0) ||
        (strsrc(cmd, "logout") > 0))
    {
        slog(0, SLOG_LIVE, "Received valid exit command, exiting..");
        kill(getpid(), SIGINT);
    }
    else if (strsrc(cmd, "disable-agent") > 0)
    {
        msl_logout();
    } 
    else if (strsrc(cmd, "silent-mode-on") > 0)
    {
        do_response = 0;
    }
    else if (strsrc(cmd, "silent-mode-off") > 0)
    {
        do_response = 1;
    }
    else 
    {
        cmd = strdup("I dont know this command bro..");
        return cmd;
    }

    cmd = strdup("ok..");
    return cmd;
}


/* 
 * watch_private_chat - Handle messages from recieved buffer and make 
 * correct response. buf is recieved buffer Return value is char pointer.
 * P.S. Add your message handling and question answer protocol here.
 */
char* watch_private_chat(char *buf, int agent)
{
    /* Check whole chat */
    char *output = watch_whole_chat(buf, agent);
    if (output != NULL) return output;

    /* Handle commands */
    output = handle_commands(buf);
    if (output != NULL) return output;

    if (do_response)
    {
        /* Blah questions */
        if (strsrc(buf, "who are you") > 0) 
        {
            output = strdup("Im Derpina, Bitch!");
            return output;
        }
        else if (strsrc(buf, "who is") > 0)
        {
            output = strdup("I dont know...");
            return output;
        }
        else if ((strsrc(buf, "you") > 0) && 
            (strsrc(buf, "alive") > 0))
        {
            output = strdup("I dont know...");
            return output;
        }
        else if ((strsrc(buf, "hey") > 0) 
            || (strsrc(buf, "hello") > 0)) 
        {
            output = strdup("Heey!");
            return output;
        }
        else if ((strsrc(buf, "PING") > 0) 
            || (strsrc(buf, "ping") > 0)) 
        {
            output = strdup("PONG :)");
            return output;
        }
        else if ((strsrc(buf, "whats up") > 0) ||
            (strsrc(buf, "what's up") > 0) ||
            (strsrc(buf, "how are you") > 0) ||
            (strsrc(buf, "whats going") > 0) )
        {
            output = strdup("Heh, Just chilling.");
            return output;
        }
        else 
        {
            if ((strsrc(buf, "join") <= 0) ||
                (strsrc(buf, "JOIN") <= 0))
            {
                output = strdup("Hm...");
                return output;
            }
        }
    }

    return NULL;
}