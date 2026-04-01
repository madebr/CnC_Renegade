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

/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Combat																		  *
 *                                                                                             *
 *                     $Archive:: /Commando/Code/Commando/slavemaster.cpp                     $*
 *                                                                                             *
 *                       Author:: Steve Tall                                                   *
 *                                                                                             *
 *                     $Modtime:: 2/15/02 12:44p                                              $*
 *                                                                                             *
 *                    $Revision:: 18                                                          $*
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include "always.h"
#include <windows.h>
#include "slavemaster.h"
#include "wwdebug.h"
#include "ini.h"
#include "openw3d.h"
#include "_globals.h"
#include "AutoStart.h"
#include "ProcessManager.h"
#include "openw3d.h"
#include "ini.h"
#include "rawfile.h"
#include "inisup.h"
#include "natter.h"
#include "gamesideservercontrol.h"
#include "win.h"
#include "gamedata.h"
#include "ServerSettings.h"
#include "bandwidth.h"
#include "ConsoleMode.h"
#include "specialbuilds.h"
#include "useroptions.h"


#include <string.h>
#include <stdio.h>
#include <system_error>

#define KEY_NUM_SLAVES				"Count"
#define KEY_SLAVE_NAME				"Name"
#define KEY_SLAVE_SERIAL			"Serial"
#define KEY_SLAVE_ENABLE			"Enable"
#define KEY_SLAVE_PORT				"Port"
#define KEY_SLAVE_RUNNING_ID		"RunningID"
#define KEY_SLAVE_SETTINGS			"Settings"
#define KEY_SLAVE_BANDWIDTH		"Bandwidth"
#define KEY_SLAVE_PASSWORD			"Password"

const char *RegistryFileName = "slave.ini";

SlaveMasterClass SlaveMaster;


/***********************************************************************************************
 * SlaveServerClass::SlaveServerClass -- SlaveServerClass constuctor                           *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/21/2001 3:51PM ST : Created                                                            *
 *=============================================================================================*/
SlaveServerClass::SlaveServerClass(void)
{
	Enable = false;
	NickName[0] = 0;
	Serial[0] = 0;
	Port = 0;
	Bandwidth = 0;
	Password[0] = 0;
}


/***********************************************************************************************
 * SlaveServerClass::~SlaveServerClass -- SlaveServerClass desturctor                          *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/21/2001 3:51PM ST : Created                                                            *
 *=============================================================================================*/
SlaveServerClass::~SlaveServerClass(void)
{
}



/***********************************************************************************************
 * SlaveServerClass::Set -- Set info about this slave                                          *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Is slave enabled?                                                                 *
 *           Nickname to use with this slave                                                   *
 *           Serial number to use with this slave                                              *
 *           Port to use with this slave                                                       *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/21/2001 3:50PM ST : Created                                                            *
 *=============================================================================================*/
void SlaveServerClass::Set(bool enable, const char *nick, const char *serial, unsigned short port, const char *settings_file, int bandwidth, const char *password)
{
	Enable = enable;
	Port = port;
	Bandwidth = bandwidth;

	if (nick) {
		strncpy(NickName, nick, sizeof(NickName));
	}

	if (serial) {
		strncpy(Serial, serial, sizeof(Serial));
	}

	if (password) {
		strncpy(Password, password, sizeof(Password));
	}

	if (settings_file) {
		strncpy(SettingsFileName, settings_file, sizeof(SettingsFileName));
	}

}



/***********************************************************************************************
 * SlaveServerClass::Get -- Get info about this slave                                          *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Is slave enabled?                                                                 *
 *           Nickname to use with this slave                                                   *
 *           Serial number to use with this slave                                              *
 *           Port to use with this slave                                                       *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/21/2001 3:49PM ST : Created                                                            *
 *=============================================================================================*/
void SlaveServerClass::Get(bool &enable, char *nick, char *serial, unsigned short &port, char *settings_file, int &bandwidth, char *password)
{
	enable = Enable;
	port = Port;
	bandwidth = Bandwidth;

	if (nick) {
		strcpy(nick, NickName);
	}

	if (serial) {
		strcpy(serial, Serial);
	}

	if (password) {
		strcpy(password, Password);
	}

	if (settings_file) {
		strcpy(settings_file, SettingsFileName);
	}
}




