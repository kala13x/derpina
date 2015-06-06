/*
 *  derpina/conf.c
 * 
 *  Copyleft (C) 2015  Sun Dro
 *
 * Config file parser for Bot Derpina.
 * Licensed by MIT license. See LICENSE file.
 */


#include "stdinc.h"
#include "conf.h"


/* 
 * parse_config - Parse IRC Bot configuration file and store parsed data 
 * in structure variables. Argument cfg_name is config file path, usr is 
 * pointer of IRCUser structure and inf is pointer of IRCInfo structure.
 */
int parse_config(char *cfg_name, IRCUser *usr, IRCInfo *inf) 
{
    /* Used variables */
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int ret = 0;

    /* Open file pointer */
    file = fopen(cfg_name, "r");
    if(file == NULL) return ret;

    /* Line-by-line read cfg file */
    while ((read = getline(&line, &len, file)) != -1) 
    {
        /* Find values in file */
        if(strstr(line, "USERNAME") != NULL) 
        {
            /* Get username level */
            usr->name = strdup(line+8);
            ret++;
        }
        else if(strstr(line, "NICKNAME") != NULL) 
        {
            /* Get nickname level */
            usr->nick = strdup(line+8);
            ret++;
        }
        else if(strstr(line, "SERVER") != NULL) 
        {
            /* Get server level */
            inf->server = strdup(line+6);
            ret++;
        }
        else if(strstr(line, "CHANNEL") != NULL) 
        {
            /* Get channel level */
            inf->channel = strdup(line+7);
            ret++;
        }
    } 

    /* Check everything is ok */
    if (ret != 4) ret = 0;

    /* Cleanup */
    if (line) free(line);
    fclose(file);

    return ret;
}
