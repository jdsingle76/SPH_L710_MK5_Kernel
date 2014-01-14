/*
 * sec_charger.h
 * Samsung Mobile Charger Header
 *
 * Copyright (C) 2012 Samsung Electronics, Inc.
 *
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __SEC_CHARGER_H
#define __SEC_CHARGER_H __FILE__

#include <linux/battery/sec_charging_common.h>

#if defined(CONFIG_CHARGER_DUMMY)
#include <linux/battery/charger/dummy_charger.h>
#elif defined(CONFIG_CHARGER_MAX8903)
#include <linux/battery/charger/max8903_charger.h>
#elif defined(CONFIG_CHARGER_SMB328)
#include <linux/battery/charger/smb328_charger.h>
#elif defined(CONFIG_CHARGER_SMB347)
#include <linux/battery/charger/smb347_charger.h>
#elif defined(CONFIG_CHARGER_BQ24157)
#include <linux/battery/charger/bq24157_charger.h>
#elif defined(CONFIG_CHARGER_BQ24190) || \
		defined(CONFIG_CHARGER_BQ24191)
#include <linux/battery/charger/bq24190_charger.h>
#elif defined(CONFIG_CHARGER_NCP1851)
#include <linux/battery/charger/ncp1851_charger.h>
#endif

#if !defined (CONFIG_MACH_COMANCHE) &&  !defined (CONFIG_MACH_ESPRESSO10_ATT) && !defined (CONFIG_MACH_APEXQ)\
	&& !defined (CONFIG_MACH_ESPRESSO10_SPR) && !defined (CONFIG_MACH_ESPRESSO10_VZW)  && !defined (CONFIG_MACH_EXPRESS)\
	&& !defined (CONFIG_MACH_ESPRESSO_VZW)
static enum power_supply_property sec_charger_props[] = {
	POWER_SUPPLY_PROP_STATUS,
	POWER_SUPPLY_PROP_CHARGE_TYPE,
	POWER_SUPPLY_PROP_HEALTH,
	POWER_SUPPLY_PROP_ONLINE,
	POWER_SUPPLY_PROP_CURRENT_NOW,
};

#endif
struct sec_charger_info {
	struct i2c_client		*client;
	sec_battery_platform_data_t *pdata;
	struct power_supply		psy_chg;
	struct delayed_work isr_work;

	int cable_type;
	bool is_charging;

	/* charging current : + charging, - OTG */
	int charging_current;

	/* register programming */
	int reg_addr;
	int reg_data;
};

bool sec_hal_chg_init(struct i2c_client *);
bool sec_hal_chg_suspend(struct i2c_client *);
bool sec_hal_chg_resume(struct i2c_client *);
bool sec_hal_chg_get_property(struct i2c_client *,
				enum power_supply_property,
				union power_supply_propval *);
bool sec_hal_chg_set_property(struct i2c_client *,
				enum power_supply_property,
				const union power_supply_propval *);

ssize_t sec_hal_chg_show_attrs(struct device *dev,
				const ptrdiff_t offset, char *buf);

ssize_t sec_hal_chg_store_attrs(struct device *dev,
				const ptrdiff_t offset,
				const char *buf, size_t count);

ssize_t sec_chg_show_attrs(struct device *dev,
				struct device_attribute *attr, char *buf);

ssize_t sec_chg_store_attrs(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count);

#define SEC_CHARGER_ATTR(_name)				\
{							\
	.attr = {.name = #_name, .mode = 0664},	\
	.show = sec_chg_show_attrs,			\
	.store = sec_chg_store_attrs,			\
}

#if !defined (CONFIG_MACH_COMANCHE) &&  !defined (CONFIG_MACH_ESPRESSO10_ATT) && !defined (CONFIG_MACH_APEXQ)\
	&& !defined (CONFIG_MACH_ESPRESSO10_SPR) && !defined (CONFIG_MACH_ESPRESSO10_VZW)  && !defined (CONFIG_MACH_EXPRESS)\
	&& !defined (CONFIG_MACH_ESPRESSO_VZW)
static struct device_attribute sec_charger_attrs[] = {
	SEC_CHARGER_ATTR(reg),
	SEC_CHARGER_ATTR(data),
	SEC_CHARGER_ATTR(regs),
};
#endif

enum {
	CHG_REG = 0,
	CHG_DATA,
	CHG_REGS,
};

#endif /* __SEC_CHARGER_H */
