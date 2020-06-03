#pragma once

#include <QMainWindow>
#include <string>

using std::wstring;

class ContentWidget;

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QPushButton;
QT_END_NAMESPACE

enum GameType {
  Winner,  // 争上游
  Hearts   // 红心大战
};
enum Suit {
  Club,     // 梅花
  Diamond,  // 方块
  Heart,    // 红桃
  Spade     // 黑桃
};

struct Card {
  Suit suit;
  // 1 for A, 11 for J, 12 for Q, 13 for K, 14 for small Joker, 15 for the big
  // Joker
  unsigned rank;
};

void NewGame(const wstring &password, const wstring &player_name,
             const GameType type);
void JoinGame(const wstring &ip, const wstring &password,
              const wstring &player_name);
void AddBot();
void StartGame();
// if return false, the cards will go back to players hand
bool Play(const Card cards[], const unsigned short size);
void PlayAgain();
void Home();
void Exit();

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow();
  void SetInfo(const wstring &info);
  // id: 0 for the south(the player), 1 for the north, 2 for the east, 3 for the
  // west status ranges from 0 to 1, 1 represents the best
  void DrawHomePage();
  void DrawInitPage();
  void DrawJoinPage();
  void DrawWaitingPage(const GameType type, const wstring &ip,
                       const wstring (&player_name)[4],
                       const double network_status[4]);
  void AddPlayer(const unsigned short id, const wstring &player_name,
                 const double network_status);  // for Waiting Page
  void RemovePlayer(const unsigned short id);   // for Waiting Page
  void SetNetworkStatus(const unsigned short id,
                        const double network_status);  // for Waiting Page
  void DrawPlayingPage(const GameType type, const wstring (&player_name)[4],
                       const unsigned short number_of_cards[4],
                       const double network_status[4],
                       const bool controlled_by_bot[4]);  // for Playing Page
  void UpdatePlayer(const unsigned short id, const double network_status,
                    const bool controlled_by_bot);  // for Playing Page
  // delta minus 0 means player draws some cards
  void UpdateCards(const unsigned short id, const short delta,
                   const Card cards[] = NULL);  // for Playing Page
  void UpdateStatistics(
      const unsigned short points[4]);  // for Playing Hearts Page
  void EndGame(const bool win_or_lose,
               const unsigned short points[4]);  // for Playing Page

 private:
  QMenu *file_menu_;
  QMenu *help_menu_;
  QAction *home_act_;
  QAction *exit_act_;
  QAction *manual_act_;
  QAction *about_act_;
  QAction *about_qt_act_;
  ContentWidget *content_;

 private slots:
  void Home();
  void About();
  void Manual();

 public slots:
  void NewGame();
  void JoinGame();
};
