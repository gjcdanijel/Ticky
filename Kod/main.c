#include "TickyGlobal.h"
#include "TickyAdmin.h"
#include "TickyClient.h"
#include "TickyUser.h"

int main() {

	struct Info info;	
	info = readNumberOfLogIns();

	if(info.firstTime == 0) {

		createEnvironment();
	}
	// Koristi se u while petljama
	// za provjeru da li se trebaju
	// ponovo izvrsavati, 1 predstavlja
	// da se treba izvrsavati 0 da se prestaje
	int checkShouldIRun = 1;
	
	do {
		
		// Ulazni "karakter" na osnovu kojeg se bira opcija
		char character[15] = { '\0' };
	
		printf("Prijava [1], Registracija [2], Kraj [0]: ");
		modifyCharacter(character, 15, stdin);
		if(checkFirstCharacter(character)) {
			
			// Prijava na sistem, postoji prijava za
			// korisnika, klijenta i admina
			int checkShouldIRun1 = 1;

			do {
				
				char character1[15] = { '\0' };
			
				printf("Prijava na: Admin [1], Klijent [2], Korisnik [3], Kraj [0]: ");
				modifyCharacter(character1, 15, stdin);
				
				if(checkFirstCharacter(character1)) {

					adminOption(&info, stdin);
				}
				else if(checkSecondCharacter(character1)) {

					clientOption(&info, stdin);
				}
				else if(checkThirdCharacter(character1)) {
				
					userOption(info, stdin);
				}
				else if(checkExitCharacter(character1)) checkShouldIRun1 = 0;
				else printf("Greska. Unesite ponovo!\n");
			} while(checkShouldIRun1);
		}
		else if(checkSecondCharacter(character)) {
		
			char realName[30] = { '\0' };
			do {

				printf("Unesite svoje ime: ");
				modifyCharacter(realName, 30, stdin);
			} while(validationPass(realName));

			char realSurName[30] = { '\0' };
			do {

				printf("Unesite svoje prezime: ");
				modifyCharacter(realSurName, 30, stdin);
			} while(validationPass(realSurName));

			char accName[30] = { '\0' };
			do {

				printf("Unesite ime naloga: ");
				modifyCharacter(accName, 30, stdin);
			} while(!checkCredentials(accName) || !compareAccountName(accName));

			char accPass[30] = { '\0' };
			do {

				printf("Unesite sifru naloga: ");
				modifyCharacter(accPass, 30, stdin);
			} while(!checkCredentials(accPass));

			int numberOfUsers = 0;
			struct User* users = getUsers(&numberOfUsers);

			users = realloc(users, (numberOfUsers + 1) * sizeof(struct User));

			strcpy(users[numberOfUsers].realName, realName);
			strcpy(users[numberOfUsers].realSurName, realSurName);
			strcpy(users[numberOfUsers].accName, accName);
			strcpy(users[numberOfUsers].accPass, accPass);

			users[numberOfUsers].numOfLogIns = 0;

			strcpy(users[numberOfUsers].accState, "Activated");
			strcpy(users[numberOfUsers].accCondition, "Active");

			users[numberOfUsers].accBalance = 0;

			writeUsers(users, ++numberOfUsers);
			free(users);
		}
		else if(checkExitCharacter(character)) checkShouldIRun = 0;
		else printf("Greska. Unesite ponovo!\n");
	} while(checkShouldIRun);
	return 0;
}
