#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Define a structure for ArenaOfT
typedef struct {
    int index;
    int alive;
    // add other fields here if needed
} Type;

#define ARENA(T, C)
typedef struct {
    Type* memory;
    int capacity;
    int smallest_index_of_dead_instance;
    int largest_index_of_living_instance;
    int living_instances;
} ArenaOf##Type##;

ArenaOf##Type##* ArenaOf##Type##_new() {
    int capacity = CAPACITY;
    ArenaOf##Type##* arena = (ArenaOf##Type##*)malloc(sizeof(ArenaOf##Type##));
    arena->memory = (Type*)malloc(sizeof(Type) * capacity);
    arena->capacity = capacity;
    arena->smallest_index_of_dead_instance = 0;
    arena->largest_index_of_living_instance = -1;
    arena->living_instances = 0;
    return arena;
}

Type* ArenaOf##Type##_get(ArenaOf##Type##* self, int index) {
    if (index < 0 || index >= self->capacity || index > self->largest_index_of_living_instance)
        return NULL;
    return &(self->memory[index]);
}

void ArenaOf##Type##_set_to_dead(ArenaOf##Type##* self, int index) {

    assert(index >= 0 && index < self->capacity);

    //printf("%d", self->largest_index_of_living_instance);
    assert(!(index > self->largest_index_of_living_instance));

    self->memory[index].alive = 0;

    if (index == self->largest_index_of_living_instance) {
        for (int i = index; i >= 0; i--) {
            if (self->memory[i].alive == 1) {
                self->largest_index_of_living_instance = i;
                break;
            }
        }
    }

    if (index < self->smallest_index_of_dead_instance){
        self->smallest_index_of_dead_instance = index;
    }

    self->living_instances--;

}

int ArenaOf##Type##_get_capacity(ArenaOf##Type##* self) {
    return self->capacity;
}

Type* ArenaOf##Type##_get_first_dead_one_and_set_back_to_live(ArenaOf##Type##* self) {
    int instance_index = self->smallest_index_of_dead_instance;
    Type* ret = &(self->memory[instance_index]);
    ret->alive = 1;
    ret->index = self->smallest_index_of_dead_instance;

    self->living_instances ++ ;

    if (instance_index > self->largest_index_of_living_instance) {
        self->largest_index_of_living_instance = instance_index;
    }

    if(instance_index == self->smallest_index_of_dead_instance){
        for (int i = self->smallest_index_of_dead_instance; i < self->capacity; i++) {
            if (self->memory[i].alive == 0) {
                self->smallest_index_of_dead_instance = i;
                break;
            }
        }
    }

    if (self->smallest_index_of_dead_instance == ret->index){
        self->smallest_index_of_dead_instance = self->capacity - 1;
    }

    return ret;
}

int ArenaOf##Type##_get_dead_instances(ArenaOf##Type##* self) {
    return self->capacity - self->living_instances;
}

int ArenaOf##Type##_is_full(ArenaOf##Type##* self) {
    return ArenaOf##Type##_get_dead_instances(self) == 0;
}

/**
 * this function checks if the arena is corrupted
 * is so an assertion is triggered.
 */
void ArenaOf##Type##_check_for_correctness(ArenaOf##Type##* self) {
    // check if all values are correct

    int real_smallest_dead_instance = -1;
    int real_biggest_living_instance = 0;
    int all_living_instances = 0;

    for(int i = 0; i < self->capacity; i++ ){
        Type e = self->memory[i];
        assert(e.index == i);
        if(e.alive){
            real_biggest_living_instance = i;
            all_living_instances++;
        }
        else{
            if(real_smallest_dead_instance == -1){
                real_smallest_dead_instance = i;
            }
        }
    }

    if(real_smallest_dead_instance == -1){
        real_smallest_dead_instance = self->capacity - 1;
    }

    assert(real_smallest_dead_instance == self->smallest_index_of_dead_instance);
    assert(real_biggest_living_instance == self->largest_index_of_living_instance);
    assert(all_living_instances == self->living_instances);

}


int main() {

    ArenaOfExampleType* a = ArenaOfExampleType_new();

    // get one
    Type* e = ArenaOfExampleType_get_first_dead_one_and_set_back_to_live(a);

    assert(e->alive);
    assert(e->index == 0);

    for (int i = 0; i < 99; i++) {
        Type* e = ArenaOfExampleType_get_first_dead_one_and_set_back_to_live(a);
        assert(e->index == i+1);
    }

    assert(a->largest_index_of_living_instance == 99);
    assert(a->living_instances == 100);
    //printf("%d\n", a->smallest_index_of_dead_instance);
    assert(ArenaOfExampleType_is_full(a));
    assert(a->smallest_index_of_dead_instance == 99);

    ArenaOfExampleType_check_for_correctness(a);

    ArenaOfExampleType_set_to_dead(a,47);
    assert(a->smallest_index_of_dead_instance == 47);
    assert(a->living_instances == 99);

    Type* e2 = ArenaOfExampleType_get_first_dead_one_and_set_back_to_live(a);
    assert(e2->index == 47);

    assert(a->largest_index_of_living_instance == 99);
    assert(a->living_instances == 100);
    //printf("%d\n", a->smallest_index_of_dead_instance);
    assert(ArenaOfExampleType_is_full(a));
    assert(a->smallest_index_of_dead_instance == 99);
    int counter = 0;
    for (int i = 0; i < 99; i+=3) {
        ArenaOfExampleType_set_to_dead(a, i);
        printf(" - %d\n", a->smallest_index_of_dead_instance);
        assert(a->smallest_index_of_dead_instance == 0);
        counter++;
        assert(a->living_instances == 100 -counter);
        ArenaOfExampleType_check_for_correctness(a);
    }

    int max = ArenaOfExampleType_get_dead_instances(a);
    for (int i = 0; i < max; i++) {
        Type* e = ArenaOfExampleType_get_first_dead_one_and_set_back_to_live(a);
    }
    ArenaOfExampleType_check_for_correctness(a);

    counter = 0;
    for (int i = 99; i > 0; i--) {
        ArenaOfExampleType_set_to_dead(a, i);
        printf(" - %d\n", a->smallest_index_of_dead_instance);
        assert(a->smallest_index_of_dead_instance == i);
        counter++;
        assert(a->living_instances == 100 -counter);
        ArenaOfExampleType_check_for_correctness(a);
    }

    return 0;
}
