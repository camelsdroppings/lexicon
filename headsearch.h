#ifndef __HEADSEARCH_H__
#define __HEADSEARCH_H__
#include <QDebug>
#include <QSplitter>
#include <QTextEdit>
#include <QSplitter>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <QLabel>
#include <QSettings>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextLine>
#include <QTextLayout>
#include <QTextBlock>
#include <QFont>
#include <QWidget>
#include <QRegExp>
class GraphicsEntry;
class FocusTable;
class SearchResultsWidget : public QWidget
{
    Q_OBJECT

 public:
    SearchResultsWidget(const QString & str,int options,QWidget * parent = 0);
    SearchResultsWidget(QWidget * parent = 0);
   GraphicsEntry * getEntry() { return m_text;}
   int count();
   void search(const QString &,int options);
   void showFirst();
   void focusTable();
 public slots:
   void itemChanged(QTableWidgetItem *,QTableWidgetItem *);
   void itemDoubleClicked(QTableWidgetItem *);

 private:
   bool eventFilter(QObject * target,QEvent * event);
   QString buildText(int);
   QRegExp m_currentRx;
   int m_searchOptions;
   QSqlQuery m_query;
   QSqlQuery m_nodeQuery;
   QString m_target;
   QLabel * m_resultsText;
   FocusTable * m_list;
   GraphicsEntry * m_text;
/// for Arabic font from QSettings
   QFont m_resultsFont;
 signals:
   void searchResult(const QString &);
};
#endif