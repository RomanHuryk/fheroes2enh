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

#include "heroes_base.h"
#include "race.h"
#include "army.h"
#include "game_static.h"
#include "rand.h"

enum
{
    SP_DISABLE = 0x01
};

struct spellstats_t
{
    std::string name;
    u8 sp;
    u16 mp;
    u8 sprite;
    u8 extra;
    u8 bits;
    cost_t cost;
    std::string description;
};

spellstats_t spells[] = {
    //  name                      sp   mp  spr value  bits cost     description
    {"Unknown", 0, 0, 0, 0, 0, COST_NONE, "Unknown spell."},
    {
        _("Fireball"), 9, 0, 8, 10, 0, COST_NONE,
        _("Causes a giant fireball to strike the selected area, damaging all nearby creatures.")
    },
    {
        _("Fireblast"), 15, 0, 9, 10, 0, COST_NONE,
        _("An improved version of fireball, fireblast affects two hexes around the center point of the spell, rather than one."
        )
    },
    {
        _("Lightning Bolt"), 7, 0, 4, 25, 0, COST_NONE,
        _("Causes a bolt of electrical energy to strike the selected creature.")
    },
    {
        _("Chain Lightning"), 15, 0, 5, 40, 0, COST_NONE,
        _(
            "Causes a bolt of electrical energy to strike a selected creature, then strike the nearest creature with half damage, then strike the NEXT nearest creature with half again damage, and so on, until it becomes too weak to be harmful.  Warning:  This spell can hit your own creatures!"
        )
    },
    {
        _("Teleport"), 9, 0, 10, 0, 0, COST_NONE,
        _("Teleports the creature you select to any open position on the battlefield.")
    },
    {
        _("Cure"), 6, 0, 6, 5, 0, COST_NONE,
        _("Removes all negative spells cast upon one of your units, and restores up to %{count} HP per level of spell power."
        )
    },
    {
        _("Mass Cure"), 15, 0, 2, 5, 0, COST_NONE,
        _("Removes all negative spells cast upon your forces, and restores up to %{count} HP per level of spell power, per creature."
        )
    },
    {
        _("Resurrect"), 12, 0, 13, 50, 0, COST_NONE,
        _("Resurrects creatures from a damaged or dead unit until end of combat.")
    },
    {
        _("Resurrect True"), 15, 0, 12, 50, 0, COST_NONE,
        _("Resurrects creatures from a damaged or dead unit permanently.")
    },
    {_("Haste"), 3, 0, 14, 0, 0, COST_NONE, _("Increases the speed of any creature by %{count}.")},
    {_("Mass Haste"), 10, 0, 14, 0, 0, COST_NONE, _("Increases the speed of all of your creatures by %{count}.")},
    {_("spell|Slow"), 3, 0, 1, 0, 0, COST_NONE, _("Slows target to half movement rate.")},
    {_("Mass Slow"), 15, 0, 1, 0, 0, COST_NONE, _("Slows all enemies to half movement rate.")},
    //
    {_("Blind "), 6, 0, 21, 0, 0, COST_NONE, _("Clouds the affected creatures' eyes, preventing them from moving.")},
    {_("Bless"), 3, 0, 7, 0, 0, COST_NONE, _("Causes the selected creatures to inflict maximum damage.")},
    {_("Mass Bless"), 12, 0, 7, 0, 0, COST_NONE, _("Causes all of your units to inflict maximum damage.")},
    {_("Stoneskin"), 3, 0, 31, 3, 0, COST_NONE, _("Magically increases the defense skill of the selected creatures.")},
    {
        _("Steelskin"), 6, 0, 30, 5, 0, COST_NONE,
        _("Increases the defense skill of the targeted creatures.  This is an improved version of Stoneskin.")
    },
    {_("Curse"), 3, 0, 3, 0, 0, COST_NONE, _("Causes the selected creatures to inflict minimum damage.")},
    {_("Mass Curse"), 12, 0, 3, 0, 0, COST_NONE, _("Causes all enemy troops to inflict minimum damage.")},
    {_("Holy Word"), 9, 0, 22, 10, 0, COST_NONE, _("Damages all undead in the battle.")},
    {
        _("Holy Shout"), 12, 0, 23, 20, 0, COST_NONE,
        _("Damages all undead in the battle.  This is an improved version of Holy Word.")
    },
    {_("Anti-Magic"), 7, 0, 17, 0, 0, COST_NONE, _("Prevents harmful magic against the selected creatures.")},
    {_("Dispel Magic"), 5, 0, 18, 0, 0, COST_NONE, _("Removes all magic spells from a single target.")},
    {_("Mass Dispel"), 12, 0, 18, 0, 0, COST_NONE, _("Removes all magic spells from all creatures.")},
    {_("Magic Arrow"), 3, 0, 38, 10, 0, COST_NONE, _("Causes a magic arrow to strike the selected target.")},
    {_("Berserker"), 12, 0, 19, 0, 0, COST_NONE, _("Causes a creature to attack its nearest neighbor.")},
    {
        _("Armageddon"), 20, 0, 16, 50, 0, COST_NONE,
        _("Holy terror strikes the battlefield, causing severe damage to all creatures.")
    },
    {
        _("Elemental Storm"), 15, 0, 11, 25, 0, COST_NONE,
        _("Magical elements pour down on the battlefield, damaging all creatures.")
    },
    {
        _("Meteor Shower"), 15, 0, 24, 25, 0, COST_NONE,
        _("A rain of rocks strikes an area of the battlefield, damaging all nearby creatures.")
    },
    {_("Paralyze"), 9, 0, 20, 0, 0, COST_NONE, _("The targeted creatures are paralyzed, unable to move or retaliate.")},
    {
        _("Hypnotize"), 15, 0, 37, 25, 0, COST_NONE,
        _("Brings a single enemy unit under your control for one combat round if its hits are less than %{count} times the caster's spell power."
        )
    },
    {_("Cold Ray"), 6, 0, 36, 20, 0, COST_NONE, _("Drains body heat from a single enemy unit.")},
    {
        _("Cold Ring"), 9, 0, 35, 10, 0, COST_NONE,
        _("Drains body heat from all units surrounding the center point, but not including the center point.")
    },
    {_("Disrupting Ray"), 7, 0, 34, 3, 0, COST_NONE, _("Reduces the defense rating of an enemy unit by three.")},
    {_("Death Ripple"), 6, 0, 28, 5, 0, COST_NONE, _("Damages all living (non-undead) units in the battle.")},
    {
        _("Death Wave"), 10, 0, 29, 10, 0, COST_NONE,
        _("Damages all living (non-undead) units in the battle.  This spell is an improved version of Death Ripple.")
    },
    {_("Dragon Slayer"), 6, 0, 32, 5, 0, COST_NONE, _("Greatly increases a unit's attack skill vs. Dragons.")},
    {_("Blood Lust"), 3, 0, 27, 3, 0, COST_NONE, _("Increases a unit's attack skill.")},
    {
        _("Animate Dead"), 10, 0, 25, 50, 0, COST_NONE,
        _("Resurrects creatures from a damaged or dead undead unit permanently.")
    },
    {
        _("Mirror Image"), 25, 0, 26, 0, 0, COST_NONE,
        _("Creates an illusionary unit that duplicates one of your existing units.  This illusionary unit does the same damages as the original, but will vanish if it takes any damage."
        )
    },
    {_("Shield"), 3, 0, 15, 2, 0, COST_NONE, _("Halves damage received from ranged attacks for a single unit.")},
    {
        _("Mass Shield"), 7, 0, 15, 0, 0, COST_NONE,
        _("Halves damage received from ranged attacks for all of your units.")
    },
    {_("Summon Earth Elemental"), 30, 0, 56, 3, 0, COST_NONE, _("Summons Earth Elementals to fight for your army.")},
    {_("Summon Air Elemental"), 30, 0, 57, 3, 0, COST_NONE, _("Summons Air Elementals to fight for your army.")},
    {_("Summon Fire Elemental"), 30, 0, 58, 3, 0, COST_NONE, _("Summons Fire Elementals to fight for your army.")},
    {_("Summon Water Elemental"), 30, 0, 59, 3, 0, COST_NONE, _("Summons Water Elementals to fight for your army.")},
    {_("Earthquake"), 15, 0, 33, 0, 0, COST_NONE, _("Damages castle walls.")},
    {_("View Mines"), 1, 0, 39, 0, 0, COST_NONE, _("Causes all mines across the land to become visible.")},
    {_("View Resources"), 1, 0, 40, 0, 0, COST_NONE, _("Causes all resources across the land to become visible.")},
    {_("View Artifacts"), 2, 0, 41, 0, 0, COST_NONE, _("Causes all artifacts across the land to become visible.")},
    {_("View Towns"), 2, 0, 42, 0, 0, COST_NONE, _("Causes all towns and castles across the land to become visible.")},
    {_("View Heroes"), 2, 0, 43, 0, 0, COST_NONE, _("Causes all Heroes across the land to become visible.")},
    {_("View All"), 3, 0, 44, 0, 0, COST_NONE, _("Causes the entire land to become visible.")},
    {
        _("Identify Hero"), 3, 0, 45, 0, 0, COST_NONE,
        _("Allows the caster to view detailed information on enemy Heroes.")
    },
    {
        _("Summon Boat"), 5, 0, 46, 0, 0, COST_NONE,
        _("Summons the nearest unoccupied, friendly boat to an adjacent shore location.  A friendly boat is one which you just built or were the most recent player to occupy."
        )
    },
    {
        _("Dimension Door"), 10, 0, 47, 0, 0, COST_NONE,
        _("Allows the caster to magically transport to a nearby location.")
    },
    {_("Town Gate"), 10, 0, 48, 0, 0, COST_NONE, _("Returns the caster to any town or castle currently owned.")},
    {
        _("Town Portal"), 20, 0, 49, 0, 0, COST_NONE,
        _("Returns the hero to the town or castle of choice, provided it is controlled by you.")
    },
    {
        _("Visions"), 6, 0, 50, 3, 0, COST_NONE,
        _("Visions predicts the likely outcome of an encounter with a neutral army camp.")
    },
    {
        _("Haunt"), 8, 0, 51, 4, 0, COST_NONE,
        _("Haunts a mine you control with Ghosts.  This mine stops producing resources.  (If I can't keep it, nobody will!)"
        )
    },
    {
        _("Set Earth Guardian"), 15, 0, 52, 4, 0, COST_NONE,
        _("Sets Earth Elementals to guard a mine against enemy armies.")
    },
    {_("Set Air Guardian"), 15, 0, 53, 4, 0, COST_NONE, _("Sets Air Elementals to guard a mine against enemy armies.")},
    {
        _("Set Fire Guardian"), 15, 0, 54, 4, 0, COST_NONE,
        _("Sets Fire Elementals to guard a mine against enemy armies.")
    },
    {
        _("Set Water Guardian"), 15, 0, 55, 4, 0, COST_NONE,
        _("Sets Water Elementals to guard a mine against enemy armies.")
    },
    {"Random", 0, 0, 0, 0, 0, COST_NONE, "Random"},
    {"Random 1", 0, 0, 0, 0, 0, COST_NONE, "Random 1"},
    {"Random 2", 0, 0, 0, 0, 0, COST_NONE, "Random 2"},
    {"Random 3", 0, 0, 0, 0, 0, COST_NONE, "Random 3"},
    {"Random 4", 0, 0, 0, 0, 0, COST_NONE, "Random 4"},
    {"Random 5", 0, 0, 0, 0, 0, COST_NONE, "Random 5"},
    {"Stone", 0, 0, 0, 0, 0, COST_NONE, "Stone spell from Medusa."},
};

