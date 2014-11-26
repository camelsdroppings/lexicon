#ifndef __HISTORYWIDGET_H__
#define __HISTORYWIDGET_H__
#include <QListWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QKeyEvent>
#include <QPushButton>
#include <QMap>
#include <QDialog>
#include <QFont>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRegExp>
#include <QCheckBox>
class Place;
class HistoryMaster;
extern HistoryMaster * getHistory();
class HistoryWidget : public QDialog {
  Q_OBJECT
 public:
  HistoryWidget(HistoryMaster * ,QWidget * parent = 0);
  Place getSelected() const;
  QSize sizeHint() const;
  bool getNewTab();
  bool getSwitchTab();
  public slots:
    void setPlace();
    void jump(QTableWidgetItem *);
 private:
    QCheckBox * m_newTab;
    QCheckBox * m_switchTab;
    void readSettings();
    QTableWidget * m_list;
    QString  m_mark;
    bool eventFilter(QObject *,QEvent *);
    QFont m_arFont;
};
#endif
