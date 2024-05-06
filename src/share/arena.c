// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are
// !!!!!!!!!!!!!!!!!!! dont edit this file -> edit on toplevel where the tests are

#define ARENA(Type, CAPACITY)\
typedef struct {\
    Type* memory;\
    int capacity;\
    int smallest_index_of_dead_instance;\
    int largest_index_of_living_instance;\
    int living_instances;\
} ArenaOf##Type;\
\
ArenaOf##Type* ArenaOf##Type##_new() {\
    int capacity = CAPACITY;\
    ArenaOf##Type* arena = (ArenaOf##Type*)malloc(sizeof(ArenaOf##Type));\
    arena->memory = (Type*)malloc(sizeof(Type) * capacity);\
    arena->capacity = capacity;\
    arena->smallest_index_of_dead_instance = 0;\
    arena->largest_index_of_living_instance = -1;\
    arena->living_instances = 0;\
    return arena;\
}\
\
Type* ArenaOf##Type##_get(ArenaOf##Type* self, int index) {\
    if (index < 0 || index >= self->capacity || index > self->largest_index_of_living_instance)\
        return NULL;\
    return &(self->memory[index]);\
}\
\
void ArenaOf##Type##_set_to_dead(ArenaOf##Type* self, int index) {\
\
    assert(index >= 0 && index < self->capacity);\
\
    printf("%d", self->largest_index_of_living_instance);\
    assert(!(index > self->largest_index_of_living_instance));\
\
    self->memory[index].alive = 0;\
\
    if (index == self->largest_index_of_living_instance) {\
        for (int i = index; i >= 0; i--) {\
            if (self->memory[i].alive == 1) {\
                self->largest_index_of_living_instance = i;\
                break;\
            }\
        }\
    }\
\
    if (index < self->smallest_index_of_dead_instance){\
        self->smallest_index_of_dead_instance = index;\
    }\
\
    self->living_instances--;\
    if(self->living_instances == 0){    \
        self->largest_index_of_living_instance=-1;\
    }\
}\
\
int ArenaOf##Type##_get_capacity(ArenaOf##Type* self) {\
    return self->capacity;\
}\
\
Type* ArenaOf##Type##_get_first_dead_one_and_set_back_to_live(ArenaOf##Type* self) {\
    int instance_index = self->smallest_index_of_dead_instance;\
    Type* ret = &(self->memory[instance_index]);\
    ret->alive = 1;\
    ret->index = self->smallest_index_of_dead_instance;\
\
    self->living_instances ++ ;\
\
    if (instance_index > self->largest_index_of_living_instance) {\
        self->largest_index_of_living_instance = instance_index;\
    }\
\
    if(instance_index == self->smallest_index_of_dead_instance){\
        for (int i = self->smallest_index_of_dead_instance; i < self->capacity; i++) {\
            if (self->memory[i].alive == 0) {\
                self->smallest_index_of_dead_instance = i;\
                break;\
            }\
        }\
    }\
\
    if (self->smallest_index_of_dead_instance == ret->index){\
        self->smallest_index_of_dead_instance = self->capacity - 1;\
    }\
\
    return ret;\
}\
\
int ArenaOf##Type##_get_dead_instances(ArenaOf##Type* self) {\
    return self->capacity - self->living_instances;\
}\
\
int ArenaOf##Type##_is_full(ArenaOf##Type* self) {\
    return ArenaOf##Type##_get_dead_instances(self) == 0;\
}\
\
void ArenaOf##Type##_check_for_correctness(ArenaOf##Type* self) {\
     /*check if all values are correct*/\
\
    int real_smallest_dead_instance = -1;\
    int real_biggest_living_instance = 0;\
    int all_living_instances = 0;\
\
    for(int i = 0; i < self->capacity; i++ ){\
        Type e = self->memory[i];\
        assert(e.index == i);\
        if(e.alive){\
            real_biggest_living_instance = i;\
            all_living_instances++;\
        }\
        else{\
            if(real_smallest_dead_instance == -1){\
                real_smallest_dead_instance = i;\
            }\
        }\
    }\
\
    if(real_smallest_dead_instance == -1){\
        real_smallest_dead_instance = self->capacity - 1;\
    }                        \
    if(all_living_instances == 0){      \
        real_biggest_living_instance =-1;                         \
    }                             \
                             \
\
    assert(real_smallest_dead_instance == self->smallest_index_of_dead_instance);              \
    printf("%d  --  %d\n" ,real_biggest_living_instance, self->largest_index_of_living_instance );                         \
    assert(real_biggest_living_instance == self->largest_index_of_living_instance);\
    assert(all_living_instances == self->living_instances);\
\
}                            \
void ArenaOf##Type##_set_all_dead(ArenaOf##Type* self) {\
     /*check if all values are correct*/\
\
\
    for(int i = 0; i < self->capacity; i++ ){\
        Type* e = &(self->memory[i]);       \
        e->alive = 0;        \
        e->index = i;                      \
    }                        \
                             \
    self->smallest_index_of_dead_instance = 0;\
    self->largest_index_of_living_instance = -1;\
    self->living_instances = 0;         \
    ArenaOf##Type##_check_for_correctness(self);\
}