#include "TickyGlobal.h"

void createEnvironment() {

	FILE* stream;

	if((stream = fopen("../Baza_podataka/admin.txt", "w")) != NULL) {

		fprintf(stream, "%d\n", 1);
		fprintf(stream, "%s %s %d\n", "admin", "admin", 0);
		
		fclose(stream);
	}

	if((stream = fopen("../Baza_podataka/client.txt", "w")) != NULL) {

		fprintf(stream, "%d\n", 0);
		fclose(stream);
	}

	if((stream = fopen("../Baza_podataka/user.txt", "w")) != NULL) {

		fprintf(stream, "%d\n", 0);
		fclose(stream);
	}

	if((stream = fopen("../Baza_podataka/user.txt", "w")) != NULL) {

		fprintf(stream, "%d\n", 0);
		fclose(stream);
	}
}

struct Admin* getAdmins(int* numberOfAdmins) {

	struct Admin* admins = NULL;
	FILE* stream;
	int numberOfAccounts;


	if((stream = fopen("../Baza_podataka/admin.txt", "r")) != NULL) {

		fscanf(stream, "%d\n", &numberOfAccounts);
		*numberOfAdmins = numberOfAccounts;

		admins = calloc(numberOfAccounts, sizeof(struct Admin));

		for(int i = 0; i < numberOfAccounts; i++)
			fscanf(stream, "%s %s %d\n", admins[i].accountName, admins[i].accountPass, &admins[i].numberOfLogIns);

		fclose(stream);
	}
	return admins;
}

int compareAccountName(char* string) {

	int numberOfAdmins = 0;
	struct Admin* admins = getAdmins(&numberOfAdmins);

	int numberOfClients = 0;
	struct Client* clients = getClients(&numberOfClients);

	int numberOfUsers = 0;
	struct User* users = getUsers(&numberOfUsers);

	for(int i = 0; i < numberOfAdmins; i++)
		if(strcmp(string, admins[i].accountName) == 0) {
			
			printf("Ime naloga vec postoji. ");
			free(admins);
			free(clients);
			free(users);
			return 0;
		}

	for(int i = 0; i < numberOfClients; i++)
		if(strcmp(string, clients[i].accName) == 0) {
			
			printf("Ime naloga vec postoji. ");
			free(admins);
			free(clients);
			free(users);
			return 0;
		}

	for(int i = 0; i < numberOfUsers; i++)
		if(strcmp(string, users[i].accName) == 0) { 
			
			printf("Ime naloga vec postoji. ");
			free(admins);
			free(clients);
			free(users);
			return 0;
		}

	free(admins);
	free(clients);
	free(users);
	
	return 1;
}

struct Client* getClients(int* numberOfClients) {

	struct Client* clients = NULL;
	FILE* stream;
	
	if((stream = fopen("../Baza_podataka/client.txt", "r")) != NULL) {

		fscanf(stream, "%d\n", numberOfClients);
		
		if(*numberOfClients == 0) {

			fclose(stream);
			return clients;
		}

		clients = calloc(*numberOfClients, sizeof(struct Client));

		for(int i = 0; i < *numberOfClients; i++)
			fscanf(stream, "%s %s %d %s %s\n", clients[i].accName, clients[i].accPass, &clients[i].numOfLogIns, clients[i].accState, clients[i].accCondition);
		
		fclose(stream);
	}
	return clients;
}

struct User* getUsers(int* numberOfUsers) {

	struct User* users = NULL;
	FILE* stream;

	if((stream = fopen("../Baza_podataka/user.txt", "r")) != NULL) {

		fscanf(stream, "%d\n", numberOfUsers);

		if(*numberOfUsers == 0) {

			fclose(stream);
			return users;
		}

		users = calloc(*numberOfUsers, sizeof(struct User));

		for(int i = 0; i < *numberOfUsers; i++)
			fscanf(stream, "%s %s %s %s %d %s %s %lf\n", users[i].realName, users[i].realSurName, users[i].accName, users[i].accPass, &users[i].numOfLogIns, users[i].accState, users[i].accCondition, &users[i].accBalance);

		fclose(stream);
	}
	return users;
}

struct Event* getEvents(int* numberOfEvents) {

	struct Event* events = NULL;
	FILE* stream;
	if ((stream = fopen("../Baza_podataka/event.txt", "r")) != NULL) {
		
		fscanf(stream, "%d\n", numberOfEvents);

		if (*numberOfEvents == 0) {
			fclose(stream);
			return events;
		}
		
		events = calloc(*numberOfEvents, sizeof(struct Event));
		
		for (int i = 0; i < *numberOfEvents; i++)
			fscanf(stream, "%d %s %s %s %d %s %s %lf %s\n", &events[i].eventCode, events[i].accName, events[i].eventName, events[i].eventPlace, &events[i].numTickets, events[i].date, events[i].time, &events[i].ticketPrice, events[i].isBlocked);
		fclose(stream);
	}

