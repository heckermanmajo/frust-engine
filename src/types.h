enum GameMode;
typedef enum GameMode GameMode;

enum MenuMode;
typedef enum MenuMode MenuMode;

struct GameModification;
typedef struct GameModification GameModification;

struct Menu;
typedef struct Menu Menu;

struct Camp;
typedef struct Camp Camp;

struct Battle;
typedef struct Battle Battle;

struct Scenario;
typedef struct Scenario Scenario;

struct Game;
typedef struct Game Game;

/// CAMP

struct Culture;
typedef struct Culture Culture;

struct Faction;
typedef struct Faction Faction;

struct RelationEvent;
typedef struct RelationEvent RelationEvent;

struct Army;
typedef struct Army Army;

struct Tile;
typedef struct Tile Tile;

//// SHARED

struct BattleConfig;
typedef struct BattleConfig BattleConfig;

struct BattleResult;
typedef struct BattleResult BattleResult;

struct Effect;
typedef struct Effect Effect;

/// BATTLE

struct Chunk;
typedef struct Chunk Chunk;

struct ChunkArena;
typedef struct ChunkArena ChunkArena;

struct Unit;
typedef struct Unit Unit;

struct UnitType;
typedef struct UnitType UnitType;

struct UnitTypeShield;
typedef struct UnitTypeShield UnitTypeShield;

struct UnitTypeWeapon;
typedef struct UnitTypeWeapon UnitTypeWeapon;

struct UnitTypeWeaponProjectileType;
typedef struct UnitTypeWeaponProjectileType UnitTypeWeaponProjectileType;

struct Projectile;
typedef struct Projectile Projectile;

struct CommandGroup;
typedef struct CommandGroup CommandGroup;

struct UnitTypeArmor;
typedef struct UnitTypeArmor UnitTypeArmor;

enum GameMode {
    GameMode_MENU,
    GameMode_CAMP,
    GameMode_BATTLE,
};

enum MenuMode {
    MenuMode_MOD_SELECTION,
    MenuMode_MAIN_MENU,
    MenuMode_SCENARIO_CONFIG,
    MenuMode_LOAD_SCENARIO_VIEW,
    MenuMode_CUSTOM_BATTLE_VIEW
};


struct Menu {
    Game *game;
    MenuMode mode;
    Scenario *current_scenario;
};



struct Battle {
    Game *game;
    Camera2D camera2D;
};

struct Scenario {
    Game *game;
    bool loaded;
    /**
     * If the player has started playing
     * this scenario, this is true.
     * This helps to discriminate against
     * checking scenarios out in the configuration
     * view-
     */
    bool played;

    // list of factions
    // diplomacy-preconditions

};





/// CAMP
struct Faction {
    int alive;
    int index;
    bool player_controlled;
    Color color;
    Culture *culture;
    const char *name;
};


/**
 * Relations from 100 to 0.
 *
 * At relation 30 there is a chance for war.
 * A relation event of war can be canceled with a relation
 * event of peace. The worse the relation, the smaller the
 * chance that the faction will accept.
 */
struct RelationEvent {
    int index;
    int alive;
    int rounds_effect;
    int can_be_broken_in_n_rounds;
    int effect_on_relation;
    char *description;
    Faction *faction_1;
    Faction *faction_2;
};

struct Army {
    int alive;
    int index;
    int command_points;
    Faction *owner;
    /**
     * Each level will allow you to use
     * the next 4 units of your faction.
     * An army with the level 5 can use all
     * units of that faction.
     */
    int tech_level;
};

typedef enum  {
    TileType_GRAS,
    TileType_MINERALS,
    TileType_FACTORY,
    TileType_MOUNTAIN,
    TileType_WATER
}TileType;

struct Tile {
    int alive;
    int index;
    TileType type;
    int x;
    int y;
    int level;
    bool is_passable;
    Army *army;
    Faction *owner;

    /**
     * Each tile type has 4 different base
     * textures. this number represents which one
     * is used for this tile.
     *
     * They have no effect, but allow for a little
     * nicer looking campaign card.
     *
     * They also get an overlay for the improvement
     * per level.
     *
     * The gras gets fortified, the industry gets more factories
     * the minerals more mines, etc.
     */
    int number_of_texture_variance;

    // todo: load the level modifications for the tile in mod
};

