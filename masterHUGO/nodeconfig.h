/*
 Copyright (C) 2011 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef __NODECONFIG_H__
#define __NODECONFIG_H__

uint8_t nodeconfig_read(void);
void nodeconfig_listen(void);

// What are the actual node values that we want to use?
// EEPROM locations are actually just indices into this array
const uint16_t node_address_set[10] = { 00, 02, 05, 012, 015, 022, 025, 032, 035, 045 };

#endif // __NODECONFIG_H__
