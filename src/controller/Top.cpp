#include <fcntl.h>
#include <io.h>
#include <QDebug>
#include <queue>
#include "src/AI/AI.h"
#include "parse.h"
#include "src/network/include/Client.h"
#include "src/network/include/MeyaS.h"
#include "src/network/include/Server.h"
#include "src/template/MainWindow.h"
#define tr(j) trans[num_of_player][j]
const int max_hist_size = 450;

wstring current_room = L"123";
GameType current_type = Winner;
wstring current_name = L"";
unsigned short num_of_player = 1;
wstring player_names[4] = {L"", L"", L"", L""};
unsigned short number_of_cards[4];
short route[4] = {-1, -1, -1, -1};
card_list current_card[4];
bool controlled_by_bot[4] = {false, false, false, false};
double network_status[4] = {0, 0, 0, 0};
unsigned short points[4];
bool isServer = false;
int current_stage = 1; //1:等待玩家 2:等待开始游戏
bool isreplied[10]={1,1,1,1,1,1,1,1,1,1};
unsigned short trans[5][4]={{0,0,0,0},{0,0,0,0},{0,1,0,0},{0,2,3,0},{0,2,1,3}};
card_list hist_card[max_hist_size];
unsigned short hist_size = 0;
unsigned short first_player = 0; // for Hearts
bool canplay = 0;
int final_winner = -1;
bool ischanged = 0;
short round = 0;
bool istypeknown = 0;
card_list tochange[4];
bool first_time_connect = 1;
extern MainWindow *window;

unsigned short card_value(Card u)
{
    if(u==Card(Spade, 12)) return 13;
    else if(u.suit==Heart) return 1;
    else return 0;
}

void DeleteFromCardList(Card* a, unsigned short &sa, const Card* const b, const unsigned short &sb)
{
    for(int i=0;i<sb;i++)
    {
        for(int j=0;j<sa;j++)
        {
            if(a[j] == b[i])
            {
                for(int k=j;k<sa-1;k++)a[k]=a[k+1];
                sa--;
                break;
            }
        }
    }
}

void NetworkEventProcess()
{
    qDebug() << "NetworkEventProcess";
    if(isServer) ServerEventProcess();
    else ClientEventProcess();
}
static auto soc = MeyaS::Server();
static auto client = MeyaS::Client();