/***********************************************************************************************
 * SlaveMasterClass::SlaveMasterClass -- SlaveMasterClass constructor                          *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/21/2001 3:49PM ST : Created                                                            *
 *=============================================================================================*/
SlaveMasterClass::SlaveMasterClass(void)
{
	NumSlaveServers = 0;
	SlaveMode = false;
}


/***********************************************************************************************
 * SlaveMasterClass::~SlaveMasterClass -- SlaveMasterClass destructor                          *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/21/2001 3:49PM ST : Created                                                            *
 *=============================================================================================*/
SlaveMasterClass::~SlaveMasterClass(void)
{
	/*
	** Make sure all slaves are gone before we quit.
	*/
	Wait_For_Slave_Shutdown();
}





/***********************************************************************************************
 * SlaveMasterClass::Wait_For_Slave_Shutdown -- Wait for slaves to exit                        *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   1/7/2002 2:58PM ST : Created                                                              *
 *=============================================================================================*/
void SlaveMasterClass::Wait_For_Slave_Shutdown(void)
{
	if (!SlaveMode) {

		unsigned int time = TIMEGETTIME();
		int num_running = 0;
		int last_num_running = 0;
		bool forced = false;

		/*
		** Don't wait longer than 35 secs. It takes 15 secs for the slave to do it's intermission.
		*/
		while (TIMEGETTIME() - time < 40000) {

			num_running = 0;

			for (int i=0 ; i<NumSlaveServers ; i++) {
				if (SlaveServers[i].IsRunning) {
					if (!SlaveServers[i].ProcessInfo->Wait(false)) {
						num_running++;
					}
				}
			}
			if (num_running && num_running != last_num_running) {
				ConsoleBox.Print("Waiting for %d slave(s) to shut down\n", num_running);
				WWDEBUG_SAY(("Waiting for %d slave(s) to shut down\n", num_running));
				last_num_running = num_running;
			}
			if (num_running == 0) {
				break;
			}

			/*
			** Force a shutdown if they are not cooperating.
			*/
			if (!forced && TIMEGETTIME() - time > 27000) {
				forced = true;
				for (int i=0 ; i<NumSlaveServers ; i++) {
					if (SlaveServers[i].ProcessInfo) {
						WWDEBUG_SAY(("Terminating process %d due to timeout\n", SlaveServers[i].ProcessInfo->Pid()));
						if (!SlaveServers[i].ProcessInfo->Kill()) {
							WWDEBUG_SAY(("Failed to get process handle for termination - error code %d\n", GetLastError()));
						}
						num_running++;
					}
				}
			}
		}
	}
}



/***********************************************************************************************
 * SlaveMasterClass::Get_Slave -- Get slave server entry by index                              *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Index                                                                             *
 *                                                                                             *
 * OUTPUT:   Ptr to slave server                                                               *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/21/2001 3:48PM ST : Created                                                            *
 *=============================================================================================*/
SlaveServerClass *SlaveMasterClass::Get_Slave(int index)
{
	WWASSERT(index < NumSlaveServers);
	WWASSERT(index >= 0);
	if (index >= 0 && index <NumSlaveServers) {
		return(&SlaveServers[index]);
	}
	return(NULL);
}



/***********************************************************************************************
 * SlaveMasterClass::Save -- Save slave server info to registry                                *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/21/2001 3:48PM ST : Created                                                            *
 *=============================================================================================*/
