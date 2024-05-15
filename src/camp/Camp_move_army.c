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
 * @param a
 * @param target
 */
void Camp_move_army(
  Army *a,
  Tile *target
) {

  if (target->owner == NULL) {

  } else if (target->owner == a->owner) {

  } else {
    // ware
  }
}