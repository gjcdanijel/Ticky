#include "TickyAdmin.h"
#include "TickyGlobal.h"
#include "TickyClient.h"

void adminOption(struct Info* info, FILE* stream) {

	if(adminLogIn(info, stream)) {

		int checkShouldIRun = 1;
		do {

			printf("Kreiranje naloga [1], Obrada naloga [2], Blokiranje dogadjaja [3], Kraj [0]: ");
			char character[15] = { '\0' };
			modifyCharacter(character, 15, stream);

			if(checkFirstCharacter(character)) {

				int checkShouldIRun1 = 1;

				do {

					char character1[15] = { '\0' };
					printf("Kreiranje admin naloga [1], Kreiranje klijent naloga [2], Kraj [0]: ");
					modifyCharacter(character1, 15, stream);

					if(checkFirstCharacter(character1)) {

						createAdmin(stream);	
					}
					else if(checkSecondCharacter(character1)) {

						createClient(stream);
					}
					else if(checkExitCharacter(character1)) checkShouldIRun1 = 0;
					else printf("Greska. Unesite ponovo!\n");

				} while(checkShouldIRun1);
			}
			else if(checkSecondCharacter(character)) {

				int checkShouldIRun1 = 1;

				do {

					char character1[15] = { '\0' };
					printf("Aktivacija [1], Suspendovanje [2], Brisanje [3], Ponistavanje sifre [4], Kraj [0]: ");
					modifyCharacter(character1, 15, stream);

					if(checkFirstCharacter(character1)) {

						activateAccount(stream);
					}
					else if(checkSecondCharacter(character1)) {

						suspendAccount(stream);
					}
					else if(checkThirdCharacter(character1)) {

						deleteAccount(stream);
					}
					else if(checkFourthCharacter(character1)) {

						cancelPass(stream);
					}
					else if(checkExitCharacter(character1)) checkShouldIRun1 = 0;
					else printf("Greska. Unesite ponovo!\n");

				} while(checkShouldIRun1);
			}
			else if(checkThirdCharacter(character)) {

				blockEvent(stream);
			}
			else if(checkExitCharacter(character)) checkShouldIRun = 0;
			else printf("Greska. Unesite ponovo!\n");
		} while(checkShouldIRun);
	}
}

int adminLogIn(struct Info* info, FILE* stream) {

	struct Admin admin;
	int shouldIRun = 1;

	do {

		if(info->firstTime == 0) {

			printf("Unesite ime naloga: ");
			modifyCharacter(admin.accountName, 30, stream);
			if(strcmp("admin", admin.accountName) != 0) {

				printf("Pogresno ime: \n");
				continue;
			}

			printf("Unesite lozinku naloga: ");
			modifyCharacter(admin.accountPass, 30, stream);
			if(strcmp("admin", admin.accountPass) != 0) {

				printf("Pogresna lozinka: \n");
				continue;
			}
			else {

				char newAdminPass[30] = { '\0' };
				changeAccountCredentials(newAdminPass, 30, stream);

				int numberOfAdmins = 0;
				struct Admin* admins = getAdmins(&numberOfAdmins);

				strcpy(admins[0].accountPass, newAdminPass);
				writeAdmins(admins, numberOfAdmins);

				info->firstTime = 1;
				shouldIRun = 0;
				free(admins);
			}
		}
		else {

			printf("Unesite ime naloga: ");
			modifyCharacter(admin.accountName, 30, stream);

			printf("Unesite lozinku naloga: ");
			modifyCharacter(admin.accountPass, 30, stream);

			int adminNumber = 0;
			if(checkAdminLogInInfo(admin, &adminNumber)) {

				int numberOfAdmins = 0;
				struct Admin* admins = getAdmins(&numberOfAdmins);
				if(++admins[adminNumber].numberOfLogIns == info->allowedNumber) {

					char newAdminPass[30] = { '\0' };
					changeAccountCredentials(newAdminPass, 30, stream);

					strcpy(admins[adminNumber].accountPass, newAdminPass);
					admins[adminNumber].numberOfLogIns = 0;
				}
				writeAdmins(admins, numberOfAdmins);
				shouldIRun = 0;
				free(admins);
			}
		}

	}while(shouldIRun);

	writeInfo(info);

	return 1;
}

