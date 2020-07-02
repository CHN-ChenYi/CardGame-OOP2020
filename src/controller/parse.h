#pragma once
#include<string>
#include<vector>
#include "src/template/MainWindow.h"
using namespace std;
void connect_join_parse(wstring &raw, wstring &player_name, wstring &password);
void player_rank_parse(wstring &raw, unsigned short& num_of_player, wstring* player_names,  double* network_status,
                 bool* controlled_by_bot, wstring current_name);
void card_list_parse(const vector<wstring>* const sv, unsigned short pos, card_list &cur);
void deal_cards_parse(wstring &raw, card_list &cur);
void play_parse(wstring &raw, card_list &cur);
void info_parse(wstring &raw, wstring &name, card_list &cur);
void score_parse(wstring &raw, const unsigned short num_of_player, const wstring* const player_names, unsigned short* points);
void change_in_parse(wstring &raw, card_list &cur);
void change_out_parse(wstring &raw, card_list &cur);
void upd_status_parse(wstring &raw, const wstring* const player_names, double* network_status, bool* controlled_by_bot);
