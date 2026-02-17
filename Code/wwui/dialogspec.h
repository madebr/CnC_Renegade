/*
**	Command & Conquer Renegade(tm)
**	Copyright 2025 OpenW3D
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

#pragma once

#include "unichar.h"
#include <cstdlib>

enum DialogResourceControlType {
    CONTROL_INVALID = -1,
    CONTROL_BUTTON	= 0x0080,
    CONTROL_EDIT,
    CONTROL_STATIC,
    CONTROL_LIST_BOX,
    CONTROL_SCROLL_BAR,
    CONTROL_COMBOBOX,
    CONTROL_SLIDER,
    CONTROL_LIST_CTRL,
    CONTROL_TAB,
    CONTROL_MAP,
    CONTROL_VIEWER,
    CONTROL_HOTKEY,
    CONTROL_SHORTCUT_BAR,
    CONTROL_MERCHANDISE_CTRL,
    CONTROL_TREE_CTRL,
    CONTROL_PROGRESS_BAR,
    CONTROL_HEALTH_BAR
};

#ifndef _WIN32
// FIXME: make DialogResourceControl strongly typed (std::variant?)
#define BS_AUTOCHECKBOX 0x00000003
#define BS_BITMAP 0x00000080
#define BS_CENTER 0x00000300
#define BS_DEFPUSHBUTTON 0x00000001
#define BS_FLAT 0x00008000
#define BS_LEFT 0x00000100
#define BS_OWNERDRAW 0x0000000B
#define BS_PUSHBUTTON 0x00000000
#define CBS_DROPDOWN 0x0002
#define CBS_DROPDOWNLIST 0x0003
#define CBS_OEMCONVERT 0x0080
#define CBS_SORT 0x0100
#define ES_AUTOHSCROLL 0x0080
#define ES_AUTOVSCROLL 0x0040
#define ES_CENTER 0x0001
#define ES_MULTILINE 0x0004
#define ES_NUMBER 0x2000
#define ES_OEMCONVERT 0x0400
#define ES_PASSWORD 0x0020
#define LVS_ALIGNTOP 0x0
#define LVS_AUTOARRANGE 0x100
#define LVS_NOCOLUMNHEADER 0x4000
#define LVS_NOSORTHEADER 0x8000
#define LVS_REPORT 0x1
#define LVS_SHOWSELALWAYS 0x8
#define LVS_SINGLESEL 0x4
#define LVS_SORTASCENDING 0x10
#define PBS_SMOOTH 0x1
#define SS_BITMAP 0x0000000E
#define SS_BLACKFRAME 0
#define SS_CENTER 0x00000001
#define SS_CENTERIMAGE 0x00000200
#define SS_ETCHEDHORZ 0x00000010
#define SS_LEFT 0x00000000
#define SS_LEFTNOWORDWRAP 0x0000000C
#define SS_REALSIZEIMAGE 0x00000800
#define SS_RIGHT 0x00000002
#define TBS_AUTOTICKS 0x1
#define TBS_BOTH 0x00000007
#define TBS_BOTTOM 0x0
#define TBS_HORZ 0x0
#define TBS_NOTICKS 0x10
#define TBS_RIGHT 0x0
#define WS_CAPTION 0x00C00000
#define WS_CHILD 0x40000000
#define WS_DISABLED 0x08000000
#define WS_GROUP 0x00020000
#define WS_TABSTOP 0x00010000
#define WS_VISIBLE 0x10000000
#define WS_VSCROLL 0x00200000
#endif

typedef struct DialogResourceControl {
    DialogResourceControlType type;
    int x;
    int y;
    int cx;
    int cy;
    int id;
    int style;
    const unichar_t *text;
} DialogResourceControl;

typedef struct DialogResource {
    int id;
    int x;
    int y;
    int cx;
    int cy;
    const unichar_t *caption;
    const DialogResourceControl *controls;
    size_t count_controls;
} DialogResource;
