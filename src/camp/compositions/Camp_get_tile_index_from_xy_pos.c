
/**
 * This function returns the index, you can use to access a tile from the tile arena.
 *
 * @param camp
 * @param y_number_pixel
 * @param x_number_pixel
 * @return The index you can use for camp->arenaOfTile functions.
 *
 */
int Camp_get_tile_index_from_xy_pos(Camp *camp, int x_number_pixel, int y_number_pixel) {

  int world_x = camp->tiles_on_x * camp->tile_size;
  int world_y = camp->tiles_on_y * camp->tile_size;

  int out_of_world =
    x_number_pixel > world_x - 1
    || y_number_pixel > world_y - 1
    || x_number_pixel < 0
    || y_number_pixel < 0;

  if (out_of_world) return -1;

  int x_index_of_tile = (int) floorf((float) x_number_pixel / (float) camp->tile_size);
  int y_index_of_tile = (int) floorf((float) y_number_pixel / (float) camp->tile_size);
  int index = x_index_of_tile * camp->tiles_on_y + y_index_of_tile;

  return index;

}