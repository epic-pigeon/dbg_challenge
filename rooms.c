#include "rooms.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int n_rooms = 0;
room_t **rooms = 0;

char* room1_check(char* line) {
    int code;
    if (sscanf(line, "%d", &code) != 1 || code < 0) {
        return "You cannot enter that on the keypad.";
    }
    return code == __RAND_1000_TO_10000 ? 0 : "The keypad flashes red.";
}

#define ROOM2_OPTION __RAND_0_TO_3
#if ROOM2_OPTION == 0
char* room2_check(char* line) {
    int nums[5];
    if (sscanf(line, "%d %d %d %d %d", nums, nums+1, nums+2, nums+3, nums+4) != 5) {
        return "The display flashes 'Please input 5 numbers'.";
    }
    if (nums[0] != __RAND_1_TO_10 || nums[1] != __RAND_1_TO_10) {
        return "The display flashes 'Try again'.";
    }
    for (int i = 2; i < 5; i++) {
        if (nums[i] != nums[i-1] * 2 + nums[i-2]) {
            return "The display flashes 'Try again'.";
        }
    }
    return 0;
}
#elif ROOM2_OPTION == 1
char* room2_check(char* line) {
    int nums[5];
    if (sscanf(line, "%d %d %d %d %d", nums, nums+1, nums+2, nums+3, nums+4) != 5) {
        return "The display flashes 'Please input 5 numbers'.";
    }
    if (nums[0] != __RAND_1_TO_10 || nums[1] != __RAND_1_TO_10) {
        return "The display flashes 'Try again'.";
    }
    for (int i = 2; i < 5; i++) {
        if (nums[i] != nums[i-1] + nums[i-2]) {
            return "The display flashes 'Try again'.";
        }
    }
    return 0;
}
#else
char* room2_check(char* line) {
    int nums[5];
    if (sscanf(line, "%d %d %d %d %d", nums, nums+1, nums+2, nums+3, nums+4) != 5) {
        return "The display flashes 'Please input 5 numbers'.";
    }
    if (nums[0] != __RAND_1_TO_10) {
        return "The display flashes 'Try again'.";
    }
    for (int i = 1; i < 5; i++) {
        if (nums[i] != nums[i-1] * 2 + 1) {
            return "The display flashes 'Try again'.";
        }
    }
    return 0;
}
#endif

int rand_room_blocked = 0;
char* rand_room_check(char* line) {
    if (rand_room_blocked) return "The keypad is blocked.";
    int num;
    if (!sscanf(line, "%d", &num)) {
        return "You cannot enter that on the keypad.";
    }
    rand_room_blocked = 1;
    if (rand() != num) {
        return "The keypad flashed red and now appears blocked.";
    }
    return 0;
}

room_t **get_rooms() {
    if (rooms) return rooms;
    room_t *rand_room = malloc(sizeof(room_t));
    *rand_room = (room_t){
            "Mind reading room",
            "a room with a lot of question marks all over it",
            "strange door with a number keypad next to it that says 'Can you read my mind?'",
            rand_room_check, ROOM_FLAG_SECRET, 0,
            0, 0, 0, 0
    };

    room_t *room6 = malloc(sizeof(room_t));
    *room6 = (room_t){
            "Room 6",
            "an interesting shaped red room with walls looking like they are made out of some sort of cloth",
            "door with a sensor display that says 'Please input 5 numbers' next to it",
            room2_check, 0, 0, 0, 0, 0, 10
    };
    room_t *room5 = malloc(sizeof(room_t));
    *room5 = (room_t){
            "Room 5",
            "a colourless room with a keypad next to a door",
            "metal door with a number keypad next to it",
            room1_check, 0, 0,
            1, ptr_array_of(1, room6), 0, 10
    };
    room_t *room4 = malloc(sizeof(room_t));
    *room4 = (room_t){
            "Room 4",
            "an interesting shaped red room with walls looking like they are made out of some sort of cloth",
            "door with a sensor display that says 'Please input 5 numbers' next to it",
            room2_check, 0, 0,
            1, ptr_array_of(1, room5), 0, 10
    };
    room_t *room3 = malloc(sizeof(room_t));
    *room3 = (room_t){
            "Room 3",
            "a colourless room with a keypad next to a door",
            "metal door with a number keypad next to it",
            room1_check, 0, 0,
            2, ptr_array_of(2, room4, rand_room), 0, 10
    };
    room_t *room2 = malloc(sizeof(room_t));
    *room2 = (room_t){
            "Room 2",
            "an interesting shaped red room with walls looking like they are made out of some sort of cloth",
            "door with a sensor display that says 'Please input 5 numbers' next to it",
            room2_check, 0, 0,
            1, ptr_array_of(1, room3), 0, 10
    };
    room_t *room1 = malloc(sizeof(room_t));
    *room1 = (room_t){
            "Room 1",
            "a colourless room with a keypad next to a door",
            "metal door with a number keypad next to it",
            room1_check, 0, ROOM_STATE_UNLOCKED,
            1, ptr_array_of(1, room2), 0, 10
    };
    return rooms = ptr_array_of(n_rooms = 7, room1, room2, room3, room4, room5, room6, rand_room);
}

int get_n_rooms() {
    return n_rooms;
}

