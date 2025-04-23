#ifndef VALIDATION_H
#define VALIDATION_H

#include <stdbool.h>
#include <glib.h>
#include <ctype.h>
#include "gui/constant.h"

gboolean is_numeric(const gchar *str);
gboolean is_name_valid(const gchar *title);
void show_warning();


#endif