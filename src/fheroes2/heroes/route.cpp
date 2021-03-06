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

#include "world.h"
#include "game.h"
#include "settings.h"
#include <sstream>

s32 Route::Step::GetIndex() const
{
    return from < 0 ? -1 : Maps::GetDirectionIndex(from, direction);
}

s32 Route::Step::GetFrom() const
{
    return from;
}

int Route::Step::GetDirection() const
{
    return direction;
}

uint32_t Route::Step::GetPenalty() const
{
    return penalty;
}

bool Route::Step::isBad() const
{
    return from < 0 || (direction == Direction::UNKNOWN || direction == Direction::CENTER);
}

/* construct */
Route::Path::Path(const Heroes& h)
    : hero(&h), dst(h.GetIndex()), hide(true)
{
}

Route::Path::Path(const Path& p) = default;

Route::Path& Route::Path::operator=(const Path& p)
{
    assign(p.begin(), p.end());

    hero = p.hero;
    dst = p.dst;
    hide = p.hide;

    return *this;
}

int Route::Path::GetFrontDirection() const
{
    return empty()
               ? (dst != hero->GetIndex()
                      ? Direction::Get(hero->GetIndex(), dst)
                      : Direction::CENTER)
               : front().GetDirection();
}

uint32_t Route::Path::GetFrontPenalty() const
{
    return empty() ? 0 : front().GetPenalty();
}

void Route::Path::PopFront()
{
    if (!empty()) pop_front();
}

void Route::Path::PopBack()
{
    if (!empty())
    {
        pop_back();
        dst = empty() ? -1 : back().GetIndex();
    }
}

s32 Route::Path::GetDestinationIndex() const
{
    return empty() ? GetDestinedIndex() : GetLastIndex();
}

s32 Route::Path::GetLastIndex() const
{
    return empty() ? -1 : back().GetIndex();
}

s32 Route::Path::GetDestinedIndex() const
{
    return dst;
}

/* return length path */
bool Route::Path::Calculate(const s32& dst_index, int limit /* -1 */)
{
    dst = dst_index;

    if (Find(dst, limit))
    {
        // check monster dst
        if (Maps::isValidAbsIndex(dst) &&
            MP2::OBJ_MONSTER == world.GetTiles(dst).GetObject())
            pop_back();
    }

    return !empty();
}

void Route::Path::Reset()
{
    dst = hero->GetIndex();

    if (!empty())
    {
        clear();
        hide = true;
    }
}

bool Route::Path::isComplete() const
{
    return dst == hero->GetIndex() ||
        empty() && Direction::UNKNOWN != Direction::Get(hero->GetIndex(), dst);
}

bool Route::Path::isValid() const
{
    return !empty();
}

int Route::Path::GetIndexSprite(int from, int to, int mod)
{
    // ICN::ROUTE
    // start index 1, 25, 49, 73, 97, 121 (size arrow path)
    int index = 1;

    switch (mod)
    {
    case 200:
        index = 121;
        break;
    case 175:
        index = 97;
        break;
    case 150:
        index = 73;
        break;
    case 125:
        index = 49;
        break;
    case 100:
        index = 25;
        break;

    default:
        break;
    }

    switch (from)
    {
    case Direction::TOP:
        switch (to)
        {
        case Direction::TOP:
            index += 8;
            break;
        case Direction::TOP_RIGHT:
            index += 17;
            break;
        case Direction::RIGHT:
            index += 18;
            break;
        case Direction::LEFT:
            index += 6;
            break;
        case Direction::TOP_LEFT:
            index += 7;
            break;
        case Direction::BOTTOM_LEFT:
            index += 5;
            break;
        case Direction::BOTTOM_RIGHT:
            index += 19;
            break;
        default:
            index = 0;
            break;
        }
        break;

    case Direction::TOP_RIGHT:
        switch (to)
        {
        case Direction::TOP:
            index += 0;
            break;
        case Direction::TOP_RIGHT:
            index += 9;
            break;
        case Direction::RIGHT:
            index += 18;
            break;
        case Direction::BOTTOM_RIGHT:
            index += 19;
            break;
        case Direction::TOP_LEFT:
            index += 7;
            break;
        case Direction::BOTTOM:
            index += 20;
            break;
        case Direction::LEFT:
            index += 6;
            break;
        default:
            index = 0;
            break;
        }
        break;

    case Direction::RIGHT:
        switch (to)
        {
        case Direction::TOP:
            index += 0;
            break;
        case Direction::BOTTOM:
            index += 20;
            break;
        case Direction::BOTTOM_RIGHT:
            index += 19;
            break;
        case Direction::RIGHT:
            index += 10;
            break;
        case Direction::TOP_RIGHT:
            index += 1;
            break;
        case Direction::TOP_LEFT:
            index += 7;
            break;
        case Direction::BOTTOM_LEFT:
            index += 21;
            break;
        default:
            index = 0;
            break;
        }
        break;

    case Direction::BOTTOM_RIGHT:
        switch (to)
        {
        case Direction::TOP_RIGHT:
            index += 1;
            break;
        case Direction::RIGHT:
            index += 2;
            break;
        case Direction::BOTTOM_RIGHT:
            index += 11;
            break;
        case Direction::BOTTOM:
            index += 20;
            break;
        case Direction::BOTTOM_LEFT:
            index += 21;
            break;
        case Direction::TOP:
            index += 0;
            break;
        case Direction::LEFT:
            index += 22;
            break;
        default:
            index = 0;
            break;
        }
        break;

    case Direction::BOTTOM:
        switch (to)
        {
        case Direction::RIGHT:
            index += 2;
            break;
        case Direction::BOTTOM_RIGHT:
            index += 3;
            break;
        case Direction::BOTTOM:
            index += 12;
            break;
        case Direction::BOTTOM_LEFT:
            index += 21;
            break;
        case Direction::LEFT:
            index += 22;
            break;
        case Direction::TOP_LEFT:
            index += 16;
            break;
        case Direction::TOP_RIGHT:
            index += 1;
            break;
        default:
            index = 0;
            break;
        }
        break;

    case Direction::BOTTOM_LEFT:
        switch (to)
        {
        case Direction::BOTTOM_RIGHT:
            index += 3;
            break;
        case Direction::BOTTOM:
            index += 4;
            break;
        case Direction::BOTTOM_LEFT:
            index += 13;
            break;
        case Direction::LEFT:
            index += 22;
            break;
        case Direction::TOP_LEFT:
            index += 23;
            break;
        case Direction::TOP:
            index += 16;
            break;
        case Direction::RIGHT:
            index += 2;
            break;
        default:
            index = 0;
            break;
        }
        break;

    case Direction::LEFT:
        switch (to)
        {
        case Direction::TOP:
            index += 16;
            break;
        case Direction::BOTTOM:
            index += 4;
            break;
        case Direction::BOTTOM_LEFT:
            index += 5;
            break;
        case Direction::LEFT:
            index += 14;
            break;
        case Direction::TOP_LEFT:
            index += 23;
            break;
        case Direction::TOP_RIGHT:
            index += 17;
            break;
        case Direction::BOTTOM_RIGHT:
            index += 3;
            break;
        default:
            index = 0;
            break;
        }
        break;

    case Direction::TOP_LEFT:
        switch (to)
        {
        case Direction::TOP:
            index += 16;
            break;
        case Direction::TOP_RIGHT:
            index += 17;
            break;
        case Direction::BOTTOM_LEFT:
            index += 5;
            break;
        case Direction::LEFT:
            index += 6;
            break;
        case Direction::TOP_LEFT:
            index += 15;
            break;
        case Direction::BOTTOM:
            index += 4;
            break;
        case Direction::RIGHT:
            index += 18;
            break;
        default:
            index = 0;
            break;
        }
        break;

    default:
        index = 0;
        break;
    }

    return index;
}

