/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_TABS_SETUP_PLAYERS_H
#define GUI_WIDGETS_TABS_SETUP_PLAYERS_H

#include "gui/widgets/tabs/setuptabscroll.h"

class NamesModel;

class Setup_Players final : public SetupTabScroll
{
    public:
        explicit Setup_Players(const Widget2 *const widget);

        ~Setup_Players();

        A_DELETE_COPY(Setup_Players)

        void apply() override final;

        void externalUpdated() override final;

    private:
        NamesModel *mBadgesList;
};

#endif  // GUI_WIDGETS_TABS_SETUP_PLAYERS_H
