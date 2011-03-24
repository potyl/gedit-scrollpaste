/*
 * scrollpaste-plugin.c - Fix scrolling behavior after a paste
 *
 * Copyright (C) 2009 - Emmanuel Rodriguez
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "scrollpaste-plugin.h"

#include <glib/gi18n-lib.h>
#include <gedit/gedit-debug.h>
#include <glib.h>

#define WINDOW_DATA_KEY	"ScrollpastePluginWindowData"

#define SCROLLPASTE_PLUGIN_GET_PRIVATE(object)	(G_TYPE_INSTANCE_GET_PRIVATE ((object), TYPE_SCROLLPASTE_PLUGIN, ScrollpastePluginPrivate))

struct _ScrollpastePluginPrivate
{
	/* Empty */
	gboolean dummy;
};

typedef struct _PasteData {
	GtkTextMark *paste_mark;
	GtkTextView *view;
} PasteData;


static void paste_data_free (gpointer data);

static void disconnect_from_view (GeditView *view, gpointer data);

static void connect_to_view (GeditView *view, gpointer data);

static gboolean button_press_event_cb (GtkWidget *widget, GdkEventButton *event, gpointer data);

static void paste_done_cb (GtkTextBuffer *buffer, GtkClipboard *clipboard, gpointer data);

static void tab_removed_cb (GeditWindow *window, GeditTab *tab, gpointer data);

static void tab_added_cb (GeditWindow *window, GeditTab *tab, gpointer data);


GEDIT_PLUGIN_REGISTER_TYPE (ScrollpastePlugin, scrollpaste_plugin)


static void
scrollpaste_plugin_init (ScrollpastePlugin *plugin)
{
	plugin->priv = SCROLLPASTE_PLUGIN_GET_PRIVATE (plugin);

	gedit_debug_message (DEBUG_PLUGINS, "ScrollpastePlugin initializing");
}


static void
scrollpaste_plugin_finalize (GObject *object)
{
	gedit_debug_message (DEBUG_PLUGINS, "ScrollpastePlugin finalizing");

	G_OBJECT_CLASS (scrollpaste_plugin_parent_class)->finalize (object);
}


static void
paste_data_free (gpointer data)
{
	PasteData *paste_data = (PasteData *) data;

	if (paste_data->view)
	{
		g_object_unref (paste_data->view);
		paste_data->view = NULL;
	}
	g_free (paste_data);
}


static void
disconnect_from_view (GeditView *view, gpointer data)
{
	GtkTextBuffer *buffer;
	PasteData *paste_data;

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

	paste_data = g_object_get_data (G_OBJECT (view), WINDOW_DATA_KEY);
	g_signal_handlers_disconnect_by_func (view, button_press_event_cb, paste_data);
	g_signal_handlers_disconnect_by_func (buffer, paste_done_cb, paste_data);

	g_object_set_data (G_OBJECT (view), WINDOW_DATA_KEY, NULL);
}


static void
connect_to_view (GeditView *view, gpointer data)
{
	GtkTextBuffer *buffer;
	PasteData *paste_data;

	paste_data = g_new0 (PasteData, 1);
	paste_data->view = g_object_ref (view);

	g_object_set_data_full (G_OBJECT (view), WINDOW_DATA_KEY, paste_data, paste_data_free);

	g_signal_connect (
		view, "button-press-event", G_CALLBACK (button_press_event_cb), paste_data);

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
	g_signal_connect (
		buffer, "paste-done", G_CALLBACK (paste_done_cb), paste_data);
}


static gboolean
button_press_event_cb (GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	PasteData *paste_data;
	GtkTextIter iter;
	GtkTextBuffer *buffer;
	gint x, y;

	if (event->button != 2) return FALSE;

	/*
	 * Fix GtkTextView's annoying paste behaviour when pasting with the mouse
	 * (middle button click). By default gtk will scroll the text view to the
	 * original place where the cursor is.
	 */
	paste_data = (PasteData *) data;

	gtk_text_view_window_to_buffer_coords (
		GTK_TEXT_VIEW (widget), GTK_TEXT_WINDOW_TEXT,
		event->x, event->y,
		&x, &y);

	gtk_text_view_get_iter_at_position (
		GTK_TEXT_VIEW (widget), &iter, NULL, x, y);


	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (widget));
	paste_data->paste_mark = gtk_text_buffer_create_mark (
		buffer, NULL, &iter, FALSE);

	return FALSE;
}


static void
paste_done_cb (GtkTextBuffer *buffer, GtkClipboard *clipboard, gpointer data)
{
	PasteData *paste_data;
	GtkTextIter iter;

	paste_data = (PasteData *) data;
	if (paste_data->paste_mark == NULL)
	{
		/* If there's no marker set then it means that the paste wasn't done with
		 * the mouse. That's ok since pasting the normal paste operation works and
		 * doesn't require a hack.
		 */
		return;
	}

	/*
	 * If a paste is done through the middle click then place the cursor at the
	 * end of the pasted text.
	 */
	gtk_text_buffer_get_iter_at_mark (buffer, &iter, paste_data->paste_mark);
	gtk_text_buffer_place_cursor (buffer, &iter);
gtk_text_view_set_cursor_visible (paste_data->view, !gtk_text_view_get_cursor_visible (paste_data->view));
//gtk_text_view_set_right_margin (paste_data->view, 10 + gtk_text_view_get_right_margin (paste_data->view));
	gtk_text_view_scroll_to_mark (paste_data->view, paste_data->paste_mark,
		0.0, FALSE, 0.0, 0.5);
	gtk_text_buffer_delete_mark (buffer, paste_data->paste_mark);
	paste_data->paste_mark = NULL;
}


static void
tab_removed_cb (GeditWindow *window, GeditTab *tab, gpointer data)
{
	disconnect_from_view (gedit_tab_get_view (tab), NULL);
}


static void
tab_added_cb (GeditWindow *window, GeditTab *tab, gpointer data)
{
	connect_to_view (gedit_tab_get_view (tab), NULL);
}


static void
impl_activate (GeditPlugin *plugin, GeditWindow *window)
{
	GList *list;

	gedit_debug (DEBUG_PLUGINS);
	g_signal_connect (window, "tab-added", G_CALLBACK (tab_added_cb), NULL);
	g_signal_connect (window, "tab-removed", G_CALLBACK (tab_removed_cb), NULL);

	list = gedit_window_get_views (window);
	g_list_foreach (list, (GFunc) connect_to_view, NULL);
	g_list_free (list);
}


static void
impl_deactivate (GeditPlugin *plugin, GeditWindow *window)
{
	GList *list;

	gedit_debug (DEBUG_PLUGINS);
	g_signal_handlers_disconnect_by_func (window, tab_added_cb, NULL);
	g_signal_handlers_disconnect_by_func (window, tab_removed_cb, NULL);

	list = gedit_window_get_views (window);
	g_list_foreach (list, (GFunc) disconnect_from_view, NULL);
	g_list_free (list);
}


static void
impl_update_ui (GeditPlugin *plugin, GeditWindow *window)
{
	gedit_debug (DEBUG_PLUGINS);
}


static void
scrollpaste_plugin_class_init (ScrollpastePluginClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GeditPluginClass *plugin_class = GEDIT_PLUGIN_CLASS (klass);

	object_class->finalize = scrollpaste_plugin_finalize;

	plugin_class->activate = impl_activate;
	plugin_class->deactivate = impl_deactivate;
	plugin_class->update_ui = impl_update_ui;

	g_type_class_add_private (object_class, sizeof (ScrollpastePluginPrivate));
}