void ServerEventProcess()
{
    if(current_stage == 0) return;
    if(current_stage == 1)
    {
        if(!soc.isFullyConnected())soc.accept();
        auto peers = soc.getPeers();
        for(int i = (int)peers.size() - 1; i >= 0; i--)
        {
          auto stream = peers.at(i)->getPeer();
          if(isreplied[i])
          {
              wstring tmp = L"-connect ";
              if(current_type == Hearts) tmp+=L'H';else tmp+=L'W';
              stream->sendLineW(tmp);
              isreplied[i]=0;
              qDebug() << L"-connect" << endl;
          }
          else
          {
             auto p = stream->getLineW();
             wstring player_name = L"", password = L"";
             connect_join_parse(p, player_name, password);
             qDebug()<<p;
             if(num_of_player < 4 && password == current_room)
             {
                 bool flag = true;
                 for(int j = 0;j < num_of_player; j++) if(player_name == player_names[j]) flag = false;
                 if(flag)
                 {
                     num_of_player++;
                     window->AddPlayer(num_of_player-1, player_name, 1.0);
                     player_names[num_of_player - 1] = player_name;
                     network_status[num_of_player - 1] = 1.0;
                     controlled_by_bot[num_of_player - 1] = false;
                     route[num_of_player - 1] = i;
                     isreplied[i] = true;
                     qDebug() << player_name << L" " << password << L" " << route[num_of_player - 1];
                 }
             }
             else if(player_name != L"")
             {
                 isreplied[i] = true;
                 qDebug()<<"wrong password or num_of_player exceeded";
                 if(password != current_room) peers.at(i)->getPeer()->sendLineW(L"-wrong_password");
                 else if(num_of_player >= 4) peers.at(i)->getPeer()->sendLineW(L"-player_exceeded");
             }
          }
        }
    }
    else if(current_stage == 2)
    {
        if(final_winner != -1)
        {
            qDebug()<<"finalwinner"<<final_winner;
            for(int i=0;i<num_of_player;i++)
            {
                if(route[i]!=-1)
                {
                    auto peers = soc.getPeers();
                    auto stream = peers.at(route[i])->getPeer();
                    stream->sendLineW(L"-over "+to_wstring(final_winner == i));
                    qDebug()<<"send over" << final_winner << i;
                }
            }
            window->EndGame(final_winner == 0);
            return;
        }
        if(current_type == Hearts && ischanged == 0)
        {
            bool flag = true;
            for(int i=0;i<num_of_player;i++)
            {
                if(tochange[i].size == 0) flag = false;
            }
            qDebug()<<"check_status "<<flag;
            for(int i=0;i<num_of_player;i++)
            {
                qDebug()<<tochange[i].size;
                for(int j=0;j<tochange[i].size;j++)qDebug()<<tochange[i].cards[j].suit<<" "<<tochange[i].cards[j].rank;
            }
            if(flag)
            {
                ischanged = 1;
                for(int i=0;i<num_of_player;i++)for(int j=0;j<3;j++)current_card[tr(i)].ins(tochange[i].cards[j]);
                window->UpdateCards(0, -3, tochange[0].cards, true);
                for(int i=0;i<num_of_player;i++)
                {
                    if(route[i]!=-1)
                    {
                        auto peers = soc.getPeers();
                        auto stream = peers.at(route[i])->getPeer();
                        wstring tmp = L"-change_in ";
                        tmp+=tochange[i].towstring();
                        stream->sendLineW(tmp);
                    }
                }
                for(int i=0;i<num_of_player;i++)
                {
                    bool flag = 0;
                    for(int j=0;j<current_card[tr(i)].size; j++)
                    {
                        if(current_card[tr(i)].cards[j] == Card(Club, 2))
                        {
                            flag = 1;
                            break;
                        }
                    }
                    if(flag)
                    {
                        first_player = i;
                        letplay(i);
                        break;
                    }
                }
                return;
            }
        }
        for(int i=0;i<num_of_player;i++)
        {
            if(route[i]!=-1)
            {
                network_status[tr(i)] *= 0.9;
                auto peers = soc.getPeers();
                auto stream = peers.at(route[i])->getPeer();
                wstring s = stream->getLineW();
                if(s.length() >= 14 && s.substr(0,14) == L"-check_network")
                {
                   network_status[tr(i)] = 1.0;
                   controlled_by_bot [tr(i)] = 0;
                   stream->sendLineW(L"-check_network");
                   continue;
                }
                if(s.length() >= 5 && s.substr(0,5) == L"-play")
                {
                    qDebug()<<"test9";
                    wstring tmp = L"-info " + player_names[tr(i)] + s.substr(5, s.length() - 5);
                    card_list cur;
                    play_parse(s, cur);
                    for(int i=0;i<cur.size;i++)hist_card[hist_size].cards[i] = cur.cards[i];
                    hist_card[hist_size].size = cur.size;
                    hist_size++;
                    qDebug()<<"hist_size"<<" "<<hist_size;
                    qDebug()<<"cur_size "<<cur.size;
                    for(int j=0;j<num_of_player;j++)
                    {
                        if(j!=i && route[j]!=-1)
                        {
                            auto stream2 = peers.at(route[j])->getPeer();
                            stream2->sendLineW(tmp);
                        }
                    }
                    qDebug()<<"first "<<current_card[tr(i)].size;
                    for(int k=0;k<cur.size;k++)
                    {
                        for(int j=current_card[tr(i)].size-1;j>=0;j--)
                        {
                            if(current_card[tr(i)].cards[j]==cur.cards[k])
                            {
                                for(int p=j;p<current_card[tr(i)].size-1;p++)
                                    current_card[tr(i)].cards[p]=current_card[tr(i)].cards[p+1];
                                current_card[tr(i)].size--;
                                break;
                            }
                        }
                    }
                    qDebug()<<"second "<<current_card[tr(i)].size;
                    window->UpdateCards(tr(i), cur.size, cur.cards, true);
                    calc_statistics();
                    if(current_type == Hearts && hist_size % num_of_player == 0)
                    {
                        letplay(first_player);
                    }
                    else
                    {
                        window->ClearDesk(tr((i+1)%num_of_player));
                        letplay((i+1)%num_of_player);
                    }
                    continue;
                }
                if(s.length() >= 11 && s.substr(0,11) == L"-change_out")
                {
                   change_out_parse(s, tochange[(i+1+round)%num_of_player]);
                   DeleteFromCardList(current_card[tr(i)].cards, current_card[tr(i)].size, tochange[(i+1+round)%num_of_player].cards, tochange[(i+1+round)%num_of_player].size);
                   for(int j=0;j<tochange[(i+1+round)%num_of_player].size;j++) qDebug()<<tochange[(i+1+round)%num_of_player].cards[j].suit<<" "<<tochange[(i+1+round)%num_of_player].cards[j].rank;
                   continue;
                }
                if(s.length() >= 7 && s.substr(0,7) == L"-escape")
                {
                    controlled_by_bot[tr(i)] = 1;
                    route[i] = -1;
                    continue;
                }
            }
        }
        for(int i=0;i<num_of_player;i++)
        {
            if(route[i]!=-1)
            {
                if(network_status[tr(i)]<0.03) controlled_by_bot[tr(i)] = 1;
                window->UpdatePlayer(tr(i), network_status[tr(i)], controlled_by_bot[tr(i)]);
            }
        }
    }
}

