/* filesorter-window.c
 *
 * Copyright 2024 Unknown
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <sys/stat.h>
#include "config.h"
#include "sorting.h"

#include "filesorter-window.h"

struct _FilesorterWindow {
    AdwApplicationWindow parent_instance;

    /* Template widgets */
    AdwHeaderBar *header_bar;
    GtkLabel *main_label;
    GtkTextView *main_text_view;
    GtkButton *open_button;
};

G_DEFINE_FINAL_TYPE (FilesorterWindow, filesorter_window, ADW_TYPE_APPLICATION_WINDOW)


static void filesorter_window_class_init(FilesorterWindowClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

    gtk_widget_class_set_template_from_resource(widget_class, "/org/miika/filesorter/filesorter-window.ui");
    gtk_widget_class_bind_template_child (widget_class, FilesorterWindow, header_bar);
    gtk_widget_class_bind_template_child (widget_class, FilesorterWindow, main_label);
    gtk_widget_class_bind_template_child (widget_class, FilesorterWindow, main_text_view);
    gtk_widget_class_bind_template_child (widget_class, FilesorterWindow, open_button);
}

static gboolean on_drop(GtkDropTarget *target,
                        const GValue *value,
                        double x,
                        double y,
                        gpointer data) {
    FilesorterWindow *self = FILESORTER_WINDOW(data);
    /* GdkFileList is a boxed value, so we use the boxed API. */
    GdkFileList *file_list = g_value_get_boxed(value);

    GSList *list = gdk_file_list_get_files(file_list);

    struct stat s;
    /* Loop through the files and print their names. */
    for (GSList *l = list; l != NULL; l = l->next) {
        GFile *file = l->data;
        g_print("%s\n", g_file_get_path(file));
        if(s.st_mode & S_IFDIR) {
            g_print("Is directory\n");
        }
        else if(s.st_mode & S_IFREG) {
            g_print("Is file\n");
        }
        else {
            g_print("Is something else\n");
        }
    }

    /* Hide the label */
    gtk_widget_set_visible(GTK_WIDGET(self->main_label), FALSE);

    /* TODO: For each folder create a folder icon and for each file a file icon and put them in a grid */


    return TRUE;
}

static GdkDragAction on_enter(GtkDropTarget *target,
                              double x,
                              double y,
                              gpointer data) {
    /* Custom code... */

    /* Tell the callee to continue. */
    return GDK_ACTION_COPY;
}

static void on_leave(GtkDropTarget *target,
                     gpointer data) {
    /* Custom code... */
}

static void filesorter_window_init(FilesorterWindow *self) {
    gtk_widget_init_template(GTK_WIDGET (self));

    GtkDropTarget *target = gtk_drop_target_new(G_TYPE_INVALID, GDK_ACTION_COPY);
    gtk_drop_target_set_gtypes(target, (GType[1]) {GDK_TYPE_FILE_LIST,}, 1);
    g_signal_connect (target, "drop", G_CALLBACK(on_drop), self);
    g_signal_connect (target, "enter", G_CALLBACK(on_enter), self);
    g_signal_connect (target, "leave", G_CALLBACK(on_leave), self);
    gtk_widget_add_controller(GTK_WIDGET (self), GTK_EVENT_CONTROLLER (target));
}

