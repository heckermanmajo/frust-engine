
/**
 * Returns the relation status between two factions.
 *
 * The status is NOT the relation as a number, but
 * "war", "peace", "neutrality", "defensive_alliance", "alliance", "puppet", "master".
 *
 * The order is important: the relation status is from the view
 * of faction 1: so if faction one is a puppet of faction two
 * this function will return "puppet", if 2 is a puppet of 1
 * this function will return "master".
 *
 * @param faction_1
 * @param faction_2
 * @param camp
 *
 * @return
 */
const char *Camp_get_relation_status(
  Faction *faction_1,
  Faction *faction_2,
  Camp *camp
) {

  assert(faction_1 != NULL);
  assert(faction_2 != NULL);
  assert(camp != NULL);
  assert(faction_1->alive);
  assert(faction_2->alive);

  RelationEvent *status_relation = NULL;

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

    if (event_belongs_to_both_factions) {

      if (relation_event->is_status_event) {

        // we check that there are not two conflicting status events
        // if this would be the case, we have a error
        assert(status_relation == NULL);

        status_relation = relation_event;

      }

    }

  }

  if (status_relation == NULL)return "neutral";

  switch (status_relation->type) {
    case RelationEventType_DELERATION_OF_WAR: return "war";
    case RelationEventType_DELERATION_OF_ALLIANCE: return "alliance";
    case RelationEventType_DELERATION_OF_PEACE: return "peace";
    case RelationEventType_DELERATION_OF_DEFENSIVE_ALLIANCE: return "defensive_alliance";
    case RelationEventType_DELERATION_OF_PUPPET: {
      if (status_relation->initiator_faction == faction_1) return "master";
      else return "puppet";
    }
    default:
      // bad type or this function does not handle an important case
      assert(false);

  }

}