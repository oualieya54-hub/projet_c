#ifndef COURSE_INTEGRATION_H
#define COURSE_INTEGRATION_H

#include <gtk/gtk.h>

/**
 * @brief Creates and displays the course management window.
 *
 * This function loads the UI for course management from the Glade file,
 * connects the necessary signals, populates the initial course list,
 * and shows the window.
 */
void open_course_window(void);
void open_course_registration_window(void);

#endif /* COURSE_INTEGRATION_H */
