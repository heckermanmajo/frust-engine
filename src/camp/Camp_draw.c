

float Camp_draw_cool_down = 0;

void Camp_draw(Camp *camp, float dt) {

  Camp_draw_cool_down -= dt;

  BeginMode2D(camp->camera2D);

  //////////////////////////////////////////////////////////////////////////
  /// tiles
  //////////////////////////////////////////////////////////////////////////

  //DrawRectangle(100, 100, 100, 100, RED);
  //DrawRectangleGradientH(600/4*2 - 90, 170, 180, 130, MAROON, GOLD);
  //DrawRectangleLines(900/4*2 - 40, 320, 80, 60, ORANGE);


  for (int i = 0; i <= camp->arenaOfTile.largest_index_of_living_instance; i++) {

    int x_num = (int) floorf((float)i / (float)camp->tiles_on_y);
    int x_pixel = x_num * camp->tile_size;
    int y_num = i - (x_num * camp->tiles_on_x);
    int y_pixel = y_num * camp->tile_size;

    // todo bool is_in_viewport

    int x_index_of_tile = x_num;
    int y_index_of_tile = y_num;

    int index = x_index_of_tile * camp->tiles_on_y + y_index_of_tile;

    Tile *t = ArenaOfTile_get(&(camp->arenaOfTile), index);

    Texture2D texture;
    Texture2D *texture_level_overlay = NULL;
    // todo: get overlay based on level

    if (t->type == TileType_GRAS) {
      texture = camp->game->mod->flat_tile[t->number_of_texture_variance];
      //todo: texture_level_overlay = camp->game->mod->flat_tile_level_modification[0];
    } else if (t->type == TileType_FACTORY) {
      texture = camp->game->mod->factory_tile[t->number_of_texture_variance];
      //todo: texture_level_overlay = camp->game->mod->flat_tile_level_modification[0];
    } else if (t->type == TileType_MINERALS) {
      texture = camp->game->mod->minerals_tile[t->number_of_texture_variance];
      //todo: texture_level_overlay = camp->game->mod->flat_tile_level_modification[0];
    } else if (t->type == TileType_MOUNTAIN) {
      texture = camp->game->mod->mountain_tile[t->number_of_texture_variance];
      // no level modification for mountain, since they are unpassable
    } else if (t->type == TileType_WATER) {
      texture = camp->game->mod->water_tile; // no variance in water ...
      // no level modification for mountain, since they are unpassable
    } else {
      printf("BAD ERROR in tile drawing... Corruppted tile state...");
      assert(false);
    }

    DrawTexture(texture, x_pixel, y_pixel, WHITE);

    if (texture_level_overlay != NULL) {
      DrawTexture(*texture_level_overlay, x_pixel, y_pixel, WHITE);
    }



    // if the tile is owned by someone draw a overlay in the color of the
    // owning faction
    {
      if (t->owner != NULL) {

        // if the diplomacy view is enabled, draw the overlay based on the relation from
        // red to green and the formal relation as outline colors
        // set the player pespective as default
        Faction *faction_to_view_relations_from = &(camp->game->mod->factions[0]);
        if (camp->selected_tile && camp->selected_tile->owner) {
          faction_to_view_relations_from = camp->selected_tile->owner;
        }
        if (camp->diplomacy_relation_view) {

          // todo

        } else {

          Color color = t->owner->color;
          color.a = 30; // make transparent
          DrawRectangle(x_pixel, y_pixel, camp->tile_size, camp->tile_size, color);
          color.a = 255;
          DrawRectangleLines(x_pixel, y_pixel, camp->tile_size, camp->tile_size, color);
        }
      }
    }



    // draw an army, if there is one on the tile
    {
      if (t->army != NULL) {

        // we want one small square per army
        int pixels_per_square = 4;
        int space_between_squares = 2;
        int outer_space = 2;
        int num_squares_to_draw = t->army->command_points / 100;
        int num_squares_per_row = (int) ceil(sqrt(num_squares_to_draw));
        int draw_width = (
          num_squares_per_row * pixels_per_square
          + num_squares_per_row * space_between_squares
          + outer_space
        );
        int draw_height = draw_width;
        int half_a_tile = (int) ((camp->tile_size - draw_width) / 2);


        // draw a line around this rect for each army level
        int army_level = t->army->tech_level;
        for (int j = 0; j < army_level; j++) {
          int line_width = 2;
          int place_for_line = (4 * outer_space + line_width);

          // todo: apply white surrounding if already moved this turn only on player

          DrawRectangleLines(
            x_pixel + half_a_tile - (place_for_line / 2) * j,
            y_pixel + half_a_tile - (place_for_line / 2) * j,
            draw_width + place_for_line * j,
            draw_height + place_for_line * j,
            t->owner->color
          );
          DrawRectangleLines(
            x_pixel + half_a_tile - (place_for_line / 2) * j - 1,
            y_pixel + half_a_tile - (place_for_line / 2) * j - 1,
            draw_width + place_for_line * j + 2,
            draw_height + place_for_line * j + 2,
            t->owner->color
          );
        }

        int counter = 0;
        for (int y_row = 0; y_row < num_squares_per_row; y_row++) {
          for (int x_row = 0; x_row < num_squares_per_row; x_row++) {
            if (counter > num_squares_to_draw) {
              break;
            }

            DrawRectangle(
              x_pixel + half_a_tile + (x_row * (pixels_per_square + space_between_squares)) + space_between_squares,
              y_pixel + half_a_tile + (y_row * (pixels_per_square + space_between_squares)) + space_between_squares,
              pixels_per_square,
              pixels_per_square,
              t->army->owner->color
            );


            counter++;
          }
          if (counter > num_squares_to_draw) {
            break;
          }
        }


      }
    }

  }


  {
    bool tile_is_clicked = camp->selected_tile != NULL && camp->selected_tile->alive;
    if (tile_is_clicked) {

      // draw outline around the selected tile
      {
        int i = camp->selected_tile->index;

        int x_num = (int) floorf((float)i / (float)camp->tiles_on_y);
        int x_pixel = x_num * camp->tile_size;
        int y_num = i - (x_num * camp->tiles_on_x);
        int y_pixel = y_num * camp->tile_size;

        DrawRectangleLines(x_pixel, y_pixel, camp->tile_size, camp->tile_size, WHITE);
      }

    }
  }



  //////////////////////////////////////////////////////////////////////////
  /// army
  //////////////////////////////////////////////////////////////////////////

  EndMode2D();

  //////////////////////////////////////////////////////////////////////////
  /// ui
  //////////////////////////////////////////////////////////////////////////

  // one tile selected view

  const int top_bar_height = camp->top_bar_height;

  // draw the selected tile if one is selected
  {

    bool tile_is_clicked = camp->selected_tile != NULL && camp->selected_tile->alive;

    if (tile_is_clicked) {

      // draw the right side view with the tile info
      {

        int layout_start_x = camp->game->screen_x - 400;

        DrawRectangle(
          layout_start_x - 20,
          top_bar_height,
          420,
          camp->game->screen_y,
          GRAY
        );


        // tile header and tile position
        {
          DrawText("TILE SELECTED ", layout_start_x, 30 + top_bar_height, 20, BLACK);
          const char *x = TextFormat("X: %i", camp->selected_tile->x);
          DrawText(x, layout_start_x + 270, 30 + top_bar_height, 20, BLACK);
          const char *y = TextFormat("Y: %i", camp->selected_tile->y);
          DrawText(y, layout_start_x + 320, 30 + top_bar_height, 20, BLACK);
        }


        // draw type
        {
          const char *type_text;
          TileType tt = camp->selected_tile->type;

          if (tt == TileType_GRAS) type_text = "Grassland";
          else if (tt == TileType_FACTORY) type_text = "Industry";
          else if (tt == TileType_MINERALS) type_text = "Minerals";
          else if (tt == TileType_MOUNTAIN) type_text = "Mountain";
          else if (tt == TileType_WATER) type_text = "Water";
          else type_text = "UNKNOWN -> BAD ERROR";

          DrawText(type_text, layout_start_x, 60 + top_bar_height, 20, BLACK);
        }

        // draw is passable
        {
          int is_passable = camp->selected_tile->is_passable;
          const char *type_text;
          if (is_passable) {
            type_text = "Is passable";
          } else {
            type_text = "Is not passable";
          }
          DrawText(type_text, layout_start_x, 80 + top_bar_height, 20, BLACK);
        }

        // draw tile variation
        {
          DrawText(
            TextFormat("Texture-Variance: %d", camp->selected_tile->number_of_texture_variance),
            layout_start_x, 100 + top_bar_height, 20, BLACK
          );
        }

        // draw owner faction name
        {
          if (camp->selected_tile->owner != NULL) {
            DrawText(
              camp->selected_tile->owner->name,
              layout_start_x, 120 + top_bar_height, 20, camp->selected_tile->owner->color
            );
          }
        }

        // draw army, if there is one on this tile
        {
          if (camp->selected_tile->army != NULL) {
            DrawText(
              TextFormat(
                "Army: %d  - Level: %d",
                camp->selected_tile->army->command_points,
                camp->selected_tile->army->tech_level
              ),
              layout_start_x, 140 + top_bar_height, 20, BLACK
            );
          }
        }

        int is_owned_by_player;
        int contains_army;
        //int is_passable;
        int level;


        // button diplomacy hirtory
        {

          if (camp->selected_tile->owner != NULL) {
            bool not_the_player = camp->selected_tile->owner != &(camp->game->mod->factions[0]);
            if (not_the_player) {
              // display buttons for diplomacy
              // todo: handle button - clicks
              GuiButton((Rectangle) {(float)layout_start_x, 170 + (float)top_bar_height, 300, 40}, "Diplomacy-History");

              GuiButton((Rectangle) {(float)layout_start_x, 170 + 60 + (float)top_bar_height, 300, 40}, "Send 100 Command Points");

              GuiButton((Rectangle) {(float)layout_start_x, 170 + 120 + (float)top_bar_height, 300, 40}, "Declare War/Request Peace");

              GuiButton((Rectangle) {(float)layout_start_x, 170 + 180 +(float) top_bar_height, 300, 40}, "Request Puppet-State");
            } else {
              GuiButton((Rectangle) {(float)layout_start_x, 170 + (float)top_bar_height, 300, 40}, "Create Army/increase army size");

              GuiButton((Rectangle) {(float)layout_start_x, 170 + 60 + (float)top_bar_height, 300, 40}, "Upgrade army");

              GuiButton((Rectangle) {(float)layout_start_x, 170 + 120 + (float)top_bar_height, 300, 40}, "Upgrade Field");

              GuiButton((Rectangle) {(float)layout_start_x, 170 + 180 + (float)top_bar_height, 300, 40}, "Sell army");
            }

            //  todo add editor buttons
            {
              if (camp->editor_mode_on) {

              }
            }

          }
        }


        // infos about the tile

        // upgrade level of the tile

        // infos about the army on the tile ...

        // upgrade army button or create army button

      }


    }

  }

  // -> if army on tile, also army view

  // status view: my cmd points, income, toggle diplomacy view

  {

    DrawRectangle(0, 0, camp->game->screen_x, top_bar_height, GRAY);

    bool next_round_button_pressed = GuiButton(
      (Rectangle) {10, 10, 100, (float) top_bar_height - 20},
      TextFormat(" (%d) Next Round + ", camp->current_round_num)
    );

    if (next_round_button_pressed && Camp_draw_cool_down < 0) {
      Camp_draw_cool_down = 0.2f;
      Camp_progress_to_next_round(camp);

      SetMousePosition(camp->game->screen_x / 2, camp->game->screen_y / 2);
    }

    GuiButton((Rectangle) {130, 10, 100, (float) top_bar_height - 20}, "Toggle Diplomacy View");

    GuiButton((Rectangle) {(float) camp->game->screen_x - 120, 10, 100, (float) top_bar_height - 20}, "MENU");

    DrawText("2134 $ Income +123 $/round", 250, 15, 30, BLACK);

  }


  // display the tile editor view
  {
    if (camp->editor_mode_on) {

    }
  }


  {

    if (camp->editor_mode_allowed) {
      if (camp->editor_mode_on) {
        DrawText("EDITOR MODE: (ACTIVE) ", 10, camp->game->screen_y - 250, 20, WHITE);
      } else {
        DrawText("EDITOR MODE: (NON-ACTIVE) ", 10, camp->game->screen_y - 250, 20, WHITE);
      }
    } else {
      DrawText("CAMPAIGN MODE ", 10, camp->game->screen_y - 250, 20, WHITE);
    }

  }

}