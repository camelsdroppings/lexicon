#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QTime>
#include <QLocale>
#include <QSettings>
#include <QSqlError>
#include <QPalette>
#include <QFont>
#include "QsLog.h"
#include "QsLogDest.h"
#include "definedsettings.h"
#include "definedsql.h"
#include "xsltsupport.h"
#include <iostream>
#include "textsearch.h"
#include "lanesupport.h"
#include "ensearchwidget.h"
#include "textsearchwidget.h"
bool showData;
LaneSupport * support = 0;
LaneSupport * getSupport() {
  return support;
}
QString fixHtml(const QString & t) {
  QString html = t;


  /*
    QRegularExpression rxStart("<!--insert_start_(\\w+)-->");
    QRegularExpressionMatch m = rxStart.match(html);
    if (m.hasMatch()) {
    html.replace(m.captured(0),QString("<%1>").arg(m.captured(1)));
    }
    QRegularExpression rxEnd("<!--insert_end_(\\w+)-->");
    m = rxEnd.match(html);
    if (m.hasMatch()) {
    html.replace(m.captured(0),QString("</%1>").arg(m.captured(1)));
    }
    html = html.remove("\n");
  */
  QRegularExpression rxInsert("<!--insert{([^}]+)}-->",QRegularExpression::MultilineOption);
  QRegularExpressionMatchIterator iter = rxInsert.globalMatch(html);
  while(iter.hasNext()) {
    QRegularExpressionMatch m = iter.next();
    html.replace(m.captured(0),m.captured(1));
  }
  return html;
}
void testSplit(const QString & txt) {
  int rtlCount = 0;
  int ltrCount = 0;
  QString p;
  QString cp;
  for(int i=0;i < txt.length();i++) {
    switch(txt.at(i).direction()) {
    case QChar::DirAL:
    case QChar::DirR:
    case QChar::DirAN:
      rtlCount++;
      p += "r";
      cp = "r";
      break;
    case QChar::DirL:
    case QChar::DirEN:
      ltrCount++;
      p += "l";
      cp = "l";
      break;
    default:
      p += cp;
      break;
    }
  }

  //  qDebug() << QString("%1 : [%2]").arg(p.size()).arg(p);
  QStringList words;
  int pos = 0;
  for(int i=1;i < p.length();i++) {
    if (p.at(i) != p.at(i-1)) {
      words << txt.mid(pos, i - pos);
      pos = i;
    }
  }
  if (pos < p.length()) {
    words << txt.mid(pos);
  }
  qDebug() << QString("%1 : [%2]").arg(txt.size()).arg(txt);
  qDebug() << "words" << words;
}
void test(QString & str) {
  /*
  TextSearch searcher;
  QList<QChar> dc;

  QString text("This is a test فتح for Graeme's crap code");
  QRegularExpression rx = searcher.buildRx("فَتَح",true,true,true);
  qDebug() << rx.pattern() << rx.isValid() << rx.match(text);
  rx = searcher.buildRx("graeme",true,true,true);
  qDebug() << rx.pattern() << rx.isValid() << rx.match(text);
  rx = searcher.buildRx("graeme",true,true,false);
  qDebug() << rx.pattern() << rx.isValid() << rx.match(text);
  rx = searcher.buildRx("rap",true,true,true);
  qDebug() << rx.pattern() << rx.isValid() << rx.match(text);
  //  QString diacritics = QString("[\\x%1]*").arg(points.join("\\x"));
  testSplit("graeme برتن is an idiot شَمس");
  qDebug() << searcher.splitText("graeme برتن is an idiot شَمس");
  testSplit("برتن is an idiot");
  testSplit("graeme");
  testSplit("برتن");
  */


  TextSearch t;
  QString bc = t.buckwalterCharacters();
  QString pattern("\\\\arb{([" + bc + "]+)}");
  //  QString pp = QRegularExpression::escape(pattern);
  //  QRegularExpression rx("\\\\arb");//rb{[" + bc + "]+}");
  QRegularExpression rx(pattern);
  qDebug() << "input" << str << str.length();
  qDebug() << "pattern" << rx.pattern() << rx.pattern().length();
  qDebug() << "match" << rx.match(str).hasMatch();
  int pos = 0;
  QRegularExpressionMatch m = rx.match(str,pos);
  QList<QStringList> matches;
  while(m.hasMatch()) {
    matches << m.capturedTexts();
    pos = m.capturedEnd(0) + 1;
    m = rx.match(str,pos);
  }
  QString ostr = str;
  for(int i=0;i < matches.size();i++) {
    if (matches[i].size() == 2) {
      ostr.replace(matches[i][0],t.fromSafe(matches[i][1]));
    }
  }
  qDebug() << ostr;
  return;

}
/*
convert and \arb{...} sequences to their Buckwalter safe equivalent
 */
