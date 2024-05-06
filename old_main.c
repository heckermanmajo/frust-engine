// "Jonathan Blow plays visual studio" 

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "src/share/raylib.h"

#include "src/menu/Menu.h"
#include "src/camp/Camp.h"
#include "src/battle/Battle.h"


// wrapper malloc and realloc adnd free to detect 
// memory leaks

typedef struct Unit Unit;


const int SCREEN_WITDH = 800;
const int SCREEN_HEIGHT = 450;


#define CRASH_IF(cond, message) if(cond) { printf("ERROR in %s   %d", __FILE__, __LINE__); printf(message); exit(1); }

//units (soldiers, tanks, ....)
//projectiles (arrows oder bullets)
//effects (feuer)

/**
 *
 */
typedef enum {
    SHIELD_NONE,
    SHIELD_WOODEN,
    SHIELD_TYPE_COUNT
} ShieldType;


typedef struct {
    ShieldType type;
    Texture2D *texture;
    int def;
    int relative_x;
    int relative_y;
    float rotation;
} UnitShield;

typedef struct {
    Texture2D *texture;
} UnitWeapon;

typedef struct {
    Texture2D *texture;
} UnitType;

typedef struct {
    UnitType **UnitType;
} Culture;

typedef struct {
    Culture *culture;
} Faction;


typedef struct {
    Culture *culture;
} BattleFaction;

struct Unit {
    int id;
    int x;
    int y;
    int width;
    int height;
    int alive;
    ShieldType shield;
};

struct Effect{
    int x,y;
};

typedef struct {
    Unit *unit_memory_block;
    int capacity;
    int unit_num;
    // last living is the end of any for loop that applies logic
    int last_living_unit;
    // first dead unit is used to create new units in the game
    int first_dead_unit;
} UnitContainer;


UnitContainer *UnitContainer_init(int number_of_units) {

    // set all to dead
    UnitContainer *uc = malloc(sizeof(UnitContainer));
    uc->unit_memory_block = malloc(sizeof(Unit) * number_of_units);

    for (int i = 0;i < number_of_units; i++){
        Unit u = uc->unit_memory_block[i];
        u.alive = false;
        u.x = 0;
        u.y = 0;
        u.shield = SHIELD_NONE;
        u.id = i; // the very important field
    }

    uc->unit_num = 0;
    uc->last_living_unit = 0;
    uc->first_dead_unit = 0;
    uc->capacity = number_of_units;

    return uc;
}


Unit *UnitContainer_newUnit(
        UnitContainer *self,
        int x,
        int y,
        int width,
        int height
) {

    Unit *u = &(self->unit_memory_block[self->first_dead_unit]);

    u->alive = true;
    u->height = height;
    u->width = width;
    u->x = x;
    u->y = y;

    // now we have lost the information about the first dead unit
    for (int i = self->first_dead_unit; i < self->capacity; i++) {

        Unit *potential_dead_unit = &(self->unit_memory_block[i]);

        if (!potential_dead_unit->alive) {
            self->first_dead_unit = i;

            // last unit has died... 
            if (self->first_dead_unit >= self->last_living_unit) {
                self->last_living_unit = self->first_dead_unit;
            }

            return u; // return the newly created unit
        }

    }


    // todo: maybe realloc the unit container?
    CRASH_IF(true, "Unit container is full, but you wanted to create another unit ...")

}


void UnitContainer_killUnit(UnitContainer* self, Unit* unit){
    
    int new_dead_id = unit->id;

    // if i was before the first dead one, update 
    // the first dead one index
    if (new_dead_id < self->first_dead_unit){
        self->first_dead_unit = new_dead_id;
    }

    // if this unit was the last living from the "left side"
    // we need to set the last living unit index to the next last one
    if (new_dead_id == self->last_living_unit){
        for (int i = self->last_living_unit; i >= 0;i--){
            Unit u = self->unit_memory_block[i];
            if (u.alive){
                self->last_living_unit = i;
                break;
            }
        }
    }

    unit->alive = false;

}


void UnitContainer_draw_all_units(UnitContainer *self, Texture2D texture) {
    //printf("%d",self->last_living_unit);
    for (int i = 0; i < self->last_living_unit; i++) {
        //printf("draw unit");
        Unit u = self->unit_memory_block[i];
        if (!u.alive) {
            continue;
        }
        //printf("draw unit");
        DrawTexture(texture, u.x - texture.width / 2, u.y - texture.height / 2, WHITE);
    }
}


typedef struct {
    Unit **units;
    unsigned int capacity;
    unsigned int number_of_units;
} UnitPtrList;


bool UnitPtrList_has(UnitPtrList *self, Unit *instance) {

    assert(instance != NULL);
    assert(self != NULL);

    for (int i = 0; i < self->number_of_units - 1; i++) {
        Unit *unit_ptr = self->units[i];

        if (unit_ptr == instance) {
            return true;
        }
    }

    return false;

}


