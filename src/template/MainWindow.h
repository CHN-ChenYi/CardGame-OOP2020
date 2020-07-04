#pragma once

#include <QMainWindow>
#include <string>
#include "src/controller/Top.h"

using std::wstring;

class ContentWidget;

QT_BEGIN_NAMESPACE
class QPushButton;
class QTextEdit;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow();
  void StartNetworkEventLoop(int interval);  // in milliseconds
  void EndNetworkEventLoop();
  void SetInfo(const wstring &info);  // for all pages except Home Page
  void DrawHomePage();
  void DrawInitPage();
  void DrawJoinPage();
  void DrawWaitingPage(const GameType type, bool is_owner);
  // network_status ranges from 0 to 1, 1 represents the best
  // true => success, false => fail
  bool AddPlayer(const unsigned short id, const wstring &player_name,
                 const double network_status);  // for Waiting Page
  bool RemovePlayer(const unsigned short id);   // for Waiting Page
  bool SetNetworkStatus(const unsigned short id,
                        const double network_status);  // for Waiting Page
  // id: 0 for the south(the player), 1 for the north,
  //     2 for the east, 3 for the west
  void DrawPlayingPage(const GameType type, const wstring (&player_name)[4],
                       const unsigned short number_of_cards[4],
                       const double network_status[4],
                       const bool controlled_by_bot[4],
                       const Card cards[]);  // for Playing Page
  // true => success, false => fail
  bool UpdatePlayer(const unsigned short id, const double network_status,
                    const bool controlled_by_bot);  // for Playing Page
  // delta minus 0 means player draws some cards
  bool UpdateCards(const unsigned short id, const short delta,
                   const Card cards[] = NULL,
                   const bool show = false);  // for Playing Page
  bool ClearDesk(const unsigned short id);    // for Playing Page
  // for Playing Hearts Page
  void UpdateStatistics(const unsigned short points[4]);
  void EndGame(const bool win_or_lose);  // for Playing Page

 private:
  ContentWidget *content_;
  QTimer *timer_;

 private slots:
  void Home();
  void Exit();
  void About();
  void Manual();
  void TimerEvent();

 public slots:
  void NewGame();
  void JoinGame();
};

class ManualWindow : public QMainWindow {
  Q_OBJECT

 public:
  ManualWindow();

 private:
  QTextEdit *text_;
  void resizeEvent(QResizeEvent *event) override;
};
