/*
 * scrollpaste-plugin.h - Fix scrolling behavior after a paste
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

#ifndef __SCROLLPASTE_PLUGIN_H__
#define __SCROLLPASTE_PLUGIN_H__

#include <glib.h>
#include <glib-object.h>
#include <gedit/gedit-plugin.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define TYPE_SCROLLPASTE_PLUGIN		(scrollpaste_plugin_get_type ())
#define SCROLLPASTE_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_SCROLLPASTE_PLUGIN, ScrollpastePlugin))
#define SCROLLPASTE_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_CAST((k), TYPE_SCROLLPASTE_PLUGIN, ScrollpastePluginClass))
#define IS_SCROLLPASTE_PLUGIN(o)	(G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_SCROLLPASTE_PLUGIN))
#define IS_SCROLLPASTE_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_SCROLLPASTE_PLUGIN))
#define SCROLLPASTE_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_SCROLLPASTE_PLUGIN, ScrollpastePluginClass))

/* Private structure type */
typedef struct _ScrollpastePluginPrivate	ScrollpastePluginPrivate;

/*
 * Main object structure
 */
typedef struct _ScrollpastePlugin		ScrollpastePlugin;

struct _ScrollpastePlugin
{
	GeditPlugin parent_instance;

	/*< private >*/
	ScrollpastePluginPrivate *priv;
};

/*
 * Class definition
 */
typedef struct _ScrollpastePluginClass	ScrollpastePluginClass;

struct _ScrollpastePluginClass
{
	GeditPluginClass parent_class;
};

/*
 * Public methods
 */
GType	scrollpaste_plugin_get_type	(void) G_GNUC_CONST;

/* All the plugins must implement this function */
G_MODULE_EXPORT GType register_gedit_plugin (GTypeModule *module);

G_END_DECLS

#endif /* __SCROLLPASTE_PLUGIN_H__ */
