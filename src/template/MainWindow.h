#pragma once

#include <QMainWindow>
#include <string>

using std::wstring;

class ContentWidget;

QT_BEGIN_NAMESPACE
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
  // Joker, 16 for unknown
  unsigned short rank;
};

extern bool (*CardLess[2])(const Card &, const Card &);

void NetworkEventProcess();
void NewGame(const wstring &password, const wstring &player_name,
             const GameType type);
void JoinGame(const wstring &password, const wstring &player_name);
void AddBot();
void StartGame();
// true => success, false => fail and please SetInfo
bool Play(const Card cards[], const unsigned short size);
void PlayAgain();
void Home();
void Exit();

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow();
  void StartNetworkEventLoop(int interval);  // in milliseconds
  void EndNetworkEventLoop();
  void SetInfo(const wstring &info);
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
                   const Card cards[] = NULL);  // for Playing Page
  // for Playing Hearts Page
  void UpdateStatistics(const unsigned short points[4]);
  void EndGame(const bool win_or_lose,
               const unsigned short points[4]);  // for Playing Page

 private:
  ContentWidget *content_;
  QTimer *timer_;

 private slots:
  void Home();
  void About();
  void Manual();
  void TimerEvent();

 public slots:
  void NewGame();
  void JoinGame();
};
