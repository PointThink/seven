#include "MainMenu.hpp"

#include <stdlib.h>
#include <raylib.h>
#include <utility>
#include <iostream>

#include "Global.hpp"
#include "GameState/InGame.hpp"
#include "GameState/LevelEditor.hpp"
#include "GameState/GameState.hpp"
#include "SoundManager.hpp"

static void PlayLevel(std::string path)
{
	InGameState* state = new InGameState;
	state->world.LoadFromFile(path);
	GameStateManager::SetState(state);
}

static void EditLevel(std::string path)
{
	LevelEditorState* state = new LevelEditorState;
	state->world.LoadFromFile(path);
	GameStateManager::SetState(state);
}

MainMenuButton::MainMenuButton(std::string text, std::function<void(MainMenu*)> onClicked)
{
	this->text = text;
	this->onClicked = onClicked;
}

MainMenu::MainMenu()
{
	buttons[MENU_MAIN].push_back(MainMenuButton("Play", [](MainMenu* menu) { menu->state = MENU_LEVEL_SELECT; }));
	// buttons[MENU_MAIN].push_back(MainMenuButton("Level Editor", [](MainMenu* menu) { menu->state = MENU_LEVEL_EDITOR_SELECT; }));
	buttons[MENU_MAIN].push_back(MainMenuButton("Quit", [](MainMenu* menu) { exit(0); }));

	std::pair<std::string, std::string> levels[] = {
		{"House", "assets/levels/house.lvl"},
		{"Apartments", "assets/levels/apartments.lvl"},
		{"Office", "assets/levels/office.lvl"},
		{"Warehouse", "assets/levels/Warehouse.lvl"}
	};

	for (std::pair<std::string, std::string> level : levels)
	{
		buttons[MENU_LEVEL_SELECT].push_back(
			MainMenuButton(level.first, [level](MainMenu* menu) {
				PlayLevel(level.second);
			})
		);
	}

	buttons[MENU_LEVEL_SELECT].push_back(
		MainMenuButton("Back", [](MainMenu* menu) { menu->state = MENU_MAIN; })
	);

	for (std::pair<std::string, std::string> level : levels)
	{
		buttons[MENU_LEVEL_EDITOR_SELECT].push_back(
			MainMenuButton(level.first, [level](MainMenu* menu) {
				EditLevel(level.second);
			})
		);
	}

	buttons[MENU_LEVEL_EDITOR_SELECT].push_back(
		MainMenuButton("Back", [](MainMenu* menu) { menu->state = MENU_MAIN; })
	);
}

MainMenu::~MainMenu()
{
}

void MainMenu::Draw()
{
	ClearBackground(BLACK);

	float textOffset = 40;

	float boxSizeX = 0;
	float boxSizeY = 0;

	for (MainMenuButton button : buttons[state])
	{
		Vector2 size = MeasureTextEx(font, button.text.c_str(), 60, 0);

		if (size.x > boxSizeX)
			boxSizeX = size.x;

		boxSizeY += textOffset;
	}

	float boxPosX = GetRenderWidth() / 2 - boxSizeX / 2;
	float boxPosY = GetRenderHeight() / 2  - boxSizeY / 2;

	// DrawRectangle(boxPosX, boxPosY, boxSizeX, boxSizeY, GRAY);

	bool hovered = false;

	float drawOffset = 0;
	int index = 0;
	for (MainMenuButton button : buttons[state])
	{
		std::string buttonText = button.text;

		Vector2 textSize = MeasureTextEx(font, buttonText.c_str(), 60, 0);
		Vector2 mousePosition = GetMousePosition();

		float textPosX = boxPosX + boxSizeX / 2 - textSize.x / 2;
		float textPosY = boxPosY + drawOffset;

		if (
			mousePosition.x > textPosY && mousePosition.y > textPosY &&
			mousePosition.x < textPosX + textSize.x && mousePosition.y < textPosY + textOffset
		)
		{
			hovered = true;

			buttonText = "> " + buttonText + " <";
			Vector2 boldTextSize = MeasureTextEx(fontBold, buttonText.c_str(), 60, 0);
			float boldTextPosX = boxPosX + boxSizeX / 2 - boldTextSize.x / 2;
			float boldTextPosY = boxPosY + drawOffset;

			DrawTextEx(fontBold, buttonText.c_str(), {boldTextPosX, boldTextPosY}, 60, 0, {167, 153, 255, 255});

			if (currentHovered != index)
			{
				currentHovered = index;
				SoundManager::Play("hover");
			}

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				button.onClicked(this);
				SoundManager::Play("click");
			}
		}
		else
			DrawTextEx(font, buttonText.c_str(), {textPosX, textPosY}, 60, 0, WHITE);
		
		drawOffset += textOffset;
		index++;
	}

	if (!hovered)
		currentHovered = -1;
}

void MainMenu::Update()
{

}