void ClientEventProcess()
{
    if(current_stage == 0) return;
    std::wstring s = client.handleMessage();
    qDebug() << s << endl;
    if(s.length() >= 10 && s.substr(0,8) == L"-connect")
    {
        wstring tmp = L"-connect_join " + current_name + L" " + current_room + L"$";
        auto stream = client.getPeer();
        stream->sendLineW(tmp);
        qDebug () << "connect"<< tmp << endl;
        current_type = (s[9]==L'H') ? Hearts : Winner;
        if(!istypeknown)  window->DrawWaitingPage(current_type, false);
        istypeknown = 1;
        return;
    }
    if(s.length() >= 15 && s.substr(0,15) == L"-wrong_password")
    {
        window->DrawJoinPage();
        window->SetInfo(L"密码错误");
        istypeknown = 0;
        window->EndNetworkEventLoop();
        return;
    }
    if(s.length() >= 16 && s.substr(0,16) == L"-player_exceeded")
    {
        window->DrawJoinPage();
        window->SetInfo(L"玩家个数超过上限");
        istypeknown = 0;
        window->EndNetworkEventLoop();
        return;
    }
    if(s.length() >= 14 && s.substr(0,14) == L"-check_network")
    {
        wstring tmp = L"-check_network 1$";
        auto stream = client.getPeer();
        stream->sendLineW(tmp);
        qDebug () << tmp << endl;
        return;
    }
    if(s.length() >= 12 && s.substr(0,12) == L"-player_rank")
    {
        player_rank_parse(s, num_of_player, player_names, network_status, controlled_by_bot, current_name);
        return;
    }
    if(s.length() >= 11 && s.substr(0,11) == L"-deal_cards")
    {
        qDebug()<<"-deal_cards";
        deal_cards_parse(s, current_card[0]);
        for(int i=0;i<4;i++) number_of_cards[i] = current_card[i].size = current_card[0].size;
        return;
    }
    if(s.length() >= 6 && s.substr(0,6) == L"-start")
    {
        window->DrawPlayingPage(current_type, player_names, number_of_cards, network_status, controlled_by_bot, current_card[0].cards);
        if(current_type==Hearts&&(round+1)%num_of_player!=0) ischanged = 0, canplay = 1;
        else ischanged = 1, canplay = 0;
        return;
    }
    if(s.length() >= 5 && s.substr(0,5) == L"-play")
    {
        canplay = 1;
        return;
    }
    if(s.length() >= 5 && s.substr(0,5) == L"-info")
    {
        qDebug()<<"-info";
        wstring name;
        card_list cur;
        info_parse(s, name, cur);
        for(int i=0;i<cur.size;i++)hist_card[hist_size].cards[i] = cur.cards[i];
        hist_card[hist_size].size = cur.size;
        hist_size++;
        for(int i=0;i<num_of_player;i++)
        {
            if(player_names[tr(i)]==name)
            {
                window->UpdateCards(tr(i), cur.size, cur.cards, true);
                if(!(current_type == Hearts && hist_size % num_of_player == 0)) window->ClearDesk(tr((i+1)%num_of_player));
            }
        }
        return;
    }
    if(s.length() >= 6 && s.substr(0,6) == L"-score")
    {
        score_parse(s, num_of_player, player_names, points);
        window->UpdateStatistics(points);
        return;
    }
    if(s.length() >= 7 && s.substr(0,5) == L"-over")
    {
        qDebug()<<"over event";
        window->EndGame(s[6]==L'1');
        return;
    }
    if(s.length() >= 7 && s.substr(0,7) == L"-replay")
    {
        qDebug()<<"-replay";
        for(int i=0;i<num_of_player;i++)window->UpdateCards(tr(i), -current_card[tr(i)].size, current_card[tr(i)].cards, false);
        if(current_type==Hearts&&(round+1)%num_of_player!=0) ischanged = 0, canplay = 1;
        else ischanged = 1, canplay = 0;
        return;
    }
    if(s.length() >= 10 && s.substr(0,10) == L"-change_in")
    {
        ischanged = 1;
        card_list tmp;
        change_in_parse(s, tmp);
        for(int i=0;i<3;i++)current_card[0].ins(tmp.cards[i]);
        window->UpdateCards(0, -3, tmp.cards, false);
        return;
    }
}

void NewGame(const wstring &password, const wstring &player_name,
             const GameType type) {
  qDebug() << "New Game" << password << player_name << type;
  window->DrawWaitingPage(type, true);
  current_room = password;
  current_type = type;
  current_name = player_name;
  isServer = true;
  current_stage = 1;
  player_names[0] = current_name;
  network_status[0] = 1.0;
  controlled_by_bot[0] = false;
  soc.startListening(3);
  window->StartNetworkEventLoop(1500);
}