void SlaveMasterClass::Save(void)
{
	for (int i=0 ; i<NumSlaveServers ; i++) {
		auto & slave_ini = SlaveServers[i].Config;

		SlaveServers[i].Config.Put_String(APPLICATION_SUB_KEY_NAME_NET_SLAVE, KEY_SLAVE_NAME, SlaveServers[i].NickName);

		slave_ini.Put_String(APPLICATION_SUB_KEY_NAME_NET_SLAVE, KEY_SLAVE_PASSWORD, SlaveServers[i].Password);

		slave_ini.Put_String(APPLICATION_SUB_KEY_NAME_NET_SLAVE, KEY_SLAVE_SETTINGS, SlaveServers[i].SettingsFileName);

		slave_ini.Put_Bool(APPLICATION_SUB_KEY_NAME_NET_SLAVE, KEY_SLAVE_ENABLE, SlaveServers[i].Enable);

		slave_ini.Put_Int(APPLICATION_SUB_KEY_NAME_NET_SLAVE, KEY_SLAVE_PORT, SlaveServers[i].Port);

		slave_ini.Put_Int(APPLICATION_SUB_KEY_NAME_NET_SLAVE, KEY_SLAVE_BANDWIDTH, SlaveServers[i].Bandwidth);

		StringClass serial(SlaveServers[i].Serial, true);
		StringClass encrypted_serial = serial;
		if (serial.Get_Length()) {
			ServerSettingsClass::Encrypt_Serial(serial, encrypted_serial);
		}
		slave_ini.Put_String(APPLICATION_SUB_KEY_NAME_NET_SLAVE, KEY_SLAVE_SERIAL, encrypted_serial.Peek_Buffer());
	}
}



/***********************************************************************************************
 * SlaveMasterClass::Load -- Fetch slave server info from registry                             *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/21/2001 3:47PM ST : Created                                                            *
 *=============================================================================================*/
void SlaveMasterClass::Load(void)
{
	auto & ini = OpenW3D::Get_INIConfig();
	NumSlaveServers = ini.Get_Int(APPLICATION_SUB_KEY_NAME_NET_SLAVE, KEY_NUM_SLAVES, 0);

	for (int i=0 ; i<NumSlaveServers ; i++) {
		ini.Get_String(APPLICATION_SUB_KEY_NAME_NET_SLAVE, KEY_SLAVE_NAME, "", SlaveServers[i].NickName, sizeof(SlaveServers[i].NickName));

		ini.Get_String(APPLICATION_SUB_KEY_NAME_NET_SLAVE, KEY_SLAVE_PASSWORD, "", SlaveServers[i].Password, sizeof(SlaveServers[i].Password));

		SlaveServers[i].Enable = ini.Get_Bool(APPLICATION_SUB_KEY_NAME_NET_SLAVE, KEY_SLAVE_ENABLE, false);

		SlaveServers[i].Port = ini.Get_Int(APPLICATION_SUB_KEY_NAME_NET_SLAVE, KEY_SLAVE_PORT, false);

		SlaveServers[i].Bandwidth = ini.Get_Int(APPLICATION_SUB_KEY_NAME_NET_SLAVE, KEY_SLAVE_BANDWIDTH, false);

		ini.Get_String(APPLICATION_SUB_KEY_NAME_NET_SLAVE, KEY_SLAVE_SETTINGS, "", SlaveServers[i].SettingsFileName, sizeof(SlaveServers[i].SettingsFileName));

		ini.Get_String(APPLICATION_SUB_KEY_NAME_NET_SLAVE, KEY_SLAVE_SERIAL, "", SlaveServers[i].Serial, sizeof(SlaveServers[i].Serial));
		if (strlen(SlaveServers[i].Serial)) {
			StringClass serial(SlaveServers[i].Serial, true);
			StringClass decrypted_serial = serial;
			if (serial.Get_Length()) {
				ServerSettingsClass::Decrypt_Serial(serial, decrypted_serial);
			}
			strcpy(SlaveServers[i].Serial, decrypted_serial.Peek_Buffer());
		}

		char filename[MAX_PATH];
		sprintf(filename, "data/%s", SlaveServers[i].SettingsFileName);
		RawFileClass file(filename);
		if (!file.Is_Available()) {
			strcpy(SlaveServers[i].SettingsFileName, "svrcfg_cnc.ini");
		}
	}
}


/***********************************************************************************************
 * SlaveMasterClass::Reset -- Clear out slave server list                                      *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/21/2001 3:47PM ST : Created                                                            *
 *=============================================================================================*/
void SlaveMasterClass::Reset(void)
{
	for (int i=0 ; i<NumSlaveServers ; i++) {
		SlaveServers[i].Set(false, "", "", 0, "", 0, "");
	}
	NumSlaveServers = 0;
}




