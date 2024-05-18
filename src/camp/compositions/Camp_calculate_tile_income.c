
/**
 * This function calculates how much income this tile
 * produces each round.
 *
 * @param tile
 * @return
 */
int Camp_calculate_tile_income(Tile *tile) {

  switch (tile->type) {

    case TileType_WATER:
    case TileType_MOUNTAIN:
      return 0;

    case TileType_GRAS:
      switch (tile->level) {
        case 0: return 5;
        case 1: return 10;
        case 2: return 15;
        case 3: return 20;
      }
      break;

    case TileType_FACTORY:
      switch (tile->level) {
        case 0: return 20;
        case 1: return 40;
        case 2: return 60;
        case 3: return 80;
      }
      break;

    case TileType_MINERALS:
      switch (tile->level) {
        case 0: return 40;
        case 1: return 80;
        case 2: return 120;
        case 3: return 160;
      }
      break;

  }

  assert(false);
}