void Spell::UpdateStats(const string& spec)
{
}

Spell::Spell(int s) : id(s > STONE ? NONE : s)
{
}

bool Spell::operator<(const Spell& s) const
{
    return id < s.id;
}

bool Spell::operator==(const Spell& s) const
{
    return s.id == id;
}

bool Spell::operator!=(const Spell& s) const
{
    return s.id != id;
}

bool Spell::isValid() const
{
    return id != NONE;
}

int Spell::operator()() const
{
    return id;
}

int Spell::GetID() const
{
    return id;
}

std::string Spell::GetName() const
{
    return _(spells[id].name);
}

std::string Spell::GetDescription() const
{
    return _(spells[id].description);
}

uint32_t Spell::MovePoint() const
{
    return spells[id].mp;
}

uint32_t Spell::SpellPoint(const HeroBase* hero) const
{
    uint32_t res = spells[id].sp;
    uint32_t acount = 0;

    if (hero)
    {
        switch (id)
        {
        case BLESS:
        case MASSBLESS:
            acount = hero->HasArtifact(Artifact::SNAKE_RING);
            if (acount)
                res = spells[id].sp / (acount * 2);
            break;

        case SUMMONEELEMENT:
        case SUMMONAELEMENT:
        case SUMMONFELEMENT:
        case SUMMONWELEMENT:
            acount = hero->HasArtifact(Artifact::ELEMENTAL_RING);
            if (acount)
                res = spells[id].sp / (acount * 2);
            break;

        case CURSE:
        case MASSCURSE:
            acount = hero->HasArtifact(Artifact::EVIL_EYE);
            if (acount)
                res = spells[id].sp / (acount * 2);
            break;

        default:
            break;
        }

        if (isMindInfluence())
        {
            acount = hero->HasArtifact(Artifact::SKULLCAP);
            if (acount)
                res = spells[id].sp / (acount * 2);
        }
    }

    return res ? res : 1;
}

