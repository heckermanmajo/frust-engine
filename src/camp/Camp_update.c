
float cool_down = 0;

void Camp_update(Camp *camp, float dt) {

  /**
   * Resolve al ai moves if we are in the resolving phase
   * If we are in the resolving phase, we jump to the
   * end of the function afterwards, so the user-input is not handled.
   */
  // TODO: if an army is destroyed before the move can happen, we are fucked
  //        THIS NEEDS TO BE FIXED ...
  //       so we need to check if all is valid, before we do the actual move ...
  // todo: we can not do anything in move army if we cannot do the move ...
  if (camp->faction_move_resolving_phase) {

    bool done_with_this_phase = camp->arenaOfArmyMovementDecisions.living_instances == 0;
    if (done_with_this_phase) {
      camp->faction_move_resolving_phase = false;
      goto END_OF_FUNCTION_Camp_update;
    }

    // roll up from the front
    int smallest_index = camp->arenaOfArmyMovementDecisions.smallest_index_of_dead_instance;
    ArmyMovementDecision *amd = ArenaOfArmyMovementDecision_get(&(camp->arenaOfArmyMovementDecisions), smallest_index);

    // this can set the mode to battle, so we can only handle one movement per function call
    Camp_move_army(amd->army, amd->tile, camp);

    // set this movement decision to dead
    ArenaOfArmyMovementDecision_set_to_dead(&(camp->arenaOfArmyMovementDecisions), smallest_index);

    // no interactions during the resolving of the ai faction movements
    goto END_OF_FUNCTION_Camp_update;
  }

  cool_down -= dt;
  bool key_cool_down_done = cool_down <= 0;

  if (camp->editor_mode_allowed && key_cool_down_done) {
    if (IsKeyDown(KEY_F11)) {
      cool_down = 0.25;
      camp->editor_mode_on = !camp->editor_mode_on;
    }
  }

  // Move over the campaign view with wasd
  if (IsKeyDown(KEY_A)) camp->camera2D.target.x -= 600 * dt;
  else if (IsKeyDown(KEY_D)) camp->camera2D.target.x += 600 * dt;
  else if (IsKeyDown(KEY_W)) camp->camera2D.target.y -= 600 * dt;
  else if (IsKeyDown(KEY_S)) camp->camera2D.target.y += 600 * dt;


  // scroll into the campaign with the mouse wheel
  {
    // never zoom out further than 0.5
    const float min_zoom = 0.5;

    // Zoom based on mouse wheel
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
      // Get the world point that is under the mouse
      Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camp->camera2D);

      // Set the offset to where the mouse is
      camp->camera2D.offset = GetMousePosition();

      // Set the target to match, so that the camera maps the world space point
      // under the cursor to the screen space point under the cursor at any zoom
      camp->camera2D.target = mouseWorldPos;

      // Zoom increment
      const float zoomIncrement = 0.125f;

      camp->camera2D.zoom += (wheel * zoomIncrement);
      if (camp->camera2D.zoom < min_zoom) camp->camera2D.zoom = min_zoom;
    }
  } // end scroll


  // select a tile with the mouse
  {

    Vector2 mousePosition = GetMousePosition();

    bool outside_top_bar = mousePosition.y > camp->top_bar_height;
    bool outside_right_bar = !camp->selected_tile || mousePosition.x < camp->game->screen_x - camp->right_bar_width;

    // select the tile, if we have clicked on one
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && outside_top_bar && outside_right_bar) {

      int max_x_position_of_world = camp->tiles_on_x * camp->tile_size;
      int max_y_position_of_world = camp->tiles_on_y * camp->tile_size;

      Vector2 real_world_position = GetScreenToWorld2D(mousePosition, camp->camera2D);

      int x_is_within_the_world = (
        real_world_position.x > 0 &&
        real_world_position.x < (float) max_x_position_of_world
      );

      int y_is_within_the_world = (
        real_world_position.y > 0 &&
        real_world_position.y < (float) max_y_position_of_world
      );

      if (x_is_within_the_world && y_is_within_the_world) {

        int x_index_of_tile = floor(real_world_position.x / camp->tile_size);
        int y_index_of_tile = floor(real_world_position.y / camp->tile_size);

        int index = x_index_of_tile * camp->tiles_on_y + y_index_of_tile;

        Tile *t = ArenaOfTile_get(&(camp->arenaOfTile), index);

        assert(t != NULL);
        assert(t->alive);

        camp->selected_tile = t;

      }


    }

      // no use for middle mouse button yet
    else if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) {

    }

      // deselect the tile
    else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
      camp->selected_tile = NULL;
    }

  } // end select tile with mouse



  // move player army input key handle
  {

    // if a tile is selected, and it has a army, we can move this army
    // but only if the army is not moved yet

    if (camp->selected_tile != NULL) {

      Faction *player_faction = &(camp->game->mod->factions[0]);
      if (camp->selected_tile->owner == player_faction) {

        int has_army = camp->selected_tile->army != NULL;
        if (has_army) {

          int can_move_this_turn = !camp->selected_tile->army->movement_this_turn;
          if (can_move_this_turn) {

            int x_where_tile_would_be = -1;
            int y_where_tile_would_be = -1;

            if (IsKeyDown(KEY_UP)) {

              x_where_tile_would_be = camp->selected_tile->x * camp->tile_size;
              y_where_tile_would_be = camp->selected_tile->y * camp->tile_size - camp->tile_size;

            } else if (IsKeyDown(KEY_DOWN)) {

              x_where_tile_would_be = camp->selected_tile->x * camp->tile_size;
              y_where_tile_would_be = camp->selected_tile->y * camp->tile_size + camp->tile_size;

            } else if (IsKeyDown(KEY_LEFT)) {

              x_where_tile_would_be = camp->selected_tile->x * camp->tile_size - camp->tile_size;
              y_where_tile_would_be = camp->selected_tile->y * camp->tile_size;

            } else if (IsKeyDown(KEY_RIGHT)) {

              x_where_tile_would_be = camp->selected_tile->x * camp->tile_size + camp->tile_size;
              y_where_tile_would_be = camp->selected_tile->y * camp->tile_size;

            }

            if(x_where_tile_would_be != -1){

              int index = Camp_get_tile_index_from_xy_pos(
                camp,
                x_where_tile_would_be,
                y_where_tile_would_be
              );

              int is_valid = index != -1;
              if (is_valid) {
                Tile *target_tile = ArenaOfTile_get(&(camp->arenaOfTile), index);
                Army* army = camp->selected_tile->army;
                bool success = Camp_move_army(camp->selected_tile->army,target_tile, camp);
                // army can move away from the selected tile ...
                if (success){
                  army->movement_this_turn = true;
                }
              }

            }

          }

        }

      }

    }

  } // end move player army input key handle


  // label to jump here, if the AI player moves are handled
  END_OF_FUNCTION_Camp_update:


}