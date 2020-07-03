#include "MainWindow.h"

#include <QDebug>
#include <QTextEdit>
#include <QtWidgets>

#include "ContentWidget.h"

bool (*CardLess[2])(const Card &, const Card &) = {
    [](const Card &lhs, const Card &rhs) {
      static constexpr short rank_winner[17] = {
          -1, 12, 13, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 14, 15, -1};
      return rank_winner[lhs.rank] < rank_winner[rhs.rank];
    },
    [](const Card &lhs, const Card &rhs) {
      static constexpr short rank_heart[17] = {-1, 13, 1,  2,  3,  4,  5,  6, 7,
                                               8,  9,  10, 11, 12, 14, 15, -1};
      return rank_heart[lhs.rank] < rank_heart[rhs.rank];
    }};

MainWindow::MainWindow() : timer_(NULL) {
  content_ = new HomeWidget(this);
  setCentralWidget(content_);

  QAction *home_act = new QAction("Home", this);
  connect(home_act, &QAction::triggered, this, &MainWindow::Home);

  QAction *exit_act = new QAction("Exit", this);
  // exit_act->setShortcuts(QKeySequence::Quit);
  // exit_act->setStatusTip("Exit the application");
  connect(exit_act, &QAction::triggered, this, &MainWindow::Exit);

  QAction *manual_act = new QAction("Manual", this);
  connect(manual_act, &QAction::triggered, this, &MainWindow::Manual);

  QAction *about_act = new QAction("About", this);
  connect(about_act, &QAction::triggered, this, &MainWindow::About);

  QAction *about_qt_act = new QAction("About Qt", this);
  connect(about_qt_act, &QAction::triggered, qApp, &QApplication::aboutQt);

  QMenu *file_menu = menuBar()->addMenu("File");
  file_menu->addAction(home_act);
  file_menu->addAction(exit_act);
  QMenu *help_menu = menuBar()->addMenu("Help");
  help_menu->addAction(manual_act);
  help_menu->addAction(about_act);
  help_menu->addAction(about_qt_act);

  // QString message = tr("Status tip");
  // statusBar()->showMessage(message);

  setWindowTitle("OOP2020 Card Game");
  setMinimumSize(480, 320);
}

void MainWindow::Home() { ::Home(); }

void MainWindow::Exit() {
  ::Exit();
  close();
}

void MainWindow::About() {
  QMessageBox::about(this, "About", "一个在面向对象课程中完成的纸牌游戏");
}

void MainWindow::Manual() {
  ManualWindow *manual_window = new ManualWindow();
  manual_window->show();
}

void MainWindow::StartNetworkEventLoop(int interval) {
  if (timer_) delete timer_;
  timer_ = new QTimer(this);
  connect(timer_, &QTimer::timeout, this, &MainWindow::TimerEvent);
  timer_->start(interval);
}

void MainWindow::TimerEvent() { ::NetworkEventProcess(); }

void MainWindow::EndNetworkEventLoop() {
  delete timer_;
  timer_ = NULL;
}

void MainWindow::NewGame() {
  delete content_;
  content_ = new InitOrJoinWidget(this, 0);
  setCentralWidget(content_);
  qDebug() << "new Game";
}

void MainWindow::JoinGame() {
  delete content_;
  content_ = new InitOrJoinWidget(this, 1);
  setCentralWidget(content_);
  qDebug() << "join Game";
}

void MainWindow::DrawHomePage() {
  delete content_;
  content_ = new HomeWidget(this);
  setCentralWidget(content_);
}

void MainWindow::DrawInitPage() { NewGame(); }

void MainWindow::DrawJoinPage() { JoinGame(); }

void MainWindow::SetInfo(const wstring &info) { content_->SetInfo(info); }

void MainWindow::DrawWaitingPage(const GameType type, bool is_owner) {
  delete content_;
  content_ = new WaitWidget(this, type, is_owner);
  setCentralWidget(content_);
}

bool MainWindow::AddPlayer(const unsigned short id, const wstring &player_name,
                           const double network_status) {
  WaitWidget *cur_widget = dynamic_cast<WaitWidget *>(content_);
  if (!cur_widget) return false;
  return cur_widget->AddPlayer(id, player_name, network_status);
}

bool MainWindow::RemovePlayer(const unsigned short id) {
  WaitWidget *cur_widget = dynamic_cast<WaitWidget *>(content_);
  if (!cur_widget) return false;
  return cur_widget->RemovePlayer(id);
}