void writeInfo(struct Info* info) {

	FILE* stream;

	if((stream = fopen("../Baza_podataka/info.txt", "w")) != NULL) {

		fprintf(stream, "%d\n", info->allowedNumber);
		fprintf(stream, "%d\n", info->firstTime);
		fclose(stream);
	}
}

int checkAdminLogInInfo(struct Admin admin, int* adminNumber) {

	int numberOfAdmins = 0;
	struct Admin* admins = getAdmins(&numberOfAdmins);
	for(int i = 0; i < numberOfAdmins; i++) {

		if(strcmp(admins[i].accountName, admin.accountName) == 0 &&
				strcmp(admins[i].accountPass, admin.accountPass) == 0) {

			*adminNumber = i;
			free(admins);
			return 1;
		}
	}
	free(admins);
	printf("Pogresno uneseni podaci!\n");
	return 0;
}

void writeAdmins(struct Admin* admins, int numberOfAdmins) {

	FILE* stream;

	if((stream = fopen("../Baza_podataka/admin.txt", "w")) != NULL) {

		fprintf(stream, "%d\n", numberOfAdmins);
		for(int i = 0; i < numberOfAdmins; i++)
			fprintf(stream, "%s %s %d\n", admins[i].accountName, admins[i].accountPass, admins[i].numberOfLogIns);

		fclose(stream);
	}
}

void createAdmin(FILE* stream) {

	struct Admin admin;
	int numberOfAdmins = 0;
	struct Admin* admins = getAdmins(&numberOfAdmins);

	do {

		printf("Unesite ime admin naloga: ");
		modifyCharacter(admin.accountName, 30, stream);

		printf("Unesite sifru admin naloga: ");
		modifyCharacter(admin.accountPass, 30, stream);
	} while(checkCredentials(admin.accountName) == 0 || checkCredentials(admin.accountPass) == 0 || compareAccountName(admin.accountName) == 0);

	admins = realloc(admins, (numberOfAdmins + 1) * sizeof(struct Admin));
	admin.numberOfLogIns = 0;

	strcpy(admins[numberOfAdmins].accountName, admin.accountName);
	strcpy(admins[numberOfAdmins].accountPass, admin.accountPass);
	admins[numberOfAdmins].numberOfLogIns = admin.numberOfLogIns;

	writeAdmins(admins, ++numberOfAdmins);
	free(admins);
}

void createClient(FILE* stream) {

	struct Client client;
	int numberOfClients = 0;
	struct Client* clients = getClients(&numberOfClients);

	do {

		printf("Unesite ime klijent naloga: ");
		modifyCharacter(client.accName, 30, stream);

		printf("Unesite sifru klijent naloga: ");
		modifyCharacter(client.accPass, 30, stream);
	} while(checkCredentials(client.accPass) == 0 || checkCredentials(client.accName) == 0 || compareAccountName(client.accName) == 0);

	clients = realloc(clients, (numberOfClients + 1) * sizeof(struct Client));
	client.numOfLogIns = 0;

	strcpy(clients[numberOfClients].accName, client.accName);
	strcpy(clients[numberOfClients].accPass, client.accPass);
	clients[numberOfClients].numOfLogIns = client.numOfLogIns;
	strcpy(clients[numberOfClients].accState, "Activated");
	strcpy(clients[numberOfClients].accCondition, "Active");

	writeClients(clients, ++numberOfClients);
	free(clients);
}

