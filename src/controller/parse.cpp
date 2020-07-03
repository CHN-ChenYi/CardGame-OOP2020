#ifndef _DEBUG
#define QT_NO_DEBUG_OUTPUT
#endif
#include <QDebug>
#include <string>
#include <vector>
#include "src/template/MainWindow.h"
using namespace std;

void ssplit(wstring& s, vector<wstring>& sv) {
    sv.clear();
    for(int i=0;i<s.length();i++)
    {
        if(s[i]==L'['||s[i]==L']'||s[i]==L',')
        {
            s=s.substr(0,i)+L' '+s[i]+L' '+s.substr(i+1,s.length()-i);
            i++;
        }
    }
    wstring temp=L"";
    int flag = 0;
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == L' ')
        {
            if (flag)
            {
                sv.push_back(temp);
                temp = L"";
                flag = 0;
            }
        }
        else
        {
            temp = temp + s[i];
            flag = 1;
        }
    }
    if (temp != L"")sv.push_back(temp);
}

void connect_join_parse(wstring &raw, wstring &player_name, wstring &password)
{
    vector<wstring> sv;
    ssplit(raw, sv);
    player_name = password = L"";
    if(sv.size()!=3)return;
    if(sv[0]!=L"-connect_join")return;
    player_name = sv[1];
    password = sv[2];
}

void player_rank_parse(wstring &raw, unsigned short& num_of_player, wstring* player_names,  double* network_status,
                 bool* controlled_by_bot, wstring current_name)
{
    vector<wstring> sv;
    ssplit(raw, sv);
    if(sv.size()<=2)return;
    if(sv[0]!=L"-player_rank")return;
    num_of_player = stoi(sv[1]);
    if(sv.size()!=2+num_of_player*3)return;

    wstring tmp_player_names[4];
    double tmp_network_status[4];
    bool tmp_controlled_by_bot[4];
    unsigned short real_pos[4];
    for(int i=0, j=2; i<num_of_player; i++, j+=3)
    {
        tmp_player_names[i] = sv[j];
        tmp_network_status[i] = stod(sv[j+1]);
        tmp_controlled_by_bot[i] = stoi(sv[j+2]);
    }
    for(int i=0; i<num_of_player; i++)
    {
        if(tmp_player_names[i] == current_name)
        {
            if(num_of_player == 2)
            {
                real_pos[i] = 0;
                real_pos[(i+1)%2] = 1;
            }
            else if(num_of_player == 3)
            {
                real_pos[i] = 0;
                real_pos[(i+1)%3] = 2;
                real_pos[(i+2)%3] = 3;
            }
            else if(num_of_player == 4)
            {
                real_pos[i] = 0;
                real_pos[(i+1)%4] = 2;
                real_pos[(i+2)%4] = 1;
                real_pos[(i+3)%4] = 3;
            }
            break;
        }
    }
    for(int i=0; i<num_of_player; i++)
    {
        player_names[real_pos[i]] = tmp_player_names[i];
        network_status[real_pos[i]] = tmp_network_status[i];
        controlled_by_bot[real_pos[i]] = tmp_controlled_by_bot[i];
    }
}

void card_list_parse(const vector<wstring>* const sv, unsigned short pos, card_list &cur)
{
    if(pos>=sv->size()||(*sv)[pos]!=L"[")return;
    pos++;
    cur.size = 0;
    for(;pos+5<sv->size();pos+=6)
    {
        Suit suit;
        unsigned short rank;
        wstring tmp = (*sv)[pos+1];
        if(tmp == L"C") suit = Club;
        else if(tmp == L"D") suit = Diamond;
        else if(tmp == L"H") suit = Heart;
        else suit = Spade;
        rank = stoul((*sv)[pos+3]);
        cur.ins(Card(suit,rank));
    }
}
void deal_cards_parse(wstring &raw, card_list &cur)
{
    vector<wstring> sv;
    ssplit(raw, sv);
    for(int i=0;i<sv.size();i++) qDebug()<<sv[i];
    if(sv.size()<=1) return;
    if(sv[0] != L"-deal_cards") return;
    card_list_parse(&sv, 1, cur);
}
void play_parse(wstring &raw, card_list &cur)
{
    vector<wstring> sv;
    ssplit(raw, sv);
    qDebug()<<"play_parse";
    for(int i=0;i<sv.size();i++) qDebug()<<sv[i];
    if(sv.size()<1) return;
    if(sv[0] != L"-play") return;
    card_list_parse(&sv, 1, cur);
}
void info_parse(wstring &raw, wstring &name, card_list &cur)
{
    vector<wstring> sv;
    ssplit(raw, sv);
    qDebug()<<"info_parse";
    for(int i=0;i<sv.size();i++) qDebug()<<sv[i];
    if(sv.size()<=1) return;
    if(sv[0] != L"-info") return;
    name = sv[1];
    card_list_parse(&sv, 2, cur);
}
void score_parse(wstring &raw, const unsigned short num_of_player, const wstring* const player_names, unsigned short* points)
{
    vector<wstring> sv;
    ssplit(raw, sv);
    qDebug()<<"score_parse";
    for(int i=0;i<sv.size();i++) qDebug()<<sv[i];
    if(sv.size()<=1) return;
    if(sv[0] != L"-score") return;
    if(sv.size()!=1+num_of_player*2) return;
    for(int i=0,j=1;i<num_of_player;i++,j+=2)
    {
        for(int k=0;k<4;k++)if(player_names[k]==sv[j])points[k]=stoul(sv[j+1]);
    }
}
void change_in_parse(wstring &raw, card_list &cur)
{
    vector<wstring> sv;
    ssplit(raw, sv);
    if(sv.size()<=1) return;
    if(sv[0] != L"-change_in") return;
    card_list_parse(&sv, 1, cur);
}
void change_out_parse(wstring &raw, card_list &cur)
{
    vector<wstring> sv;
    ssplit(raw, sv);
    if(sv.size()<=1) return;
    if(sv[0] != L"-change_out") return;
    card_list_parse(&sv, 1, cur);
}
void upd_status_parse(wstring &raw, const wstring* const player_names, double* network_status, bool* controlled_by_bot)
{
    vector<wstring> sv;
    ssplit(raw, sv);
    if(sv.size()<=2) return;
    if(sv[0] != L"-upd_status") return;
    for(int i=1;i<sv.size();i+=3)
    {
        for(int j=0;j<4;j++)if(player_names[j]==sv[i])network_status[j] = stod(sv[i+1]), controlled_by_bot[j] = stoul(sv[i+2]);
    }
}