	for(int i = 0; i < *numberOfEvents; i++) {

		char newFile[50] = { '\0' };
		strcat(newFile, "../Baza_podataka/");
		
		char dump[50] = { '\0' };
		sprintf(dump, "%d", events[i].eventCode);

		strcat(newFile, dump);
		strcat(newFile, ".txt");

		int oldTemp = 0;

		if((stream = fopen(newFile, "r")) != NULL) {

			fscanf(stream, "%d\n", &oldTemp);
			int* temp = NULL;
			temp = calloc(events[i].numTickets, sizeof(int));
			for(int j = 0; j < events[i].numTickets; j++) {

				int dump = 0;
				fscanf(stream, "%d\n", &dump);
				temp[j] = dump;
			}
			events[i].soldTickets = temp;
			temp = NULL;
			fclose(stream);
		}
	}

	return events;
}

struct Info readNumberOfLogIns() {

	FILE* stream;
	struct Info info;
	if((stream = fopen("../Baza_podataka/info.txt", "r")) != NULL) {

		fscanf(stream, "%d\n", &info.allowedNumber);
		fscanf(stream, "%d\n", &info.firstTime);
		fclose(stream);
	}
	return info;
}

int checkFirstCharacter(char* string) {

	// Pise u dokumentaciji sta funkcija radi
	if(string[0] == '1' && string[1] == '\0') return 1;
	else return 0;
}

int checkSecondCharacter(char* string) {

	// Isto samo za '2'
	if(string[0] == '2' && string[1] == '\0') return 1;
	else return 0;
}

int checkThirdCharacter(char* string) {

	// Isto samo za '3'	
	if(string[0] == '3' && string[1] == '\0') return 1;
	else return 0;
}


int checkFourthCharacter(char* string) {

	// Isto samo za '4'	
	if(string[0] == '4' && string[1] == '\0') return 1;
	else return 0;
}


int checkFifthCharacter(char* string) {

	// Isto samo za '5'	
	if(string[0] == '5' && string[1] == '\0') return 1;
	else return 0;
}

int checkExitCharacter(char* string) {

	// Pise u dokumentaciji
	if(string[0] == '0' && string[1] == '\0') return 1;
	else return 0;
}

void modifyCharacter(char* string, int size, FILE* stream) {

	// Pise u dokumentaciji sta funkcija radi
	fgets(string, size, stream);
	string[strcspn(string, "\n")] = '\0';
}



int changeAccountCredentials(char* string, int size, FILE* stream) {

	printf("Potrebna promjena sifre! ");
	do {

		modifyCharacter(string, size, stream);
	}while(checkCredentials(string) == 0);

	return 1;
}

int checkCredentials(char* string) {

	if(strlen(string) > 20 || strlen(string) < 5) {

		printf("Akreditacija treba biti duzine [5-20] karaktera sa slovima ili brojevima: ");
		return 0;
	}
	for(int i = 0; i < strlen(string); i++)
		if(!isalpha(string[i]) && !isdigit(string[i])) {
	
			printf("Akreditacija treba biti duzine [5-20] karaktera sa slovima ili brojevima: ");
			return 0;
		}
	return 1;
}

void printClients() {

	int numberOfClients = 0;
	struct Client* clients = getClients(&numberOfClients);

	if(numberOfClients != 0) printf("\n");
	for(int i = 0; i < numberOfClients; i++)
		printf("%s\n", clients[i].accName);

	free(clients);
}

void printUsers() {

	int numberOfUsers = 0;
	struct User* users = getUsers(&numberOfUsers);

	if(numberOfUsers != 0) printf("\n");
	for(int i = 0; i < numberOfUsers; i++)
		printf("%s\n", users[i].accName);

	free(users);
}

void writeUsers(struct User* users, int numberOfUsers) {

	FILE* stream;

	if((stream = fopen("../Baza_podataka/user.txt", "w")) != NULL) {

		fprintf(stream, "%d\n", numberOfUsers);
		for(int i = 0; i < numberOfUsers; i++)
			fprintf(stream, "%s %s %s %s %d %s %s %lf\n", users[i].realName, users[i].realSurName, users[i].accName, users[i].accPass, users[i].numOfLogIns, users[i].accState, users[i].accCondition, users[i].accBalance);

		fclose(stream);
	}
}