void activateAccount(FILE* stream) {

	int checkShouldIRun = 1;

	do {

		char character[15] = { '\0' };
		printf("Aktiviranje klijent naloga [1], Aktiviranje korisnik naloga [2], Kraj [0]: ");

		modifyCharacter(character, 15, stream);

		if(checkFirstCharacter(character)) {

			int numberOfClients = 0;
			struct Client* clients = getClients(&numberOfClients);

			if(numberOfClients != 0) {

				printClients();
				char accountName[30] = { '\0' };
				printf("Unesite ime naloga: ");
				modifyCharacter(accountName, 30, stream);

				for(int i = 0; i < numberOfClients; i++)

					if(strcmp(clients[i].accName, accountName) == 0) {

						if(strcmp(clients[i].accState, "Activated") == 0) {

							printf("Nalog je vec bio aktivan. \n");
						}
						else {

							printf("Nalog je aktiviran. \n");
							strcpy(clients[i].accState, "Activated");
							writeClients(clients, numberOfClients);
						}
					}
			}
			else printf("Nema klijent naloga. \n");

			free(clients);
		}
		else if(checkSecondCharacter(character)) {

			int numberOfUsers = 0;
			struct User* users = getUsers(&numberOfUsers);

			if(numberOfUsers != 0) {

				printUsers();
				char accountName[30] = { '\0' };
				printf("Unesite ime naloga: ");
				modifyCharacter(accountName, 30, stream);

				for(int i = 0; i < numberOfUsers; i++)

					if(strcmp(users[i].accName, accountName) == 0) {

						if(strcmp(users[i].accState, "Acivated") == 0) {

							printf("Nalog je vec bio aktivan. \n");
						}
						else {

							printf("Nalog je aktiviran. \n");
							strcpy(users[i].accState, "Activated");
							writeUsers(users, numberOfUsers);
						}
					}
			}
			else printf("Nema korisnik naloga. \n");

			free(users);
		}
		else if(checkExitCharacter(character)) checkShouldIRun = 0;
		else printf("Greska. Unesite ponovo!\n"); 

	} while(checkShouldIRun);
}

