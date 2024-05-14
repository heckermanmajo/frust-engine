
// todo: this path need to be updated later if we move the
//       executable into the main game folder ...

#define LOAD_TEXTURE(mod_field, filename)\
snprintf(image_path_buffer, 200, "../src/mods/mages/res/%s.png", filename ); \
mod->mod_field = LoadTexture(image_path_buffer);


GameModification *MAGES_init() {

  GameModification *mod = malloc(sizeof(GameModification));

  // load all the tile images ...
  {
    char *image_path_buffer = malloc(sizeof(char) * 200);

    LOAD_TEXTURE(flat_tile[0], "flat_0")
    LOAD_TEXTURE(flat_tile[1], "flat_1")
    LOAD_TEXTURE(flat_tile[2], "flat_2")
    LOAD_TEXTURE(flat_tile[3], "flat_3")

    LOAD_TEXTURE(factory_tile[0], "factory_0")
    LOAD_TEXTURE(factory_tile[1], "factory_1")
    LOAD_TEXTURE(factory_tile[2], "factory_2")
    LOAD_TEXTURE(factory_tile[3], "factory_3")

    LOAD_TEXTURE(minerals_tile[0], "minerals_0")
    LOAD_TEXTURE(minerals_tile[1], "minerals_1")
    LOAD_TEXTURE(minerals_tile[2], "minerals_2")
    LOAD_TEXTURE(minerals_tile[3], "minerals_3")

    LOAD_TEXTURE(mountain_tile[0], "mountain_0")
    LOAD_TEXTURE(mountain_tile[1], "mountain_1")
    LOAD_TEXTURE(mountain_tile[2], "mountain_2")
    LOAD_TEXTURE(mountain_tile[3], "mountain_3")

    // todo: load the level modifications for the tile

    LOAD_TEXTURE(water_tile, "water")

    free(image_path_buffer);
  }

  const int SELEUKIDEN = 0;

  // create all cultures...
  {

    mod->cultures[SELEUKIDEN].name = "SELEUKIDEN";
    mod->cultures[SELEUKIDEN].desc = "Some guys with elephants in the east.";
    //mod->cultures[SELEUKIDEN].unit_types[0]->

    // set all factions to dead and empty
    for(int i = 0; i < FACTIONS_MAX_NUM; i++){
      Faction* faction = &(mod->factions[i]);
      faction->alive = 0;
      faction->culture = NULL;
      faction->player_controlled = false;
      faction->index = i;
    }

    mod->factions[0].alive = 1;
    mod->factions[0].player_controlled = true;
    mod->factions[0].color = (Color){ 0, 0, 255, 255 };
    mod->factions[0].name = "You";

    mod->factions[1].alive = 1;
    mod->factions[1].player_controlled = false;
    mod->factions[1].color = (Color){ 255, 0, 0, 255 };
    mod->factions[1].name = "The Keks";


  }



  return mod;

}

#undef LOAD_TEXTURE