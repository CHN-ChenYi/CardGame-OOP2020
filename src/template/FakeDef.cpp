﻿#include <QDebug>

#include "MainWindow.h"

extern MainWindow *window;

void NewGame(const wstring &password, const wstring &player_name,
             const GameType type) {
  qDebug() << "New Game" << password << player_name << type;
  window->DrawWaitingPage(type, L"127.0.0.1", true);
}
void JoinGame(const wstring &ip, const wstring &password,
              const wstring &player_name) {
  qDebug() << "Join Game" << ip << password << player_name;
  window->DrawWaitingPage(Winner, ip, false);
}
void AddBot() {
  qDebug() << "AddBot";
  window->SetInfo(L"AddBot");
  static wchar_t c[2] = L"0";
  c[0]++;
  window->AddPlayer(c[0] - '0', c, (c[0] - '0') * 0.25);
  window->SetNetworkStatus(c[0] - '0' - 1, 0);
}
void StartGame() {
  qDebug() << "StartGame";
  window->SetInfo(L"StartGame");
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
bool MainWindow::RemovePlayer(const unsigned short id) {}  // for Waiting Page
void MainWindow::DrawPlayingPage(const GameType type,
                                 const wstring (&player_name)[4],
                                 const unsigned short number_of_cards[4],
                                 const double network_status[4],
                                 const bool controlled_by_bot[4]) {
}  // for Playing Page
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
