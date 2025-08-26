#pragma once

#include <functional>
#include <string>

#include "GameState.hpp"

enum MainMenuState
{
	MENU_MAIN,
	MENU_LEVEL_SELECT,
	MENU_COUNT
};

struct MainMenu;

struct MainMenuButton
{
	std::string text;
	std::function<void(MainMenu*)> onClicked;

	MainMenuButton(std::string text, std::function<void(MainMenu*)> onClicked);
};

struct MainMenu : public GameState
{
	std::vector<MainMenuButton> buttons[MENU_COUNT];
	MainMenuState state = MENU_MAIN;

	MainMenu();

	void Draw() override;
	void Update() override;
};