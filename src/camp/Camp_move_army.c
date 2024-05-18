// if a enemy if on the other tile
// attack

// if my army is on the other tile merge
// into the higher tech one

// if the other tile is not yet owned
// own it


/**
 * This is the central campaign logic.
 *
 *
 *
 * @param a
 * @param target
 */
void Camp_move_army(
  Army *a,
  Tile *target,
  Camp *camp
) {

  // if there is no owner, we just move there
  if (target->owner == NULL) {

    // MOVE TO THE TILE AND SET THE OWNER
    a->tile_i_am_on->army = NULL;
    target->army = a;
    // now this tile is owned by the owner to the tile
    target->owner = a->owner;

  }

  // if there is an owner, but it is the same as the
  // moving army, we merge the lower level army into the
  // higher level one.
  else if (target->owner == a->owner) {

    // SIMPLE MOVE TO TILE
    if (target->army == NULL) {

      a->tile_i_am_on->army = NULL;
      target->army = a;

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

      // increase the command points
      higher_tech_army->command_points += lower_tech_army->command_points;

      // set army to dead
      ArenaOfArmy_set_to_dead(&(camp->arenaOfArmy), lower_tech_army->index);

      // move there
      lower_tech_army->tile_i_am_on = NULL;
      higher_tech_army->tile_i_am_on = NULL;

      // set the higher army to the target tile
      target->army = higher_tech_army;

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
    }

  }

}