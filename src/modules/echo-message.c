/*
 *   IRC - Internet Relay Chat, src/modules/echo-message.c
 *   (C) 2019 Syzop & The UnrealIRCd Team
 *
 *   See file AUTHORS in IRC package for additional names of
 *   the programmers.
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 1, or (at your option)
 *   any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "unrealircd.h"

ModuleHeader MOD_HEADER
  = {
	"echo-message",
	"5.0",
	"Batch CAP", 
	"UnrealIRCd Team",
	"unrealircd-5",
	};

/* Variables */
long CAP_ECHO_MESSAGE = 0L;

/* Forward declarations */
int em_chanmsg(Client *client, Channel *channel, int sendflags, int prefix, char *target, MessageTag *mtags, char *text, int notice);
int em_usermsg(Client *client, Client *to, MessageTag *mtags, char *text, int notice);

MOD_INIT()
{
	ClientCapabilityInfo cap;

	MARK_AS_OFFICIAL_MODULE(modinfo);

	memset(&cap, 0, sizeof(cap));
	cap.name = "echo-message";
	ClientCapabilityAdd(modinfo->handle, &cap, &CAP_ECHO_MESSAGE);

	HookAdd(modinfo->handle, HOOKTYPE_CHANMSG, 0, em_chanmsg);
	HookAdd(modinfo->handle, HOOKTYPE_USERMSG, 0, em_usermsg);

	return MOD_SUCCESS;
}

MOD_LOAD()
{
	return MOD_SUCCESS;
}

MOD_UNLOAD()
{
	return MOD_SUCCESS;
}

int em_chanmsg(Client *client, Channel *channel, int sendflags, int prefix, char *target, MessageTag *mtags, char *text, int notice)
{
	if (MyUser(client) && HasCapabilityFast(client, CAP_ECHO_MESSAGE))
	{
		sendto_prefix_one(client, client, mtags, ":%s %s %s :%s",
			client->name,
			notice ? "NOTICE" : "PRIVMSG",
			target,
			text);
	}
	return 0;
}

int em_usermsg(Client *client, Client *to, MessageTag *mtags, char *text, int notice)
{
	if (MyUser(client) && HasCapabilityFast(client, CAP_ECHO_MESSAGE))
	{
		sendto_prefix_one(client, client, mtags, ":%s %s %s :%s",
			client->name,
			notice ? "NOTICE" : "PRIVMSG",
			to->name,
			text);
	}
	return 0;
}
