/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */


#include "GameManager.h"


int main(int argc, char** argv) {
	
	if (argc > 1) {
		printf("Parameters: %i\n", argc);
		for (int i = 1; i < argc; i++) {
			printf("Parameter %i: %s\n", i, argv[i]);
		}
	}

	GameManager* gameManager = new GameManager();
	gameManager->start();

	return 0;

}