void JoinGame(const wstring &password, const wstring &player_name) {
  qDebug() << "Join Game" << password << player_name;
  current_room = password;
  current_name = player_name;
  isServer = false;
  if(first_time_connect)
  {
      first_time_connect = 0;
      auto serverList = client.probeServer();
      while (serverList.empty()) serverList = client.probeServer();
      client.connectTo(MeyaS::Address::createAddress(serverList.at(0), DEFAULT_PORT));
      client.getPeer()->setWaitTime(1);
  }
  window->StartNetworkEventLoop(1500);
}

void AddBot() {
  qDebug() << "AddBot";
  //window->SetInfo(L"AddBot");
  if(num_of_player >= 4) return;
  num_of_player ++;
  network_status[num_of_player - 1] = 1.0;
  player_names [num_of_player - 1] = L"Rob" + to_wstring(num_of_player - 1);
  controlled_by_bot [num_of_player - 1] = true;
  window->AddPlayer(num_of_player - 1, player_names[num_of_player - 1], 1.0);
  //window->SetNetworkStatus(c[0] - '0' - rand() % 4, 1.0);
}
void deal_cards()
{
    Card tmpcard[108];
    Suit tmp[4] = {Club, Heart, Diamond, Spade};
    for(int i = 0; i < 52; i++) tmpcard[i] = Card(tmp[i/13],i%13+1);
    tmpcard[52] = Card(Club, 14);
    tmpcard[53] = Card(Club, 15);
    for(int i = 54; i < 108 ;i++) tmpcard[i] = tmpcard[i-54];
    if(num_of_player == 2)
    {
        if(current_type == Winner)
        {
            for(int i=0;i<rand()%50;i++)random_shuffle(tmpcard, tmpcard+54);
            current_card[0].size = current_card[1].size = 0;
            for(int i=0;i<22;i++) current_card[0].ins(tmpcard[i]);
            for(int i=22;i<44;i++) current_card[1].ins(tmpcard[i]);
        }
        else
        {
            for(int i=0;i<rand()%50;i++)random_shuffle(tmpcard, tmpcard+52);
            current_card[0].size = current_card[1].size = 0;
            for(int i=0;i<26;i++) current_card[0].ins(tmpcard[i]);
            for(int i=26;i<52;i++) current_card[1].ins(tmpcard[i]);
        }
    }
    else if(num_of_player == 3)
    {
        if(current_type == Winner)
        {
            for(int i=0;i<rand()%50;i++)random_shuffle(tmpcard, tmpcard+54);
            current_card[0].size = current_card[2].size = current_card[3].size = 0;
            for(int i=0;i<18;i++) current_card[0].ins(tmpcard[i]);
            for(int i=18;i<36;i++) current_card[2].ins(tmpcard[i]);
            for(int i=36;i<54;i++) current_card[3].ins(tmpcard[i]);
        }
        else
        {
            for(int i=0;i<rand()%50;i++)random_shuffle(tmpcard, tmpcard+52);
            while(tmpcard[52]==Card(Spade, 12)||tmpcard[52].suit==Heart) random_shuffle(tmpcard, tmpcard+52);
            current_card[0].size = current_card[2].size = current_card[3].size = 0;
            for(int i=0;i<17;i++) current_card[0].ins(tmpcard[i]);
            for(int i=17;i<34;i++) current_card[2].ins(tmpcard[i]);
            for(int i=34;i<51;i++) current_card[3].ins(tmpcard[i]);
        }
    }
    else if(num_of_player == 4)
    {
        if(current_type == Winner)
        {
            for(int i=0;i<rand()%50;i++)random_shuffle(tmpcard, tmpcard+54);
            current_card[0].size = current_card[1].size = current_card[2].size = current_card[3].size = 0;
            for(int i=0;i<13;i++) current_card[0].ins(tmpcard[i]);
            for(int i=13;i<26;i++) current_card[1].ins(tmpcard[i]);
            for(int i=26;i<39;i++) current_card[2].ins(tmpcard[i]);
            for(int i=39;i<52;i++) current_card[3].ins(tmpcard[i]);
        }
        else
        {
            for(int i=0;i<rand()%50;i++)random_shuffle(tmpcard, tmpcard+52);
            current_card[0].size = current_card[1].size = current_card[2].size = current_card[3].size = 0;
            for(int i=0;i<13;i++) current_card[0].ins(tmpcard[i]);
            for(int i=13;i<26;i++) current_card[1].ins(tmpcard[i]);
            for(int i=26;i<39;i++) current_card[2].ins(tmpcard[i]);
            for(int i=39;i<52;i++) current_card[3].ins(tmpcard[i]);
        }
    }
}

