#include "TickyGlobal.h"

int main() {

	FILE* stream;

	int numberOfUsers = 0;
	struct User* users = getUsers(&numberOfUsers);

	if(users != NULL) {

		for(int i = 0; i < numberOfUsers; i++)
			printf("[%d] %s\n", i + 1, users[i].accName);

		int index;
		do {

			printf("Unesite korisnika: ");
			char character[30] = { '\0' };
			modifyCharacter(character, 30, stdin);

			sscanf(character, "%d", &index);

		} while(index > numberOfUsers || index < 0);

		index--;

		int shouldIRun = 1;
		double accBalance = 0;

		do {
			printf("Unesite koliko zelite novca uplatiti: ");
			char character[30] = { '\0' };
			modifyCharacter(character, 30, stdin);

			sscanf(character, "%lf", &accBalance);
			if(accBalance > 0) shouldIRun = 0;
		} while(shouldIRun);

		users[index].accBalance = accBalance;
		writeUsers(users, numberOfUsers);
	}

	free(users);
}
