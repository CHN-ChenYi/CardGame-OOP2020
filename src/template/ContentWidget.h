#pragma once

#include "MainWindow.h"

#include <QDebug>
#include <QWidget>
#include <string>
using std::wstring;

class MainWindow;

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QComboBox;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
QT_END_NAMESPACE

class NetworkCircle : public QWidget {
 public:
  explicit NetworkCircle(QWidget *parent, const double network_status);
  void UpdateNetworkStatus(const double network_status);

 private:
  QWidget *parent_;
  double network_status_;
  void paintEvent(QPaintEvent *) override;
};

class ContentWidget : public QWidget {
  Q_OBJECT
 public:
  explicit ContentWidget(QWidget *parent = nullptr);
  virtual void SetInfo(const wstring &info) = 0;
  virtual ~ContentWidget(){};
 signals:
};

class HomeWidget : public ContentWidget {
  Q_OBJECT

 public:
  explicit HomeWidget(MainWindow *parent);
  ~HomeWidget();
  void SetInfo(const wstring &info) {
    qDebug() << "Can't find HomeWidget::SetInfo";
  }

 protected:
  void resizeEvent(QResizeEvent *event) override;

 private:
  QVBoxLayout *vlayout_;
  QPixmap *bg_image_;
  void SetBackgroundImage();
};

class InitOrJoinWidget : public ContentWidget {
  Q_OBJECT

 public:
  explicit InitOrJoinWidget(MainWindow *parent, bool widget_type);
  ~InitOrJoinWidget();
  void SetInfo(const wstring &info);

 private slots:
  void Accept();

 private:
  bool widget_type_;  // 0 for Init, 1 for Join
  QHBoxLayout *hlayout_;
  QLabel *info_label_;
  QLineEdit *first_input_, *second_input_, *third_input_;
  QComboBox *combo_box_;
};

class WaitWidget : public ContentWidget {
  Q_OBJECT

 public:
  explicit WaitWidget(MainWindow *parent, const GameType type,
                      const wstring &ip, bool is_owner);
  ~WaitWidget();
  void SetInfo(const wstring &info);
  bool AddPlayer(const unsigned short id, const wstring &player_name,
                 const double network_status);
  bool RemovePlayer(const unsigned short id);
  bool SetNetworkStatus(const unsigned short id, const double network_status);

 private slots:
  void AddBot();
  void StartGame();

 private:
  int id_[5], id_top_;
  QGridLayout *glayout_;
  QHBoxLayout *hlayout_;
  QLabel *info_label_;
};