/***********************************************************************************************
 * SlaveMasterClass::Add_Slave -- Add slave to list                                            *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Is slave enabled?                                                                 *
 *           Nickname for this slave to use                                                    *
 *           Serial number for this slave to use                                               *
 *           Port number for this slave to use                                                 *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/21/2001 3:46PM ST : Created                                                            *
 *=============================================================================================*/
void SlaveMasterClass::Add_Slave(bool enable, const char *nick, const char *serial, unsigned short port, const char *settings_file, int bandwidth, const char *password)
{
	WWASSERT(NumSlaveServers < MAX_SLAVES);
	SlaveServers[NumSlaveServers++].Set(enable, nick, serial, port, settings_file, bandwidth, password);
}

/***********************************************************************************************
 * SlaveMasterClass::Startup_Slaves -- Create extra slave server processes                     *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/21/2001 3:46PM ST : Created                                                            *
 *=============================================================================================*/
void SlaveMasterClass::Startup_Slaves(void)
{
	if (!SlaveMode) {

		/*
		** Make sure we are a dedicated server.
		*/
		if (!The_Game() || The_Game()->IsDedicated.Is_True()) {

			/*
			** Slaves only available in windowed or console mode.
			*/
			if (WW3D::Is_Windowed() || ConsoleBox.Is_Exclusive()) {

				/*
				** Slaves only available in internet mode.
				*/
				GameModeClass *game_mode = GameModeManager::Find("WOL");
				if (game_mode && game_mode->Is_Active()) {

					Wait_For_Slave_Shutdown();

					Load();
					Create_Registry_Copies();

					/*
					** Spawn the servers.
					*/
					for (int i=0 ; i<NumSlaveServers ; i++) {
						if (SlaveServers[i].Enable) {

							/* Store INI config of slave to disk */
							SlaveServers[i].Config.Save(SlaveServers[i].ConfigPath.Peek_Buffer());

							/*
							** Figure out the name of the .exe to run.
							*/
							char path_to_exe[256];
							char drive[_MAX_DRIVE];
							char dir[_MAX_DIR];
							char path[_MAX_PATH];
							GetModuleFileNameA(ProgramInstance, path_to_exe, sizeof(path_to_exe));
							_splitpath(path_to_exe, drive, dir, NULL, NULL);
#ifdef FREEDEDICATEDSERVER
							_makepath(path, drive, dir, "renegadeserver", "exe");
#else  //FREEDEDICATEDSERVER
							_makepath(path, drive, dir, "renegade", "exe");
#endif //FREEDEDICATEDSERVER
							const char * args[] = {
								path,
								"--multi",
								"--slave",
								"--ini",
								SlaveServers[i].ConfigPath.Peek_Buffer(),
								nullptr,
								nullptr,
							};
							if (ConsoleBox.Is_Exclusive()) {
								args[5] = "--nodx";
							}
							SlaveServers[i].ProcessInfo = ProcessManager::Create_Process(args);
							if (SlaveServers[i].ProcessInfo) {
								SlaveServers[i].IsRunning = true;

								/*
								** Set a registry flag to say this server is active. We need to know this if the master server (us)
								** crashes and restarts.
								*/
								auto & ini = OpenW3D::Get_INIConfig();
								char entry[128];
								sprintf(entry, "%s%d", KEY_SLAVE_RUNNING_ID, i);
								ini.Put_Int(APPLICATION_SUB_KEY_NAME_NET_SLAVE, entry, SlaveServers[i].ProcessInfo->Pid());

							} else {
								WWDEBUG_SAY(("Failed to start slave process - error code %d\n", GetLastError()));
								SlaveServers[i].IsRunning = false;
							}
						}
					}
				}
			}
		}
	}
	GameSideServerControlClass::Set_Welcome_Message();
}





/***********************************************************************************************
 * SlaveMasterClass::Shutdown_Slaves -- Send quit message to all slaves                        *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/21/2001 3:45PM ST : Created                                                            *
 *=============================================================================================*/
