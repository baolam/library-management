#ifndef VALIDATION_H
#define VALIDATION_H

#include <stdbool.h>
#include <ctype.h>
#include "gui/constant.h"

gboolean is_numeric(const gchar *str);
void show_warning();


#endif