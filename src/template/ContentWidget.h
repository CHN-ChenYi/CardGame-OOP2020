#pragma once

#include "MainWindow.h"

#include <QBoxLayout>
#include <QDebug>
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
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
QT_END_NAMESPACE

class NetworkCircle : public QWidget {
 public:
  explicit NetworkCircle(QWidget *parent, const double network_status,
                         const int height);
  void UpdateNetworkStatus(const double network_status);

 private:
  QWidget *parent_;
  double network_status_;
  int height_;
  void paintEvent(QPaintEvent *) override;
};

class ContentWidget : public QWidget {
  Q_OBJECT
 public:
  explicit ContentWidget(QWidget *parent = nullptr);
  virtual void SetInfo(const wstring &info) = 0;
  virtual ~ContentWidget(){};
 signals:
};

class HomeWidget : public ContentWidget {
  Q_OBJECT

 public:
  explicit HomeWidget(MainWindow *parent);
  ~HomeWidget();
  void SetInfo(const wstring &) override {
    qDebug() << "Can't find HomeWidget::SetInfo";
  }

 protected:
  void resizeEvent(QResizeEvent *event) override;

 private:
  QVBoxLayout *vlayout_;
  QPixmap *bg_image_;
  void SetBackgroundImage();
};

class InitOrJoinWidget : public ContentWidget {
  Q_OBJECT

 public:
  explicit InitOrJoinWidget(MainWindow *parent, bool widget_type);
  ~InitOrJoinWidget();
  void SetInfo(const wstring &info);

 private slots:
  void Accept();

 private:
  bool widget_type_;  // 0 for Init, 1 for Join
  QHBoxLayout *hlayout_;
  QLabel *info_label_;
  QLineEdit *first_input_, *second_input_;
  QComboBox *combo_box_;
};

class WaitWidget : public ContentWidget {
  Q_OBJECT

 public:
  explicit WaitWidget(MainWindow *parent, const GameType type, bool is_owner);
  ~WaitWidget();
  void SetInfo(const wstring &info);
  bool AddPlayer(const unsigned short id, const wstring &player_name,
                 const double network_status);
  bool RemovePlayer(const unsigned short id);
  bool SetNetworkStatus(const unsigned short id, const double network_status);

 private slots:
  void AddBot();
  void StartGame();

 private:
  int id_[10], id_top_;
  QGridLayout *glayout_;
  QHBoxLayout *hlayout_;
  QLabel *info_label_;
};

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
  QImage img_;
  bool direction_, selectable_;
  int max_y_, min_y_;
  void resizeEvent(QResizeEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
};

struct PCardLabel {
  CardLabel *value;
  PCardLabel(CardLabel *value) : value(value) {}
  bool operator=(const PCardLabel &rhs) const {
    return value->card_.suit == rhs.value->card_.suit &&
           value->card_.rank == rhs.value->card_.rank;
  }
};

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
  bool Exist() const;
  bool UpdatePlayer(const double network_status, const bool controlled_by_bot);
  void UpdateCards(const short delta, const Card cards[],
                   const bool is_not_south);
  unsigned short GetNumOfChoosenCard() const;
  const Card *GetChoosenCard(const unsigned short num) const;

 private:
  void resizeEvent(QResizeEvent *) override;
  bool exist_;
  bool direction_, selectable_;
  QBoxLayout *main_layout_;
  QWidget *cards_widget_;
  QVBoxLayout *vlayout_;
  QHBoxLayout *hlayout_;
  NetworkCircle *network_circle_;
  QLabel *bot_label_, *network_label_;
  multiset<PCardLabel, bool (*)(const PCardLabel &, const PCardLabel &)>
      *card_multiset_;
};

class DeskWidget : public QWidget {
  Q_OBJECT

 public:
  explicit DeskWidget(QWidget *parent, const bool direction,
                      const GameType type, const unsigned short number_of_cards,
                      const Card cards[] = NULL);

 private:
  void resizeEvent(QResizeEvent *) override;
  bool direction_;
  QLabel *text_label_;
  multiset<PCardLabel, bool (*)(const PCardLabel &, const PCardLabel &)>
      *card_multiset_;
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
  void SetInfo(const wstring &info);
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
  static const int pos_x[4], pos_y[4];
  const GameType type_;
  QHBoxLayout *main_layout_;
  QVBoxLayout *vlayout_;
  QGridLayout *glayout_;
  QLabel *info_label_, *score_label[4];
  DeckWidget *deck[4];
};
