/*
 *  derpina/proto.h
 *
 *  Copyleft (C) 2015  Sun Dro
 *
 * Message handling for Internet Realy Chat bot.
 * Licensed by MIT license. See LICENSE file.
 */


/* For include header in CPP code */
#ifdef __cplusplus
extern "C" {
#endif


/*
 * search_str - Search string in another string. If string 
 * is found, function returns 1, else 0 or -1. Argument str 
 * is string to search and srch is string we want search for.
 */
int search_str(char *str, char *srch);


/* 
 * get_answer - Handle messages from recieved buffer and make correct 
 * response. buf is recieved buffer Return value is char pointer.
 * P.S. Add your message gandling and question answer protocol here.
 */
char* get_answer(char *buf);


/* For include header in CPP code */
#ifdef __cplusplus
}
#endif