QString arbInput(QString & str) {
  TextSearch t;
  QString bc = t.buckwalterCharacters();
  QString pattern("\\\\arb{([" + bc + "]+)}");
  QRegularExpression rx(pattern);
  //  qDebug() << "input" << str << str.length();
  //  qDebug() << "pattern" << rx.pattern() << rx.pattern().length();
  //  qDebug() << "match" << rx.match(str).hasMatch();
  int pos = 0;
  QRegularExpressionMatch m = rx.match(str,pos);
  QList<QStringList> matches;
  while(m.hasMatch()) {
    matches << m.capturedTexts();
    pos = m.capturedEnd(0) + 1;
    m = rx.match(str,pos);
  }
  QString ostr = str;
  for(int i=0;i < matches.size();i++) {
    if (matches[i].size() == 2) {
      ostr.replace(matches[i][0],t.fromSafe(matches[i][1]));
    }
  }
  //  qDebug() << ostr;
  return ostr;

}
int main(int argc, char *argv[])
{
  QString str;
  bool ok;
  bool verbose;
  QApplication app(argc,argv);
  QString settingsFile("Resources/themes/default/settings.ini");
  QCommandLineParser parser;
  parser.addHelpOption();
  //  parser.setApplicationDescription("Full text search of Lane's Lexicon.");
  parser.addPositionalArgument("<search pattern>", QCoreApplication::translate("main", "Search pattern"));
 QCommandLineOption nodeOption(QStringList() <<"n" << "node",QObject::tr("Node or comma delimited list of nodes to be searched"),"node");
  parser.addOption(nodeOption);

  parser.addHelpOption();

  QCommandLineOption patternOption(QStringList() <<"p" << "pattern",QObject::tr("Search pattern, enclose multi-word patterns in quotes"),"pattern");
  parser.addOption(patternOption);

  QCommandLineOption outputOption(QStringList() <<"o" << "output",QObject::tr("Send results to file"),"output file name");
  parser.addOption(outputOption);

  QCommandLineOption separatorOption(QStringList() <<"s" << "separator",QObject::tr("Output field separator,default \"|\""),"separator character");
  separatorOption.setDefaultValue("|");
  parser.addOption(separatorOption);

  QCommandLineOption verboseOption(QStringList() <<"v" << "verbose",QObject::tr("Show search progress"));
  parser.addOption(verboseOption);
  QCommandLineOption resourcesOption(QStringList() <<"r" << "resources",QObject::tr("Location of Resources directory"),"Path to resources directory");
#ifdef __APPLE__
    resourcesOption.setDefaultValue(QCoreApplication::applicationDirPath() + "/../Resources");
#else
    resourcesOption.setDefaultValue(QCoreApplication::applicationDirPath() + QDir::separator() + "Resources");
#endif

  parser.addOption(resourcesOption);

  QCommandLineOption testOption(QStringList() <<"t" << "test",QObject::tr("Invoke test routine"));
  parser.addOption(testOption);

  QCommandLineOption diacriticsOption(QStringList() <<"d" << "ignore-diacritics",QObject::tr("Ignore diacritics (Arabic only)"));
  parser.addOption(diacriticsOption);

  QCommandLineOption caseOption(QStringList() <<"c" << "case-sensitive",QObject::tr("Case sensitive"));
  parser.addOption(caseOption);

  QCommandLineOption wholeOption(QStringList() <<"w" << "whole-word",QObject::tr("Whole word match"));
  parser.addOption(wholeOption);

  QCommandLineOption regexOption(QStringList() <<"x" << "regex",QObject::tr("Regular expression search"));
  parser.addOption(regexOption);

  QCommandLineOption dbOption(QStringList() <<"b" << "db-file",QObject::tr("Database file name relative to current directory"),"dbfile name");
  parser.addOption(dbOption);

  QCommandLineOption sizeOption(QStringList() <<"z" << "size",QObject::tr("Show number of characters to the left and right of search hit"),"number");
  sizeOption.setDefaultValue("30");
  parser.addOption(sizeOption);

  QCommandLineOption buckOption(QStringList() <<"i" << "buckwalter-input",QObject::tr("Buckwalter safe input"));
  parser.addOption(buckOption);

  //
  // have paged table with option of number of rows in table
  //
  QCommandLineOption guiOption(QStringList() <<"g" << "gui",QObject::tr("Show graphical interface"));
  parser.addOption(guiOption);

  QCommandLineOption listOption(QStringList() <<"l" << "list-size",QObject::tr("Table rows for graphical interface"),"rows");
  listOption.setDefaultValue("10");
  parser.addOption(listOption);


  QCommandLineOption fieldOption(QStringList() <<"f" << "fields",QObject::tr("Specify output information: R(oot), H(ead word, N(ode), O(ffset),P(age), T(ext), V(olume). Defaults to all.)"),"RHNOTVP");
  fieldOption.setDefaultValue("RHNOTVP");
  parser.addOption(fieldOption);


  parser.process(app);
  QStringList posargs = parser.positionalArguments();

  TextSearch searcher;

  QString resourcesDir = parser.value(resourcesOption);
  verbose = parser.isSet(verboseOption);
  QFileInfo configFile(resourcesDir,"config.ini");

  if (! configFile.exists()) {
    std::cerr << qPrintable(QString("Cannot find \"config.ini\" in %1").arg(parser.value(resourcesOption))) << std::endl;
    std::cerr << "Use -r option to specify the directory containing \"config.ini\"" << std::endl;
    return 1;
  }
  support = new LaneSupport(resourcesDir);
  if (parser.isSet(testOption)) {
    QString fileName;
    if (posargs.size() > 0) {
      fileName = posargs[0];
    }

    test(fileName);
    return 0;
  }
  //
  QSettings config(configFile.absoluteFilePath(),QSettings::IniFormat);
  config.beginGroup("System");
  QString currentTheme = config.value("Theme","default").toString();
  QString themeDirectory = config.value("Theme directory","themes").toString();
  QFileInfo sd(resourcesDir,themeDirectory);
  sd.setFile(sd.absoluteFilePath(),currentTheme);
  QFileInfo si(sd.absoluteFilePath(),"settings.ini");
  if (si.exists()) {
    searcher.setSettingsPath(si.absoluteFilePath());
    // set the XSLT file name
    QSettings settings(si.absoluteFilePath(),QSettings::IniFormat);
    settings.beginGroup("Resources");
    QString xsltdir = settings.value("XSLT","xslt").toString();
    settings.endGroup();
    settings.beginGroup("XSLT");
    QString xslt = settings.value(SID_XSLT_ENTRY,"entry.xslt").toString();
    settings.endGroup();
    QFileInfo xsltFile(sd.absoluteFilePath(),xsltdir);
    xsltFile.setFile(xsltFile.absoluteFilePath(),xslt);
    if (xsltFile.exists()) {
      searcher.setXsltFileName(xsltFile.absoluteFilePath());
    }
    // set the database name
    settings.beginGroup("System");
    QString db = settings.value(SID_SYSTEM_DATABASE).toString();
    settings.endGroup();
    QFileInfo dbFile(resourcesDir,db);
    if (dbFile.exists()) {
      searcher.setDbFileName(dbFile.absoluteFilePath());
    }
  }

  //  QFile dbfile;
  QSqlDatabase db;
///  QString dbname;

  if (parser.isSet(dbOption)) {
    QString dbname = parser.value(dbOption);
    QFileInfo fi(dbname);
    if (! fi.exists()) {
      std::cerr <<  qPrintable(QString("Cannot find supplied database file: %1").arg(dbname)) << std::endl;
      return 0;
    }
    else {
      searcher.setDbFileName(fi.absoluteFilePath());
    }
  }


  //  dbfile.setFileName(searcher.dbFile());

  if (db.isOpen()) {
    db.close();
  }
  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(searcher.dbFile()); //dbname);
  db.open();
  if (! db.isOpen()) {
    std::cerr  << qPrintable(QString("Could not open database:%1").arg(searcher.dbFile())) << std::endl;
    if (support != 0) {
      delete support;
    }
    return 0;
  }

  QFileInfo fi;
  QString pattern;

  if (! parser.isSet(patternOption)) {
      if (posargs.size() > 0) {
        pattern = posargs[0];
      }
  }
  else {
    pattern = parser.value(patternOption);
  }
  if (pattern.length() == 0) {
    std::cerr << qPrintable(QString("No search pattern supplied")) << std::endl;
    parser.showHelp();
    return 0;
  }

  if (parser.isSet(buckOption) && ! parser.isSet(regexOption)) {
    QString t(pattern);
    pattern = searcher.fromSafe(pattern);
  }

  pattern = arbInput(pattern);
  //  fi.setFile(parser.value(iniOption));
  //  if (fi.exists()) {
  //    searcher.setSettingsPath(fi.absoluteFilePath());
  //  }
  QString fields = parser.value(fieldOption);
  QRegularExpression fx("[^RHOPNTV]+");
  if (fx.match(fields).hasMatch()) {
    std::cerr << qPrintable(QString("Unknown output field requested, ignored (use only RHNOPTV)")) << std::endl;
  }
  searcher.setVerbose(verbose);
  searcher.m_separator = parser.value(separatorOption);
  int padding = 30;
  //  searcher.m_pattern = pattern;
  if (parser.isSet(sizeOption)) {

    QString v = parser.value(sizeOption);
    int sz  = v.toInt(&ok);
    if (ok) {
      padding = sz;
      searcher.setPadding(padding);
    }
  }

  searcher.setFields(parser.value(fieldOption));
  searcher.setSearch(pattern,
                     parser.isSet(regexOption),
                     parser.isSet(caseOption),
                     parser.isSet(wholeOption),
                     parser.isSet(diacriticsOption));

  searcher.setNode(parser.value(nodeOption));
  QStringList nodes;
  QSqlQuery query;
  int rows = 0;

  str = parser.value(listOption);

  rows = str.toInt(&ok,10);
  if (! ok ) {
    rows = 10;
  }

  if (! parser.isSet(guiOption)) {
    SearchRunner * r = new SearchRunner;

    if (verbose) {
      QObject::connect(&searcher,SIGNAL(recordsRead(int)),r,SLOT(recordsRead(int)));
    }
    searcher.search();
    delete r;
    QFile of;
    if (parser.isSet(outputOption)) {
      searcher.toFile(parser.value(outputOption));
    }
    else {
      searcher.toFile();
    }
    searcher.setPages(rows);
    qDebug() << searcher.getPage(1,false);
    //    searcher.dumpPages(false);
  }
  else {
    qDebug() << rows;
    EnsearchWidget * w = new EnsearchWidget(rows);
    w->setPadding(padding);
    w->setFields("RHOPNTV");
    w->setNode(parser.value(nodeOption));
    w->setSearch(pattern,
                 parser.isSet(regexOption),
                 parser.isSet(caseOption),
                 parser.isSet(wholeOption),
                 parser.isSet(diacriticsOption));
    w->search();
    w->show();
    app.exec();
  }
  if (support != 0) {
    delete support;
  }
  db.close();
  return 0;
}
