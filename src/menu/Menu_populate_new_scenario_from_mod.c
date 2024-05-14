/**
 * This function populates the scenario memory
 * of the game state with a fresh set of data
 * effectively creating a new game.
 *
 * After the game state is set to camp, the
 * game logic executes on this new scenario memory
 * and the player plays a new game.
 *
 * If we want to add configuration made by the player
 * we can add theses as options to the menu.
 */
void Menu_populate_new_scenario_from_mod(
  Menu *menu
) {

  const int all_tiles = TILES_ON_X * TILES_ON_Y;

  Game *g = menu->game;
  GameModification *mod = g->mod;
  Scenario *scenario = &(g->current_scenario);

  printf("Set scenario to game modification defaults\n");

  /// populate all tiles of the campaign map
  {


    g->camp.tile_size = 128;
    g->camp.tiles_on_x = TILES_ON_X;
    g->camp.tiles_on_y = TILES_ON_Y;

    g->camp.selected_tile = NULL;

    g->camp.camera2D.target = (Vector2) {0, 0};
    g->camp.camera2D.offset = (Vector2) {0, 0};//(Vector2){ g->screen_x/2.0f, g->screen_y/2.0f };
    g->camp.camera2D.rotation = 0.0f;
    g->camp.camera2D.zoom = 1.0f;

    g->camp.arenaOfTile = *ArenaOfTile_new();

    ArenaOfTile *arena = &(g->camp.arenaOfTile);

    ArenaOfTile_set_all_dead(arena);

    // raylib
    SetRandomSeed(0xaabbccff);   // Set a custom random seed if desired, by default: "time(NULL)"

    for (int x = 0; x < TILES_ON_X; x++) {

      for (int y = 0; y < TILES_ON_Y; y++) {

        int arena_index_by_xy = TILES_ON_Y * x + y;
        Tile *t = ArenaOfTile_get_first_dead_one_and_set_back_to_live(arena);

        assert(t->index == arena_index_by_xy);

        /////////////////////////////////////////////////////////////////////////
        ////// Initialize the tile, so it not full of shit from previous games
        ///////////////////////////////////////////////////////////////////////////

        t->x = x;
        t->y = y;

        // todo: generate by random for now ...

        t->owner = NULL;
        t->army = NULL;
        t->level = 0;
        t->is_passable = true;

        // create a random map
        {
          int randValue = GetRandomValue(0, all_tiles);

          if (randValue < ((float)all_tiles / 100.0) * 3 ){
            t->is_passable = false;
            t->type = TileType_WATER;
          }
          else if(randValue < ((float)all_tiles / 100.0) * 10){
            t->is_passable = false;
            t->type = TileType_MOUNTAIN;
          }
          else if(randValue < ((float)all_tiles / 100.0) * 20) t->type = TileType_FACTORY;
          else if(randValue < ((float)all_tiles / 100.0) * 40) t->type = TileType_MINERALS;
          else t->type = TileType_GRAS;
        }

        t->number_of_texture_variance = GetRandomValue(0, 3);

      } // inner for loop, y

    } // outer for loop, x

  }


  // set the owner of some tiles to some factions
  {

    // todo place armies on random positions on the map

    const int needed_faction_placements_per_faction = 3;

    for (int i = 0; i < FACTIONS_MAX_NUM; i++){

      Faction *faction = &(mod->factions[i]);

      if(faction->alive){

        int attempts = 0;
        int successful_placements = 0;
        while(true){

          attempts++;

          if(attempts > all_tiles){
            // not exact but, exact enough...
            break;
          }

          if(successful_placements > needed_faction_placements_per_faction){
            break;
          }

          int random_tile_index = GetRandomValue(0, all_tiles-1);
          Tile *t = ArenaOfTile_get(&(g->camp.arenaOfTile), random_tile_index);

          if(t->owner != NULL) continue;
          if(!t->is_passable) continue;

          t->owner = faction;

          successful_placements++;

        } // end while(true)

      } // if alive

    } // for all factions

  }


  // now we can set all values for the scenario
  // create the tiles etc.

  // wipe all tiles -> tile - arena
  // fill them with the data from an image

  // for this we need to have a chunk arena

}