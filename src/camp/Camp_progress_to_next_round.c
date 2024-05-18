

void Camp_progress_to_next_round(Camp *camp){

  printf("NEXT ROUND\n");

  // ai moves -> into queue
  // todo:

  struct TileAiInformation{
    Tile* tile;
    bool is_chokepoint;
    int economic_value;
    bool contains_enemy_units;
    int distance_to_enemy_factory;
    int distance_to_my_factory;
    int distance_to_enemy_minerals;
    int distance_to_my_minerals;
    int biggest_neighbour_enemy_army_command_points;
  };


  struct TileAiInformation tile_ai_info[TILES_ON_X*TILES_ON_Y];

  // how to determine the ai moves?
  // chokepoints
  // defend minerals and factories
  // try tpo get minerals and factories
  // move away frm big armies or try to pick off weak armies



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