
GameModification* MAGES_init(){

    GameModification * mod = malloc(sizeof (GameModification));

    // load all textures here ...
    Texture2D texture = LoadTexture("../res/res/grass.png");

    mod->flat_tile[0] = texture;

    return mod;
}