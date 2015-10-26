/*
 *  src/keygen.c
 * 
 *  Copyleft (C) 2015  Sun Dro
 *
 * Key generator for bot commands
 * Licensed by MIT license. See LICENSE file.
 */


#include "stdinc.h"
#include "../slog/slog.h"

static int force_date = 0;

/* Date variables */
typedef struct {
    char datestr[64];
    int year; 
    int mon; 
    int day;
    int hour;
    int min;
    int sec;
} SystemDate;


/*
 * set_system_date - Set date string 
 * at SystemDate structure pointer.
 */
void set_system_date (SystemDate *sdate, char *time) 
{
    sprintf(sdate->datestr, "%s", time);
}


/*
 * SystemDate - Intialize date with system date.
 * Argument is pointer of SystemDate structure.
 */
void get_system_date(SystemDate *sdate)
{
    time_t rawtime;
    struct tm *timeinfo;
    rawtime = time(NULL);
    timeinfo = localtime(&rawtime);

    /* Get System Date */
    sdate->year = timeinfo->tm_year+1900;
    sdate->mon = timeinfo->tm_mon+1;
    sdate->day = timeinfo->tm_mday;
    sdate->hour = timeinfo->tm_hour;
    sdate->min = timeinfo->tm_min;
    sdate->sec = timeinfo->tm_sec;
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
 * get_derp_key - Get valid enctypted key for command.
 */
char *get_derp_key(char *cmd, SystemDate *date) 
{
    /* Get date */
    get_system_date(date);

    /* Get ready */
    char pre_krypt[80];

    if (!force_date) 
    {
        sprintf(pre_krypt, "derp-%s:key-%d.%d.%d.%d.%d", 
            cmd, date->year, date->mon, date->day, date->hour, date->min);
    }
    else 
    {
        sprintf(pre_krypt, "derp-%s:key-%s", 
            cmd, date->datestr);
    }

    /* Crypt */
    char *valid_key = strcrypt(pre_krypt);
    return valid_key;
}


int main(int argc, char *argv[]) 
{
    SystemDate date;
    
    /* Chek args */
    if (argc < 2) 
    {
        printf("Input command to get the key\n");
        printf("Example: %s logout\n", argv[0]);
        return 0;
    }

    /* Force set system date */
    if (argv[2] != NULL) 
    {
        force_date = 1;
        set_system_date(&date, argv[2]);
    }

    /* Get and print key */
    char *key = get_derp_key(argv[1], &date);
    printf("Key for command (%s) is: %s\n", argv[1], key);

    return 0;
}