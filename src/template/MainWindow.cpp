#include "MainWindow.h"

#include <QDebug>
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
  connect(exit_act, &QAction::triggered, this, &QWidget::close);

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

  setWindowTitle("Title");
  setMinimumSize(480, 320);
  resize(480, 320);
}

void MainWindow::Home() { ::Home(); }

void MainWindow::About() {
  QMessageBox::about(this, "About", "一个面向对象课程中完成的纸牌游戏");
}

void MainWindow::Manual() { QMessageBox::about(this, "Manual", "TODO"); }

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