ARENA(RelationEvent, 100)
//ARENA(Army,  1000)
#define TILES_ON_X 10
#define TILES_ON_Y 10
ARENA(Tile, TILES_ON_X * TILES_ON_Y)
// --> camp


struct Camp {
    Game *game;
    Camera2D camera2D;
    ArenaOfTile arenaOfTile;
    int tile_size;
    int tiles_on_x;
    int tiles_on_y;
    Tile* selected_tile;
};


//// SHARED

struct BattleConfig {
    // up to 6 armies can meet on the battle field
    // player -> blue, enemy red, ally yellow,
    // for this reason we have the "flags"
    Army armies[6];

};

struct BattleResult {

};

struct Effect {

};

/// BATTLE

struct Chunk {
    int index;
    int alive;
    int x;
    int y;
    Unit **units;
    Projectile **projectiles;
    bool checkpoint;
    Faction *owner;
};

struct Unit {
    int index;
    int alive;
    int x;
    int y;
    int hp;
};

struct Projectile {

};

enum CommandGroupMode{
    CommandGroupMode_IDLE,
    CommandGroupMode_SEARCHING,
    CommandGroupMode_ATTACKING,
    CommandGroupMode_GO_TO_TARGET
};

struct CommandGroup {
    Faction *faction;
    Unit **units;
    int number_of_units;
    int x_target, y_target;

};

// Arena(Chunks)
// Arena(Units)
// Arena(Projectiles)
// Arena(Effects)
// --> battle

struct UnitType {
    const char *name;
    const char *desc;
    int base_hp;
    int base_defense;
    int base_moral;
    int base_costs;
    int base_command_group_size;
    int base_command_group_number_per_spawn;
    Culture *culture;
    UnitTypeShield *shield;
    UnitTypeWeapon *weapon;
    UnitTypeArmor *armor;
    int width;
    int height;
    int speed;
    // basically turrets ot walls.
    bool stationary;
    bool can_burn;

    // in a real time battle 4 armies can meet at once

    // all command groups have some flags in the color of the army
    // one red one yellow, one green, one blue
    // the flag indicates the faction
    // the first 3 are an overlay for all units
    // the next 3 are not doe all unist built all 3 of a command group or so
    Texture2D base_flag_textures[6];
    // 4 different base textures for some variance
    Texture2D base_textures[4];
    Texture2D dead_sprites[5];
    /**
     * If humanoid units "walk", we can update them
     * moving and collision wise each "step", instead
     * of each frame.
     *
     * -> This however does not work for tanks, ships or flying
     * units.
     */
    bool walking;
};

struct Culture {
    UnitType unit_types[20];
    const char *name;
    const char *desc;
};

struct UnitTypeShield {

};

struct UnitTypeWeapon {
    UnitTypeWeaponProjectileType *projectileType;
    bool ranged;
    int damage;
    Texture2D texture;
};

struct UnitTypeArmor {
    int defense;
    Texture2D texture;
};

struct UnitTypeWeaponProjectileType {
    bool explodes;
    int explosion_radius;
    UnitTypeWeaponProjectileType *explosion_fragment;
    int speed;
    Texture2D texture;
};

#define CULTURE_MAX_NUM 20
#define FACTIONS_MAX_NUM 100

/**
 * Do we have orcs, landsknechts or ww2 soldiers?
 * -> loaded at the start of the game by the modview
 * into the game.
 *
 * basically all assets.
 */
struct GameModification {
    Culture cultures[CULTURE_MAX_NUM];
    Faction factions[FACTIONS_MAX_NUM];
    Texture2D flat_tile[4];
    Texture2D mountain_tile[4];
    Texture2D factory_tile[4];
    Texture2D minerals_tile[4];
    Texture2D flat_tile_level_modification[4]; // one per level
    Texture2D factory_tile_level_modification[4]; // one per level
    Texture2D minerals_tile_level_modification[4]; // one per level
    Texture2D water_tile;
    // todo: load the level modifications for the tile
    // button images
};


struct Game {
    GameMode game_mode;
    Menu menu;
    Camp camp;
    Battle battle;
    GameModification *mod;
    int screen_x;
    int screen_y;
    /**
     * Scenario lives in game.
     */
    Scenario current_scenario;

    /**
     * Setting this to false will end
     * the program.
     */
    bool run_program;
};




