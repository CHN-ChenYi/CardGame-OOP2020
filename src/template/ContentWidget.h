#pragma once

#include <QDebug>
#include <QWidget>
#include <string>
using std::wstring;

class MainWindow;

QT_BEGIN_NAMESPACE
class QPushButton;
class QHBoxLayout;
class QLabel;
class QVBoxLayout;
class QGridLayout;
class QLineEdit;
class QComboBox;
QT_END_NAMESPACE

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
  void resizeEvent(QResizeEvent *event);

 private:
  QHBoxLayout *hlayout_;
  QVBoxLayout *vlayout_;
  QPushButton *new_game_button_;
  QPushButton *join_game_button_;
  QPixmap *bg_image_;
  void SetBackgroundImage();
};

class InitOrJoinWidget : public ContentWidget {
  Q_OBJECT

 public:
  explicit InitOrJoinWidget(MainWindow *parent, bool widget_type);
  ~InitOrJoinWidget() {}
  void SetInfo(const wstring &info);

 private slots:
  void Accept();

 private:
  bool widget_type_;  // 0 for Init, 1 for Join
  QGridLayout *layout_;
  QHBoxLayout *hlayout_;
  QLabel *first_label_, *second_label_, *third_label_, *info_label_;
  QLineEdit *first_input_, *second_input_, *third_input_;
  QComboBox *combo_box_;
  QPushButton *button_;
};
