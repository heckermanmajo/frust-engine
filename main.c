// load all the standard stuff
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

// load all shared external libs
#include "src/share/raylib.h"

#define RAYGUI_IMPLEMENTATION

#include "src/share/raygui.h"



// my libs
#include "src/share/arena.c"

// load all data types of the engine
#include "src/types.h"

// load all utils
// ...

// load all game functions
// ...

// load all mods -> need to be loaded before the mod selection function in the menu
#include "src/mods/mages/MAGES_init.c"
#include "src/mods/lerman/LERMAN_init.c"

// load all menu functions
#include "src/menu/Menu_populate_new_scenario_from_mod.c"
#include "src/menu/Menu_draw.c"


// load all camp functions
// ...
#include "src/camp/Camp_draw.c"
#include "src/camp/Camp_update.c"

// load all battle functions
// ...



Game *Game_init() {

  Game *game = malloc(sizeof(Game));

  game->game_mode = GameMode_MENU;
  game->menu.mode = MenuMode_MOD_SELECTION;
  game->menu.game = game;

  game->run_program = true;
  game->battle.game = game;
  game->camp.game = game;
  game->screen_x = 1900; // TODO: THIS SEEMS NOT TO WORK GetScreenWidth();                           // Get specified monitor width (current video mode used by monitor)
  game->screen_y = 1200;//GetScreenHeight();

  // the rest of the state of game and camp is
  // set when a camp or battle is started
  return game;

}

void Game_update_frame(Game *self, float dt) {

  switch (self->game_mode) {

    case GameMode_MENU:

      break;

    case GameMode_CAMP:
      Camp_update(&self->camp, dt);
      break;

    case GameMode_BATTLE:
      break;

  }

}

void Game_draw(Game *self) {

  switch (self->game_mode) {

    case GameMode_MENU:
      Menu_draw(&self->menu);
      break;

    case GameMode_CAMP:
      Camp_draw(&self->camp);
      break;

    case GameMode_BATTLE:
      break;

  }

}

int main(void) {

  SetTraceLogLevel(LOG_ERROR);

  Game *game = Game_init();


  InitWindow(game->screen_x, game->screen_y, "Frust-Engine");
  //GuiLoadStyle("./dark.rgs");
  //Image i = LoadImage("./res/blue_marble.png");
  //Texture2D texture = LoadTextureFromImage(i);

  SetWindowMonitor(0);

  // Vector2 GetScreenToWorld2D(Vector2 position, Camera2D camera); // Get the world space position for a 2d camera screen space position
  Camera2D camera = {0};

  //GuiLoadStyle("./dark.rgs");
  char fps[100];

  while (!WindowShouldClose()) {

    sprintf((char *) &fps, "%d fps", GetFPS());

    // game logic here
    Game_update_frame(game, GetFrameTime());


    BeginDrawing();
    ClearBackground((Color) {0, 108, 30, 255});

    // move this into campauign and rts battle
    //BeginMode2D(camera);

    //EndMode2D();

    //UnitContainer_draw_all_units(unit_container, texture);

    Game_draw(game);

    if (IsKeyDown(KEY_ESCAPE)) {
      break;
    }


    DrawText(fps, 10, 10, 20, LIGHTGRAY);

    //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
    //DrawTexture(texture, SCREEN_WITDH/2 - texture.width/2, SCREEN_HEIGHT/2 - texture.height/2, WHITE);
    EndDrawing();

    if (!game->run_program) {
      break;
    }
  }

  CloseWindow();

  return 0;
}