bool MainWindow::SetNetworkStatus(const unsigned short id,
                                  const double network_status) {
  WaitWidget *cur_widget = dynamic_cast<WaitWidget *>(content_);
  if (!cur_widget) return false;
  return cur_widget->SetNetworkStatus(id, network_status);
}

void MainWindow::DrawPlayingPage(const GameType type,
                                 const wstring (&player_name)[4],
                                 const unsigned short number_of_cards[4],
                                 const double network_status[4],
                                 const bool controlled_by_bot[4],
                                 const Card cards[]) {
  delete content_;
  content_ = new PlayWidget(this, type, player_name, number_of_cards,
                            network_status, controlled_by_bot, cards);
  setCentralWidget(content_);
}

bool MainWindow::UpdatePlayer(const unsigned short id,
                              const double network_status,
                              const bool controlled_by_bot) {
  PlayWidget *cur_widget = dynamic_cast<PlayWidget *>(content_);
  if (!cur_widget) return false;
  return cur_widget->UpdatePlayer(id, network_status, controlled_by_bot);
}

bool MainWindow::UpdateCards(const unsigned short id, const short delta,
                             const Card cards[], const bool show) {
  PlayWidget *cur_widget = dynamic_cast<PlayWidget *>(content_);
  if (!cur_widget) return false;
  return cur_widget->UpdateCards(id, delta, cards, show);
}

void MainWindow::UpdateStatistics(const unsigned short points[4]) {
  PlayWidget *cur_widget = dynamic_cast<PlayWidget *>(content_);
  if (!cur_widget) return;
  cur_widget->UpdateStatistics(points);
}

bool MainWindow::ClearDesk(const unsigned short id) {
  PlayWidget *cur_widget = dynamic_cast<PlayWidget *>(content_);
  if (!cur_widget) return false;
  return cur_widget->ClearDesk(id);
}

void MainWindow::EndGame(const bool win_or_lose) {
  PlayWidget *cur_widget = dynamic_cast<PlayWidget *>(content_);
  if (!cur_widget) return;
  return cur_widget->EndGame(win_or_lose);
}

ManualWindow::ManualWindow() {
  setWindowTitle("Manual");
  setMinimumSize(480, 320);
  text_ = new QTextEdit(this);
  text_->setReadOnly(true);
  text_->setMarkdown(
      R"(# 游戏操作

* 在同一局域网中，一人新建游戏后其他人加入即可
* 同一局域网不要在同一时间进行多个游戏的组局操作
* 若有人掉线，则 AI 模块会自动接管

# 游戏规则

## 争上游

* 2/3个人 每人22/18张（2个人的时候，留10张作为底牌，故每人22张）
* 牌型
  * 单张、对子、三张、炸弹（4张相同的）、王炸、三带二、顺子（>=5张）、连对（>=3个）、三顺（>=2个）、四带二（四个一样的带两张任意的牌）、飞机（三顺+相同个数的对子，对子不要求一定相邻但不能重复，如3334445566, 3334445556699KK），**三带一不支持**
  * 单张牌点数比较为大王>小王>2>A>K>Q>J>10>...>3
  * 所有的顺子、双顺、三顺、连对等牌型都必须按照上述规则，如334455, 34567, QQQKKKAAA等，必须从>=3的数开始，到<=A的数结束，2不可以参与所有的这类牌型，如以下牌型是不合法的：23456, AA2233
* 大小比较：10,J,Q,K,A > 9,10,J,Q,K > 8,9,10,J,Q > ... > 3,4,5,6,7 其他类推
  * 三带二、四带二、飞机这种“大带小”的牌型，按“大”的点数大小比，如99944>88855，如果一样则后者不能大过前者，如99944（先出）不可被99955（后出）压过
  * 王炸>四张炸>普通牌           四带二不算炸弹，算普通牌型
  * 当前玩家是本轮第一个出牌的玩家，可以任意出，否则，所出的牌必须大过上一个出牌的人才可以出牌

## 红心大战

* 4个人 每人13张 去掉大小王 
* 第一次向下家传，第二次向上家传，第三次向对家传，最后一次不传
* 传完之后，持有草花2的人先出牌，且必须出草花2
* 黑桃Q是13分，所有红桃牌都是1分，分数最小的胜
* 点数排名A>K>Q>...>3>2
* 当前玩家是本轮第一个出牌的玩家，可以任意出（除了第一轮必须出草花2），否则，若手中有和第一个出牌的人所出的牌同样的花色的牌，则必须出一张同花色的牌，否则可以任意出
)");
}

void ManualWindow::resizeEvent(QResizeEvent *) {
  text_->setGeometry(0, 0, geometry().width(), geometry().height());
}
