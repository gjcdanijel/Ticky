#ifndef _TickyClient_h_
#define _TickyClient_h_

#include "TickyGlobal.h"

void clientOption(struct Info*, FILE*);

int checkClientLogInInfo(struct Client, int*);

void createEvent(FILE*, char*);

void writeEvent(struct Event*, int);

int generateEventCode();

void printSoldTickets(char*);

void printByPeriod(char*, FILE*);

int clientLogIn(struct Info*, FILE*, char*);

#endif
