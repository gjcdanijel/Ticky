#ifndef _TickyAdmin_h_
#define _TickyAdmin_h_

#include "TickyGlobal.h"

void adminOption(struct Info*, FILE*);

void createAdmin(FILE*);

void createClient(FILE*);

void activateAccount(FILE*);

void suspendAccount(FILE*);

void deleteAccount(FILE*);

void blockEvent(FILE*);

void printEvents();

void cancelPass(FILE*);

int checkAdminLogInInfo(struct Admin, int*);

void writeAdmins(struct Admin*, int);

void writeInfo(struct Info*);

int adminLogIn(struct Info*, FILE*);

#endif