void suspendAccount(FILE* stream) {

	int checkShouldIRun = 1; 
	do {
		char character[15] = { '\0' };
		printf("Suspendovanje klijent naloga [1], Suspendovanje korisnik naloga [2], Kraj [0]: ");
		modifyCharacter(character, 15, stream);
		if(checkFirstCharacter(character)) {
			int numberOfClients = 0;
			struct Client* clients = getClients(&numberOfClients);
			if(numberOfClients != 0) {
				printClients();
				char accountName[30] = { '\0' };
				printf("Unesite ime naloga: ");
				modifyCharacter(accountName, 30, stream);
				for(int m = 0; m < numberOfClients; m++) {
					if(strcmp(clients[m].accName, accountName) == 0) {
						if(strcmp(clients[m].accState, "Suspended") == 0) {
							printf("Nalog je vec bio suspendovan. \n");
						}
						else {
							printf("Nalog je suspendovan. \n");
							strcpy(clients[m].accState, "Suspended");
							writeClients(clients, numberOfClients);
							int numberOfEvents = 0;
							struct Event* events = getEvents(&numberOfEvents);
							for(int i = 0; i < numberOfEvents; i++) {
								if(strcmp(accountName, events[i].accName) == 0) {
									int* eventTickets = events[i].soldTickets;
									events[i].soldTickets = NULL;
									char dumpFile[50] = { '\0' };
									sprintf(dumpFile, "../Baza_podataka/%d.txt", events[i].eventCode);
									FILE* file_stream;

									if((file_stream = fopen(dumpFile, "w")) != NULL) {

										fprintf(file_stream, "%d\n", 0);
										fclose(file_stream);
									}
									int numberOfUsers = 0;
									struct User* users = getUsers(&numberOfUsers);
									for(int j = 0; j < numberOfUsers; j++) {
										char userFile[50] = { '\0' };
										strcat(userFile, "../Baza_podataka/");
										strcat(userFile, users[j].accName);
										strcat(userFile, ".txt");
										int counter = 0;
										int* tempArray = NULL;
										int numberOfTickets = 0;
										if((file_stream = fopen(userFile, "r")) != NULL) {

											fscanf(file_stream, "%d\n", &numberOfTickets);
											tempArray = calloc(numberOfTickets, sizeof(int));

											for(int k = 0; k < numberOfTickets; k++) {

												fscanf(file_stream, "%d\n", &tempArray[k]);
											}
											fclose(file_stream);
										}
										for(int x = 0; x < events[i].numTickets; x++) {
											for(int y = 0; y < numberOfTickets; y++) {
												if(eventTickets[x] == tempArray[y]) {

													tempArray[y] = -1;
													users[j].accBalance += events[i].ticketPrice;
													counter++;
												}
											}
										}
										int* newArray = calloc(numberOfTickets - counter, sizeof(int));
										for(int x = 0, y = 0; x < numberOfTickets; x++)
											if(tempArray[x] != -1) newArray[y++] = tempArray[x];

										if((file_stream = fopen(userFile, "w")) != NULL) {

											fprintf(file_stream, "%d\n", numberOfTickets - counter);
											for(int x = 0; x < numberOfTickets - counter; x++)
												fprintf(file_stream, "%d\n", newArray[x]);
											fclose(file_stream);
										}
										free(tempArray);
										free(newArray);
									}
									writeUsers(users, numberOfUsers);
									free(users);
									events[i].numTickets = 0;
									writeEvent(events, numberOfEvents);
									free(eventTickets);
								}
							}
							freeEvent(events, numberOfEvents);
						}
					}
				}
			}
			else printf("Nema klijent naloga. \n");
			free(clients);
		}
		else if(checkSecondCharacter(character)) {

			int numberOfUsers = 0;
			struct User* users = getUsers(&numberOfUsers);

			if(numberOfUsers != 0) {

				printUsers();
				char accountName[30] = { '\0' };
				printf("Unesite ime naloga: ");
				modifyCharacter(accountName, 30, stream);

				for(int m = 0; m < numberOfUsers; m++) {

					if(strcmp(users[m].accName, accountName) == 0) {

						if(strcmp(users[m].accState, "Suspended") == 0) {

							printf("Nalog je vec bio suspendovan. \n");
						}
						else {

							printf("Nalog je suspendovan. \n");
							strcpy(users[m].accState, "Suspended");

							FILE* file_stream;
							char userFile[50] = { '\0' };
							strcat(userFile, "../Baza_podataka/");
							strcat(userFile, users[m].accName);
							strcat(userFile, ".txt");

							int* arrayOfTickets = NULL;
							int numberOfTickets = 0;

							if((file_stream = fopen(userFile, "r")) != NULL) {

								fscanf(file_stream, "%d\n", &numberOfTickets);
								arrayOfTickets = calloc(numberOfTickets, sizeof(int));
								for(int i = 0; i < numberOfTickets; i++)
									fscanf(file_stream, "%d\n", &arrayOfTickets[i]);
								fclose(file_stream);
							}

							if((file_stream = fopen(userFile, "w")) != NULL) {

								fprintf(file_stream, "%d\n", 0);
								fclose(file_stream);
							}

							int numberOfEvents = 0;
							struct Event* events = getEvents(&numberOfEvents);
							for(int i = 0; i < numberOfEvents; i++) {

								for(int j = 0; j < events[i].numTickets; j++) {

									for(int k = 0; k < numberOfTickets; k++) {

										if(events[i].soldTickets[j] == arrayOfTickets[k]) {

											users[m].accBalance += events[i].ticketPrice;
											events[i].soldTickets[j] = -1;
										}
									}
								}
							}
							free(arrayOfTickets);
							writeEvent(events, numberOfEvents);
							freeEvent(events, numberOfEvents);
						}
					}
				}
				writeUsers(users, numberOfUsers);
			}
			else printf("Nema korisnik naloga. \n");

			free(users);
		}
		else if(checkExitCharacter(character)) checkShouldIRun = 0;
		else printf("Greska. Unesite ponovo!\n");	

	} while(checkShouldIRun);
}

