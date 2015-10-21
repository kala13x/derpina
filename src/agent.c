/*
 *  derpina/agent.c
 *
 *  Copyleft (C) 2015  Sun Dro
 *
 * SMS Agent control source.
 */


#include "stdinc.h"
#include "agent.h"


/*
 * parse_agent_cfg - Parse alert config file and get alert definitions.
 * Argument cfg_name is path of alert config and alerts is massive where
 * parsed parameters will be saved. On success function returns total 
 * number of alerts, otherwise is error is occured function returns 0.
 */
int parse_agent_cfg(const char *cfg_name, char *alerts[])
{
    /* Used variables */
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int count = 0;

    /* Open file pointer */
    file = fopen(cfg_name, "r");
    if(file == NULL) return 0;

    /* Line-by-line read cfg file */
    while ((read = getline(&line, &len, file)) != -1)
    {
        /* Find alert in file */
        if(strstr(line, "ALERT") != NULL)
        {
            /* Get alert definition */
            alerts[count] = strdup(line+5);
            count++;
        }
    }

    /* Cleanup */
    if (line) free(line);
    fclose(file);

    return count;
}


/*
 * get_alerts_number - Function searchs alerts and returns 
 * alert number in config file. On error function returns 0.
 */
int get_alerts_number(const char *cfg_name)
{
    /* Used variables */
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int count = 0;

    /* Open file pointer */
    file = fopen(cfg_name, "r");
    if(file == NULL) return 0;

    /* Line-by-line read cfg file */
    while ((read = getline(&line, &len, file)) != -1)
    {
        /* Find alert in file */
        if(strstr(line, "ALERT") != NULL) count++;
    }

    /* Cleanup */
    if (line) free(line);
    fclose(file);

    return count;
}


/*
 * get_agent_number - Function searches and parses and returns
 * agent number from config file. On error return value is NULL.
 */
char* get_agent_number(const char *cfg_name)
{
    /* Used variables */
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *number = NULL;

    /* Open file pointer */
    file = fopen(cfg_name, "r");
    if(file == NULL) return 0;

    /* Line-by-line read cfg file */
    while ((read = getline(&line, &len, file)) != -1)
    {
        /* Find number in file */
        if(strstr(line, "NUMBER") != NULL) 
        {
            number = strdup(line+6);
            break;
        }
    }

    /* Cleanup */
    if (line) free(line);
    fclose(file);

    return number;
}