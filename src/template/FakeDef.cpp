#include <QDebug>

#include "MainWindow.h"

extern MainWindow *window;

void NewGame(const wstring &password, const wstring &player_name,
             const GameType type) {
  qDebug() << "New Game" << password << player_name << type;
  window->SetInfo(L"Success");
}
void JoinGame(const wstring &ip, const wstring &password,
              const wstring &player_name) {
  qDebug() << "Join Game" << ip << password << player_name;
  window->SetInfo(L"Success");
}
void AddBot() {}
void StartGame() {}
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
void MainWindow::DrawWaitingPage(const GameType type, const wstring &ip,
                                 const wstring (&player_name)[4],
                                 const double network_status[4]) {}
void MainWindow::AddPlayer(const unsigned short id, const wstring &player_name,
                           const double network_status) {}  // for Waiting Page
void MainWindow::RemovePlayer(const unsigned short id) {}   // for Waiting Page
void MainWindow::SetNetworkStatus(const unsigned short id,
                                  const double network_status) {
}  // for Waiting Page
void MainWindow::DrawPlayingPage(const GameType type,
                                 const wstring (&player_name)[4],
                                 const unsigned short number_of_cards[4],
                                 const double network_status[4],
                                 const bool controlled_by_bot[4]) {
}  // for Playing Page
void MainWindow::UpdatePlayer(const unsigned short id,
                              const double network_status,
                              const bool controlled_by_bot) {
}  // for Playing Page
// delta minus 0 means player draws some cards
void MainWindow::UpdateCards(const unsigned short id, const short delta,
                             const Card cards[]) {}  // for Playing Page
void MainWindow::UpdateStatistics(const unsigned short points[4]) {
}  // for Playing Hearts Page
void MainWindow::EndGame(const bool win_or_lose,
                         const unsigned short points[4]) {}  // for Playing Page
