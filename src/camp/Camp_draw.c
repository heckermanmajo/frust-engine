

void Camp_draw(Camp *camp) {

  DrawText("Campaign mode: ", 10, 50, 20, GRAY);

  BeginMode2D(camp->camera2D);

  //////////////////////////////////////////////////////////////////////////
  /// tiles
  //////////////////////////////////////////////////////////////////////////

  //DrawRectangle(100, 100, 100, 100, RED);
  //DrawRectangleGradientH(600/4*2 - 90, 170, 180, 130, MAROON, GOLD);
  //DrawRectangleLines(900/4*2 - 40, 320, 80, 60, ORANGE);


  for (int i = 0; i <= camp->arenaOfTile.largest_index_of_living_instance; i++) {

    int x_num = floor(i / camp->tiles_on_y);
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

        Color color = t->owner->color;
        color.a = 30; // make transparent
        DrawRectangle(x_pixel, y_pixel, camp->tile_size, camp->tile_size, color);
        color.a = 255;
        DrawRectangleLines(x_pixel, y_pixel, camp->tile_size, camp->tile_size, color);

      }
    }

  }


  {
    bool tile_is_clicked = camp->selected_tile != NULL && camp->selected_tile->alive;
    if (tile_is_clicked) {

      // draw outline around the selected tile
      {
        int i = camp->selected_tile->index;

        int x_num = floor(i / camp->tiles_on_y);
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

  // draw the selected tile if one is selected
  {

    bool tile_is_clicked = camp->selected_tile != NULL && camp->selected_tile->alive;

    if (tile_is_clicked) {

      // draw the right side view with the tile info
      {

        int layout_start_x = camp->game->screen_x - 400;

        DrawRectangle(
          layout_start_x - 20,
          0,
          420,
          camp->game->screen_y,
          GRAY
        );


        // tile header and tile position
        {
          DrawText("TILE SELECTED ", layout_start_x, 30, 20, BLACK);
          const char *x = TextFormat("X: %i", camp->selected_tile->x);
          DrawText(x, layout_start_x + 270, 30, 20, BLACK);
          const char *y = TextFormat("Y: %i", camp->selected_tile->y);
          DrawText(y, layout_start_x + 320, 30, 20, BLACK);
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

          DrawText(type_text, layout_start_x, 60, 20, BLACK);
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
          DrawText(type_text, layout_start_x, 80, 20, BLACK);
        }

        // draw tile variation
        {
          DrawText(
            TextFormat("Texture-Variance: %d", camp->selected_tile->number_of_texture_variance),
            layout_start_x, 100, 20, BLACK
          );
        }

        // draw owner faction name
        {
          if(camp->selected_tile->owner != NULL){
            DrawText(
              camp->selected_tile->owner->name,
              layout_start_x, 120, 20, camp->selected_tile->owner->color
            );
          }
        }

        int is_owned_by_player;
        int contains_army;
        //int is_passable;
        int level;

        // infos about the tile

        // upgrade level of the tile

        // infos about the army on the tile ...

        // upgrade army button or create army button

      }


    }

  }

  // -> if army on tile, also army view

  // status view: my cmd points, income

  // faction/diplomacy view

}