void AIChange(unsigned short i)
{
    unsigned short *tmp = Exchange_Hearts(current_card[tr(i)]);
    sort(tmp,tmp+3);
    for(int j=2;j>=0;j--)
    {
        tochange[(1+i+round)%num_of_player].ins(current_card[tr(i)].cards[tmp[j]]);
        current_card[tr(i)].DeleteCard(j);
    }
}
void StartGame() {
  qDebug() << "StartGame";
  soc.stopListening();
  final_winner = -1;
  window->EndNetworkEventLoop();
  wstring tmp_player_names[4];
  double tmp_network_status[4];
  bool tmp_controlled_by_bot[4];
  for(int i=0;i<num_of_player;i++) tmp_player_names[i] = player_names[i], tmp_network_status[i] = network_status[i], tmp_controlled_by_bot[i] = controlled_by_bot[i];
  for(int i=0;i<4;i++) player_names[i] = L"", network_status[i] = 0, controlled_by_bot[i] = 0;
  for(int i=0;i<num_of_player;i++) player_names[tr(i)] = tmp_player_names[i], network_status[tr(i)] = tmp_network_status[i], controlled_by_bot[tr(i)] = tmp_controlled_by_bot[i];
  deal_cards();
  for(int i = 0; i < num_of_player; i++)
  {
      if(route[i] != -1)
      {
          wstring s = L"-check_network";
          auto peers = soc.getPeers();
          auto stream = peers.at(route[i])->getPeer();
          stream->sendLineW(s);
          s = L"-player_rank " + to_wstring(num_of_player);
          for(int j = 0 ; j < num_of_player; j++)
          {
              s = s + L" " + player_names[tr(j)] + L" " + to_wstring(network_status[tr(j)]) + L" " + to_wstring((int)controlled_by_bot[tr(j)]);
          }
          //s += L"$";
          qDebug() << s << endl;
          stream->sendLineW(s);
          s = L"-deal_cards ";
          s += current_card[tr(i)].towstring();
          qDebug() << s << endl;
          stream->sendLineW(s);
          s = L"-start";
          qDebug() << s << endl;
          stream->sendLineW(s);
      }
  }

  for(int i=0;i<4;i++)number_of_cards[i]=current_card[i].size;
  /*for(int i=0;i<4;i++)
  {    qDebug()<<current_type<<" "<<player_names[i]<<" "<<number_of_cards[i]<<" "<<network_status[i]<<" "<<controlled_by_bot[i]<<endl;
        for(int j=0;j<current_card[i].size;j++)qDebug()<<(current_card[i].cards[j].suit) <<" "<<current_card[i].cards[j].rank<<endl;
  }*/
  window->DrawPlayingPage(current_type, player_names, number_of_cards, network_status,
                          controlled_by_bot, current_card[0].cards);

  current_stage = 2;
  window->StartNetworkEventLoop(1500);
  if(current_type == Hearts)
  {
      if((round+1)%num_of_player!=0) //换牌
      {
          for(int i=0;i<num_of_player;i++) tochange[i].size=0;
          canplay = 1;
          ischanged = 0;
          for(int i=1;i<num_of_player;i++)
          {
              if(controlled_by_bot[tr(i)]) //aichange
              {
                  AIChange(i);
              }
          }
      }
      else
      {
          ischanged = 1;
          canplay = 0;
          for(int i=0;i<num_of_player;i++)
          {
              bool flag = 0;
              for(int j=0;j<current_card[tr(i)].size; j++)
              {
                  if(current_card[tr(i)].cards[j] == Card(Club, 2))
                  {
                      flag = 1;
                      break;
                  }
              }
              if(flag)
              {
                  first_player = i;
                  letplay(i);
                  break;
              }
          }
      }
  }
  else
  {
      first_player = rand() % num_of_player;
      letplay(first_player);
      qDebug()<<"first_player "<<first_player;
  }
}

void letplay(const unsigned short id)
{
    //static int cnt = 0;
    qDebug()<<"letplay"<<id;
    if(id == 0)
    {
        canplay = 1;
        return;
    }
    if(controlled_by_bot[tr(id)] || route[id] == -1)
    {
        //window->EndNetworkEventLoop();
        //get ai play
        card_list cur;
        unsigned short* tmp;
        if(current_type == Winner) tmp = Play_Winners(current_card[tr(id)], hist_card, hist_size, cur.size);
        else tmp = Play_Hearts(current_card[tr(id)], hist_card, hist_size), cur.size = 1;
        for(int i=0;i<cur.size;i++) cur.cards[i] = current_card[tr(id)].cards[tmp[i]];
        sort(tmp,tmp+cur.size);
        for(int i=cur.size-1;i>=0;i--) current_card[tr(id)].DeleteCard(tmp[i]);
        hist_card[hist_size++]=cur;
        window->UpdateCards(tr(id), cur.size, cur.cards, true);
        for(int i=1;i<num_of_player;i++)
        {
            if(i!=id && route[i]!=-1)
            {
                wstring s=L"-info "+player_names[tr(id)]+L" "+cur.towstring();
                qDebug()<<s;
                auto peers = soc.getPeers();
                auto stream = peers.at(route[i])->getPeer();
                stream->sendLineW(s);
            }
        }
        calc_statistics();
        if(final_winner == -1)
        {
            window->ClearDesk(tr((id+1)%num_of_player));
            letplay((id+1)%num_of_player);
        }
    }
    else
    {
        wstring s = L"-play";
        auto peers = soc.getPeers();
        auto stream = peers.at(route[id])->getPeer();
        stream->sendLineW(s);
    }
}

