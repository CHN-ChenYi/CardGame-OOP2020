#pragma once

#include "MainWindow.h"

#include <QLabel>
#include <QWidget>
#include <set>
#include <string>
using std::multiset;
using std::wstring;

class MainWindow;

QT_BEGIN_NAMESPACE
class QLineEdit;
class QComboBox;
class QBoxLayout;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
QT_END_NAMESPACE

// operator< for each game mode
extern bool (*CardLess[2])(const Card &, const Card &);

// the network status circle
// when status == 1, it is green; when status == 0, it is red
class NetworkCircle : public QWidget {
 public:
  explicit NetworkCircle(QWidget *parent, const double network_status,
                         const int height);
  void UpdateNetworkStatus(const double network_status);

 private:
  QWidget *parent_;
  double network_status_;
  int height_;
  // repaint the circle to fit into the size of the parent widget
  void paintEvent(QPaintEvent *) override;
};

class ContentWidget : public QWidget {
  Q_OBJECT
 public:
  explicit ContentWidget(QWidget *parent = nullptr);
  virtual void SetInfo(const wstring &info) = 0;
  virtual ~ContentWidget(){};
};

class HomeWidget : public ContentWidget {
  Q_OBJECT

 public:
  explicit HomeWidget(MainWindow *parent);
  ~HomeWidget();
  // there's no place to show info in Home Page
  void SetInfo(const wstring &) override { return; }

 private:
  QVBoxLayout *vlayout_;  // main layout
  QPixmap *bg_image_;     // background image
  void SetBackgroundImage();
  // resize the background image
  void resizeEvent(QResizeEvent *event) override;
};

class InitOrJoinWidget : public ContentWidget {
  Q_OBJECT

 public:
  explicit InitOrJoinWidget(MainWindow *parent, bool widget_type);
  ~InitOrJoinWidget();
  void SetInfo(const wstring &info) override;

 private slots:
  void Accept();

 private:
  bool widget_type_;      // 0 for Init, 1 for Join
  QHBoxLayout *hlayout_;  // main layout
  QLabel *info_label_;
  QLineEdit *first_input_, *second_input_;
  QComboBox *combo_box_;
};

class WaitWidget : public ContentWidget {
  Q_OBJECT

 public:
  explicit WaitWidget(MainWindow *parent, const GameType type, bool is_owner);
  ~WaitWidget();
  void SetInfo(const wstring &info) override;
  bool AddPlayer(const unsigned short id, const wstring &player_name,
                 const double network_status);
  bool RemovePlayer(const unsigned short id);
  bool SetNetworkStatus(const unsigned short id, const double network_status);

 private slots:
  void AddBot();
  void StartGame();

 private:
  int id_[10], id_top_;   // a stack to record the place of each id
  QGridLayout *glayout_;  // the layout to show players
  QHBoxLayout *hlayout_;  // the bottom layout containing info_label and buttons
  QLabel *info_label_;
};

// a label which prints poker card
class CardLabel : public QLabel {
  Q_OBJECT

 public:
  Card card_;
  // direction true => vertical
  explicit CardLabel(QWidget *parent, Card card, bool direction = true,
                     bool selectable = false, int width = 0, int height = 0);
  void SetMaxY(const int y);
  void SetMinY(const int y);
  bool IsChoosen() const;

 private:
  QImage img_;  // the image of the card
  bool direction_, selectable_;
  int max_y_, min_y_;  // used for selecting cards
  // resize the img
  void resizeEvent(QResizeEvent *event) override;
  // if the card is selectable,
  // the card's y position will switch between min_y and max_y
  void mousePressEvent(QMouseEvent *event) override;
};

// the cards in hand
class DeckWidget : public QWidget {
  Q_OBJECT

 public:
  // direction false => vertical
  explicit DeckWidget(QWidget *parent, const bool direction,
                      const bool selectable_, const GameType type,
                      const wstring &player_name,
                      const unsigned short number_of_cards,
                      const double network_status, const bool controlled_by_bot,
                      const Card cards[] = NULL);
  ~DeckWidget();
  bool Exist() const;  // whether the deck exists
  bool UpdatePlayer(const double network_status, const bool controlled_by_bot);
  void UpdateCards(const short delta, const Card cards[],
                   const bool is_not_south);
  unsigned short GetNumOfChoosenCard() const;
  const Card *GetChoosenCard(const unsigned short num) const;

 private:
  // recalc the position of each card
  void resizeEvent(QResizeEvent *) override;
  bool exist_;
  bool direction_, selectable_;
  QBoxLayout *main_layout_;
  QWidget *cards_widget_;
  QVBoxLayout *vlayout_;  // the player info layout
  QHBoxLayout *hlayout_;  // the network info layout
  NetworkCircle *network_circle_;
  QLabel *bot_label_, *network_label_;
  multiset<CardLabel *, bool (*)(const CardLabel *, const CardLabel *)>
      *card_multiset_;
};

// the cards have been played
class DeskWidget : public QWidget {
  Q_OBJECT

 public:
  explicit DeskWidget(QWidget *parent, const bool overlap, const GameType type,
                      const short number_of_cards, const Card cards[] = NULL);

 private:
  // recalc the position of each card
  void resizeEvent(QResizeEvent *) override;
  bool overlap_;        // whether the cards can overlap each other
  QLabel *text_label_;  // if there's no cards, it prints the text info
  multiset<CardLabel *, bool (*)(const CardLabel *const,
                                 const CardLabel *const)> *card_multiset_;
};

class PlayWidget : public ContentWidget {
  Q_OBJECT

 public:
  explicit PlayWidget(MainWindow *parent, const GameType type,
                      const wstring (&player_name)[4],
                      const unsigned short number_of_cards[4],
                      const double network_status[4],
                      const bool controlled_by_bot[4], const Card cards[]);
  ~PlayWidget();
  void SetInfo(const wstring &info) override;
  bool UpdatePlayer(const unsigned short id, const double network_status,
                    const bool controlled_by_bot);
  bool UpdateCards(const unsigned short id, const short delta,
                   const Card cards[], const bool show);
  void UpdateStatistics(const unsigned short points[4]);
  bool ClearDesk(const unsigned short id);
  void EndGame(const bool win_or_lose);

 private slots:
  void Skip();
  void Confirm();

 private:
  static const int pos_x[4], pos_y[4];  // the pos of each player in gridlayout
  const GameType type_;
  QHBoxLayout *main_layout_;
  QVBoxLayout *vlayout_;  // the north player, glayout, the south player
  QGridLayout *glayout_;  // central cell's layout
  QLabel *info_label_, *score_label[4];
  DeckWidget *deck[4];
};
