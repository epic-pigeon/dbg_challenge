#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "rooms.h"

_Noreturn void print_usage(char* argv0) {
    printf("Usage:\n"
           "    %1$s: run the maze\n"
           "    %1$s --grade (-g) /path/to/solutions: grade a solution file\n"
           "    %1$s --continue (-c) /path/to/solutions: continue from a solution file\n"
           "    %1$s --help (-h): print this message\n", argv0);
    exit(1);
}

int main(int argc, char** argv) {
    int verbose = 0;
    char* grade_path = 0;
    char* continue_path = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--grade") == 0 || strcmp(argv[i], "-g") == 0) {
            i++;
            if (i >= argc) print_usage(argv[0]);
            grade_path = argv[i];
        } else if (strcmp(argv[i], "--continue") == 0 || strcmp(argv[i], "-c") == 0) {
            i++;
            if (i >= argc) print_usage(argv[0]);
            continue_path = argv[i];
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = 1;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
        } else {
            printf("Unrecognized option '%s'\n", argv[i]);
            print_usage(argv[0]);
        }
    }
    if (grade_path && continue_path) {
        printf("Cannot both grade and continue\n");
        print_usage(argv[0]);
    }
    if (!grade_path) {
        room_t **rooms = get_rooms();
        int n_rooms = get_n_rooms();
        if (continue_path) {
            FILE *file = fopen(continue_path, "r");
            char line[51];
            for (int i = 0; i < n_rooms; i++) {
                if (fgets(line, 51, file) == 0) break;
                if (line[0] == '\n') continue;
                if (try_complete_room(rooms[i], line)) {
                    printf("Bad input on line #%d\n", i+1);
                }
            }
            fclose(file);
        }
        run_room(rooms[0]);
    } else {
        room_t **rooms = get_rooms();
        int n_rooms = get_n_rooms();
        FILE *file = fopen(grade_path, "r");
        char line[51];
        double score = 0;
        for (int i = 0; i < n_rooms; i++) {
            if (fgets(line, 51, file) == 0) break;
            char* msg = line[0] == '\n' ? "empty line" : (rooms[i]->func(line) ? "wrong answer" : 0);
            if (verbose) {
                if (msg) {
                    printf("%s: %s\n",
                           (rooms[i]->flags & ROOM_FLAG_SECRET) ? "????????????????" : rooms[i]->name,
                           msg);
                } else {
                    printf("%s: solved, +%.1f\n", rooms[i]->name, rooms[i]->score);
                }
            }
            if (!msg) score += rooms[i]->score;
        }
        if (verbose) printf("Score: ");
        printf("%.1f", score);
        if (verbose) printf("\n");
        fclose(file);
    }
    return 0;
}
