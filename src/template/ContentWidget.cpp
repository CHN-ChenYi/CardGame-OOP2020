#include "ContentWidget.h"

#include <QComboBox>
#include <QDebug>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>

#include "MainWindow.h"

NetworkCircle::NetworkCircle(QWidget *parent, const double network_status)
    : QWidget(parent), parent_(parent), network_status_(network_status) {}

void NetworkCircle::UpdateNetworkStatus(const double network_status) {
  network_status_ = network_status;
  repaint();
}

void NetworkCircle::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.setPen(Qt::NoPen);
  painter.setBrush(
      QColor(255 * (1 - network_status_), 255 * network_status_, 0));
  painter.drawEllipse(0, 0, parent_->height(), parent_->height());
}

ContentWidget::ContentWidget(QWidget *parent) : QWidget(parent) {}

HomeWidget::HomeWidget(MainWindow *parent) : ContentWidget(parent) {
  QHBoxLayout *hlayout = new QHBoxLayout(this);
  hlayout->setContentsMargins(0, 0, 0, 0);
  hlayout->setSpacing(0);
  hlayout->addStretch();

  bg_image_ = new QPixmap(":/Resource/home.jpg");
  setAutoFillBackground(true);
  SetBackgroundImage();

  vlayout_ = new QVBoxLayout();
  vlayout_->setContentsMargins(10, 0, 10, 0);
  vlayout_->addStretch();
  QPushButton *new_game_button = new QPushButton("新建游戏", this);
  new_game_button->setMinimumSize(150, 45);
  connect(new_game_button, &QPushButton::released, parent,
          &MainWindow::NewGame);
  vlayout_->addWidget(new_game_button);
  vlayout_->addSpacing(new_game_button->height() / 2);
  QPushButton *join_game_button = new QPushButton("加入游戏", this);
  join_game_button->setMinimumSize(150, 45);
  connect(join_game_button, &QPushButton::released, parent,
          &MainWindow::JoinGame);
  vlayout_->addWidget(join_game_button);
  vlayout_->addSpacing(join_game_button->height() / 2);
  hlayout->addLayout(vlayout_);
  this->setLayout(hlayout);
}

HomeWidget::~HomeWidget() {
  delete vlayout_;
  delete bg_image_;
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
  QGridLayout *glayout = new QGridLayout(this);

  QLabel *first_label =
      new QLabel(widget_type_ ? "服务器 IP 地址" : "服务器密码", this);
  glayout->addWidget(first_label, 0, 0);

  QLabel *second_label =
      new QLabel(widget_type_ ? "服务器密码" : "玩家昵称", this);
  glayout->addWidget(second_label, 1, 0);

  QLabel *third_label =
      new QLabel(widget_type_ ? "玩家昵称" : "游戏类型", this);
  glayout->addWidget(third_label, 2, 0);

  first_input_ = new QLineEdit(this);
  first_input_->setPlaceholderText(first_label->text());
  glayout->addWidget(first_input_, 0, 1);

  second_input_ = new QLineEdit(this);
  second_input_->setPlaceholderText(second_label->text());
  glayout->addWidget(second_input_, 1, 1);

  if (widget_type_) {
    third_input_ = new QLineEdit(this);
    third_input_->setPlaceholderText(third_label->text());
    glayout->addWidget(third_input_, 2, 1);
  } else {
    combo_box_ = new QComboBox(this);
    combo_box_->addItem("争上游");
    combo_box_->addItem("红心大战");
    glayout->addWidget(combo_box_, 2, 1);
  }

  hlayout_ = new QHBoxLayout();
  hlayout_->addStretch();

  info_label_ = new QLabel(this);
  hlayout_->addWidget(info_label_);

  QPushButton *button = new QPushButton(widget_type_ ? "加入" : "新建", this);
  connect(button, &QPushButton::released, this, &InitOrJoinWidget::Accept);
  hlayout_->addWidget(button);

  glayout->addLayout(hlayout_, 3, 1);
  setLayout(glayout);
}

InitOrJoinWidget::~InitOrJoinWidget() { delete hlayout_; }

