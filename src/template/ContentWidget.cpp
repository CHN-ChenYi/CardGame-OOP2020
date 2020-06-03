#include "ContentWidget.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "MainWindow.h"

ContentWidget::ContentWidget(QWidget *parent) : QWidget(parent) {}

HomeWidget::HomeWidget(MainWindow *parent) : ContentWidget(parent) {
  hlayout_ = new QHBoxLayout(this);
  hlayout_->setContentsMargins(0, 0, 0, 0);
  hlayout_->setSpacing(0);
  hlayout_->addStretch();

  bg_image_ = new QPixmap(":/Resource/home.jpg");
  setAutoFillBackground(true);
  SetBackgroundImage();

  vlayout_ = new QVBoxLayout();
  vlayout_->setContentsMargins(10, 0, 10, 0);
  vlayout_->addStretch();
  new_game_button_ = new QPushButton("新建游戏", this);
  new_game_button_->setMinimumSize(150, 45);
  connect(new_game_button_, &QPushButton::released, parent,
          &MainWindow::NewGame);
  vlayout_->addWidget(new_game_button_);
  vlayout_->addSpacing(new_game_button_->height() / 2);
  join_game_button_ = new QPushButton("加入游戏", this);
  join_game_button_->setMinimumSize(150, 45);
  connect(join_game_button_, &QPushButton::released, parent,
          &MainWindow::JoinGame);
  vlayout_->addWidget(join_game_button_);
  vlayout_->addSpacing(join_game_button_->height() / 2);
  hlayout_->addLayout(vlayout_);
  this->setLayout(hlayout_);
}

HomeWidget::~HomeWidget() {
  delete bg_image_;
  delete vlayout_;
}

void HomeWidget::SetBackgroundImage() {
  QPalette palette = this->palette();
  palette.setBrush(this->backgroundRole(),
                   QBrush(bg_image_->scaled(this->size(), Qt::IgnoreAspectRatio,
                                            Qt::SmoothTransformation)));
  this->setPalette(palette);
}

void HomeWidget::resizeEvent(QResizeEvent *event) { SetBackgroundImage(); }