void deleteAccount(FILE* stream) {

	int checkShouldIRun = 1;

	do {

		char character[15] = { '\0' };
		printf("Brisanje klijent naloga [1], Brisanje korisnik naloga [2], Kraj [0]: ");

		modifyCharacter(character, 15, stream);

		if(checkFirstCharacter(character)) {

			int numberOfClients = 0;
			struct Client* clients = getClients(&numberOfClients);

			if(numberOfClients != 0) {

				printClients();
				char accountName[30] = { '\0' };
				printf("Unesite ime naloga: ");
				modifyCharacter(accountName, 30, stream);

				for(int m = 0; m < numberOfClients; m++) {

					if(strcmp(clients[m].accName, accountName) == 0) {

						if(strcmp(clients[m].accCondition, "Deleted") == 0) {

							printf("Nalog je vec bio obrisan. \n");
						}
						else {

							printf("Nalog obrisan. \n");
							strcpy(clients[m].accCondition, "Deleted");
							writeClients(clients, numberOfClients);

							int numberOfEvents = 0;
							struct Event* events = getEvents(&numberOfEvents);
							for(int i = 0; i < numberOfEvents; i++) {

								if(strcmp(accountName, events[i].accName) == 0) {

									int* eventTickets = events[i].soldTickets;
									events[i].soldTickets = NULL;

									char dumpFile[50] = { '\0' };
									sprintf(dumpFile, "../Baza_podataka/%d.txt", events[i].eventCode);
									FILE* file_stream;
									if((file_stream = fopen(dumpFile, "w")) != NULL) {

										fprintf(stream, "%d\n", 0);
										fclose(file_stream);
									}

									int numberOfUsers = 0;
									struct User* users = getUsers(&numberOfUsers);
									for(int j = 0; j < numberOfUsers; j++) {
										char userFile[50] = { '\0' };
										strcat(userFile, "../Baza_podataka/");
										strcat(userFile, users[j].accName);
										strcat(userFile, ".txt");
										int counter = 0;
										int* tempArray = NULL;
										int numberOfTickets = 0;
										if((file_stream = fopen(userFile, "r")) != NULL) {

											fscanf(file_stream, "%d\n", &numberOfTickets);
											tempArray = calloc(numberOfTickets, sizeof(int));

											for(int k = 0; k < numberOfTickets; k++) {

												fscanf(file_stream, "%d\n", &tempArray[k]);
											}
											fclose(file_stream);
										}
										for(int x = 0; x < events[i].numTickets; x++) {
											for(int y = 0; y < numberOfTickets; y++) {
												if(eventTickets[x] == tempArray[y]) {

													tempArray[y] = -1;
													users[j].accBalance += events[i].ticketPrice;
													counter++;
												}
											}
										}
										int* newArray = calloc(numberOfTickets - counter, sizeof(int));
										for(int x = 0, y = 0; x < numberOfTickets; x++)
											if(tempArray[x] != -1) newArray[y++] = tempArray[x];

										if((file_stream = fopen(userFile, "w")) != NULL) {

											fprintf(file_stream, "%d\n", numberOfTickets - counter);
											for(int x = 0; x < numberOfTickets - counter; x++)
												fprintf(file_stream, "%d\n", newArray[x]);
											fclose(file_stream);
										}
										free(tempArray);
										free(newArray);
									}
									writeUsers(users, numberOfUsers);
									free(users);
									events[i].numTickets = 0;
									writeEvent(events, numberOfEvents);
									free(eventTickets);
								}
							}
							freeEvent(events, numberOfEvents);
						}
					}
				}
			} 
			else printf("Nema klijent naloga. \n");
			free(clients);
		} 
		else if(checkSecondCharacter(character)) {

			int numberOfUsers = 0;
			struct User* users = getUsers(&numberOfUsers);

			if(numberOfUsers != 0) {

				printUsers();
				char accountName[30] = { '\0' };
				printf("Unesite ime naloga: ");
				modifyCharacter(accountName, 30, stream);

				for(int m = 0; m < numberOfUsers; m++) {

					if(strcmp(users[m].accName, accountName) == 0) {

						if(strcmp(users[m].accCondition, "Deleted") == 0) {

							printf("Nalog je vec bio obrisan. \n");
						}
						else {

							printf("Nalog obrisan: \n");
							strcpy(users[m].accCondition, "Deleted");
							
							FILE* file_stream;
							char userFile[50] = { '\0' };
							strcat(userFile, "../Baza_podataka/");
							strcat(userFile, users[m].accName);
							strcat(userFile, ".txt");

							int* arrayOfTickets = NULL;
							int numberOfTickets = 0;

							if((file_stream = fopen(userFile, "r")) != NULL) {

								fscanf(file_stream, "%d\n", &numberOfTickets);
								arrayOfTickets = calloc(numberOfTickets, sizeof(int));
								for(int i = 0; i < numberOfTickets; i++)
									fscanf(file_stream, "%d\n", &arrayOfTickets[i]);
								fclose(file_stream);
							}

							if((file_stream = fopen(userFile, "w")) != NULL) {

								fprintf(file_stream, "%d\n", 0);
								fclose(file_stream);
							}

							int numberOfEvents = 0;
							struct Event* events = getEvents(&numberOfEvents);
							for(int i = 0; i < numberOfEvents; i++) {

								for(int j = 0; j < events[i].numTickets; j++) {

									for(int k = 0; k < numberOfTickets; k++) {

										if(events[i].soldTickets[j] == arrayOfTickets[k]) {

											users[m].accBalance += events[i].ticketPrice;
											events[i].soldTickets[j] = -1;
										}
									}
								}
							}
							free(arrayOfTickets);
							writeEvent(events, numberOfEvents);
							freeEvent(events, numberOfEvents);
						}
					}
				}
				writeUsers(users, numberOfUsers);
			} 
			else printf("Nema korisnik naloga. \n");
			free(users);

		} 
		else if(checkExitCharacter(character)) checkShouldIRun = 0;
		else printf("Greska. Unesite ponovno!\n");

	} while(checkShouldIRun);
}


