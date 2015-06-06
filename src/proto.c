

/* 
 * send_message - Send message in chat room. Function sends buffer with socket 
 * in channel room. Argument sock is socket and  buf is buffer we want to send.
 */
void send_message(int sock, char *chan, char *buf) 
{
    char msg[256];
}


/* 
 * handle_msg - Handle messages from recieved buffer and make 
 * correct response. Argument sock is socket descriptor, buf 
 * is recieved buffer and usr is bot nickname on irc server.
 */
void handle_msg(int sock, char *buf, char *usr, char *chan) 
{
    /* Check if they are talking to us */
    if(search_str(buf, usr) > 0) 
    {
        /* Blah questions */
        if (search_str(buf, "who are you") > 0)
            send_message(sock, chan, "Im Derpina, Bitch!");

        if (search_str(buf, "who is") > 0)
            send_message(sock, chan, "I dont fucking know, gtfo bitch!");

        /* Sey hello */ 
        if ((search_str(buf, "hey") > 0) || (search_str(buf, "hello") > 0))
            send_message(sock, chan, "Heey!");

        /* More blaah questions */
        if ((search_str(buf, "whats up") > 0)    ||
            (search_str(buf, "what's up") > 0)   ||
            (search_str(buf, "how are you") > 0) ||
            (search_str(buf, "whats going") > 0) )
            {
                send_message(sock, chan, "Heh, Just chilling. Yay!");
            }
    }

    /* Check if ping request and send pong */
    if (search_str(buf, "PING") > 0) send_keepalive(sock, buf);
}