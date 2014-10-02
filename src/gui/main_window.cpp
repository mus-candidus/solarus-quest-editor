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
#include "gui/main_window.h"
#include "gui/text_editor.h"  // TODO remove
#include "quest_manager.h"
#include <solarus/Arguments.h>
#include <solarus/MainLoop.h>
#include <solarus/SolarusFatal.h>
#include <solarus/lowlevel/Debug.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QPlainTextEdit>  // TODO remove
#include <iostream>

/**
 * @brief Creates a main window.
 * @param parent The parent widget or nullptr.
 * @param quest_manager The quest manager.
 */
MainWindow::MainWindow(QWidget* parent, QuestManager& quest_manager) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  quest_manager(quest_manager) {

  ui->setupUi(this);

  // Set up children.
  ui->quest_tree_view->set_quest_manager(quest_manager);

  int tree_width = 400;
  ui->splitter->setSizes(QList<int>() << tree_width << width() - tree_width);

  connect(&quest_manager, SIGNAL(current_quest_changed(Quest&)),
          this, SLOT(current_quest_changed(Quest&)));

  // TODO remove
  ui->tabWidget->addTab(new TextEditor("map"),
         QIcon(":/images/icon_resource_map.png"),
         "Map");
  ui->tabWidget->addTab(new TextEditor("tileset"),
         QIcon(":/images/icon_resource_tileset.png"),
         "Tileset");
  ui->tabWidget->addTab(new TextEditor("script"),
         QIcon(":/images/icon_script.png"),
         "Script");
}

/**
 * @brief Returns the quest manager.
 * @return The quest manager.
 */
QuestManager& MainWindow::get_quest_manager() {
  return quest_manager;
}

/**
 * @brief Slot called when the user triggers the "Exit" action.
 */
void MainWindow::on_actionExit_triggered() {
  std::exit(0);
}

/**
 * @brief Slot called when the user triggers the "Load quest" action.
 */
void MainWindow::on_actionLoad_quest_triggered() {

  QFileDialog dialog(nullptr, tr("Select quest directory"));
  dialog.setFileMode(QFileDialog::Directory);
  dialog.setOption(QFileDialog::ShowDirsOnly);
  if (!dialog.exec()) {
    return;
  }

  QStringList file_names = dialog.selectedFiles();
  if (file_names.empty()) {
    return;
  }

  QString quest_path = file_names.first();
  if (!quest_manager.set_quest(quest_path)) {
    QMessageBox messageBox;
    messageBox.setIcon(QMessageBox::Warning);
    messageBox.setText("No quest was found in directory\n'" + quest_path + "'");
    messageBox.exec();
  }
}

/**
 * @brief Slot called when the user triggers the "Close" action.
 */
void MainWindow::on_actionClose_triggered() {

  ui->tabWidget->on_tab_close_requested(ui->tabWidget->currentIndex());
}

/**
 * @brief Slot called when the user triggers the "Run quest" action.
 */
void MainWindow::on_actionRun_quest_triggered() {

  QString quest_path = quest_manager.get_quest().get_root_path();
  if (quest_path.isEmpty()) {
    return;
  }

  // TODO run quest in a separate thread
  try {
    Solarus::Arguments arguments;
    arguments.add_argument(quest_path.toStdString());
    Solarus::MainLoop main_loop(arguments);
    main_loop.run();
  }
  catch (const Solarus::SolarusFatal& ex) {
    // The run did not end well.
    std::cout << "Quest terminated unexpectedly: " << ex.what() << std::endl;
  }
}

/**
 * @brief Slot called when the user opens another quest.
 */
void MainWindow::current_quest_changed(Quest& /* quest */) {
  update_title();
}

/**
 * @brief Updates the title of the window from the current quest.
 */
void MainWindow::update_title() {

  QString title = "Solarus Quest Editor";
  QString quest_name = quest_manager.get_quest().get_name();
  if (!quest_name.isEmpty()) {
    title = quest_name + " - " + title;
  }

  setWindowTitle(title);
}