payment_t Spell::GetCost() const
{
    return payment_t(spells[id].cost);
}

bool Spell::isLevel(int lvl) const
{
    return Level() == lvl;
}

int Spell::Level() const
{
    switch (id)
    {
    case BLESS:
    case BLOODLUST:
    case CURE:
    case CURSE:
    case DISPEL:
    case HASTE:
    case ARROW:
    case SHIELD:
    case SLOW:
    case STONESKIN:

    case VIEWMINES:
    case VIEWRESOURCES:
        return 1;

    case BLIND:
    case COLDRAY:
    case DEATHRIPPLE:
    case DISRUPTINGRAY:
    case DRAGONSLAYER:
    case LIGHTNINGBOLT:
    case STEELSKIN:

    case HAUNT:
    case SUMMONBOAT:
    case VIEWARTIFACTS:
    case VISIONS:
        return 2;

    case ANIMATEDEAD:
    case ANTIMAGIC:
    case COLDRING:
    case DEATHWAVE:
    case EARTHQUAKE:
    case FIREBALL:
    case HOLYWORD:
    case MASSBLESS:
    case MASSCURSE:
    case MASSDISPEL:
    case MASSHASTE:
    case PARALYZE:
    case TELEPORT:

    case IDENTIFYHERO:
    case VIEWHEROES:
    case VIEWTOWNS:
        return 3;

    case BERSERKER:
    case CHAINLIGHTNING:
    case ELEMENTALSTORM:
    case FIREBLAST:
    case HOLYSHOUT:
    case MASSCURE:
    case MASSSHIELD:
    case MASSSLOW:
    case METEORSHOWER:
    case RESURRECT:

    case SETEGUARDIAN:
    case SETAGUARDIAN:
    case SETFGUARDIAN:
    case SETWGUARDIAN:
    case TOWNGATE:
    case VIEWALL:
        return 4;

    case ARMAGEDDON:
    case HYPNOTIZE:
    case MIRRORIMAGE:
    case RESURRECTTRUE:
    case SUMMONEELEMENT:
    case SUMMONAELEMENT:
    case SUMMONFELEMENT:
    case SUMMONWELEMENT:

    case DIMENSIONDOOR:
    case TOWNPORTAL:
        return 5;

    default:
        break;
    }

    return 0;
}

