#ifndef AUTH_H
#define AUTH_H

/**
 * @brief Authenticates a user based on username and password.
 *
 * This function reads the user database and checks if the provided
 * credentials are valid.
 *
 * @param username The username to check.
 * @param password The password to check.
 * @return The role of the user ("Admin", "Coach", "Membre") if authentication
 *         is successful, otherwise returns NULL. The returned string
 *         is statically allocated and should not be freed.
 */
const char* authenticate_user(const char *username, const char *password);

#endif /* AUTH_H */
