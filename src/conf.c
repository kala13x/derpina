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
    char value[16];
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
            /* Get username */
            if (sscanf(line, "%s %s", value, usr->name) == 2) 
                ret++;
        }
        else if(strstr(line, "NICKNAME") != NULL) 
        {
            /* Get nickname */
            if (sscanf(line, "%s %s", value, usr->nick) == 2) 
                ret++;
        }
        else if(strstr(line, "SERVER") != NULL) 
        {
            /* Get server */
            if (sscanf(line, "%s %s", value, inf->server) == 2) 
                ret++;
        }
        else if(strstr(line, "CHANNEL") != NULL) 
        {
            /* Get channel */
            if (sscanf(line, "%s %s", value, inf->channel) == 2) 
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