bool Spell::isCombat() const
{
    switch (id)
    {
    case NONE:
    case VIEWMINES:
    case VIEWRESOURCES:
    case VIEWARTIFACTS:
    case VIEWTOWNS:
    case VIEWHEROES:
    case VIEWALL:
    case IDENTIFYHERO:
    case SUMMONBOAT:
    case DIMENSIONDOOR:
    case TOWNGATE:
    case TOWNPORTAL:
    case VISIONS:
    case HAUNT:
    case SETEGUARDIAN:
    case SETAGUARDIAN:
    case SETFGUARDIAN:
    case SETWGUARDIAN:
        return false;
    default:
        break;
    }
    return true;
}

bool Spell::isAdventure() const
{
    return !isCombat();
}

bool Spell::isDamage() const
{
    return Damage();
}

uint32_t Spell::Damage() const
{
    switch (id)
    {
    case ARROW:
    case FIREBALL:
    case FIREBLAST:
    case LIGHTNINGBOLT:
    case COLDRING:
    case DEATHWAVE:
    case HOLYWORD:
    case CHAINLIGHTNING:
    case ARMAGEDDON:
    case ELEMENTALSTORM:
    case METEORSHOWER:
    case COLDRAY:
    case HOLYSHOUT:
    case DEATHRIPPLE:
        return spells[id].extra;

    default:
        break;
    }

    return 0;
}

