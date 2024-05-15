
/**
 * This function returns a number between 0 and 100 representing
 * the relation of the two passed in functions.
 *
 * The order of the factions does not matter.
 *
 * @param faction_1 one faction, needs to be alive and not null.
 * @param faction_2 one faction, needs to be alive and not null.
 * @param camp The camp struct.
 *
 * @return integer between 0 and 100
 */
int Camp_get_relation_number(
  Faction *faction_1,
  Faction *faction_2,
  Camp *camp
) {

  assert(faction_1 != NULL);
  assert(faction_2 != NULL);
  assert(camp != NULL);
  assert(faction_1->alive);
  assert(faction_2->alive);

  int baseline_of_relation = 50;
  int relation = baseline_of_relation;

  int largest_living_index = camp->arenaOfRelationEvents.largest_index_of_living_instance;

  for (int index = 0; index < largest_living_index; index++) {

    RelationEvent *relation_event
      = ArenaOfRelationEvent_get(&(camp->arenaOfRelationEvents), index);

    bool event_belongs_to_both_factions = (
      (
        relation_event->initiator_faction == faction_1
        && relation_event->other_faction == faction_2
      )
      ||
      (
        relation_event->initiator_faction == faction_2
        && relation_event->other_faction == faction_1
      )
    );

    if(event_belongs_to_both_factions){
      relation += relation_event->effect_on_relation;
    }

  }

  if(relation < 0 ) relation = 0;
  if(relation > 100) relation = 100;

  return relation;

}