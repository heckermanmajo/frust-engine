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
    Game *g = menu->game;
    GameModification *mod = g->mod;
    Scenario *scenario = &(g->current_scenario);

    printf("Set scenario to game modification defaults\n");

    /// populate all tiles of the campaign map
    {

        const int tiles_on_x = 10;
        const int tiles_on_y = 10;

        g->camp.tile_size = 128;
        g->camp.tiles_on_x = tiles_on_x;
        g->camp.tiles_on_y = tiles_on_y;

        g->camp.selected_tile = NULL;

        g->camp.camera2D.target = (Vector2){ 0, 0 };
        g->camp.camera2D.offset = (Vector2){ g->screen_x/2.0f, g->screen_y/2.0f };
        g->camp.camera2D.rotation = 0.0f;
        g->camp.camera2D.zoom = 1.0f;

        g->camp.arenaOfTile = *ArenaOfTile_new();

        ArenaOfTile* arena = &(g->camp.arenaOfTile);

        ArenaOfTile_set_all_dead(arena);

        for (int x=0;x<10;x++){
            for (int y=0;y<10;y++){

                int arena_index_by_xy = tiles_on_y * x + y;
                Tile* t = ArenaOfTile_get_first_dead_one_and_set_back_to_live(arena);

                assert(t->index == arena_index_by_xy);

                /////////////////////////////////////////////////////////////////////////
                ////// Initialize the tile, so it not full of shit from previous games
                ///////////////////////////////////////////////////////////////////////////

                t->x = x;
                t->y = y;

                t->is_passable = true;
                t->owner = NULL;
                t->army = NULL;
                t->level = 0;
                t->type = TileType_GRAS;

            }
        }

    }

    // now we can set all values for the scenario
    // create the tiles etc.

    // wipe all tiles -> tile - arena
    // fill them with the data from an image

    // for this we need to have a chunk arena

}