bool Spell::isMindInfluence() const
{
    switch (id)
    {
    case BLIND:
    case PARALYZE:
    case BERSERKER:
    case HYPNOTIZE:
        return true;

    default:
        break;
    }

    return false;
}

uint32_t Spell::IndexSprite() const
{
    return spells[id].sprite;
}

uint32_t Spell::InlIndexSprite() const
{
    switch (id)
    {
    case HASTE:
    case MASSHASTE:
        return 0;
    case SLOW:
    case MASSSLOW:
        return 1;
    case BLIND:
        return 2;
    case BLESS:
    case MASSBLESS:
        return 3;
    case CURSE:
    case MASSCURSE:
        return 4;
    case BERSERKER:
        return 5;
    case PARALYZE:
        return 6;
    case HYPNOTIZE:
        return 7;
    case DRAGONSLAYER:
        return 8;
    case BLOODLUST:
        return 9;
    case SHIELD:
    case MASSSHIELD:
        return 10;
    case STONE:
        return 11;
    case ANTIMAGIC:
        return 12;
    case STONESKIN:
        return 13;
    case STEELSKIN:
        return 14;
    default:
        break;
    }

    return 0;
}

uint32_t Spell::Restore() const
{
    switch (id)
    {
    case CURE:
    case MASSCURE:
        return spells[id].extra;

    default:
        break;
    }

    return Resurrect();
}

uint32_t Spell::Resurrect() const
{
    switch (id)
    {
    case ANIMATEDEAD:
    case RESURRECT:
    case RESURRECTTRUE:
        return spells[id].extra;

    default:
        break;
    }

    return 0;
}

bool Spell::isRestore() const
{
    return Restore();
}

bool Spell::isResurrect() const
{
    return Resurrect();
}


uint32_t Spell::ExtraValue() const
{
    return spells[id].extra;
}

Spell Spell::Rand(int lvl, bool adv)
{
    vector<Spell> v;
    v.reserve(15);

    for (uint32_t sp = NONE; sp < STONE; ++sp)
    {
        const Spell spell(sp);
        if ((adv && !spell.isCombat() || !adv && spell.isCombat()) &&
            lvl == spell.Level() &&
            !(spells[sp].bits & SP_DISABLE))
            v.push_back(spell);
    }
    return !v.empty() ? *Rand::Get(v) : Spell(NONE);
}

Spell Spell::RandCombat(int lvl)
{
    return Rand(lvl, false);
}

Spell Spell::RandAdventure(int lvl)
{
    Spell res = Rand(lvl, true);
    return res.isValid() ? res : RandCombat(lvl);
}

bool Spell::isUndeadOnly() const
{
    switch (id)
    {
    case ANIMATEDEAD:
    case HOLYWORD:
    case HOLYSHOUT:
        return true;

    default:
        break;
    }

    return false;
}