void cancelPass(FILE* stream) {

	int checkShouldIRun = 1;

	do {

		char character[15] = { '\0' };
		printf("Ponistavanje sifre klijent naloga [1], Ponistavanje sifre korisnik naloga [2], Kraj [0]: ");

		modifyCharacter(character, 15, stream);

		if(checkFirstCharacter(character)) {

			int numberOfClients = 0;
			struct Client* clients = getClients(&numberOfClients);

			if(numberOfClients != 0) {

				printClients();
				char accountName[30] = { '\0' };
				printf("Unesite ime naloga: ");
				modifyCharacter(accountName, 30, stream);

				for(int i = 0; i < numberOfClients; i++)

					if(strcmp(clients[i].accName, accountName) == 0) {

						char accountPass[30] = { '\0' };
						printf("Unesite novu sifru naloga: ");
						do {

							modifyCharacter(accountPass, 30, stream);

						} while(checkCredentials(accountPass) == 0);

						printf("Sifra je promenjena. \n");
						strcpy(clients[i].accPass, accountPass);
						writeClients(clients, numberOfClients);
					}
			} 
			else printf("Nema klijent naloga. \n");

			free(clients);

		} 
		else if(checkSecondCharacter(character)) {

			int numberOfUsers = 0;
			struct User* users = getUsers(&numberOfUsers);

			if(numberOfUsers != 0) {

				printUsers();
				char accountName[30] = { '\0' };
				printf("Unesite ime naloga: ");
				modifyCharacter(accountName, 30, stream);

				for(int i = 0; i < numberOfUsers; i++)

					if(strcmp(users[i].accName, accountName) == 0) {

						char accountPass[30] = { '\0' };
						printf("Unesite novu sifru naloga: ");
						do {

							modifyCharacter(accountPass, 30, stream);

						} while(checkCredentials(accountPass) == 0);

						printf("Sifra je promenjena. \n");
						strcpy(users[i].accPass, accountPass);
						writeUsers(users, numberOfUsers);
					}

			} 
			else printf("Nema korisnik naloga. \n");

			free(users);

		} 
		else if(checkExitCharacter(character)) checkShouldIRun = 0;
		else printf("Greska. Unesite ponovno!\n");

	} while(checkShouldIRun);
}

