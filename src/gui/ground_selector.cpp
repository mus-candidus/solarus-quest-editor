/*
 * Copyright (C) 2014 Christopho, Solarus - http://www.solarus-games.org
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
#include "gui/ground_selector.h"
#include "ground_helper.h"
#include <solarus/entities/GroundInfo.h>

/**
 * @brief Creates a ground selector.
 * @param parent The parent widget or nullptr.
 */
GroundSelector::GroundSelector(QWidget* parent) :
  QComboBox(parent),
  with_none(false) {

  for (const auto& kvp : Solarus::GroundInfo::get_ground_names()) {
    Ground ground = kvp.first;
    QString ground_name = QString::fromStdString(kvp.second);

    addItem(QIcon(":/images/ground_" + ground_name + ".png"),
            GroundHelper::get_ground_friendly_name(ground));
  }
}

/**
 * @brief Returns whether this selector includes an empty option.
 * @return @c true if an empty option is included.
 */
bool GroundSelector::is_with_none() const {
  return with_none;
}

/**
 * @brief Sets whether this selector includes an empty option.
 * @param with_none @c true to include an empty option.
 */
void GroundSelector::set_with_none(bool with_none) {

  if (with_none == this->with_none) {
    return;
  }

  this->with_none = with_none;
  if (with_none) {
    insertItem(0, "");
  }
  else {
    removeItem(0);
  }
}

/**
 * @brief Returns whether no ground is selected.
 *
 * Only possible if the selector includes an empty option.
 *
 * @return @c true if no ground is selected.
 */
bool GroundSelector::is_empty() const {

  return with_none && currentIndex() == 0;
}

/**
 * @brief Unselects any ground.
 *
 * Only possible if the selector includes an empty option
 * (otherwise this function has no effect).
 */
void GroundSelector::set_empty() {

  if (!is_with_none()) {
    return;
  }

  setCurrentIndex(0);
}

/**
 * @brief Returns the selected ground.
 *
 * It is an error to call this function is no ground is selected
 * (in this case a default-constructed ground value is returned).
 *
 * @return The selected ground.
 */
Ground GroundSelector::get_selected_ground() const {

  if (is_empty()) {
    return Ground();
  }

  int index = currentIndex();
  if (with_none) {
    --index;
  }
  return static_cast<Ground>(index);
}

/**
 * @brief Changes the selected ground.
 * @param ground The ground to select.
 */
void GroundSelector::set_selected_ground(Ground ground) {

  int index = static_cast<int>(ground);
  if (with_none) {
    ++index;
  }
  setCurrentIndex(index);
}
