#ifndef DBG_CHALLENGE_ROOMS_H
#define DBG_CHALLENGE_ROOMS_H

typedef char* (*room_function_t)(char*);
typedef struct __room_t {
    char *name, *room_desc, *door_desc;
    room_function_t func;
    int flags;
    int state;
    int unlocks_n;
    struct __room_t **unlocks;
    char *solution;
    double score;
} room_t;

#define ROOM_STATE_UNLOCKED 1
#define ROOM_STATE_COMPLETED 2

#define ROOM_FLAG_SECRET 1

room_t **get_rooms();
int get_n_rooms();
room_t **reset_rooms();

char* try_complete_room(room_t *room, char* input);
void run_room(room_t *room);


#endif //DBG_CHALLENGE_ROOMS_H