void SlaveMasterClass::Shutdown_Slaves(void)
{
	if (!SlaveMode) {
		char password[64] = DEFAULT_SERVER_CONTROL_PASSWORD;
		auto & ini = OpenW3D::Get_INIConfig();
		ini.Get_String(APPLICATION_SUB_KEY_NAME_NET_SERVER_CONTROL, SERVER_CONTROL_PASSWORD_KEY, password, password, sizeof(password));

		for (int i=0 ; i<NumSlaveServers ; i++) {
			if (SlaveServers[i].IsRunning) {

				/*
				** Set the slaves auto-restart flag to false or it will just start right up again.
				*/
				ini.Put_Int(APPLICATION_SUB_KEY_NAME_WOLSETTINGS, AutoRestartClass::REG_VALUE_AUTO_RESTART_FLAG, 0);

				/*
				** Send the password to the slave to authenticate the connection.
				*/
				GameSideServerControlClass::Send_Message(password, ntohl(INADDR_LOOPBACK), SlaveServers[i].ControlPort);
				Sleep(10);
				GameSideServerControlClass::Send_Message("quit", ntohl(INADDR_LOOPBACK), SlaveServers[i].ControlPort);

				/*
				** Remember that we shut this guy down.
				*/
				char entry[128];
				sprintf(entry, "%s%d", KEY_SLAVE_RUNNING_ID, i);
				ini.Put_Int(APPLICATION_SUB_KEY_NAME_NET_SLAVE, entry, 0);
			}
		}
	}
	OpenW3D::Save_Config();
}






/***********************************************************************************************
 * SlaveMasterClass::Shutdown_Slaves -- Send quit message to all slaves                        *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Slave login name                                                                  *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/21/2001 3:45PM ST : Created                                                            *
 *=============================================================================================*/
bool SlaveMasterClass::Shutdown_Slave(char *slave_login)
{
	if (!SlaveMode && slave_login) {
		char password[64];

		auto & ini = OpenW3D::Get_INIConfig();
		ini.Get_String(APPLICATION_SUB_KEY_NAME_NET_SERVER_CONTROL, SERVER_CONTROL_PASSWORD_KEY, DEFAULT_SERVER_CONTROL_PASSWORD, password, sizeof(password));

		for (int i=0 ; i<NumSlaveServers ; i++) {
			if (SlaveServers[i].IsRunning && stricmp(slave_login, SlaveServers[i].NickName) == 0) {

				/*
				** Set the slaves auto-restart flag to false or it will just start right up again.
				*/
				INIClass slave_ini(SlaveServers[i].ConfigPath);
				slave_ini.Put_Int(APPLICATION_SUB_KEY_NAME_WOLSETTINGS, AutoRestartClass::REG_VALUE_AUTO_RESTART_FLAG, 0);
				(void)slave_ini.Save(SlaveServers[i].ConfigPath);

				/*
				** Send the password to the slave to authenticate the connection.
				*/
				GameSideServerControlClass::Send_Message(password, ntohl(INADDR_LOOPBACK), SlaveServers[i].ControlPort);
				Sleep(10);
				GameSideServerControlClass::Send_Message("quit", ntohl(INADDR_LOOPBACK), SlaveServers[i].ControlPort);

				/*
				** Remember that we shut this guy down.
				*/
				char entry[128];
				sprintf(entry, "%s%d", KEY_SLAVE_RUNNING_ID, i);
				ini.Put_Int(APPLICATION_SUB_KEY_NAME_NET_SLAVE, entry, 0);
				SlaveServers[i].IsRunning = false;
				return(true);
			}
		}
	}
	OpenW3D::Save_Config();
	return(false);
}




/***********************************************************************************************
 * SlaveMasterClass::Get_Slave_Info -- Get text slave info                                     *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Ptr to text buffer                                                                *
 *           buffer size                                                                       *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/21/2001 3:45PM ST : Created                                                            *
 *=============================================================================================*/
char *SlaveMasterClass::Get_Slave_Info(char *buffer, int buflen)
{
	bool any = false;
	if (buffer) {
		assert(buflen >= 500);
		*buffer = 0;

		for (int i=0 ; i<NumSlaveServers ; i++) {
			if (SlaveServers[i].IsRunning) {
				any = true;
				char temp[64];
				sprintf(temp, " Slave %d on port %d\n", i+1, SlaveServers[i].ControlPort);
				if (strlen(temp) + strlen(buffer) < (unsigned)buflen) {
					strcat(buffer, temp);
				}
			}
		}
		if (!any) {
			strcpy(buffer, "No slave servers active\n");
		}
	}
	return(buffer);
}




