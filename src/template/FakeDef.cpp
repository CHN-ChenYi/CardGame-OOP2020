#include <QDebug>

#include "MainWindow.h"

extern MainWindow *window;

void NetworkEventProcess() { qDebug() << "NetworkEventProcess"; }

void NewGame(const wstring &password, const wstring &player_name,
             const GameType type) {
  qDebug() << "New Game" << password << player_name << type;
  window->DrawWaitingPage(type, true);
}

void JoinGame(const wstring &password, const wstring &player_name) {
  qDebug() << "Join Game" << password << player_name;
  // window->DrawWaitingPage(Winner, false);

  // id: 0 for the south(the player), 1 for the north,
  //     2 for the east, 3 for the west
  wstring player_names[4] = {L"south", L"north", L"east", L"west"};
  unsigned short number_of_cards[4] = {0, 10, 10, 10};
  double network_status[4] = {1, 0, 0.2, 0.7};
  bool controlled_by_bot[4] = {false, true, false, false};
  window->DrawPlayingPage(Winner, player_names, number_of_cards, network_status,
                          controlled_by_bot, NULL);

  for (int i = 0; i < 10; i++) {
    Card cardss[1] = {
        Card{Suit(rand() % 4), (unsigned short)(rand() % 15 + 1)}};
    window->UpdateCards(0, -1, cardss);
  }
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
void Play(const Card cards[], const unsigned short size) {
  unsigned short sta[4] = {(unsigned short)rand(), (unsigned short)rand(),
                           (unsigned short)rand(), (unsigned short)rand()};
  window->UpdateCards(0, size, cards, true);
  window->UpdateCards(1, size, cards, true);
  if (!size) {
    return;
  }
  window->UpdateStatistics(sta);
  window->UpdatePlayer(rand() % 4, rand() / RAND_MAX, rand() & 1);

  int num = 1;
  Card cardss[1] = {Card{Spade, 2}};
  // for (int i = 0; i < abs(num); i++) {
  //  cardss[i] =
  //    Card{rand() & 1 ? Club : Spade, (unsigned short)(rand() % 15 + 1)};
  //}
  int id = 0;
  qDebug() << id << " " << num;
  // window->UpdateCards(id, num, cardss);

  for (int i = 0; i < size; i++)
    qDebug() << cards[i].suit << " " << cards[i].rank;
  window->SetInfo(L"Play");
  return;
}
void PlayAgain() { qDebug() << "play again"; }
void Home() {
  qDebug() << "home";
  window->DrawHomePage();
}
void Exit() {}
