/*
 *
 *  bluez-tools - a set of tools to manage bluetooth devices for linux
 *
 *  Copyright (C) 2010  Alexander Orlenko <zxteam@gmail.com>
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "dbus-common.h"
#include "adapter.h"
#include "manager.h"

#define MANAGER_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), MANAGER_TYPE, ManagerPrivate))

struct _ManagerPrivate {
	DBusGProxy *dbus_g_proxy;
};

G_DEFINE_TYPE(Manager, manager, G_TYPE_OBJECT);

enum {
	PROP_0,

	PROP_ADAPTERS
};

static void manager_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void manager_get_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void manager_class_init(ManagerClass *klass)
{
	g_type_class_add_private(klass, sizeof(ManagerPrivate));

	GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
	GParamSpec *pspec;

	gobject_class->set_property = manager_set_property;
	gobject_class->get_property = manager_get_property;

	pspec = g_param_spec_string("Adapters", "adapters", "List of adapters", NULL, G_PARAM_READABLE);
	g_object_class_install_property(gobject_class, PROP_ADAPTERS, pspec);
}

static void manager_init(Manager *self)
{
	ManagerPrivate *priv;

	self->priv = priv = MANAGER_GET_PRIVATE(self);

	// TODO: Assert for conn
	// g_assert(!conn);

	priv->dbus_g_proxy = dbus_g_proxy_new_for_name(conn, BLUEZ_DBUS_NAME, BLUEZ_DBUS_MANAGER_PATH, BLUEZ_DBUS_MANAGER_INTERFACE);
}

static void manager_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
	Manager *self = MANAGER(object);

	switch (property_id) {
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

static void manager_get_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
	Manager *self = MANAGER(object);

	switch (property_id) {
	case PROP_ADAPTERS:
		
		g_value_set_object(value, self->priv->dbus_g_proxy);
		break;

	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

gboolean manager_get_default_adapter(Manager *self, GError **error, Adapter **adapter)
{
	DBusGProxy *adapter_proxy = NULL;

	if (!dbus_g_proxy_call(self->priv->dbus_g_proxy, "DefaultAdapter", error, G_TYPE_INVALID, DBUS_TYPE_G_PROXY, &adapter_proxy, G_TYPE_INVALID)) {
		return FALSE;
	} else {
		*adapter = g_object_new(ADAPTER_TYPE, "adapter_proxy", adapter_proxy, NULL);
	}
}

gboolean manager_find_adapter(Manager *self, const gchar *adapter_name, GError **error, Adapter **adapter)
{
	if (adapter == NULL) {
		return manager_get_default_adapter(self, error, adapter);
	}

	//if (!dbus_g_proxy_call(manager_obj, "FindAdapter", error, G_TYPE_STRING, adapter, G_TYPE_INVALID, DBUS_TYPE_G_PROXY, adapter_obj, G_TYPE_INVALID)) {
	//	return FALSE;
	//}

	return TRUE;
}

