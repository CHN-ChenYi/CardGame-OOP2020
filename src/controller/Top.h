#pragma once

#include <string>
using std::wstring;

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
  bool operator==(Card &u) { return suit == u.suit && rank == u.rank; }
  bool operator==(const Card &u) { return suit == u.suit && rank == u.rank; }
  Card(Suit suit = Club, unsigned short rank = 16) {
    this->suit = suit;
    this->rank = rank;
  }
  Card(const Card &u) {
    this->suit = u.suit;
    this->rank = u.rank;
  }
  Card &operator=(const Card &u) {
    this->suit = u.suit;
    this->rank = u.rank;
    return *this;
  }
  wstring towstring() const {
    wstring ret = L"[";
    if (suit == Club)
      ret += L"C";
    else if (suit == Diamond)
      ret += L"D";
    else if (suit == Heart)
      ret += L"H";
    else
      ret += L"S";
    ret += L",";
    ret += std::to_wstring(rank);
    ret += L"]";
    return ret;
  }
};

class card_list {
 public:
  unsigned short size;
  Card *cards;
  card_list() {
    size = 0;
    cards = (Card *)malloc(sizeof(Card) * 27);
  }
  card_list(const card_list &u) {
    size = u.size;
    cards = (Card *)malloc(sizeof(Card) * 27);
    for (int i = 0; i < size; i++) cards[i] = u.cards[i];
  }
  ~card_list() { delete[] cards; }
  void ins(Card u) {
    if (size < 27) cards[size++] = u;
  }
  void DeleteCard(unsigned short pos) {
    for (int j = pos; j < size - 1; j++) cards[j] = cards[j + 1];
    size--;
  }
  card_list &operator=(card_list &u) {
    size = u.size;
    for (int i = 0; i < size; i++) cards[i] = u.cards[i];
    return *this;
  }
  wstring towstring() const {
    wstring ret = L"[";
    for (int i = 0; i < size; i++) {
      ret += cards[i].towstring();
      if (i != size - 1) ret += L",";
    }
    ret += L"]";
    return ret;
  }
};

void ServerEventProcess();
void ClientEventProcess();
void calc_statistics();
void letplay(const unsigned short id);
bool CheckRules(const Card cards[], const unsigned short size,
                const GameType type, card_list &cur);
void NetworkEventProcess();
void NewGame(const wstring &password, const wstring &player_name,
             const GameType type);
void JoinGame(const wstring &password, const wstring &player_name);
void AddBot();
void StartGame();
// true => success, false => (fail and then SetInfo) / (the game has ended)
void Play(const Card cards[], const unsigned short size);
void PlayAgain();
void Home();
void Exit();
