#ifndef MEMBER_INTEGRATION_H
#define MEMBER_INTEGRATION_H

#include <gtk/gtk.h>

/**
 * @brief Creates and displays the member management window.
 *
 * This function loads the UI for member management from the Glade file,
 * connects the necessary signals, populates the initial member list,
 * and shows the window.
 */
void open_member_window(void);
void open_private_coach_window(void);
void open_subscriptions_window(void);

#endif /* MEMBER_INTEGRATION_H */
