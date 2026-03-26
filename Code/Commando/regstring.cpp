/*
**	Command & Conquer Renegade(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//
// Filename:     regstring.cpp
// Author:       Tom Spencer-Smith
// Date:         Dec 1998
// Description:
//

#include "regstring.h" // I WANNA BE FIRST!

#include "ini.h"
#include "openw3d.h"
#include "string.h"
#include "wwdebug.h"

//
// Class statics
//

//-----------------------------------------------------------------------------
cRegistryString::cRegistryString(const char *registry_location, const char *key_name,
	const char *default_value)
{
   WWASSERT(default_value != NULL);

   if (registry_location == NULL) {
      strcpy(RegistryLocation, "");
      strcpy(KeyName, "");
      Set(default_value);
   } else {
      WWASSERT(strlen(registry_location) < sizeof(RegistryLocation));
      WWASSERT(key_name != NULL);
      WWASSERT(strlen(key_name) < sizeof(KeyName));
      strcpy(RegistryLocation, registry_location);
      strcpy(KeyName, key_name);
   }

   WWASSERT(default_value != NULL);
   strcpy(Value, default_value);
   Initialized = false;
}

//-----------------------------------------------------------------------------
void cRegistryString::Initialize() {
	if (strcmp(RegistryLocation, "") != 0) {
		auto & ini = OpenW3D::Get_INIConfig();
		ini.Get_String(RegistryLocation, KeyName, Value, Value, sizeof(Value));
	}
	Initialized = true;
}

//-----------------------------------------------------------------------------
void cRegistryString::Set(const char *value)
{
   WWASSERT(value != NULL);
   WWASSERT(strlen(value) < sizeof(Value));

	strcpy(Value, value);
	Initialized = true;

	if (strcmp(RegistryLocation, "")) {
		auto & ini = OpenW3D::Get_INIConfig();
		ini.Put_String(RegistryLocation, KeyName, Value);
		OpenW3D::Save_Config();
   }
}
