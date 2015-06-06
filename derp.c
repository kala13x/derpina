/*
 *  derpina/derp.c
 *
 *  Copyleft (C) 2015  Sun Dro
 *
 * Concept of Internet Realy Chat bot.
 * Licensed by MIT license. See LICENSE file.
 */

#include "stdinc.h"
#include "slog/slog.h"
#include "info.h"
#include "irc.h"

int main ()
{
	/* Greet users */
    greet("IRC Bot Derpina");
    slog(0, SLOG_INFO, "Logger Version: %s", slog_version(1));

    /* Initialize logger */
    init_slog("derpina", "conf.cfg", 2);

    slog(0, SLOG_DEBUG, "We run!");

    return 0;
}