void InitOrJoinWidget::SetInfo(const wstring &info) {
  info_label_->setText(QString::fromStdWString(info));
  QRect rect = QFontMetrics(font()).boundingRect(info_label_->text());
  info_label_->setMinimumWidth(rect.width());
  info_label_->setMinimumHeight(rect.height());
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

WaitWidget::WaitWidget(MainWindow *parent, const GameType type,
                       const wstring &ip, bool is_owner)
    : ContentWidget(parent) {
  QVBoxLayout *vlayout_ = new QVBoxLayout(this);

  QLabel *ip_label =
      new QLabel("服务器 IP: " + QString::fromStdWString(ip), this);
  vlayout_->addWidget(ip_label);

  QLabel *type_label = new QLabel(type == Hearts ? "红心大战" : "争上游", this);
  vlayout_->addWidget(type_label);

  id_top_ = 0;
  glayout_ = new QGridLayout();
  glayout_->setColumnStretch(0, 2);
  glayout_->setColumnStretch(1, 1);
  glayout_->setColumnStretch(2, 2);
  vlayout_->addLayout(glayout_);

  QLabel *wait_label = new QLabel("等待中...", this);
  vlayout_->addWidget(wait_label);

  vlayout_->addStretch();

  info_label_ = new QLabel(this);
  vlayout_->addWidget(info_label_);

  hlayout_ = new QHBoxLayout();
  if (is_owner) {
    QPushButton *add_bot_button = new QPushButton("加入 Bot", this);
    connect(add_bot_button, &QPushButton::released, this, &WaitWidget::AddBot);
    hlayout_->addWidget(add_bot_button);
    QPushButton *start_game_button = new QPushButton("开始游戏", this);
    connect(start_game_button, &QPushButton::released, this,
            &WaitWidget::StartGame);
    hlayout_->addWidget(start_game_button);
  }
  vlayout_->addLayout(hlayout_);

  setLayout(vlayout_);
}

WaitWidget::~WaitWidget() {
  delete hlayout_;
  delete glayout_;
}

void WaitWidget::SetInfo(const wstring &info) {
  info_label_->setText(QString::fromStdWString(info));
  QRect rect = QFontMetrics(font()).boundingRect(info_label_->text());
  info_label_->setMinimumWidth(rect.width());
  info_label_->setMinimumHeight(rect.height());
}

bool WaitWidget::AddPlayer(const unsigned short id, const wstring &player_name,
                           const double network_status) {
  for (int i = 0; i < id_top_; i++) {
    if (id_[i] == id) return false;
  }

  id_[id_top_] = id;

  QLabel *name_label = new QLabel(QString::fromStdWString(player_name), this);
  name_label->setAttribute(Qt::WA_DeleteOnClose);
  glayout_->addWidget(name_label, id_top_, 0);
  name_label->setMinimumHeight(
      QFontMetrics(font()).boundingRect(name_label->text()).height());

  QLabel *network_label = new QLabel("网络情况：", name_label);
  network_label->setAttribute(Qt::WA_DeleteOnClose);
  network_label->setAlignment(Qt::AlignRight);
  glayout_->addWidget(network_label, id_top_, 1);
  network_label->setMinimumHeight(
      QFontMetrics(font()).boundingRect(network_label->text()).height() * 1.2);

  NetworkCircle *network_circle =
      new NetworkCircle(network_label, network_status);
  network_circle->setAttribute(Qt::WA_DeleteOnClose);
  glayout_->addWidget(network_circle, id_top_, 2);

  id_top_++;
  return true;
}

bool WaitWidget::RemovePlayer(const unsigned short id) {
  for (int i = 0; i < id_top_; i++) {
    if (id_[i] == id) {
      QLayoutItem *item;
      for (int j = 0; j < 3; j++) {
        item = glayout_->itemAtPosition(i, j);
        glayout_->removeWidget(item->widget());
        item->widget()->close();
      }
      for (int j = i + 1; j < id_top_; j++) {
        id_[j - 1] = id_[j];
        for (int k = 0; k < 3; k++) {
          item = glayout_->itemAtPosition(j, k);
          glayout_->removeWidget(item->widget());
          glayout_->addWidget(item->widget(), j - 1, k);
        }
      }
      id_top_--;
      return true;
    }
  }
  return false;
}

bool WaitWidget::SetNetworkStatus(const unsigned short id,
                                  const double network_status) {
  for (int i = 0; i < id_top_; i++) {
    if (id_[i] == id) {
      dynamic_cast<NetworkCircle *>(glayout_->itemAtPosition(i, 2)->widget())
          ->UpdateNetworkStatus(network_status);
      return true;
    }
  }
  return false;
}

void WaitWidget::AddBot() { ::AddBot(); }

void WaitWidget::StartGame() { ::StartGame(); }

CardLabel::CardLabel(QWidget *parent, Card card, int width, int height,
                     bool selectable)
    : QLabel(parent), selectable_(selectable) {
  QImage img;
  if (card.rank > 13) {
    if (card.rank == 14)
      img.load(":/Resource/53.jpg");
    else if (card.rank == 15)
      img.load(":/Resource/54.jpg");
    else if (card.rank == 16)
      img.load(":/Resource/0.jpg");
    else
      qDebug() << "Unknown card rank";
  } else {
    const int id = int(card.suit) * 13 + card.rank;
    char path[18];
    sprintf(path, ":/Resource/%d.jpg", id);
    img.load(path);
  }
  if (width <= height) {
    setPixmap(QPixmap::fromImage(img.scaled(width, height, Qt::KeepAspectRatio,
                                            Qt::SmoothTransformation)));
  } else {
    QMatrix matrix;
    matrix.rotate(90.0);
    setPixmap(QPixmap::fromImage(img.transformed(matrix, Qt::FastTransformation)
                                     .scaled(width, height, Qt::KeepAspectRatio,
                                             Qt::SmoothTransformation)));
  }
}

void CardLabel::mousePressEvent(QMouseEvent *event) {
  if (!selectable_) return;
  const QRect cur_geo = geometry();
  setGeometry(cur_geo.x(), cur_geo.y() == 0 ? cur_geo.height() * 0.25 : 0,
              cur_geo.width(), cur_geo.height());
}

PlayWidget::PlayWidget(MainWindow *parent, const GameType type,
                       const wstring (&player_name)[4],
                       const unsigned short number_of_cards[4],
                       const double network_status[4],
                       const bool controlled_by_bot[4], const Card cards[]) {
  CardLabel *a = new CardLabel(this, Card{Diamond, 14}, 200, 300, true);
  a->setFixedSize(200, 300);
  CardLabel *b = new CardLabel(this, Card{Spade, 16}, 300, 200, false);
  b->setFixedSize(300, 200);
  b->setGeometry(50, 0, 0, 0);
  a->raise();
}

PlayWidget::~PlayWidget() {}

void PlayWidget::SetInfo(const wstring &info) {}