// if return false, the cards will go back to players hand
void Play(const Card cards[], const unsigned short size)
{
    if(canplay == 0)
    {
        window->UpdateCards(0, 0);
        return;
    }
    qDebug()<<"PlayNow";
    qDebug()<<size;
    for(int i=0;i<size;i++) qDebug()<<cards[i].suit<<" "<<cards[i].rank;

    if(current_type == Hearts && ischanged == 0) //换牌
    {
        if(size!=3)
        {
            window->UpdateCards(0, 0);
            return;
        }
        DeleteFromCardList(current_card[0].cards, current_card[0].size, cards, size);
        window->UpdateCards(0, 3, cards, false);
        canplay = 0;
        if(isServer)
        {
            tochange[(1+round)%num_of_player].size = size;
            for(int i=0;i<size;i++)tochange[(1+round)%num_of_player].cards[i]=cards[i];
            return;
        }
        else
        {
            wstring s = L"-change_out ";
            card_list tmp;
            tmp.size=size; for(int i=0;i<size;i++)tmp.cards[i]=cards[i];
            s = s+tmp.towstring()+L"$";
            auto stream = client.getPeer();
            stream->sendLineW(s);
            qDebug()<<s;
            return;
        }
    }

    if(!CheckRules(cards, size, current_type, current_card[0]))
    {
        window->UpdateCards(0, 0);
        return;
    }
    for(int i=0;i<size;i++)hist_card[hist_size].cards[i] = cards[i];
    hist_card[hist_size].size = size;
    hist_size++;
    window->UpdateCards(0, size, cards, true);
    canplay = 0;
    if(isServer)
    {
        calc_statistics();
        for(int i=1;i<num_of_player;i++)
        {
            if(route[i]!=-1)
            {
                card_list tmp;
                tmp.size = size;
                for(int j=0;j<size;j++)tmp.cards[j]=cards[j];
                wstring s=L"-info "+current_name+L" "+tmp.towstring();
                qDebug()<<s;
                auto peers = soc.getPeers();
                auto stream = peers.at(route[i])->getPeer();
                stream->sendLineW(s);
            }
        }
        if(current_type == Hearts && hist_size % num_of_player == 0)
        {
            letplay(first_player);
        }
        else
        {
            window->ClearDesk(tr(1));
            letplay(1);
        }
    }
    else
    {
        wstring s = L"-play ";
        card_list tmp; tmp.size=size; for(int i=0;i<size;i++)tmp.cards[i]=cards[i];
        s += tmp.towstring();
        s += L"$";
        auto stream = client.getPeer();
        stream->sendLineW(s);
        if(!(current_type == Hearts && hist_size % num_of_player == 0)) window->ClearDesk(tr(1));
    }
    qDebug()<<"test8";
}

