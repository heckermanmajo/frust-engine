
/**
 * This function calculates all ai moves on the campaign map.
 *
 * This function can take long and this is fine ...
 *
 * @param camp
 */
void Camp_progress_to_next_round(Camp *camp){

  printf("NEXT ROUND\n");

  // ai moves -> into queue
  // todo:

  struct TileAiInformation{
    Tile* tile;

    int economic_value;
    // ignore for now, later this is a nice feature
    // if the ai on the campaign map blocks chokepoints
    bool is_chokepoint;
    bool contains_enemy_units;
    int distance_to_enemy_factory;
    int distance_to_my_factory;
    int distance_to_enemy_minerals;
    int distance_to_my_minerals;
    int biggest_neighbour_enemy_army_command_points;
    int biggest_neighbour_neutral_army_command_points;
  };

  struct TileAiInformation tile_ai_info[TILES_ON_X*TILES_ON_Y];

  // how to determine the ai moves?
  // chokepoints
  // defend minerals and factories
  // try tpo get minerals and factories
  // move away frm big armies or try to pick off weak armies


  // todo: we need a path finding algo ... -
  //       tiefensuche_example ...


  // todo: apply the targets for the ai
  //       initially we can just try to get the nearest factory



  // apply the income
  {
    for (int i = 0; i < FACTIONS_MAX_NUM; i++) {

      Faction *faction = &(camp->game->mod->factions[i]);

      int income = Camp_calculate_income(faction, camp);

      faction->command_points += income;

    }
  }


  // update diplomacy
  {
    // declare war, ask for peace, alliance, etc.
  }
}