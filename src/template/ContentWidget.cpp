#include "ContentWidget.h"

#include <QPushButton>

#include "MainWindow.h"

ContentWidget::ContentWidget(QWidget *parent) : QWidget(parent) {}

InitWidget::InitWidget(MainWindow *parent) { CreateButtons(parent); }

void InitWidget::CreateButtons(MainWindow *parent) {
  static int a;
  char x[100];
  sprintf(x, "%d", a++);

  new_game_button_ = new QPushButton(x, this);
  // new_game_button_ = new QPushButton("新建游戏", this);
  new_game_button_->setGeometry(0, 0, 50, 50);
  connect(new_game_button_, &QPushButton::released, parent, &MainWindow::NewGame);

  join_game_button_ = new QPushButton("加入游戏", this);
  join_game_button_->setGeometry(100, 0, 50, 50);
  connect(join_game_button_, &QPushButton::released, parent,
          &MainWindow::JoinGame);
}