bool Spell::isALiveOnly() const
{
    switch (id)
    {
    case BLESS:
    case MASSBLESS:
    case CURSE:
    case MASSCURSE:
    case DEATHRIPPLE:
    case DEATHWAVE:
    case RESURRECT:
    case RESURRECTTRUE:
        return true;

    default:
        break;
    }

    return false;
}

bool Spell::isApplyWithoutFocusObject() const
{
    if (isMassActions() || isSummon())
        return true;
    switch (id)
    {
    case DEATHRIPPLE:
    case DEATHWAVE:
    case EARTHQUAKE:
    case HOLYWORD:
    case HOLYSHOUT:
    case ARMAGEDDON:
    case ELEMENTALSTORM:
        return true;

    default:
        return false;
    }
}

bool Spell::isSummon() const
{
    switch (id)
    {
    case SUMMONEELEMENT:
    case SUMMONAELEMENT:
    case SUMMONFELEMENT:
    case SUMMONWELEMENT:
        return true;

    default:
        return false;
    }
}

bool Spell::isApplyToAnyTroops() const
{
    switch (id)
    {
    case DISPEL:
    case MASSDISPEL:
        return true;

    default:
        return false;
    }
}

bool Spell::isApplyToFriends() const
{
    switch (id)
    {
    case BLESS:
    case BLOODLUST:
    case CURE:
    case HASTE:
    case SHIELD:
    case STONESKIN:
    case DRAGONSLAYER:
    case STEELSKIN:
    case ANIMATEDEAD:
    case ANTIMAGIC:
    case TELEPORT:
    case RESURRECT:
    case MIRRORIMAGE:
    case RESURRECTTRUE:

    case MASSBLESS:
    case MASSCURE:
    case MASSHASTE:
    case MASSSHIELD:
        return true;

    default:
        return false;
    }
}

bool Spell::isMassActions() const
{
    switch (id)
    {
    case MASSCURE:
    case MASSHASTE:
    case MASSSLOW:
    case MASSBLESS:
    case MASSCURSE:
    case MASSDISPEL:
    case MASSSHIELD:
        return true;

    default:
        break;
    }

    return false;
}

bool Spell::isApplyToEnemies() const
{
    switch (id)
    {
    case MASSSLOW:
    case MASSCURSE:

    case CURSE:
    case ARROW:
    case SLOW:
    case BLIND:
    case COLDRAY:
    case DISRUPTINGRAY:
    case LIGHTNINGBOLT:
    case CHAINLIGHTNING:
    case PARALYZE:
    case BERSERKER:
    case HYPNOTIZE:
        return true;

    default:
        return false;
    }
}

bool Spell::isRaceCompatible(int race) const
{
    switch (id)
    {
    case MASSCURE:
    case MASSBLESS:
    case HOLYSHOUT:
    case HOLYWORD:
    case BLESS:
    case CURE:
        if (Race::NECR == race) return false;
        break;

    case DEATHWAVE:
    case DEATHRIPPLE:
    case ANIMATEDEAD:
        if (Race::NECR != race) return false;
        break;
    }
    return true;
}

uint32_t Spell::CalculateDimensionDoorDistance(uint32_t current_sp, uint32_t total_hp)
{
    if (GameStatic::Spell_DD_Distance() && GameStatic::Spell_DD_HP() && GameStatic::Spell_DD_SP() && total_hp)
    {
        const uint32_t res = GameStatic::Spell_DD_Distance() * current_sp * GameStatic::Spell_DD_HP() /
            (GameStatic::Spell_DD_SP() * total_hp);
        return res ? (res < 255 ? res : 255) : 1;
    }
    // original h2 variant
    return 14;
}

ByteVectorWriter& operator<<(ByteVectorWriter& msg, const Spell& spell)
{
    return msg << spell.id;
}

ByteVectorReader& operator>>(ByteVectorReader& msg, Spell& spell)
{
    return msg >> spell.id;
}

void Spell::ReadFrom(ByteVectorReader& msg)
{
    msg >> this->id;
}