void UnitPtrList_double_capacity(UnitPtrList *self) {

    int needed_size = self->capacity * 2;

    Unit **new_memory = (Unit **) realloc((void *) self->units, sizeof(Unit *) * needed_size);

    assert(new_memory == NULL);

    self->units = new_memory;

    self->capacity = needed_size;

}

// TODO: WE DONT NEED THE REFRAGMENTATION .... 
void UnitPtrList_remove_dead_units_and_order_list(UnitPtrList *self, Unit *unit_ptr){


    // remove all dead units from the end
    while(self->number_of_units != 0){
        Unit* last_unit = self->units[self->number_of_units -1];
        self->number_of_units -= 1;
        if(last_unit->alive){
            break;
        }
        self->units[self->number_of_units -1] = NULL;
    }


    // fill in the wholes, by swapping dead ones with alive ones
    for(int i = 0; i < self->number_of_units;i++){

      Unit* u = self->units[i];

      if (!u->alive){
        // we want to swap, since this one is dead

        Unit* last_unit = NULL;
        while(self->number_of_units != 0){
            last_unit = self->units[self->number_of_units -1];
            assert(last_unit != NULL);
            self->number_of_units -= 1;
            if(last_unit->alive){
                break;
            }
            self->units[self->number_of_units -1] = NULL;
        }

        // now we have one to swap or are at the end
        // or this list is empty 
        if(self->number_of_units == 0){
            return;
        }

        // swap
        self->units[i] = last_unit;


     }

   }
    
}


void UnitPtrList_add(UnitPtrList *self, Unit *unit_ptr) {

    assert(unit_ptr != NULL);
    assert(self != NULL);

    {
        bool is_full = self->capacity == self->number_of_units - 1;

        if (is_full) {
            UnitPtrList_double_capacity(self);
        }


        is_full = self->capacity == self->number_of_units - 1;
        CRASH_IF(is_full, "Doubling of array has failed")
    }

    // todo: check if we have this unit already in this list ...

    // add the new unit to this list
    {
        int index = self->number_of_units;
        self->units[index] = unit_ptr;
    }


}


typedef struct {
    int x;
    int y;
    Unit **units;
    int max_units_in_chunk;
    int units_in_chunk;
} Chunk;


Chunk **chunks;

//chunks[24][2];


//Chunk* newChunk(){

//}



UnitShield *get_shields() {

    static UnitShield shields[SHIELD_TYPE_COUNT];
    static bool initialized = false;

    if (initialized) {
        return shields;
    }

    shields[SHIELD_NONE] = (UnitShield) {
            .def = 0
    };

    shields[SHIELD_WOODEN] = (UnitShield) {
            .def = 123
    };

    initialized = true;
    return shields;

}

/*
typedef struct{
    ShieldType type;
}B;

B* get_shield(ShieldType s){
    for ()
      if _s. tyoe == s return 
}*/


typedef struct {
    int value;
} A;
A as[] = {
        {.value = 3},
        {.value = 4}
};

int *createArray(unsigned int length, int value) {
    int *array;
    array = (int *) malloc(length * sizeof(int));
    for (unsigned int i = 0; i < length; i++) {
        array[i] = value;
    }
    return array;
}


int main(void) {

    SetTraceLogLevel(LOG_ERROR);

    InitWindow(SCREEN_WITDH, SCREEN_HEIGHT, "raylib [core] example - basic window");

    Image i = LoadImage("./res/blue_marble.png");
    Texture2D texture = LoadTextureFromImage(i);


    UnitContainer *unit_container = UnitContainer_init(20000);

    Unit *u = UnitContainer_newUnit(
            unit_container,
            100,
            100,
            64,
            64
    );


    // Vector2 GetScreenToWorld2D(Vector2 position, Camera2D camera); // Get the world space position for a 2d camera screen space position
    Camera2D camera = {0};


    char fps[100];
    int counter = 0;
    float average_fps = 0;
    float fps_num = 0;

    while (!WindowShouldClose()) {

        float dt = GetFrameTime();
        SetTargetFPS(60);
        fps_num += (float) GetFPS();
        counter++;
        average_fps = fps_num / (float) counter;

        sprintf(&fps, "%d fps", GetFPS());
        //sprintf(&fps, "%f fps", average_fps);

        BeginDrawing();
        ClearBackground((Color) {0, 108, 30, 255});

        BeginMode2D(camera);

        EndMode2D();

        //UnitContainer_draw_all_units(unit_container, texture);


        DrawText(fps, 190, 200, 20, LIGHTGRAY);

        //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        //DrawTexture(texture, SCREEN_WITDH/2 - texture.width/2, SCREEN_HEIGHT/2 - texture.height/2, WHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}