room_t **reset_rooms() {
    n_rooms = rooms = 0;
    return get_rooms();
}

void run_room(room_t *room) {
    int new_room = 1;
    char line[51];
    while (1) { outer: // there are no labeled loops, so I had to resort to this
        if (new_room) {
            printf("You see %s. You also see a sign saying it is %s.\nWhat do you do?\n", room->room_desc, room->name);
        } else {
            printf("You came back into the room.\nWhat do you do now?\n");
        }
        int option = prompt_option(3, "Come up to the door", "Look at the map", "Settings");
        if (option == 0) {
            if ((room->state & ROOM_STATE_COMPLETED) && room->solution) {
                printf("You came up to the door. You have already seen this %s."
                       " You solved it by inputting '%s'.\nWhat do you want to try now?"
                       " (input an empty line to come back)\n", room->door_desc, room->solution);
            } else {
                printf("You came up to the door. You see a %s.\nWhat do you want to input?"
                       " (input an empty line to come back)\n", room->door_desc);
            }
            while (1) {
                if (prompt_line(line, 51)) {
                    if (line[0] == '\n') {
                        new_room = 0;
                        goto outer;
                    }
                    char *msg = try_complete_room(room, line);
                    if (!msg) {
                        printf("The door opens, ");
                        if (room->unlocks_n == 0) {
                            printf("but all you can see is a brick wall. Anyway, you are happy.\n");
                            new_room = 0;
                        } else if (room->unlocks_n == 1) {
                            printf("and you see a dark corridor. You decide to follow it.\n\n\n");
                            room = room->unlocks[0];
                            new_room = 1;
                        } else {
                            printf("and you see many corridors going in different directions."
                                   " You feel confused. (check out new locations on the map)\n");
                            new_room = 0;
                        }
                        goto outer;
                    } else {
                        printf("%s\n", msg);
                    }
                } else {
                    printf("The line is too long.\n");
                }
            }
        } else if (option == 1) {
            while (1) {
                printf("Where do you want to go?\n");
                char **arr = malloc((n_rooms+1) * sizeof(char *));
                for (int i = 0; i < n_rooms; i++) {
                    asprintf(arr + i, "%s%s",
                             (rooms[i]->flags & ROOM_FLAG_SECRET) && !(rooms[i]->state & ROOM_STATE_UNLOCKED) ?
                             "????????????????" :
                             rooms[i] == room ?
                                "Put down the map" :
                                rooms[i]->name, (rooms[i]->state & ROOM_STATE_UNLOCKED) ? "" : " (blocked)");
                }
                arr[n_rooms] = "Exit";
                int room_number = prompt_option_array(n_rooms+1, arr);
                for (int i = 0; i < n_rooms; i++) free(arr[i]);
                if (room_number == n_rooms) {
                    printf("You decided to go to the exit.\n");
                    return;
                }
                if (rooms[room_number]->state & ROOM_STATE_UNLOCKED) {
                    new_room = room != rooms[room_number];
                    if (new_room) printf("\n\n");
                    room = rooms[room_number];
                    goto outer;
                } else {
                    printf("You don't have access to this room for now.\n");
                }
            }
        } else if (option == 2) {
            while (1) {
                int option = prompt_option(2, "Create a solutions file", "Go back");
                if (option == 0) {
                    int last_solved_room = -1;
                    for (int i = n_rooms-1; i >= 0; i--) {
                        if (rooms[i]->state & ROOM_STATE_COMPLETED) {
                            last_solved_room = i;
                            break;
                        }
                    }
                    if (last_solved_room == -1) {
                        printf("Solve something first!\n");
                        continue;
                    }
                    char path[4096];
                    printf("Specify a path (input an empty line to go back):\n");
                    prompt_line(path, 4096);
                    if (path[0] == '\n') continue;
                    path[strlen(path)-1] = 0;
                    FILE *f = fopen(path, "w");
                    if (!f) {
                        printf("Could not open file\n");
                        continue;
                    }
                    int error = 0;
                    for (int i = 0; i <= last_solved_room; i++) {
                        if (fprintf(f, "%s\n",
                                    (rooms[i]->state & ROOM_STATE_COMPLETED) ? rooms[i]->solution : "") <= 0) {
                            error = 1;
                            break;
                        }
                        // if ((rooms[i]->state & ROOM_STATE_COMPLETED) && rooms[i]->solution) {
                        //     printf("%s: %s\n", rooms[i]->name, rooms[i]->solution);
                        // }
                    }
                    if (error) {
                        printf("An error occurred: %s\n", strerror(errno));
                    }
                    fclose(f);
                } else {
                    new_room = 0;
                    goto outer;
                }
            }
        }
    }
}

char* try_complete_room(room_t *room, char* line) {
    char *msg = room->func(line);
    if (!msg) {
        room->state |= ROOM_STATE_COMPLETED;
        if (room->solution) free(room->solution);
        room->solution = malloc(51);
        strcpy(room->solution, line);
        room->solution[strlen(room->solution) - 1] = 0;
        for (int i = 0; i < room->unlocks_n; i++) {
            room->unlocks[i]->state |= ROOM_STATE_UNLOCKED;
        }
    }
    return msg;
}
