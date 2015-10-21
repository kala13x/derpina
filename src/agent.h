/*
 *  derpina/agent.h
 *
 *  Copyleft (C) 2015  Sun Dro
 *
 * SMS Agent control source.
 */

#ifndef __DERP_ALERTS__
#define __DERP_ALERTS__


/* For include header in CPP code */
#ifdef __cplusplus
extern "C" {
#endif


/*
 * parse_agent_cfg - Parse alert config file and get alert definitions.
 * Argument cfg_name is path of alert config and alerts is massive where
 * parsed parameters will be saved. On success function returns total 
 * number of alerts, otherwise is error is occured function returns 0.
 */
int parse_agent_cfg(const char *cfg_name, char *alerts[]);


/*
 * get_alerts_number - Function searchs alerts and returns 
 * alert number in config file. On error function returns 0.
 */
int get_alerts_number(const char *cfg_name);


/*
 * get_agent_number - Function searches and parses and returns
 * agent number from config file. On error return value is NULL.
 */
char* get_agent_number(const char *cfg_name);


/* For include header in CPP code */
#ifdef __cplusplus
}
#endif


#endif /* __DERP_ALERTS__ */