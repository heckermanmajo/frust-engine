
/**
 * Calculate the income of a faction for the current
 * round.
 *
 * @param faction
 * @param camp
 * @return
 */
int Camp_calculate_income(Faction* faction, Camp* camp){

  int faction_income = 0;

  int default_value = 100;

  faction_income = default_value;

  for(int i = 0; i < camp->arenaOfTile.largest_index_of_living_instance; i++){

    Tile* t = ArenaOfTile_get(&(camp->arenaOfTile), i);

    if(t->owner == faction){

      int income_per_tile = Camp_calculate_tile_income(t);

      faction_income += income_per_tile;

    }

  }

  return faction_income;

}