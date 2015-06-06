/*
 *  derpina/irc.c
 *
 *  Copyleft (C) 2015  Sun Dro
 *
 * Operations for Internet Realy Chat bot.
 * Licensed by MIT license. See LICENSE file.
 */


#include "stdinc.h"
#include "irc.h"


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