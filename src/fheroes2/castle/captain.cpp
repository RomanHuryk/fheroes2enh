/***************************************************************************
 *   Copyright (C) 2009 by Andrey Afletdinov <fheroes2@gmail.com>          *
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

#include "castle.h"
#include "luck.h"
#include "morale.h"
#include "race.h"
#include "agg.h"
#include "icn.h"

Captain::Captain(Castle& cstl) : HeroBase(CAPTAIN, cstl.GetRace()), home(cstl)
{
    SetCenter(home.GetCenter());
}

bool Captain::isValid() const
{
    return home.isBuild(BUILD_CAPTAIN);
}

int Captain::GetAttack() const
{
    return attack + GetAttackModificator(nullptr);
}

int Captain::GetDefense() const
{
    return defense + GetDefenseModificator(nullptr);
}

int Captain::GetPower() const
{
    return power + GetPowerModificator(nullptr);
}

int Captain::GetKnowledge() const
{
    return knowledge + GetKnowledgeModificator(nullptr);
}

int Captain::GetMorale() const
{
    int result = Morale::NORMAL;

    // global modificator
    result += GetMoraleModificator(nullptr);

    // result
    if (result < Morale::AWFUL) return Morale::TREASON;
    if (result < Morale::POOR) return Morale::AWFUL;
    if (result < Morale::NORMAL) return Morale::POOR;
    if (result < Morale::GOOD) return Morale::NORMAL;
    if (result < Morale::GREAT) return Morale::GOOD;
    if (result < Morale::BLOOD) return Morale::GREAT;

    return Morale::BLOOD;
}

int Captain::GetLuck() const
{
    int result = Luck::NORMAL;

    // global modificator
    result += GetLuckModificator(nullptr);

    // result
    if (result < Luck::AWFUL) return Luck::CURSED;
    if (result < Luck::BAD) return Luck::AWFUL;
    if (result < Luck::NORMAL) return Luck::BAD;
    if (result < Luck::GOOD) return Luck::NORMAL;
    if (result < Luck::GREAT) return Luck::GOOD;
    if (result < Luck::IRISH) return Luck::GREAT;

    return Luck::IRISH;
}

int Captain::GetRace() const
{
    return home.GetRace();
}

int Captain::GetColor() const
{
    return home.GetColor();
}

const string& Captain::GetName() const
{
    return home.GetName();
}

int Captain::GetType() const
{
    return CAPTAIN;
}

int Captain::GetLevelSkill(int) const
{
    return 0;
}

uint32_t Captain::GetSecondaryValues(int) const
{
    return 0;
}

const Army& Captain::GetArmy() const
{
    return home.GetArmy();
}

Army& Captain::GetArmy()
{
    return home.GetArmy();
}

uint32_t Captain::GetMaxSpellPoints() const
{
    return knowledge * 10;
}

int Captain::GetControl() const
{
    return home.GetControl();
}

s32 Captain::GetIndex() const
{
    return home.GetIndex();
}

void Captain::ActionAfterBattle()
{
    SetSpellPoints(GetMaxSpellPoints());
}

void Captain::ActionPreBattle()
{
    SetSpellPoints(GetMaxSpellPoints());
}

const Castle* Captain::inCastle() const
{
    return &home;
}

Surface Captain::GetPortrait(int type) const
{
    switch (type)
    {
    case PORT_BIG:
        switch (GetRace())
        {
        case Race::KNGT:
            return AGG::GetICN(ICN::PORT0090, 0);
        case Race::BARB:
            return AGG::GetICN(ICN::PORT0091, 0);
        case Race::SORC:
            return AGG::GetICN(ICN::PORT0092, 0);
        case Race::WRLK:
            return AGG::GetICN(ICN::PORT0093, 0);
        case Race::WZRD:
            return AGG::GetICN(ICN::PORT0094, 0);
        case Race::NECR:
            return AGG::GetICN(ICN::PORT0095, 0);
        default:
            break;
        }
        break;

    case PORT_MEDIUM:
    case PORT_SMALL:
        switch (GetRace())
        {
        case Race::KNGT:
            return AGG::GetICN(ICN::MINICAPT, 0);
        case Race::BARB:
            return AGG::GetICN(ICN::MINICAPT, 1);
        case Race::SORC:
            return AGG::GetICN(ICN::MINICAPT, 2);
        case Race::WRLK:
            return AGG::GetICN(ICN::MINICAPT, 3);
        case Race::WZRD:
            return AGG::GetICN(ICN::MINICAPT, 4);
        case Race::NECR:
            return AGG::GetICN(ICN::MINICAPT, 5);
        default:
            break;
        }
        break;
    }

    return Surface();
}

void Captain::PortraitRedraw(s32 px, s32 py, int type, Surface& dstsf) const
{
    GetPortrait(type).Blit(px, py, dstsf);
}
