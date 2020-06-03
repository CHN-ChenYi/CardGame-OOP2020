#include "ContentWidget.h"

#include <QComboBox>
#include <QDebug>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
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

InitOrJoinWidget::InitOrJoinWidget(MainWindow *parent, bool widget_type)
    : ContentWidget(parent), widget_type_(widget_type) {
  layout_ = new QGridLayout(this);

  first_label_ =
      new QLabel(widget_type_ ? "服务器 IP 地址" : "服务器密码", this);
  layout_->addWidget(first_label_, 0, 0);

  second_label_ = new QLabel(widget_type_ ? "服务器密码" : "玩家昵称", this);
  layout_->addWidget(second_label_, 1, 0);

  third_label_ = new QLabel(widget_type_ ? "玩家昵称" : "游戏类型", this);
  layout_->addWidget(third_label_, 2, 0);

  first_input_ = new QLineEdit(this);
  first_input_->setPlaceholderText(first_label_->text());
  layout_->addWidget(first_input_, 0, 1);

  second_input_ = new QLineEdit(this);
  second_input_->setPlaceholderText(second_label_->text());
  layout_->addWidget(second_input_, 1, 1);

  if (widget_type_) {
    third_input_ = new QLineEdit(this);
    third_input_->setPlaceholderText(third_label_->text());
    layout_->addWidget(third_input_, 2, 1);
  } else {
    combo_box_ = new QComboBox(this);
    combo_box_->addItem("争上游");
    combo_box_->addItem("红心大战");
    layout_->addWidget(combo_box_, 2, 1);
  }

  hlayout_ = new QHBoxLayout(this);
  hlayout_->addStretch();

  info_label_ = new QLabel(this);
  hlayout_->addWidget(info_label_);

  button_ = new QPushButton(widget_type_ ? "加入" : "新建", this);
  connect(button_, &QPushButton::released, this, &InitOrJoinWidget::Accept);
  hlayout_->addWidget(button_);

  layout_->addLayout(hlayout_, 3, 1);
  setLayout(layout_);
}

void InitOrJoinWidget::SetInfo(const wstring &info) {
  info_label_->setText(QString::fromStdWString(info));
  QFontMetrics fm = QFontMetrics(this->font());
  info_label_->setMinimumWidth(fm.boundingRect(info_label_->text()).width());
}

void InitOrJoinWidget::Accept() {
  if (widget_type_) {
    ::JoinGame(first_input_->text().toStdWString(),
               second_input_->text().toStdWString(),
               third_input_->text().toStdWString());
  } else {
    ::NewGame(first_input_->text().toStdWString(),
              second_input_->text().toStdWString(),
              combo_box_->currentIndex() ? Hearts : Winner);
  }
}
