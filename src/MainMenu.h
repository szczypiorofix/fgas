/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#pragma once

#include <SDL2/SDL_events.h>

class MainMenu {

public:
	MainMenu();
	~MainMenu();

	void update();
	void input(SDL_Event& e);
	void render();



};

