/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2015  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RESOURCES_NPCDIALOGINFO_H
#define RESOURCES_NPCDIALOGINFO_H

#include "resources/npcbuttoninfo.h"

#include "utils/stringvector.h"

#include "localconsts.h"

struct NpcDialogInfo final
{
    NpcDialogInfo() :
        buttons(),
        name()
    {
    }

    std::vector<NpcButtonInfo*> buttons;
    std::string name;
};

#endif  // RESOURCES_NPCDIALOGINFO_H