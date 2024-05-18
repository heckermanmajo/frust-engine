void Camp_progress_to_next_round(Camp *camp){

  printf("NEXT ROUND\n");

  // ai moves -> into queue
  // todo:

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