void writeClients(struct Client* clients, int numberOfClients) {

	FILE* stream;

	if((stream = fopen("../Baza_podataka/client.txt", "w")) != NULL) {

		fprintf(stream, "%d\n", numberOfClients);
		for(int i = 0; i < numberOfClients; i++)
			fprintf(stream, "%s %s %d %s %s\n", clients[i].accName, clients[i].accPass, clients[i].numOfLogIns, clients[i].accState, clients[i].accCondition);

		fclose(stream);
	}
}

int validationPass(char* string) {

	if(strlen(string) > 30 || strlen(string) < 5) {

		printf("Ime nije zadovoljavajuce duzine: ");
		return 1;
	}
	for(int i = 0; i < strlen(string); i++)
		if(!isalpha(string[i])) {

			printf("Karakter %c nije dozvoljen. ", string[i]);
			return 1;
		}
	return 0;
}

void freeEvent(struct Event* events, int numberOfEvents) {

	if(events != NULL) {

		if(numberOfEvents != 0) {

			for(int i = 0; i < numberOfEvents; i++) {

				if(events[i].soldTickets != NULL)
					free(events[i].soldTickets);
			}
			free(events);
		}
	}
}

int checkIfDateValid(char* date) {
	
	int dd, mm, yyyy;
	if(sscanf(date, "%d.%d.%d", &dd, &mm, &yyyy) != 3) { 
		return 0;
	}
	if(dd < 1 || dd > 31 || mm < 1 || mm > 12 || yyyy <= 2022) {
		return 0;
	}
	if (mm == 2) {
		if (dd > 29) return 0;
		if (dd == 29 && (yyyy % 4 != 0 || (yyyy % 100 == 0 && yyyy % 400 != 0))) return 0;
	}
	else if (mm == 4 || mm == 6 || mm == 9 || mm == 11) {
		if (dd > 30) return 0;
	}
	return 1;
}

int checkIfTimeValid(char* time) {

	int hh, mm;
	if(sscanf(time, "%d:%d", &hh, &mm) != 2) {
		return 0;
	}
	if(hh < 0 || hh > 23 || mm < 0 || mm > 59) {
		return 0;
	}
	return 1;
}

struct Date convertToDate(char* date) {

	int dd, mm, yyyy;
	sscanf(date, "%d.%d.%d", &dd, &mm, &yyyy);
	struct Date temp;
	temp.dd = dd;
	temp.mm = mm;
	temp.yyyy = yyyy;
	return temp;
}

void allTickets(struct Date start, struct Date end, struct Data* array) {

	int numberOfEvents = 0;
	struct Event* events = getEvents(&numberOfEvents);


	for(int i = 0; i < numberOfEvents; i++) {

		struct Date current = convertToDate(events[i].date);
		if(current.yyyy > start.yyyy && current.yyyy < end.yyyy) {

			array->numberOfData++;
		}	
		else if (current.yyyy == start.yyyy && current.yyyy == end.yyyy) {

			if (current.mm > start.mm && current.mm < end.mm) {

				array->numberOfData++;
			}
			else if (current.mm == start.mm && current.mm == end.mm) {

				if (current.dd >= start.dd && current.dd <= end.dd) {

					array->numberOfData++;
				}
			}
		}

	}

	array->soldTickets = calloc(array->numberOfData, sizeof(int));
	array->eventName = calloc(array->numberOfData, sizeof(char*));

	int j = 0;

	for(int i = 0; i < numberOfEvents; i++) {

		struct Date current = convertToDate(events[i].date);
		if(current.yyyy > start.yyyy && current.yyyy < end.yyyy) {

			array->soldTickets[j] = events[i].numTickets;
			array->eventName[j] = calloc(strlen(events[i].eventName) + 1, 1);
			strcpy(array->eventName[j++], events[i].eventName);
		}	
		else if (current.yyyy == start.yyyy && current.yyyy == end.yyyy) {

			if (current.mm > start.mm && current.mm < end.mm) {
				array->soldTickets[j] = events[i].numTickets;
				array->eventName[j] = calloc(strlen(events[i].eventName) + 1, 1);
				strcpy(array->eventName[j++], events[i].eventName);
			}
		}
		else if (current.mm == start.mm && current.mm == end.mm) {

			if (current.dd >= start.dd && current.dd <= end.dd) {
				array->soldTickets[j] = events[i].numTickets;
				array->eventName[j] = calloc(strlen(events[i].eventName) + 1, 1);
				strcpy(array->eventName[j++], events[i].eventName);
			}
		}
	}
}

	

