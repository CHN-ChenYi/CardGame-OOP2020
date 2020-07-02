#include "ContentWidget.h"

#include <QComboBox>
#include <QCoreApplication>
#include <QDebug>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPainter>
#include <QPushButton>
#include <QResizeEvent>
#include <QVBoxLayout>
#include <algorithm>
#include <sstream>

#include "MainWindow.h"

NetworkCircle::NetworkCircle(QWidget *parent, const double network_status,
                             const int height)
    : QWidget(parent),
      parent_(parent),
      network_status_(network_status),
      height_(height) {
  setAttribute(Qt::WA_DeleteOnClose);
}

void NetworkCircle::UpdateNetworkStatus(const double network_status) {
  network_status_ = network_status;
  repaint();
}

void NetworkCircle::paintEvent(QPaintEvent *) {
  setGeometry(parent_->x() + parent_->width(),
              parent_->y() + (parent_->height() - height_) / 2, height_,
              height_);
  QPainter painter(this);
  painter.setPen(Qt::NoPen);
  painter.setBrush(
      QColor(255 * (1 - network_status_), 255 * network_status_, 0));
  painter.drawEllipse(0, 0, height_, height_);
  setFixedWidth(height_ * 1.1);
  setFixedWidth(height_ * 1.1);
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

void HomeWidget::resizeEvent(QResizeEvent *) { SetBackgroundImage(); }

InitOrJoinWidget::InitOrJoinWidget(MainWindow *parent, bool widget_type)
    : ContentWidget(parent), widget_type_(widget_type) {
  QGridLayout *glayout = new QGridLayout(this);

  QLabel *first_label = new QLabel("服务器密码", this);
  glayout->addWidget(first_label, 0, 0);

  first_input_ = new QLineEdit(this);
  first_input_->setPlaceholderText(first_label->text());
  glayout->addWidget(first_input_, 0, 1);

  QLabel *second_label = new QLabel("玩家昵称", this);
  glayout->addWidget(second_label, 1, 0);

  second_input_ = new QLineEdit(this);
  second_input_->setPlaceholderText(second_label->text());
  glayout->addWidget(second_input_, 1, 1);

  if (!widget_type_) {
    QLabel *third_label = new QLabel("游戏类型", this);
    glayout->addWidget(third_label, 2, 0);

    combo_box_ = new QComboBox(this);
    combo_box_->addItem("争上游");
    combo_box_->addItem("红心大战");
    glayout->addWidget(combo_box_, 2, 1);
  }

  hlayout_ = new QHBoxLayout();
  hlayout_->addStretch();

  info_label_ = new QLabel(this);
  hlayout_->addWidget(info_label_);

  QPushButton *button =
      new QPushButton(widget_type_ ? "加入游戏" : "新建游戏", this);
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
  if (!first_input_->text().length()) {
    SetInfo(L"服务器密码不能为空");
    return;
  }
  if (!second_input_->text().length()) {
    SetInfo(L"玩家昵称不能为空");
    return;
  }
  if (widget_type_) {
    ::JoinGame(first_input_->text().toStdWString(),
               second_input_->text().toStdWString());
  } else {
    ::NewGame(first_input_->text().toStdWString(),
              second_input_->text().toStdWString(),
              combo_box_->currentIndex() ? Hearts : Winner);
  }
}

WaitWidget::WaitWidget(MainWindow *parent, const GameType type, bool is_owner)
    : ContentWidget(parent) {
  QVBoxLayout *vlayout_ = new QVBoxLayout(this);

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
  // network_label->setMinimumHeight(
  //    QFontMetrics(font()).boundingRect(network_label->text()).height()
  //    * 1.1);

  NetworkCircle *network_circle = new NetworkCircle(
      network_label, network_status,
      QFontMetrics(font()).boundingRect(network_label->text()).height());
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

CardLabel::CardLabel(QWidget *parent, Card card, bool direction,
                     bool selectable, int width, int height)
    : QLabel(parent),
      card_(card),
      direction_(direction),
      selectable_(selectable) {
  setAttribute(Qt::WA_DeleteOnClose);
  if (card.rank > 13) {
    if (card.rank == 14)
      img_.load(":/Resource/53.jpg");
    else if (card.rank == 15)
      img_.load(":/Resource/54.jpg");
    else if (card.rank == 16)
      img_.load(":/Resource/0.jpg");
    else
      qDebug() << "Unknown card rank";
  } else {
    const int id = int(card.suit) * 13 + card.rank;
    char path[18];
    sprintf(path, ":/Resource/%d.jpg", id);
    img_.load(path);
  }
  if (direction_) {
    setPixmap(QPixmap::fromImage(img_.scaled(width, height, Qt::KeepAspectRatio,
                                             Qt::SmoothTransformation)));
  } else {
    QMatrix matrix;
    matrix.rotate(90.0);
    setPixmap(
        QPixmap::fromImage(img_.transformed(matrix, Qt::FastTransformation)
                               .scaled(width, height, Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation)));
  }
}

void CardLabel::SetMaxY(const int y) { max_y_ = y; }

void CardLabel::SetMinY(const int y) { min_y_ = y; }

bool CardLabel::IsChoosen() const { return y() == min_y_; }

void CardLabel::resizeEvent(QResizeEvent *) {
  if (direction_) {
    setPixmap(QPixmap::fromImage(img_.scaled(
        width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
  } else {
    QMatrix matrix;
    matrix.rotate(90.0);
    setPixmap(
        QPixmap::fromImage(img_.transformed(matrix, Qt::FastTransformation)
                               .scaled(width(), height(), Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation)));
  }
}

void CardLabel::mousePressEvent(QMouseEvent *) {
  if (!selectable_) return;
  const QRect cur_geo = geometry();
  setGeometry(cur_geo.x(), cur_geo.y() == max_y_ ? min_y_ : max_y_,
              cur_geo.width(), cur_geo.height());
}

static bool (*PCardLabelLess[2])(const PCardLabel &, const PCardLabel &) = {
    [](const PCardLabel &lhs, const PCardLabel &rhs) {
      const Card &lhs_card = lhs.value->card_;
      const Card &rhs_card = rhs.value->card_;
      return lhs_card.rank != rhs_card.rank
                 ? CardLess[Winner](lhs_card, rhs_card)
                 : lhs_card.suit < rhs_card.suit;
    },
    [](const PCardLabel &lhs, const PCardLabel &rhs) {
      const Card &lhs_card = lhs.value->card_;
      const Card &rhs_card = rhs.value->card_;
      return lhs_card.suit != rhs_card.suit
                 ? lhs_card.suit < rhs_card.suit
                 : CardLess[Hearts](lhs_card, rhs_card);
    }};

DeckWidget::DeckWidget(QWidget *parent, const bool direction,
                       const bool selectable, const GameType type,
                       const wstring &player_name,
                       const unsigned short number_of_cards,
                       const double network_status,
                       const bool controlled_by_bot, const Card cards[])
    : QWidget(parent), direction_(direction), selectable_(selectable) {
  if (player_name[0] == '\0') {
    exist_ = false;
    return;
  }
  exist_ = true;
  main_layout_ = new QBoxLayout(direction_ ? QBoxLayout::LeftToRight
                                           : QBoxLayout::TopToBottom);
  main_layout_->setContentsMargins(0, 0, 0, 0);
  main_layout_->setSpacing(0);

  cards_widget_ = new QWidget(this);
  main_layout_->addWidget(cards_widget_);

  vlayout_ = new QVBoxLayout();

  if (selectable_) vlayout_->addWidget(new QLabel("", this));

  QLabel *name_label = new QLabel(QString::fromStdWString(player_name));
  vlayout_->addWidget(name_label);

  hlayout_ = new QHBoxLayout();
  network_label_ = new QLabel("网络状态", this);
  hlayout_->addWidget(network_label_);
  network_label_->setMinimumHeight(QFontMetrics(network_label_->font())
                                       .boundingRect(network_label_->text())
                                       .height() *
                                   1.2);
  network_circle_ = new NetworkCircle(
      network_label_, network_status,
      QFontMetrics(font()).boundingRect(network_label_->text()).height());
  hlayout_->addWidget(network_circle_);
  vlayout_->addLayout(hlayout_);

  bot_label_ =
      new QLabel(controlled_by_bot ? "机器人托管中" : "机器人未托管", this);
  vlayout_->addWidget(bot_label_);
  main_layout_->addLayout(vlayout_);

  main_layout_->setStretch(0, 8);
  main_layout_->setStretch(1, 1);

  card_multiset_ = new multiset<PCardLabel, bool (*)(const PCardLabel &,
                                                     const PCardLabel &)>(
      PCardLabelLess[type]);
  if (cards) {
    for (int i = 0; i < number_of_cards; i++)
      card_multiset_->insert(
          new CardLabel(cards_widget_, cards[i], direction_, selectable_));
  } else {
    for (int i = 0; i < number_of_cards; i++)
      card_multiset_->insert(new CardLabel(cards_widget_, Card{Club, 16},
                                           direction_, selectable_));
  }

  setLayout(main_layout_);
}

DeckWidget::~DeckWidget() {
  if (!exist_) return;
  delete hlayout_;
  delete vlayout_;
  delete main_layout_;
}

bool DeckWidget::Exist() const { return exist_; }

bool DeckWidget::UpdatePlayer(const double network_status,
                              const bool controlled_by_bot) {
  if (!exist_) return false;
  network_circle_->UpdateNetworkStatus(network_status);
  bot_label_->setText(controlled_by_bot ? "机器人托管中" : "机器人未托管");
  repaint();
  return true;
}

void DeckWidget::UpdateCards(const short delta, const Card cards[],
                             const bool is_not_south) {
  if (delta > 0) {
    if (is_not_south) {
      for (int i = 0; i < delta; i++) {
        auto pcard = card_multiset_->begin();
        pcard->value->close();
        card_multiset_->erase(pcard);
      }
    } else {
      for (int i = 0; i < delta; i++) {
        auto pcard =
            card_multiset_->find(new CardLabel(cards_widget_, cards[i]));
        pcard->value->close();
        card_multiset_->erase(pcard);
      }
    }
  } else {
    const short delta_true = delta * -1;
    if (is_not_south) {
      for (int i = 0; i < delta_true; i++)
        card_multiset_->insert(new CardLabel(cards_widget_, Card{Club, 16},
                                             direction_, selectable_));
    } else {
      for (int i = 0; i < delta_true; i++)
        card_multiset_->insert(
            new CardLabel(cards_widget_, cards[i], direction_, selectable_));
    }
  }
  QCoreApplication::postEvent(this, new QResizeEvent(size(), size()));
}

unsigned short DeckWidget::GetNumOfChoosenCard() const {
  unsigned short ret = 0;
  for (auto &card_label : *card_multiset_) ret += card_label.value->IsChoosen();
  return ret;
}

const Card *DeckWidget::GetChoosenCard(const unsigned short num) const {
  Card *const ret = new Card[num];
  unsigned short id = 0;
  for (auto &card_label : *card_multiset_) {
    if (card_label.value->IsChoosen()) ret[id++] = card_label.value->card_;
  }
  return ret;
}

void DeckWidget::resizeEvent(QResizeEvent *) {
  if (!exist_) return;
  const int height = cards_widget_->height();
  const int width = cards_widget_->width();
  if (direction_) {
    const int base_len = std::min<int>(height / (3 + selectable_),
                                       width / (card_multiset_->size() + 1));
    int cur_x = (width - base_len * (card_multiset_->size() + 1)) / 2;
    const int min_y = (height - base_len * (3 + selectable_)) / 2;
    const int max_y = min_y + base_len * selectable_;
    for (auto &card_label : *card_multiset_) {
      if (selectable_) {
        card_label.value->SetMinY(min_y);
        card_label.value->SetMaxY(max_y);
      }
      card_label.value->setGeometry(cur_x, max_y, base_len * 2, base_len * 3);
      card_label.value->show();
      card_label.value->raise();
      cur_x += base_len;
    }
  } else {
    const int base_len =
        std::min<int>(height / (card_multiset_->size() + 1), width / 3);
    int cur_y = (height - base_len * (card_multiset_->size() + 1)) / 2;
    const int base_x = (width - base_len * 3) / 2;
    for (auto &card_label : *card_multiset_) {
      card_label.value->setGeometry(base_x, cur_y, base_len * 3, base_len * 2);
      card_label.value->show();
      card_label.value->raise();
      cur_y += base_len;
    }
  }
}

DeskWidget::DeskWidget(QWidget *parent, const bool direction,
                       const GameType type, const short number_of_cards,
                       const Card cards[])
    : QWidget(parent), direction_(direction) {
  setAttribute(Qt::WA_DeleteOnClose);
  if (number_of_cards <= 0) {
    text_label_ = new QLabel(this);
    if (number_of_cards == 0) {
      text_label_->setText("已跳过");
    } else {
      std::wostringstream new_text;
      new_text << L"已摸" << -number_of_cards << L"张牌";
      text_label_->setText(QString::fromStdWString(new_text.str()));
    }
    text_label_->setAlignment(Qt::AlignCenter);
    card_multiset_ = NULL;
    return;
  }
  card_multiset_ = new multiset<PCardLabel, bool (*)(const PCardLabel &,
                                                     const PCardLabel &)>(
      PCardLabelLess[type]);
  for (int i = 0; i < number_of_cards; i++)
    card_multiset_->insert(new CardLabel(this, cards[i], direction_));
}

void DeskWidget::resizeEvent(QResizeEvent *) {
  if (!card_multiset_) {
    text_label_->setGeometry(0, 0, geometry().width(), geometry().height());
    return;
  }
  const int cur_height = height();
  const int cur_width = width();
  if (direction_) {
    const int base_len =
        std::min<int>(cur_height / 3, cur_width / (card_multiset_->size() * 2));
    int cur_x = (cur_width - base_len * (card_multiset_->size() * 2)) / 2;
    const int cur_y = (cur_height - base_len * 3) / 2;
    for (auto &card_label : *card_multiset_) {
      card_label.value->setGeometry(cur_x, cur_y, base_len * 2, base_len * 3);
      card_label.value->raise();
      cur_x += base_len * 2;
    }
  } else {
    const int base_len =
        std::min<int>(cur_height / (card_multiset_->size() * 2), cur_width / 3);
    int cur_y = (cur_height - base_len * (card_multiset_->size() * 2)) / 2;
    const int base_x = (cur_width - base_len * 3) / 2;
    for (auto &card_label : *card_multiset_) {
      card_label.value->setGeometry(base_x, cur_y, base_len * 3, base_len * 2);
      card_label.value->raise();
      cur_y += base_len * 2;
    }
  }
}

// id: 0 for the south(the player), 1 for the north,
//     2 for the east, 3 for the west
const int PlayWidget::pos_x[4] = {2, 0, 1, 1};
const int PlayWidget::pos_y[4] = {1, 1, 2, 0};

PlayWidget::PlayWidget(MainWindow *parent, const GameType type,
                       const wstring (&player_name)[4],
                       const unsigned short number_of_cards[4],
                       const double network_status[4],
                       const bool controlled_by_bot[4], const Card cards[])
    : ContentWidget(parent), type_(type) {
  for (int i = 0; i < 4; i++)
    deck[i] = new DeckWidget(this, i < 2, !i, type, player_name[i],
                             number_of_cards[i], network_status[i],
                             controlled_by_bot[i], i ? NULL : cards);

  for (int i = 0; i < 4; i++) qDebug() << player_name[i];

  main_layout_ = new QHBoxLayout(this);
  main_layout_->setMargin(2);
  main_layout_->setSpacing(2);

  QVBoxLayout *west_layout = new QVBoxLayout();
  west_layout->addWidget(new QLabel("", this));
  west_layout->addWidget(deck[3]);
  west_layout->addWidget(new QLabel("", this));
  west_layout->setStretch(0, 3);
  west_layout->setStretch(1, 15);
  west_layout->setStretch(2, 3);
  main_layout_->addLayout(west_layout);

  vlayout_ = new QVBoxLayout();
  vlayout_->addWidget(deck[1]);

  glayout_ = new QGridLayout();
  for (int i = 0; i < 4; i++)
    glayout_->addWidget(new QWidget(this), pos_x[i], pos_y[i]);
  if (type == Hearts) {
    QGridLayout *score_layout = new QGridLayout();
    QLabel *score_text = new QLabel("得分", this);
    score_text->setAlignment(Qt::AlignCenter);
    score_layout->addWidget(score_text, 1, 1);
    for (int i = 0; i < 4; i++) {
      score_label[i] = new QLabel(player_name[i][0] != '\0' ? "0" : "", this);
      score_label[i]->setAlignment(Qt::AlignCenter);
      score_layout->addWidget(score_label[i], pos_x[i], pos_y[i]);
    }
    for (int i = 0; i < 3; i++) {
      score_layout->setRowStretch(i, 1);
      score_layout->setColumnStretch(i, 1);
    }
    glayout_->addLayout(score_layout, 1, 1);
  }
  glayout_->setColumnStretch(0, 4);
  glayout_->setColumnStretch(1, 5);
  glayout_->setColumnStretch(2, 4);
  glayout_->setRowStretch(0, 4);
  glayout_->setRowStretch(1, 5);
  glayout_->setRowStretch(2, 4);
  vlayout_->addLayout(glayout_);

  QHBoxLayout *info_layout = new QHBoxLayout();
  info_label_ = new QLabel(this);
  info_label_->setStyleSheet("color:red;");
  info_label_->setSizePolicy(QSizePolicy::Expanding,
                             QSizePolicy::MinimumExpanding);
  info_label_->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  info_layout->addWidget(info_label_);
  QPushButton *skip_button = new QPushButton("跳过", this);
  skip_button->setFixedSize(
      QFontMetrics(font()).boundingRect(skip_button->text()).size() * 2);
  connect(skip_button, &QPushButton::released, this, &PlayWidget::Skip);
  info_layout->addWidget(skip_button);
  QPushButton *confirm_button = new QPushButton("确认", this);
  confirm_button->setFixedSize(
      QFontMetrics(font()).boundingRect(confirm_button->text()).size() * 2);
  connect(confirm_button, &QPushButton::released, this, &PlayWidget::Confirm);
  info_layout->addWidget(confirm_button);
  info_layout->addWidget(new QLabel("     ", this));
  vlayout_->addLayout(info_layout);

  vlayout_->addWidget(deck[0]);
  vlayout_->setStretch(0, 3);
  vlayout_->setStretch(1, 13);
  vlayout_->setStretch(2, 1);
  vlayout_->setStretch(3, 6);
  main_layout_->addLayout(vlayout_);

  QVBoxLayout *east_layout = new QVBoxLayout();
  east_layout->addWidget(new QLabel("", this));
  east_layout->addWidget(deck[2]);
  east_layout->addWidget(new QLabel("", this));
  east_layout->setStretch(0, 3);
  east_layout->setStretch(1, 15);
  east_layout->setStretch(2, 3);
  main_layout_->addLayout(east_layout);

  main_layout_->setStretch(0, 3);
  main_layout_->setStretch(1, 15);
  main_layout_->setStretch(2, 3);
  setLayout(main_layout_);
}

PlayWidget::~PlayWidget() {}

void PlayWidget::SetInfo(const wstring &info) {
  info_label_->setText(QString::fromStdWString(info));
  QRect rect = QFontMetrics(font()).boundingRect(info_label_->text());
  info_label_->setMinimumWidth(rect.width());
  info_label_->setMinimumHeight(rect.height());
}

bool PlayWidget::UpdatePlayer(const unsigned short id,
                              const double network_status,
                              const bool controlled_by_bot) {
  if (!deck[id]->Exist()) return false;
  return deck[id]->UpdatePlayer(network_status, controlled_by_bot);
}

bool PlayWidget::UpdateCards(const unsigned short id, const short delta,
                             const Card cards[], const bool show) {
  if (!deck[id]->Exist()) return false;
  deck[id]->UpdateCards(delta, cards, id);
  if (id == 0 && delta == 0)
    QCoreApplication::postEvent(
        deck[0], new QResizeEvent(deck[0]->size(), deck[0]->size()));
  if (show) {
    delete glayout_->itemAtPosition(pos_x[id], pos_y[id])->widget();
    glayout_->addWidget(new DeskWidget(this, true, type_, delta, cards),
                        pos_x[id], pos_y[id]);
  }
  return true;
}

bool PlayWidget::ClearDesk(const unsigned short id) {
  if (!deck[id]->Exist()) return false;
  delete glayout_->itemAtPosition(pos_x[id], pos_y[id])->widget();
  glayout_->addWidget(new QWidget(this), pos_x[id], pos_y[id]);
  return true;
}

void PlayWidget::UpdateStatistics(const unsigned short points[4]) {
  if (type_ != Hearts) return;
  for (int i = 0; i < 4; i++) {
    if (!deck[i]->Exist()) continue;
    std::ostringstream new_text;
    new_text << points[i];
    score_label[i]->setText(QString::fromStdString(new_text.str()));
  }
}

void PlayWidget::EndGame(const bool win_or_lose) {
  QMessageBox question;
  question.setWindowTitle("游戏结束");
  question.setText(win_or_lose ? "你赢了!" : "你输了...");
  question.addButton("再来一局", QMessageBox::YesRole)->setDefault(true);
  question.addButton("主页", QMessageBox::NoRole);
  const int index = question.exec();
  if (index == 0)
    ::PlayAgain();
  else if (index == 1)
    ::Home();
}

void PlayWidget::Skip() { ::Play(NULL, 0); }

void PlayWidget::Confirm() {
  const unsigned short num = deck[0]->GetNumOfChoosenCard();
  const Card *const cards = deck[0]->GetChoosenCard(num);
  ::Play(cards, num);
}
