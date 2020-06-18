#include <QDebug>

#include "MainWindow.h"

extern MainWindow *window;

void MainWindow::StartNetworkEventLoop(int interval) {}
void MainWindow::EndNetworkEventLoop() {}

void NetworkEventProcess() {}

void NewGame(const wstring &password, const wstring &player_name,
             const GameType type) {
  qDebug() << "New Game" << password << player_name << type;
  // window->DrawWaitingPage(type, L"127.0.0.1", true);

  // id: 0 for the south(the player), 1 for the north,
  //     2 for the east, 3 for the west
  wstring player_names[4] = {L"south", L"north", L"east", L"west"};
  unsigned short number_of_cards[4] = {10, 10, 10, 10};
  double network_status[4] = {1, 0, 0.2, 0.7};
  bool controlled_by_bot[4] = {false, true, false, false};
  window->DrawPlayingPage(Winner, player_names, number_of_cards, network_status,
                          controlled_by_bot, NULL);
}
void JoinGame(const wstring &password, const wstring &player_name) {
  qDebug() << "Join Game" << password << player_name;
  window->DrawWaitingPage(Winner, false);
}

wchar_t c[2] = L"0";
void AddBot() {
  qDebug() << "AddBot";
  window->SetInfo(L"AddBot");
  c[0]++;
  window->AddPlayer(c[0] - '0', c, 1.0 * rand() / RAND_MAX);
  window->SetNetworkStatus(c[0] - '0' - rand() % 4, 1.0 * rand() / RAND_MAX);
}
void StartGame() {
  qDebug() << "StartGame";
  window->SetInfo(L"StartGame");
  window->RemovePlayer(c[0] - '0' - 2);
}

// if return false, the cards will go back to players hand
bool Play(const Card cards[], const unsigned short size) {}
void PlayAgain() {}
void Home() {
  qDebug() << "home";
  window->DrawHomePage();
}
void Exit() {}

// id: 0 for the south(the player), 1 for the north, 2 for the east, 3 for the
// west status ranges from 0 to 1, 1 represents the best
void MainWindow::DrawInitPage() {}
void MainWindow::DrawJoinPage() {}
bool MainWindow::UpdatePlayer(const unsigned short id,
                              const double network_status,
                              const bool controlled_by_bot) {
}  // for Playing Page
// delta minus 0 means player draws some cards
bool MainWindow::UpdateCards(const unsigned short id, const short delta,
                             const Card cards[]) {}  // for Playing Page
void MainWindow::UpdateStatistics(const unsigned short points[4]) {
}  // for Playing Hearts Page
void MainWindow::EndGame(const bool win_or_lose,
                         const unsigned short points[4]) {}  // for Playing Page
