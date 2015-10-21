/*
 *  derpina/send.c
 *
 *  Copyleft (C) 2015  Sun Dro
 *
 * Send report message via magtisun library.
 * Licensed by MIT license. See LICENSE file.
 */


#include "../sms/magtisun.h"
#include "../slog/slog.h"
#include "stdinc.h"
#include "agent.h"
#include "send.h"


/* 
 * init_msl_sms - Initialize magtisun sms to send sms to owner.
 * Function initializes magtifun user name, password, number
 * and message text. Argument buf is text for message to send. 
 */
void init_msl_sms(MagtiSunLib * msl, char *buf) 
{

    sprintf(msl->num, "%s", get_agent_number("agent.cfg"));
    strcpy(msl->txt, buf);
}


/* 
 * sender_thread - Send message via magtifun to destination.
 * Thread sends chat buffer to defined number. arg is buffer.
 */
void* sender_thread(void *arg) 
{
    char *buf = (char*)arg;
    MagtiSunLib msl;

    /* Initialize magtisun */
    msl_init(&msl);

    /* Check if we are logged in magtisun */
    if (msl.logged) 
    {
        init_msl_sms(&msl, buf);

        /* Send message */
        if (msl_send(&msl) >= 0)
            slog(0, SLOG_LIVE, "Message sent!");
    }

    return 0;
}


/* 
 * send_sms - Create sender thread and send message to owner.
 * Argument buf is a message which we want to send to user.
 */
void send_sms(char *buf) 
{
    int status;
    pthread_t sender;

    /* Create sender thread */
    status = pthread_create(&sender, NULL, &sender_thread, buf);
    if (status) slog(0, SLOG_ERROR, "Can not create sender thread: %s", strerror(status));
    else slog(0, SLOG_LIVE, "Sender thread created successfully");
}