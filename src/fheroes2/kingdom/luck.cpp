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

#include "gamedefs.h"
#include "luck.h"

std::string Luck::String(int luck)
{
    std::string str_luck[] = {
        "Unknown", _("luck|Cursed"), _("luck|Awful"), _("luck|Bad"), _("luck|Normal"),
        _("luck|Good"), _("luck|Great"), _("luck|Irish")
    };

    switch (luck)
    {
    case CURSED:
        return str_luck[1];
    case AWFUL:
        return str_luck[2];
    case BAD:
        return str_luck[3];
    case NORMAL:
        return str_luck[4];
    case GOOD:
        return str_luck[5];
    case GREAT:
        return str_luck[6];
    case IRISH:
        return str_luck[7];
    default:
        break;
    }

    return str_luck[0];
}

std::string Luck::Description(int luck)
{
    std::string str_desc_luck[] = {
        "Unknown",
        _("Bad luck sometimes falls on your armies in combat, causing their attacks to only do half damage."),
        _("Neutral luck means your armies will never get lucky or unlucky attacks on the enemy."),
        _("Good luck sometimes lets your armies get lucky attacks (double strength) in combat.")
    };

    switch (luck)
    {
    case CURSED:
    case AWFUL:
    case BAD:
        return str_desc_luck[1];
    case NORMAL:
        return str_desc_luck[2];
    case GOOD:
    case GREAT:
    case IRISH:
        return str_desc_luck[3];
    default:
        break;
    }

    return str_desc_luck[0];
}