void PlayAgain()
{
    qDebug() << "play again";
    final_winner = -1;
    round ++;
    for(int i=0;i<hist_size;i++)hist_card[i].size = 0;
    hist_size = 0;
    canplay = 0;
    for(int i=0;i<num_of_player;i++)tochange[i].size = 0;
    if(isServer)
    {
        deal_cards();
        for(int i=0;i<4;i++)number_of_cards[i]=current_card[i].size;
        window->DrawPlayingPage(current_type, player_names, number_of_cards, network_status,
                                controlled_by_bot, current_card[0].cards);
    }
    else
    {
        for(int i=0;i<4;i++) number_of_cards[i] = current_card[i].size = 0;
        window->DrawPlayingPage(current_type, player_names, number_of_cards, network_status, controlled_by_bot, NULL);
        return;
    }
    for(int i = 0; i < num_of_player; i++)
    {
        if(route[i] != -1)
        {
            auto peers = soc.getPeers();
            auto stream = peers.at(route[i])->getPeer();
            wstring s;
            s = L"-deal_cards ";
            s += current_card[tr(i)].towstring();
            qDebug() << s << endl;
            stream->sendLineW(s);
            stream->sendLineW(L"-replay");
        }
    }
    if(current_type == Hearts)
    {
        if((round+1)%num_of_player!=0) //换牌
        {
            for(int i=0;i<num_of_player;i++) tochange[i].size=0;
            canplay = 1;
            ischanged = 0;
            for(int i=1;i<num_of_player;i++)
            {
                if(controlled_by_bot[tr(i)])
                {
                    AIChange(i);
                }
            }
        }
        else
        {
            ischanged = 1;
            canplay = 0;
            for(int i=0;i<num_of_player;i++)
            {
                bool flag = 0;
                for(int j=0;j<current_card[tr(i)].size; j++)
                {
                    if(current_card[tr(i)].cards[j] == Card(Club, 2))
                    {
                        flag = 1;
                        break;
                    }
                }
                if(flag)
                {
                    first_player = i;
                    letplay(i);
                    break;
                }
            }
        }
    }
    else
    {
        first_player = rand() % num_of_player;
        letplay(first_player);
        qDebug()<<"first_player "<<first_player;
    }
}
void Home() {
  qDebug() << "home";
  //window->EndNetworkEventLoop();
  window->DrawHomePage();
  current_room = L"123";
  current_name = L"";
  num_of_player = 1;
  for(int i=0;i<4;i++)player_names[i]=L"",current_card[i].size = tochange[i].size = 0;
  memset(route,-1,sizeof(route));
  memset(controlled_by_bot,0,sizeof(controlled_by_bot));
  memset(network_status,0,sizeof(network_status));
  memset(points,0,sizeof(points));
  isServer = false;
  current_stage = 0;
  for(int i=0;i<10;i++)isreplied[i] = 1;
  for(int i=0;i<hist_size;i++)hist_card[i].size = 0;
  hist_size = 0;
  first_player = 0;
  canplay = 0;
  final_winner = -1;
  ischanged = 0;
  round = 0;
  istypeknown = 0;
}
void Exit()
{
    if(isServer)
    {
        return; //不知道干啥
    }
    else
    {
        wstring tmp = L"-escape";
        auto stream = client.getPeer();
        stream->sendLineW(tmp);
    }
}

bool winner_cmp(const unsigned short flag, const unsigned short last_flag,
                const pair<unsigned short, unsigned short> feat_value, const pair<unsigned short, unsigned short> last_value)
{
    short rank_winner[17] = {-1, 12, 13, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 14, 15, -1};
    if(flag == 10) return true;
    if(last_flag == 10) return false;
    if(flag == 4 && last_flag != 4) return true;
    else if(flag != 4 && last_flag == 4) return false;
    else if(flag == 4 && last_flag == 4) return rank_winner[feat_value.first] > rank_winner[last_value.first];
    if(flag != last_flag) return false;
    if(flag <= 3) return rank_winner[feat_value.first] > rank_winner[last_value.first];
    if(flag <= 7 || flag == 11)
    {
        if(feat_value.second - feat_value.first != last_value.second - last_value.first) return false;
        return rank_winner[feat_value.first] > rank_winner[last_value.first];
    }
    if(flag <= 9) return rank_winner[feat_value.first] > rank_winner[last_value.first];
    return false;
}

unsigned short winner_checktype(const Card cards[], const unsigned short size, pair<unsigned short, unsigned short> &feat_value)
{
    int cnt[16];memset(cnt,0,sizeof(cnt));
    for(int i=0;i<size;i++)cnt[cards[i].rank]++;
    for(int i=1;i<16;i++)
    {
        if(cnt[i]==size)
        {
            feat_value = make_pair(i,0);
            return size;
        }
    }
    for(int i=3,j=0;i<=14;i++)
    {
        if(cnt[(i==14)?1:i]!=1) j=0; else j++;
        if(j>=5&&j==size)
        {
            feat_value = make_pair(i-j+1, (i==14)?1:i);
            return 5; //顺子
        }
    }
    for(int i=3,j=0;i<=14;i++)
    {
        if(cnt[(i==14)?1:i]!=2) j=0; else j++;
        if(j>=3&&j*2==size)
        {
            feat_value = make_pair(i-j+1, (i==14)?1:i);
            return 6; //连对
        }
    }
    for(int i=3,j=0;i<=14;i++)
    {
        if(cnt[(i==14)?1:i]!=3) j=0; else j++;
        if(j>=2&&j*3==size)
        {
            feat_value = make_pair(i-j+1, (i==14)?1:i);
            return 7; //三连对
        }
    }
    if(size == 5)
    for(int i=1,j=-1,k=-1;i<=13;i++)
    {
        if(cnt[i]==3) j=i;
        else if(cnt[i]== 2) k=i;
        if(j!=-1&&k!=-1)
        {
            feat_value = make_pair(j,0);
            return 8; //三带二
        }
    }
    if(size == 6)
    for(int i=1;i<=13;i++)
    {
        if(cnt[i]==4)
        {
            feat_value = make_pair(i,0);
            return 9; //四带二
        }
    }
    if(size == 2)
    {
        if(cnt[14]==1&&cnt[15]==1)
        {
            feat_value = make_pair(0,0);
            return 10; //王炸
        }
    }
    bool flag = 1;
    unsigned short st = 0, ed = 13;
    for(int i=1;i<=13;i++)
    {
        if(cnt[i]!=2&&cnt[i]!=3&&cnt[i]!=0) flag=0;
        if(cnt[i]==3)
        {
            st = i;
            for(ed=i;ed<=13;ed++)if(cnt[ed]!=3)break;
            ed--;
            for(int j=ed+1;j<=13;j++)if(cnt[j]==3)flag=0;
            break;
        }
    }
    if(flag && size == (ed-st+1) * 5)
    {
        feat_value = make_pair(st,ed);
        return 11; //飞机
    }
    return 0;
}

