#include "auth.h"
#include <stdio.h>
#include <string.h>

#define USERS_FILE "data/users.txt"

const char* authenticate_user(const char *username, const char *password) {
    static char role[50];
    char line[256];
    char file_username[100];
    char file_password[100];
    char file_role[50];
    FILE *f = fopen(USERS_FILE, "r");

    if (!f) {
        perror("Could not open users file");
        return NULL;
    }

    while (fgets(line, sizeof(line), f)) {
        // Use sscanf to parse the line buffer
        if (sscanf(line, "%99[^;];%99[^;];%49[^\n]", file_username, file_password, file_role) == 3) {
            if (strcmp(username, file_username) == 0 && strcmp(password, file_password) == 0) {
                strcpy(role, file_role);
                fclose(f);
                return role;
            }
        }
    }

    fclose(f);
    return NULL;
}
