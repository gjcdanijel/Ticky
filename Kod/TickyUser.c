#include "TickyUser.h"

void userOption(struct Info info, FILE* stream) {

	char loggedUser[30] = { '\0' };
	if(userLogIn(info, stream, loggedUser)) {

		int checkShouldIRun = 1;

		do
		{
			printf("Pregled dogadjaja [1], Obrada ulaznica [2], Kraj [0]: ");
			char character[15] = { '\0' };
			modifyCharacter(character, 15, stream);

			if(checkFirstCharacter(character)) {

				int numOfEvents = 0;
				struct Event* events = getEvents(&numOfEvents);
				
				if(numOfEvents != 0) printf("\n");
				for(int i = 0; i < numOfEvents; i++)
					printf("%s %s %s %s %lf\n", events[i].eventName, events[i].eventPlace, events[i].date, events[i].time, events[i].ticketPrice);

				freeEvent(events, numOfEvents);

			}
			else if(checkSecondCharacter(character)) {

				int checkShouldIRun1 = 1;
				do
				{
					printf("Kupovina ulaznica [1], Pregled kupljenih ulaznica [2], Ponistavanje kupljene ulaznice [3], Kraj [0]: ");

					char character1[15] = { '\0' };
					modifyCharacter(character1, 15, stream);

					if(checkFirstCharacter(character1)) {

						buyTicket(loggedUser, stream);
					}
					else if(checkSecondCharacter(character1)) {

						processTickets(loggedUser, stream, 0);
					}
					else if (checkThirdCharacter(character1)) {

						processTickets(loggedUser, stream, 1);
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

int userLogIn(struct Info info, FILE* stream, char* loggedUser) {

	struct User user;
	int shouldIRun = 1;

	do {

		printf("Unesite ime naloga: ");
		modifyCharacter(user.accName, 30, stream);

		printf("Unesite lozinku naloga: ");
		modifyCharacter(user.accPass, 30, stream);

		int userNumber = 0;
		if(checkUserLogInInfo(user, &userNumber)) {

			int numberOfUsers = 0;
			struct User* users = getUsers(&numberOfUsers);

			if(++users[userNumber].numOfLogIns == info.allowedNumber) {

				char newUserPass[30] = { '\0' };
				changeAccountCredentials(newUserPass, 30, stream);

				strcpy(users[userNumber].accPass, newUserPass);
				users[userNumber].numOfLogIns = 0;
			}

			writeUsers(users, numberOfUsers);
			shouldIRun = 0;
			free(users);
		}
		else {

			printf("Nema takvog korisnika.\n");
			return 0;
		}

	} while(shouldIRun);

	int numberOfUsers = 0;
	struct User* users = getUsers(&numberOfUsers);

	for(int i = 0; i < numberOfUsers; i++) {

		if(strcmp(users[i].accName, loggedUser) == 0)
			if(strcmp(users[i].accState, "Suspended") == 0 || strcmp(users[i].accCondition, "Deleted") == 0) {

				printf("Nedozvoljen pristup.\n");
				free(users);
				return 0;
			}
	}
	free(users);
	strcpy(loggedUser, user.accName);
	return 1;
}

int checkUserLogInInfo(struct User user, int* userNumber) {

	int numberOfUsers = 0;
	struct User* users = getUsers(&numberOfUsers);

	for(int i = 0; i < numberOfUsers; i++) {

		if(strcmp(users[i].accName, user.accName) == 0 && strcmp(users[i].accPass, user.accPass) == 0) {

			if(strcmp(users[i].accState, "Suspended") != 0 && strcmp(users[i].accCondition, "Deleted") != 0) {
			
				*userNumber = i;
				free(users);
				return 1;
			}
			else {

				free(users);
				return 0;
			}
		}
	}

	free(users);
	printf("Pogresni uneseni podaci!\n");
	return 0;
}

void buyTicket(char* loggedUser, FILE* stream) {

	int numOfEvents = 0;
	struct Event* events = getEvents(&numOfEvents);
	if(numOfEvents != 0) {

		printf("\n");
		for(int i = 0; i < numOfEvents; i++) 
			printf("[%d] %s %s %s %s %lf\n", i + 1, events[i].eventName, events[i].eventPlace, events[i].date, events[i].time, events[i].ticketPrice);

		int index;
		do {

			printf("Unesite redni broj dogadjaja: ");
			char character[15] = { '\0' };
			modifyCharacter(character, 15, stream);

			sscanf(character, "%d", &index);
		} while(index > numOfEvents || index < 0);
		index--;

		if(strcmp(events[index].isBlocked, "Yes") == 0) {

			printf("Trenutno se ne mogu kupiti ulaznice za dogadjaj.\n");
			freeEvent(events, numOfEvents);
			return;
		}

		char character[15] = { '\0' };
		int shouldIRun = 1;
		do {

			printf("Preuzimanje elektronski [1], Preuzimanje na lokaciji [2]: ");
			modifyCharacter(character, 15, stream);
			if(checkFirstCharacter(character)) shouldIRun = 0;
			else if(checkSecondCharacter(character)) shouldIRun = 0;
			else printf("Greska! Unesite ponovo. \n");
		} while(shouldIRun);

		int numberOfUsers = 0;
		struct User* users = getUsers(&numberOfUsers);

		double tempBalance = 0;

		int userIndex = 0;

		for(int i = 0; i < numberOfUsers; i++)
			if(strcmp(loggedUser, users[i].accName) == 0) {

				userIndex = i;
				tempBalance = users[i].accBalance;
			}

		if(events[index].ticketPrice > tempBalance) {

			printf("Nedovoljno kredita na racunu. \n");
		}
		else {

			users[userIndex].accBalance -= events[index].ticketPrice;
			int ticketCode = generateTicketCode();

			writeUsers(users, numberOfUsers);

			events[index].soldTickets = realloc(events[index].soldTickets, (events[index].numTickets + 1) * sizeof(int));

			events[index].soldTickets[events[index].numTickets] = ticketCode;
			events[index].numTickets++;
			writeEvent(events, numOfEvents);

			char dump[50] = { '\0' };
			sprintf(dump, "%d", events[index].eventCode);

			char newFile[50] = { '\0' };
			strcat(newFile, "../Baza_podataka/");
			strcat(newFile, dump);
			strcat(newFile, ".txt");

			FILE* stream;

			if((stream = fopen(newFile, "w")) != NULL) {

				fprintf(stream, "%d\n", events[index].numTickets);
				for(int i = 0; i < events[index].numTickets; i++)
					fprintf(stream, "%d\n", events[index].soldTickets[i]);
				fclose(stream);
			}

			char userFile[50] = { '\0' };
			strcat(userFile, "../Baza_podataka/");
			strcat(userFile, loggedUser);
			strcat(userFile, ".txt");

			int numberOfTickets = 0;
			int* array = NULL;

			if((stream = fopen(userFile, "r")) != NULL) {

				fscanf(stream, "%d\n", &numberOfTickets);
				int dump = 0;
				array = calloc(numberOfTickets, sizeof(int));

				for(int i = 0; i < numberOfTickets; i++) {

					fscanf(stream, "%d\n", &dump);
					array[i] = dump;
				}

				fclose(stream);
			}
			else {

				if((stream = fopen(userFile, "w")) != NULL) {

					fprintf(stream, "%d\n", numberOfTickets);
					fclose(stream);
				}
			}

			array = realloc(array, (numberOfTickets + 1) * sizeof(int));
			array[numberOfTickets++] = ticketCode;

			if((stream = fopen(userFile, "w")) != NULL) {

				fprintf(stream, "%d\n", numberOfTickets);
				for(int i = 0; i < numberOfTickets; i++)
					fprintf(stream, "%d\n", array[i]);
				fclose(stream);
			}

			free(array);
		}

		free(users);
	}
	freeEvent(events, numOfEvents);
}

int generateTicketCode() {

	int newTicketCode = 0;

	int numberOfEvents = 0;
	struct Event* events = getEvents(&numberOfEvents);

	int shouldIRun = 1;
	srand(time(NULL));

	do {

		shouldIRun = 0;
		int newID = 1000 + rand() % 9000;

		for(int i = 0; i < numberOfEvents; i++) {

			for(int j = 0; j < events[i].numTickets; j++) {

				if(newID == events[i].soldTickets[j]) {

					shouldIRun = 1;
					break;
				}
			}
		}

		if(shouldIRun == 0) newTicketCode = newID;

	} while(shouldIRun);

	freeEvent(events, numberOfEvents);

	return newTicketCode;
}

void processTickets(char* loggedUser, FILE* inputStream, int globalChecker){

	char userFile[50] = { '\0' };
	strcat(userFile, "../Baza_podataka/");
	strcat(userFile, loggedUser);
	strcat(userFile, ".txt");
	FILE* stream;

	int numberOfTickets = 0;
	int* arrayOfTickets = NULL;

	if((stream = fopen(userFile, "r")) != NULL) {

		fscanf(stream, "%d\n", &numberOfTickets);
		arrayOfTickets = calloc(numberOfTickets, sizeof(int));

		for(int i = 0; i < numberOfTickets; i++) {

			int dump = 0;
			fscanf(stream, "%d\n", &dump);
			arrayOfTickets[i] = dump;
		}

		fclose(stream);
	}
	else return;

	char** nameOfTickets = calloc(numberOfTickets, sizeof(char*));

	int numberOfEvents = 0;
	struct Event* events = getEvents(&numberOfEvents);

	for(int i = 0; i < numberOfEvents; i++) {

		char eventFile[50] = { '\0' };
		sprintf(eventFile, "../Baza_podataka/%d.txt", events[i].eventCode);

		int* arrayOfEventCode = calloc(events[i].numTickets, sizeof(int));

		int oldTemp = 0;

		if((stream = fopen(eventFile, "r")) != NULL) {

			fscanf(stream, "%d\n", &oldTemp);
			for(int j = 0; j < events[i].numTickets; j++) {

				int dump = 0;
				fscanf(stream, "%d\n", &dump);
				arrayOfEventCode[j] = dump;
			}
			fclose(stream);
		}

		for(int j = 0; j < numberOfTickets; j++) {

			for(int k = 0; k < events[i].numTickets; k++) {

				if(arrayOfTickets[j] == arrayOfEventCode[k]) {

					nameOfTickets[j] = calloc(strlen(events[i].eventName) + 1, 1);
					strcat(nameOfTickets[j], events[i].eventName);
				}
			}
		}
		free(arrayOfEventCode);
	}
	if(globalChecker == 0) {

		free(arrayOfTickets);
		if(numberOfTickets != 0) printf("\n");
		for(int i = 0; i < numberOfTickets; i++) {

			printf("%s\n", nameOfTickets[i]);
			free(nameOfTickets[i]);
		}
		free(nameOfTickets);
		freeEvent(events, numberOfEvents);
	}
	else {

		int indexName = 0;

		if(numberOfTickets == 0) {

			free(arrayOfTickets);
			free(nameOfTickets);
			freeEvent(events, numberOfEvents);
			return;
		}

		printf("\n");
		for(int i = 0; i < numberOfTickets; i++)
			printf("[%d] %s\n", i + 1, nameOfTickets[i]);

		do {

			printf("Unesite redni broj dogadjaja: ");
			char character[15] = { '\0' };
			modifyCharacter(character, 15, inputStream);

			sscanf(character, "%d", &indexName);

		} while(indexName > numberOfTickets || indexName <= 0);

		indexName--;

		int temp = arrayOfTickets[indexName];

		for(int i = indexName; i < numberOfTickets - 1; i++)
			arrayOfTickets[i] = arrayOfTickets[i + 1];

		numberOfTickets--;

		if((stream = fopen(userFile, "w")) != NULL) {

			fprintf(stream, "%d\n", numberOfTickets);
			for(int i = 0; i < numberOfTickets; i++) {

				fprintf(stream, "%d\n", arrayOfTickets[i]);
			}
			fclose(stream);
		}

		int refundPrice = 0;

		for(int i = 0; i < numberOfEvents; i++) {

			for(int j = 0; j < events[i].numTickets; j++)
				if(events[i].soldTickets[j] == temp) {

					refundPrice = events[i].ticketPrice;
					events[i].soldTickets[j] = -1;
				}
		}

		int numberOfUsers = 0;
		struct User* users = getUsers(&numberOfUsers);

		for(int i = 0; i < numberOfUsers; i++)
			if(strcmp(loggedUser, users[i].accName) == 0)
				users[i].accBalance += refundPrice;

		writeUsers(users, numberOfUsers);
		free(users);

		writeEvent(events, numberOfEvents);

		free(arrayOfTickets);

		for(int i = 0; i < numberOfTickets + 1; i++)
			free(nameOfTickets[i]);
		free(nameOfTickets);
		freeEvent(events, numberOfEvents);
	}
}  
