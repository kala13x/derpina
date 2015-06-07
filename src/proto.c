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
#include "proto.h"

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
 * watch_whole_chat - Handle messages in public from recieved buffer 
 * and make correct response. buf is recieved buffer Return value is 
 * char pointer. P.S. Add your questions and answers protocol here.
 */
char* watch_whole_chat(char *buf, int agent) 
{
    static char output[MAXMSG];
    bzero(output, sizeof(output));

    /* Check if they are talking about sundro */
    if((search_str(buf, ALERT1) > 0)    || 
        (search_str(buf, ALERT2) > 0)   || 
        (search_str(buf, ALERT3) > 0)   ||
        (search_str(buf, ALERT4) > 0)   || 
        (search_str(buf, ALERT5) > 0)   ||
        (search_str(buf, ALERT6) > 0)   ||
        (search_str(buf, ALERT7) > 0))
    {
        /* Send sms to owner */
        if (agent) send_sms(buf);

        sprintf(output, "%s", "What did you say? I got you!");
        return output;
    }

    return NULL;
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

    /* Blah questions */
    if (search_str(buf, "who are you") > 0) 
    {
        output = strdup("Im Derpina, Bitch!");
        return output;
    }

    /* Who questions */
    if (search_str(buf, "who is") > 0)
    {
        output = strdup("I dont fucking know, gtfo bitch!");
        return output;
    }

    /* Sey hello */ 
    if ((search_str(buf, "hey") > 0) || (search_str(buf, "hello") > 0)) 
    {
        output = strdup("Heey!");
        return output;
    }

    /* More blaah questions */
    if ((search_str(buf, "whats up") > 0)    ||
        (search_str(buf, "what's up") > 0)   ||
        (search_str(buf, "how are you") > 0) ||
        (search_str(buf, "whats going") > 0) )
    {
        output = strdup("Heh, Just chilling. Yay!");
        return output;
    }

    return NULL;
}