// if a enemy if on the other tile
// attack

// if my army is on the other tile merge
// into the higher tech one

// if the other tile is not yet owned
// own it


/**
 * This is the central campaign logic: Move a army to another field.
 *
 * @param a the army to move.
 * @param target the target to move to.
 * @param camp The campaign context object.
 *
 * @return true if the movement was success-full and consumed
 */
bool Camp_move_army(
  Army *a,
  Tile *target,
  Camp *camp
) {

  // todo: check if it can be done at all
  //       if the target is blocked, just do nothing....

  if(!target->is_passable){
    return false;
  }

  // if there is no owner, we just move there
  if (target->owner == NULL) {

    // MOVE TO THE TILE AND SET THE OWNER
    a->tile_i_am_on->army = NULL;
    target->army = a;
    // now this tile is owned by the owner to the tile
    target->owner = a->owner;
    a->tile_i_am_on = target;

  }

  // if there is an owner, but it is the same as the
  // moving army, we merge the lower level army into the
  // higher level one.
  else if (target->owner == a->owner) {

    // SIMPLE MOVE TO TILE
    if (target->army == NULL) {

      a->tile_i_am_on->army = NULL;
      target->army = a;
      a->tile_i_am_on = target;

    } else {

      // MERGE
      assert(a->owner == target->owner);
      assert(a->owner == target->army->owner);

      Army *lower_tech_army = a;
      Army *higher_tech_army = target->army;

      if (a->tech_level > target->army->tech_level) {
        lower_tech_army = target->army;
        higher_tech_army = a;
      }

      // increase the command points of the higher tech army
      higher_tech_army->command_points += lower_tech_army->command_points;

      // set army to dead
      ArenaOfArmy_set_to_dead(&(camp->arenaOfArmy), lower_tech_army->index);

      // move there (remove from the tiles)
      lower_tech_army->tile_i_am_on->army = NULL;
      higher_tech_army->tile_i_am_on->army = NULL;
      lower_tech_army->tile_i_am_on = NULL;
      higher_tech_army->tile_i_am_on = NULL;

      // set the higher army to the target tile
      target->army = higher_tech_army;
      higher_tech_army->tile_i_am_on = target;

    }

  // if the owner is a different one
  // we start a battle if we are at war
  // or we do nothing
  } else {

    // war
    // cannot be done, if we are not at war
    Faction *other_faction = target->owner;
    const char *state = Camp_get_relation_status(a->owner, target->owner, camp);

    if (EQUAL_STRING(state, "war")) {

      // todo: attack
      printf("NOT implemented yet -> start the campaign battle.\n");
      // todo: if both factions are not the player onces
      //       auto-calculate the battle

    } else {
      // do nothing ...
      // because we don't want to accidentally attack an ally or neutral
      printf("ignore move, because the other faction is not at war...");
      return false;
    }

  }

  return true;

}