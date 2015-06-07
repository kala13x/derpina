## Derpina 1.0.43 - Internet Realy Chat (IRC) Bot
Derpina is Internet Realy Chat (IRC) Bot which is responsible to answer your basic questions, answer PING requests with PONG to stay online and log everything from chat. You can add your question/answers in proto.c file to handle more messages from chat and make bot more communicative. Software is written for educational purposes and is distributed in the hope that it will be useful for anyone interested in this field.

###Usage
```
Usage: derp [-u <user>] [-n <nick>] [-s <server>] [-p <port>] [-c <room>] [-h]
options are:
 -u         # Username for IRC
 -n         # Nickname for IRC
 -s         # Server address for IRC
 -p         # Port for IRC
 -c         # Channel room for IRC
```
Without those arguments, bot can parse all options from config file. Here is conf.cfg example:
```
LOGLEVEL 3
LOGTOFILE 0

USERNAME Derpity
NICKNAME Derp

SERVER irc.server.com
PORT 6667
CHANNEL channel
```
###Compilation
Compilation is possible with build.sh script and Makefiles
```
git clone git@github.com:kala13x/derpina.git
cd derpina
chmod +x build.sh
./build.sh
```
After succsessful compilation, executable binaries will be moved in bin directory which is located in your derpina directory.

###Get more info from blog
- Offencive-Security Georgia [http://off-sec.com/](http://off-sec.com)
