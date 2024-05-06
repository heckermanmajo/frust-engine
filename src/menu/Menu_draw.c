
void Menu_draw(Menu *self) {

    switch (self->mode) {


        case MenuMode_MOD_SELECTION:

            DrawText("MOD-SELECTION: ", 10, 50, 20, GRAY);

            if (GuiButton((Rectangle) {12, 90, 100, 100}, "Mages")) {
                self->game->mod = MAGES_init();
                SetWindowTitle("Frust v0.1 - Mages");
                self->mode = MenuMode_MAIN_MENU;
            }

            if (GuiButton((Rectangle) {120, 90, 100, 100}, "Lerman")) {
                self->game->mod = LERMAN_init();
                SetWindowTitle("Frust v0.1 - Lerman");
                self->mode = MenuMode_MAIN_MENU;
            }

            break;


        case MenuMode_MAIN_MENU:

            if (GuiButton((Rectangle) {100, 100, 300, 50}, "New Scenario")) {

                // populate the scenario memory with the defaults from the game config
                // menu contains the prt to the game, which contains the mod config
                Menu_populate_new_scenario_from_mod(self);

                self->mode = MenuMode_SCENARIO_CONFIG;
            }

            if (GuiButton((Rectangle) {100, 200, 300, 50}, "Load")) {
                self->mode = MenuMode_LOAD_SCENARIO_VIEW;
            }

            if (GuiButton((Rectangle) {100, 300, 300, 50}, "Custom Battle")) {
                self->mode = MenuMode_CUSTOM_BATTLE_VIEW;
            }

            if (GuiButton((Rectangle) {100, 400, 300, 50}, "Exit")) {
                self->game->run_program = false;
            }

            break;


        case MenuMode_CUSTOM_BATTLE_VIEW:
            DrawText("Custom battle view", 100, 100, 100, WHITE);

            if (GuiButton((Rectangle) {100, 400, 300, 50}, " <= BACK")) {
                self->mode = MenuMode_MAIN_MENU;
            }

            break;


        case MenuMode_LOAD_SCENARIO_VIEW:
            DrawText("Load a scenario view", 100, 100, 100, WHITE);

            if (GuiButton((Rectangle) {100, 400, 300, 50}, " <= BACK")) {
                self->mode = MenuMode_MAIN_MENU;
            }

            break;


        case MenuMode_SCENARIO_CONFIG:
            DrawText("Configure a new scenario", 100, 100, 100, WHITE);

            if (GuiButton((Rectangle) {100, 300, 300, 50}, "START Scenario")) {
                // set game mode to camp
                self->game->game_mode = GameMode_CAMP;

            }

            if (GuiButton((Rectangle) {100, 400, 300, 50}, " <= BACK")) {
                self->mode = MenuMode_MAIN_MENU;
            }

            break;


    }
}