bool CheckRules(const Card cards[], const unsigned short size, const GameType type, card_list &cur)
{
    if(size>cur.size)return false;
    int vis[27];memset(vis,0,sizeof(vis));
    for(int i=0;i<size;i++)
    {
       bool flag=false;
       for(int j=0;j<cur.size;j++)
       {
           if(vis[j])continue;
           if(cur.cards[j]==cards[i])
           {
              vis[j]=1;
              flag=true;
              break;
           }
       }
       if(!flag)return false;
    }
    if(type == Hearts)
    {
        if(size != 1) return false;
        if(hist_size == 0)
        {
            if(!(cards[0].suit == Club && cards[0].rank == 2)) return false;
        }
        else if(hist_size % num_of_player != 0)
        {
            Card tmp = hist_card[hist_size / num_of_player * num_of_player].cards[0];
            if(tmp.suit!=cards[0].suit)
            {
                for(int i=0;i<cur.size;i++) if(cur.cards[i].suit==tmp.suit) return false;
            }
        }
    }
    else
    {
        bool isfirst = true;
        for(int i = 1; i < num_of_player; i++) if(hist_size >= i && hist_card[hist_size - i].size != 0) isfirst = false;
        if(isfirst && size == 0) return false;
        if(size == 0) return true;
        pair<unsigned short, unsigned short> feat_value, last_feat_value;
        unsigned short flag = winner_checktype(cards, size, feat_value);
        if(flag == 0) return false;
        if(!isfirst)
        {
            unsigned short last_flag = 0;
            for(int i = 1; i < num_of_player; i++) if(hist_size >= i && hist_card[hist_size - i].size != 0)
            {
                last_flag = winner_checktype(hist_card[hist_size - i].cards, hist_card[hist_size - i].size, last_feat_value);
                break;
            }
            if(!winner_cmp(flag, last_flag, feat_value, last_feat_value)) return false;
        }
    }
    for(int i=cur.size-1;i>=0;i--)
    {
        if(vis[i])
        {
            for(int j=i;j<cur.size-1;j++) cur.cards[j] = cur.cards[j+1];
            cur.size--;
        }
    }
    return 1;
}

void calc_statistics()
{
    qDebug()<<"calc_statistics";
    unsigned short now = (hist_size+first_player-1)%num_of_player;
    qDebug()<<now<<" "<<points[tr(now)]<<" "<<current_card[tr(now)].size;
    if(current_type == Hearts)
    {
        if(hist_size % num_of_player != 0) return;
        unsigned short pos=(now+1)%num_of_player;
        unsigned short pt = 0;
        Card tmp = hist_card[hist_size-num_of_player].cards[0];
        pt += card_value(tmp);
        for(int i=1;i<num_of_player;i++)
        {
            Card tmp2 = hist_card[hist_size-i].cards[0];
            pt += card_value(tmp2);
            if(tmp.suit == tmp2.suit && (tmp2.rank == 1 || tmp2.rank > tmp.rank))
            {
                tmp = tmp2;
                pos = (now-i+1+num_of_player)%num_of_player;
            }
        }
        first_player = pos;
        qDebug()<<"lastwinner"<<" "<<pos;
        if(pt == 0)return;
        points[tr(pos)] += pt;
        qDebug()<<"points";
        for(int i=0;i<num_of_player;i++)qDebug()<<points[i];
        window->UpdateStatistics(points);
    }
    else
    {
        if(current_card[tr(now)].size==0)points[tr(now)]++;
        else return;
        window->UpdateStatistics(points);
    }

    for(int i=1;i<num_of_player;i++)
    {
        if(route[i]!=-1)
        {
            wstring s = L"-score";
            for(int j=0;j<num_of_player;j++) s+=L" "+player_names[tr(j)]+L" "+to_wstring(points[tr(j)]);
            auto peers = soc.getPeers();
            auto stream = peers.at(route[i])->getPeer();
            stream->sendLineW(s);
            qDebug()<<s;
        }
    }

    if(current_card[tr(now)].size == 0)
    {
        final_winner = now;
        if(current_type == Hearts)
        {
            for(int i=0;i<num_of_player;i++)if(points[tr(i)]<points[tr(now)]) final_winner=i;
        }
    }
}
