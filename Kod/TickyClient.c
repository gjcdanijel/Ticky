#include "TickyClient.h"
#include "TickyGlobal.h"

void clientOption(struct Info* info, FILE* stream) {

	char loggedClient[30] = { '\0' };
	if (clientLogIn(info, stream, loggedClient)) {

		int checkShouldIRun = 1;
		do {

			printf("Obrada dogadjaja [1], Izvjestaji [2], Kraj [0]: ");

			char character[15] = { '\0' };
			modifyCharacter(character, 15, stream);
			if (checkFirstCharacter(character)) {

				int checkShouldIRun1 = 1;
				do {

					char character1[15] = { '\0' };
					printf("Kreiranje dogadjaja [1], Kraj [0]: ");
					modifyCharacter(character1, 15, stream);

					if(checkFirstCharacter(character1)) {

						createEvent(stream, loggedClient);
					}
					else if(checkExitCharacter(character1)) checkShouldIRun1 = 0;					
					else printf("Greska. Unesite ponovo!\n");

				} while(checkShouldIRun1);
			}
			else if(checkSecondCharacter(character)) {

				int checkShouldIRun1 = 1;
				do
				{
					char character1[15] = { '\0' };
					printf("Izvjestaj prodanih ulaznica [1], Izvjestaj prodanih ulaznica po periodu [2], Kraj[0]: ");
					modifyCharacter(character1, 15, stream);

					if(checkFirstCharacter(character1)) {

						printSoldTickets(loggedClient);
					}
					else if(checkSecondCharacter(character1)) {

						printByPeriod(loggedClient, stream);
					}
					else if (checkExitCharacter(character1)) checkShouldIRun1 = 0;
					else printf("Greska. Unesite ponovo!\n");

				} while(checkShouldIRun1);
			}
			else if (checkExitCharacter(character)) checkShouldIRun = 0;
			else printf("Greska. Unesite ponovo!\n");
		} while (checkShouldIRun);
	}
}

int clientLogIn(struct Info* info, FILE* stream, char* string) {

	struct Client client;
	int shouldIRun = 1;

	do {

		printf("Unesite ime naloga: ");
		modifyCharacter(client.accName, 30, stream);

		printf("Unesite lozinku naloga: ");
		modifyCharacter(client.accPass, 30, stream);

		int clientNumber = 0;
		if (checkClientLogInInfo(client, &clientNumber)) {

			int numberOfClient = 0;
			struct Client* clients = getClients(&numberOfClient);
			if (++clients[clientNumber].numOfLogIns == info->allowedNumber) {

				char newClientPass[30] = { '\0' };
				changeAccountCredentials(newClientPass, 30, stream);

				strcpy(clients[clientNumber].accPass, newClientPass);
				clients[clientNumber].numOfLogIns = 0;
			}
			writeClients(clients, numberOfClient);
			shouldIRun = 0;
			free(clients);
		}

	} while(shouldIRun);

	int numberOfClients = 0;
	struct Client* clients = getClients(&numberOfClients);

	for(int i = 0; i < numberOfClients; i++) {

		if(clients[i].accName, client.accName)
			if(strcmp(clients[i].accState, "Suspended") == 0 || strcmp(clients[i].accCondition, "Deleted") == 0) {

				printf("Nedozvoljen pristup.\n");
				free(clients);
				return 0;
			}
	}

	free(clients);
	strcpy(string, client.accName);

	return 1;
}

int checkClientLogInInfo(struct Client client, int* clientNumber) {

	int numberOfClients = 0;
	struct Client* clients = getClients(&numberOfClients);
	for(int i = 0; i < numberOfClients; i++) {

		if(strcmp(clients[i].accName, client.accName) == 0 && strcmp(clients[i].accPass, client.accPass) == 0) {

			*clientNumber = i;
			free(clients);
			return 1;
		}
	}
	free(clients);
	printf("Pogresno uneseni podaci!\n");
	return 0;
}	

void writeEvent(struct Event* events, int numberOfEvents) {

	FILE* stream;
	int j = numberOfEvents;

	if ((stream = fopen("../Baza_podataka/event.txt", "w")) != NULL) {

		fprintf(stream, "%d\n", j);
		for(int i = 0; i < j; i++) {

			FILE* stream_1;

			char newFile[50] = { '\0' };
			strcat(newFile, "../Baza_podataka/");

			char dump[50] = { '\0' };
			sprintf(dump, "%d", events[i].eventCode);

			strcat(newFile, dump);
			strcat(newFile, ".txt");

			int checker = 0;
			int checker1;
			int counter = 0;	// broji koliko ulaznica je skinuto

			int leftTickets = 0;
			if((stream_1 = fopen(newFile, "r")) != NULL) {

				int dump = 0;
				fscanf(stream_1, "%d\n", &dump);
				if(dump != 0) {

					for(int s = 0; s < events[i].numTickets; s++)
						if(events[i].soldTickets[s] == -1) leftTickets++;
				}
				fclose(stream_1);
			}

			if((stream_1 = fopen(newFile, "w")) != NULL) {

				fprintf(stream_1, "%d\n", events[i].numTickets - leftTickets);
				if(events[i].numTickets != 0) {
					for(int k = 0; k < events[i].numTickets; k++) {

						if(events[i].soldTickets[k] != -1)
							fprintf(stream_1, "%d\n", events[i].soldTickets[k]);
						else {
							checker = 1;
							checker1 = i;
							counter++;
						}
					}
					if(checker) events[checker1].numTickets -= counter;
				}
				fclose(stream_1);
			}
			fprintf(stream, "%d %s %s %s %d %s %s %lf %s\n", events[i].eventCode, events[i].accName, events[i].eventName, events[i].eventPlace, events[i].numTickets, events[i].date, events[i].time, events[i].ticketPrice, events[i].isBlocked);
		}
		fclose(stream);
	}
}

