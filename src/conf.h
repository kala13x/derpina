/*
 *  derpina/conf.h
 * 
 *  Copyleft (C) 2015  Sun Dro
 *
 * Header of config file parser for Bot Derpina.
 * Licensed by MIT license. See LICENSE file.
 */


/* For include header in CPP code */
#ifdef __cplusplus
extern "C" {
#endif


/* User infrmation */
typedef struct {
    char name[16];
    char nick[16];
} IRCUser;


/* Server Information */
typedef struct {
    char channel[16];
    char server[16];
    char port[6];
} IRCInfo;


/* 
 * parse_config - Parse IRC Bot configuration file and store parsed data 
 * in structure variables. Argument cfg_name is config file path, usr is 
 * pointer of IRCUser structure and inf is pointer of IRCInfo structure.
 */
int parse_config(char *cfg_name, IRCUser *usr, IRCInfo *inf);


/* For include header in CPP code */
#ifdef __cplusplus
}
#endif