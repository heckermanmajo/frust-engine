

void Camp_draw(Camp* camp){

    DrawText("Campaign mode: ", 10, 50, 20, GRAY);

    BeginMode2D(camp->camera2D);

    //////////////////////////////////////////////////////////////////////////
    /// tiles
    //////////////////////////////////////////////////////////////////////////

    //DrawRectangle(100, 100, 100, 100, RED);
    //DrawRectangleGradientH(600/4*2 - 90, 170, 180, 130, MAROON, GOLD);
    //DrawRectangleLines(900/4*2 - 40, 320, 80, 60, ORANGE);


    for (int i = 0; i <= camp->arenaOfTile.largest_index_of_living_instance; i++){

        int x_num = floor(i / camp->tiles_on_y);
        int x_pixel = x_num * camp->tile_size;
        int y_num =  i - (x_num * camp->tiles_on_x);
        int y_pixel = y_num * camp->tile_size;

        //bool is_in_viewport

        //DrawRectangle(x_pixel, y_pixel, 100, 100, BLUE);
        Texture2D texture = camp->game->mod->flat_tile[0];
        DrawTexture(texture, x_pixel, y_pixel, WHITE);

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

        if (camp->selected_tile != NULL && camp->selected_tile->alive) {

            // draw outline around the selected tile
            {
                int i = camp->selected_tile->index;

                int x_num = floor(i / camp->tiles_on_y);
                int x_pixel = x_num * camp->tile_size;
                int y_num = i - (x_num * camp->tiles_on_x);
                int y_pixel = y_num * camp->tile_size;

                DrawRectangleLines(x_pixel, y_pixel, camp->tile_size, camp->tile_size, WHITE);
            }

            // draw the right side view with the tile info
            {

                // upgrade army button or create army button

                // infos about the tile

                // upgrade level of the tile

                // infos about the army on the tile ...

            }


        }

    }

    // -> if army on tile, also army view

    // status view: my cmd points, income

    // faction/diplomacy view

}