/* total penalty cast */
uint32_t Route::Path::GetTotalPenalty() const
{
    uint32_t result = 0;

    for (auto it : *this)
        result += it.GetPenalty();

    return result;
}

s32 Route::Path::GetAllowStep() const
{
    s32 green = 0;
    uint32_t move_point = hero->GetMovePoints();

    for (auto it : *this)
    {
        if (move_point < it.GetPenalty())
            break;
        move_point -= it.GetPenalty();
        ++green;
    }

    return green;
}

string Route::Path::String() const
{
    ostringstream os;

    os << "from: " << hero->GetIndex() << ", to: " << GetLastIndex() <<
        ", obj: " << MP2::StringObject(world.GetTiles(dst).GetObject()) << ", dump: ";

    for (auto it : *this)
        os << Direction::String(it.GetDirection()) << "(" << it.GetPenalty() << ")" << ", ";

    os << "end";
    return os.str();
}

bool StepIsObstacle(const Route::Step& s)
{
    const s32 index = s.GetIndex();
    int obj = 0 <= index
                  ? world.GetTiles(index).GetObject()
                  : MP2::OBJ_ZERO;

    switch (obj)
    {
    case MP2::OBJ_HEROES:
    case MP2::OBJ_MONSTER:
        return true;

    default:
        break;
    }

    return false;
}

bool StepIsPassable(const Route::Step& s, const Heroes* h)
{
    return world.GetTiles(s.GetFrom()).isPassable(h, s.GetDirection(), false);
}

bool Route::Path::hasObstacle() const
{
    const auto it = find_if(begin(), end(), StepIsObstacle);
    return it != end() && (*it).GetIndex() != GetLastIndex();
}

void Route::Path::RescanObstacle()
{
    // scan obstacle
    const auto it = find_if(begin(), end(), StepIsObstacle);

    if (it == end() || (*it).GetIndex() == GetLastIndex())
        return;
    const size_t size1 = size();
    const s32 reduce = (*it).GetFrom();
    Calculate(dst);
    // reduce
    if (size() > size1 * 2) Calculate(reduce);
}

void Route::Path::RescanPassable()
{
    // scan passable
    auto it = begin();

    for (; it != end(); ++it)
        if (!world.GetTiles((*it).GetFrom()).isPassable(nullptr, (*it).GetDirection(), false))
            break;

    if (hero->isControlAI())
    {
        Reset();
    }
    else if (it != end())
    {
        if (it == begin())
            Reset();
        else
        {
            dst = (*it).GetFrom();
            erase(it, end());
        }
    }
}

ByteVectorWriter& Route::operator<<(ByteVectorWriter& msg, const Step& step)
{
    return msg << step.from << step.direction << step.penalty;
}

ByteVectorWriter& Route::operator<<(ByteVectorWriter& msg, const Path& path)
{
    return msg << path.dst << path.hide << static_cast<list<Step>>(path);
}

ByteVectorReader& Route::operator>>(ByteVectorReader& msg, Step& step)
{
    return msg >> step.from >> step.direction >> step.penalty;
}


ByteVectorReader& Route::operator>>(ByteVectorReader& msg, Path& path)
{
    list<Step>& base = path;
    return msg >> path.dst >> path.hide >> base;
}
