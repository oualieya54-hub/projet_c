#ifndef TRAINER_INTEGRATION_H
#define TRAINER_INTEGRATION_H

#include <gtk/gtk.h>

/**
 * @brief Creates and displays the trainer management window.
 *
 * This function loads the UI for trainer management from the Glade file,
 * connects the necessary signals, populates the initial trainer list,
 * and shows the window.
 */
void open_trainer_window(void);
void open_trainer_course_registration_window(void);

#endif /* TRAINER_INTEGRATION_H */
