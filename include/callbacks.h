#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <gtk/gtk.h>

// Xử lý khi nút chung được nhấn
void on_button_clicked(GtkButton *button, gpointer user_data);

// Xử lý khi nút liên quan đến ldt được nhấn
void on_ldt_clicked(GtkButton *button, gpointer user_data);

// Xử lý khi nhấn nút mượn sách
void on_borrow_button_clicked(GtkButton *button, gpointer user_data);

// Xử lý khi nhấn nút trả sách
void on_return_button_clicked(GtkButton *button, gpointer user_data);

#endif // CALLBACKS_H