void blockEvent(FILE* stream) {

	int numberOfEvents = 0;
	struct Event* events = getEvents(&numberOfEvents);

	if(numberOfEvents != 0) {

		printEvents();
		char eventName[30] = { '\0' };
		printf("Unesite ime dogadjaja: ");
		modifyCharacter(eventName, 30, stream);

		for(int m = 0; m < numberOfEvents; m++) {

			if(strcmp(events[m].eventName, eventName) == 0) {

				if(strcmp(events[m].isBlocked, "Yes") == 0) {

					printf("Dogadjaj je vec bio blokiran.\n");
				}
				else {

					printf("Dogadjaj je blokiran. \n");
					strcpy(events[m].isBlocked, "Yes");
					
					int numberOfUsers = 0;
					struct User* users = getUsers(&numberOfUsers);
					for(int i = 0; i < numberOfUsers; i++) {

						char userFile[50] = { '\0' };
						strcat(userFile, "../Baza_podataka/");
						strcat(userFile, users[i].accName);
						strcat(userFile, ".txt");

						FILE* file_stream;
						int* arrayOfTickets = NULL;
						int numberOfTickets = 0;

						if((file_stream = fopen(userFile, "r")) != NULL) {

							fscanf(file_stream, "%d\n", &numberOfTickets);
							arrayOfTickets = calloc(numberOfTickets, sizeof(int));

							for(int j = 0; j < numberOfTickets; j++) {

								fscanf(file_stream, "%d\n", &arrayOfTickets[j]);
							}
							fclose(file_stream);
						}
						int counter = 0;
						for(int x = 0; x < events[m].numTickets; x++) {

							for(int y = 0; y < numberOfTickets; y++) {

								if(events[m].soldTickets[x] == arrayOfTickets[y]) {

									arrayOfTickets[y] = -1;
									users[i].accBalance += events[m].ticketPrice;
									counter++;
								}
							}
						}
						int* newArray = calloc(numberOfTickets - counter, sizeof(int));
						for(int x = 0, y = 0; x < numberOfTickets; x++)
							if(arrayOfTickets[x] != -1) newArray[y++] = arrayOfTickets[x];

						if((file_stream = fopen(userFile, "w")) != NULL) {

							fprintf(file_stream, "%d\n", numberOfTickets - counter);
							for(int x = 0; x < numberOfTickets - counter; x++)
								fprintf(file_stream, "%d\n", newArray[x]);
							fclose(file_stream);
						}
						free(arrayOfTickets);
						free(newArray);
					}
					writeUsers(users, numberOfUsers);
					free(users);
					events[m].numTickets = 0;
					writeEvent(events, numberOfEvents);
				}
			}
		}
	}
	freeEvent(events, numberOfEvents);
}

void printEvents() {

	int numberOfEvents = 0;
	struct Event* events = getEvents(&numberOfEvents);

	if(numberOfEvents != 0) printf("\n");
	for(int i = 0; i < numberOfEvents; i++) {

		printf("%s\n", events[i].eventName);
	}
	freeEvent(events, numberOfEvents);
}