/***********************************************************************************************
 * SlaveMasterClass::Create_Registry_Copies -- Create 'shadow' registry copies for slaves      *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/21/2001 3:44PM ST : Created                                                            *
 *=============================================================================================*/
void SlaveMasterClass::Create_Registry_Copies(void)
{
	WWASSERT(!SlaveMode);

	auto & master_ini = OpenW3D::Get_INIConfig();

	// FIXME:
	// - Start child processes with cloned Config, written to temporary ini (perhaps remove the ini when the process finishes?)

	/*
	** Make sure the Process ID isn't set in our base registry. It's shouldn't be unless I ran with the --slave command during dev.
	*/
	master_ini.Remove_Entry(APPLICATION_SUB_KEY_NAME, "ProcessId");

	char new_path[1024];
	char slave_name[64];

	for (int i=0 ; i<NumSlaveServers ; i++) {
		if (SlaveServers[i].Enable) {
			/*
			** Construct temporary path to store INI for child process.
			*/
			char slave_config_path[L_tmpnam];
			std::tmpnam(slave_config_path);
			SlaveServers[i].ConfigPath = slave_config_path;

			/*
			** Copy the config for the new child process.
			*/
			SlaveServers[i].Config.Clear();
			master_ini.Copy_Into(SlaveServers[i].Config);
			auto & slave_ini = SlaveServers[i].Config;

			/*
			** Port numbers.
			*/
			{
				if (SlaveServers[i].Port != 0) {
					slave_ini.Put_Int(APPLICATION_SUB_KEY_NAME_NET_FIREWALL, "ForcePort", SlaveServers[i].Port);
				} else {
					slave_ini.Put_Int(APPLICATION_SUB_KEY_NAME_NET_FIREWALL, "ForcePort", 0);

					int port = slave_ini.Get_Int(APPLICATION_SUB_KEY_NAME_NET_FIREWALL, "PortBase", PORT_BASE_MIN);
					port = port + ((i+1) * 256);
					if (port >= PORT_BASE_MAX-1) {
						port -= (PORT_BASE_MAX - PORT_BASE_MIN);
					}
					slave_ini.Put_Int(APPLICATION_SUB_KEY_NAME_NET_FIREWALL, "PortBase", port);

					port = slave_ini.Get_Int(APPLICATION_SUB_KEY_NAME_NET_FIREWALL, "PortPool", PORT_BASE_MIN);
					port = port + ((i+1) * 1024);
					if (port >= PORT_POOL_MAX-1) {
						port -= (PORT_POOL_MAX - PORT_POOL_MIN);
					}
					slave_ini.Put_Int(APPLICATION_SUB_KEY_NAME_NET_FIREWALL, "PortPool", port);
				}
			}


			/*
			** Server control info.
			*/
			{
				/*
				** The password will be the same for all slaves but they each need a port to listen on.
				*/
				int my_sc_port = slave_ini.Get_Int(APPLICATION_SUB_KEY_NAME_NET_SERVER_CONTROL, SERVER_CONTROL_PORT_KEY, DEFAULT_SERVER_CONTROL_PORT);
				int slave_port = my_sc_port;
				if (my_sc_port == 0) {
					/*
					** If server control isn't enabled for me then we need to make up some port.
					*/
					slave_port = DEFAULT_SERVER_CONTROL_PORT;
				}
				slave_port += i;
				slave_port++;
				SlaveServers[i].ControlPort = slave_port;
				slave_ini.Put_Int(APPLICATION_SUB_KEY_NAME_NET_SERVER_CONTROL, SERVER_CONTROL_PORT_KEY, slave_port);

				/*
				** Inherit this from the master now.
				*/
				//if (my_sc_port == 0) {
				//	ini.Put_Int(APPLICATION_SUB_KEY_NAME_NET_SERVER_CONTROL, SERVER_CONTROL_LOOPBACK_KEY, 1);
				//} else {
				//	ini.Put_Int(APPLICATION_SUB_KEY_NAME_NET_SERVER_CONTROL, SERVER_CONTROL_LOOPBACK_KEY, 0);
				//}
			}

			/*
			** Login name.
			*/
			{
				slave_ini.Put_String(APPLICATION_SUB_KEY_NAME_WOLSETTINGS, "AutoLogin", SlaveServers[i].NickName);
				slave_ini.Put_String(APPLICATION_SUB_KEY_NAME_WOLSETTINGS, "LastLogin", SlaveServers[i].NickName);
			}

			/*
			** Password name.
			*/
			{
				slave_ini.Put_String(APPLICATION_SUB_KEY_NAME_WOLSETTINGS, "AutoPassword", SlaveServers[i].Password);
			}


			/*
			** Serial number.
			*/
			{
				StringClass serial(SlaveServers[i].Serial, true);
				StringClass encrypted_serial = serial;
				if (serial.Get_Length()) {
					ServerSettingsClass::Encrypt_Serial(serial, encrypted_serial);
				}
				slave_ini.Put_String(APPLICATION_SUB_KEY_NAME, KEY_SLAVE_SERIAL, encrypted_serial.Peek_Buffer());
			}

			/*
			** Make it autostart.
			*/
			{
				slave_ini.Put_Int(APPLICATION_SUB_KEY_NAME_WOLSETTINGS, AutoRestartClass::REG_VALUE_AUTO_RESTART_FLAG, 1);

				int game_type = 0;
				GameModeClass *game_mode = GameModeManager::Find("WOL");
				if (game_mode && game_mode->Is_Active()) {
					game_type = 1;
				}
				slave_ini.Put_Int(APPLICATION_SUB_KEY_NAME_WOLSETTINGS, AutoRestartClass::REG_VALUE_AUTO_RESTART_TYPE, game_type);
			}

			/*
			** Tell it which multiplayer settings to use.
			*/
			{
				slave_ini.Put_String(APPLICATION_SUB_KEY_NAME_OPTIONS, "MultiplayerSettings", SlaveServers[i].SettingsFileName);
			}

			/*
			** Set the SKU number to be the FDS SKU. Do this whether the Master is a FDS or not.
			*/
			{
				slave_ini.Put_Int(APPLICATION_SUB_KEY_NAME, "SKU", RENEGADE_FDS_SKU);
			}

			/*
			** Set the bandwidth information.
			** A value of 0 means auto. A value of 0xffffffff means not specified (i.e. use master settings).
			*/
			{
				int bw = SlaveServers[i].Bandwidth;
				if (bw != -1) {

					//ini.Put_Int(APPLICATION_SUB_KEY_NAME_NETOPTIONS, "BandwidthType", BANDWIDTH_AUTO);
					cUserOptions::Set_Bandwidth_Type(BANDWIDTH_AUTO);
					int slave_bw = bw;

					/*
					** If bandwidth is set to auto then divide it by the number of servers on this box.
					*/
					if (slave_bw == 0) {
						slave_bw = slave_ini.Get_Int(APPLICATION_SUB_KEY_NAME_BANDTEST, "Up", 0);
						int num = Get_Num_Enabled_Slaves();
						if (num) {
							slave_bw = slave_bw / (num+1);
						}
					}
					slave_ini.Put_Int(APPLICATION_SUB_KEY_NAME_BANDTEST, "Up", slave_bw);
					slave_ini.Put_Int(APPLICATION_SUB_KEY_NAME_BANDTEST, "Down", slave_bw);

					OpenW3D::Save_Config();
				}
			}


#if (0)
			/*
			** Give the window a different position so we are not completely overlapping.
			*/
			slave_ini.Put_Int(APPLICATION_SUB_KEY_NAME_OPTIONS, "WindowX", (i * 32) + 32);
			slave_ini.Put_Int(APPLICATION_SUB_KEY_NAME_OPTIONS, "WindowY", (i * 32) + 32);
#endif //(0)
		}
	}
}



/***********************************************************************************************
 * SlaveMasterClass::Get_Num_Enabled_Slaves -- How many slaves are enabled?                    *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Number of enabled slaves                                                          *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   1/5/2002 11:47PM ST : Created                                                             *
 *=============================================================================================*/
int SlaveMasterClass::Get_Num_Enabled_Slaves(void)
{
	int enabled = 0;

	for (int i=0 ; i<NumSlaveServers ; i++) {
		if (SlaveServers[i].Enable) {
			enabled++;
		}
	}
	return(enabled);
}
