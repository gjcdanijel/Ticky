#ifndef _TickyUser_h_
#define _TickyUser_h_

#include "TickyGlobal.h"
#include "TickyClient.h"

void userOption(struct Info, FILE*); // DONE

int userLogIn(struct Info, FILE*, char*);

int checkUserLogInInfo(struct User, int*);

int generateTicketCode();

void buyTicket(char* ,FILE*);

void processTickets(char*, FILE*, int);

#endif 