int generateEventCode() {

	int newEventCode = 0;

	int numberOfEvents = 0;
	struct Event* events = getEvents(&numberOfEvents);

	int shouldIRun = 1;
	srand(time(NULL));

	do {
		shouldIRun = 0;
		int newID = 1000 + rand() % 9000;

		for(int i = 0; i < numberOfEvents; i++) 
			if(newID == events[i].eventCode)
				shouldIRun = 1;

		if(shouldIRun == 0)
			newEventCode = newID;

	} while(shouldIRun);

	freeEvent(events, numberOfEvents);

	return newEventCode;
}

void createEvent(FILE* stream, char* loggedClient) {

	int newEventCode = generateEventCode();

	char eventName[30] = { '\0' };
	printf("Unesite ime dogadjaja: ");
	modifyCharacter(eventName, 30, stream);

	int shouldIRun = 1;

	char date[30] = { '\0' };
	do {
		printf("Unesite datum odrzavanja: ");
		printf("\nDan, mjesec i godina: ");
		modifyCharacter(date, 30, stream);
		if(checkIfDateValid(date)) shouldIRun = 0;
	} while(shouldIRun);
	printf("Unesite mjesto odrzavanja: ");
	char eventPlace[30] = { '\0' };
	modifyCharacter(eventPlace, 30, stream);

	char time[30] = { '\0' };
	shouldIRun = 1;
	do {
		printf("Unesite vrijeme odrzavanja: ");
		printf("\nSati i minute: ");
		modifyCharacter(time, 30, stream);
		if(checkIfTimeValid(time)) shouldIRun = 0;
	} while(shouldIRun);

	printf("Unesite cijenu ulaznica: ");
	double price;
	char ticketPrice[30] = { '\0' };
	modifyCharacter(ticketPrice, 30, stream);
	price = atof(ticketPrice);

	int numberOfEvents = 0;
	struct Event* events = getEvents(&numberOfEvents);

	events = realloc(events, (numberOfEvents + 1) * sizeof(struct Event));

	int i = numberOfEvents;

	events[i].eventCode = newEventCode;

	strcpy(events[i].eventName, eventName);
	strcpy(events[i].eventPlace, eventPlace);

	strcpy(events[i].accName, loggedClient);

	events[i].numTickets = 0;
	events[i].ticketPrice = price;

	strcpy(events[i].date, date);
	strcpy(events[i].time, time);

	events[i].soldTickets = NULL;

	strcpy(events[i].isBlocked, "No");

	writeEvent(events, ++i);

	freeEvent(events, i);
}

void printSoldTickets(char* loggedClient) {

	int numberOfEvents = 0;
	struct Event* events = getEvents(&numberOfEvents);

	int counter = 0;

	for(int i = 0; i < numberOfEvents; i++) {

		if(strcmp(events[i].accName, loggedClient) == 0) {

			counter++;
		}
	}

	int* numTickets = calloc(counter, sizeof(int));
	char** eventNames = calloc(counter, sizeof(char*));

	int j = 0;

	for(int i = 0; i < numberOfEvents; i++) {

		if(strcmp(events[i].accName, loggedClient) == 0) {

			numTickets[j] = events[i].numTickets;
			char dump[30] = { '\0' };
			strcpy(dump, events[i].eventName);

			eventNames[j] = calloc(strlen(events[i].eventName) + 1, 1);
			strcpy(eventNames[j], dump);
			j++;
		}
	}

	for(int i = 0; i < counter; i++) {

		if(numTickets[i] != 0) {

			printf("%d x %s\n", numTickets[i], eventNames[i]);
		}
	}

	freeEvent(events, numberOfEvents);
	free(numTickets);
	for(int i = 0; i < counter; i++) {

		free(eventNames[i]);
	}
	free(eventNames);
}

void printByPeriod(char* loggedClient, FILE* stream) {

	printf("Unesite pocetni datum: ");
	char dump[30] = { '\0' };
	modifyCharacter(dump, 30, stream);

	printf("Unesite krajnji datum: ");
	char dump1[30] = { '\0' };
	modifyCharacter(dump1, 30, stream);

	struct Date date = convertToDate(dump);
	struct Date date1= convertToDate(dump1);

	struct Data data;
	allTickets(date, date1, &data);

	FILE* file_stream;
	printf("Unesite ime fajla: ");
	char temp_name[30] = { '\0' };
	modifyCharacter(temp_name, 30, stream);
	strcat(temp_name, ".txt");

	if((file_stream = fopen(temp_name, "w")) != NULL) {

		for(int i = 0; i < data.numberOfData; i++) {

			fprintf(file_stream, "%d x %s\n", data.soldTickets[i], data.eventName[i]);
			free(data.eventName[i]);
		}
		free(data.soldTickets);
		fclose(file_stream);
	}

}
