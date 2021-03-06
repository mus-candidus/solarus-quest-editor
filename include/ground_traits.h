/*
 * Copyright (C) 2014-2016 Christopho, Solarus - http://www.solarus-games.org
 *
 * Solarus Quest Editor is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Solarus Quest Editor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef SOLARUSEDITOR_GROUND_TRAITS_H
#define SOLARUSEDITOR_GROUND_TRAITS_H

#include "enum_traits.h"
#include <solarus/entities/Ground.h>

namespace SolarusEditor {

using Ground = Solarus::Ground;
using GroundTraits = EnumTraits<Ground>;

/**
 * \brief Provides useful properties of ground enum values.
 */
template<>
class EnumTraits<Ground> {

public:

  static QList<Ground> get_values();
  static QString get_friendly_name(Ground value);
  static QIcon get_icon(Ground value);

  static QString get_lua_name(Ground value);
  static Ground get_by_lua_name(const QString& name);

  static bool is_traversable(Ground value);

};

}

#endif
