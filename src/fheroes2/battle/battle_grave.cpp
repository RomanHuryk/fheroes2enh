/***************************************************************************
 *   Copyright (C) 2012 by Andrey Afletdinov <fheroes2@gmail.com>          *
 *                                                                         *
 *   Part of the Free Heroes2 Engine:                                      *
 *   http://sourceforge.net/projects/fheroes2                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <algorithm>
#include "battle_troop.h"

Battle::Indexes Battle::Graveyard::GetClosedCells() const
{
    Indexes res;
    res.reserve(size());

    for (const auto& it : *this)
        res.push_back(it.first);

    return res;
}

void Battle::Graveyard::AddTroop(const Unit& b)
{
    Graveyard& map = *this;

    map[b.GetHeadIndex()].push_back(b.GetUID());

    if (b.isWide())
        map[b.GetTailIndex()].push_back(b.GetUID());
}

void Battle::Graveyard::RemoveTroop(const Unit& b)
{
    Graveyard& map = *this;
    TroopUIDs& ids = map[b.GetHeadIndex()];

    auto it = std::find(ids.begin(), ids.end(), b.GetUID());
    if (it != ids.end()) ids.erase(it);

    if (!b.isWide()) return;
    TroopUIDs& ids2 = map[b.GetTailIndex()];

    it = std::find(ids2.begin(), ids2.end(), b.GetUID());
    if (it != ids2.end()) ids2.erase(it);
}

uint32_t Battle::Graveyard::GetLastTroopUID(s32 index) const
{
    for (auto troop : *this)
        if (index == troop.first && !troop.second.empty())
            return troop.second.back();

    return 0;
}
