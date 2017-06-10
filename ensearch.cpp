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
bool showData;
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
void test(QString & fileName) {
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
  QByteArray a;
  QByteArray b;
  QMap<QChar,QChar> safe;
  safe.insert(QChar('C'),QChar(0x621));
  safe.insert(QChar('M'),QChar(0x622));
  safe.insert(QChar('O'),QChar(0x623));
  safe.insert(QChar('W'),QChar(0x624));
  safe.insert(QChar('I'),QChar(0x625));
  safe.insert(QChar('Q'),QChar(0x626));
  safe.insert(QChar('A'),QChar(0x627));
  safe.insert(QChar('b'),QChar(0x628));
  safe.insert(QChar('p'),QChar(0x629));
  safe.insert(QChar('t'),QChar(0x62A));
  safe.insert(QChar('v'),QChar(0x62B));
  safe.insert(QChar('j'),QChar(0x62C));
  safe.insert(QChar('H'),QChar(0x62D));
  safe.insert(QChar('x'),QChar(0x62E));
  safe.insert(QChar('d'),QChar(0x62F));
  safe.insert(QChar('V'),QChar(0x630));
  safe.insert(QChar('r'),QChar(0x631));
  safe.insert(QChar('z'),QChar(0x632));
  safe.insert(QChar('s'),QChar(0x633));
  safe.insert(QChar('c'),QChar(0x634));
  safe.insert(QChar('S'),QChar(0x635));
  safe.insert(QChar('D'),QChar(0x636));
  safe.insert(QChar('T'),QChar(0x637));
  safe.insert(QChar('Z'),QChar(0x638));
  safe.insert(QChar('E'),QChar(0x639));
  safe.insert(QChar('g'),QChar(0x63A));
  safe.insert(QChar('f'),QChar(0x641));
  safe.insert(QChar('q'),QChar(0x642));
  safe.insert(QChar('k'),QChar(0x643));
  safe.insert(QChar('l'),QChar(0x644));
  safe.insert(QChar('m'),QChar(0x645));
  safe.insert(QChar('n'),QChar(0x646));
  safe.insert(QChar('h'),QChar(0x647));
  safe.insert(QChar('w'),QChar(0x648));
  safe.insert(QChar('Y'),QChar(0x649));
  safe.insert(QChar('y'),QChar(0x64A));
  safe.insert(QChar('F'),QChar(0x64B));
  safe.insert(QChar('N'),QChar(0x64C));
  safe.insert(QChar('K'),QChar(0x64D));
  safe.insert(QChar('a'),QChar(0x64E));
  safe.insert(QChar('u'),QChar(0x64F));
  safe.insert(QChar('i'),QChar(0x650));
  safe.insert(QChar('~'),QChar(0x651));
  safe.insert(QChar('e'),QChar(0x670));
  safe.insert(QChar('L'),QChar(0x671));
  safe.insert(QChar('_'),QChar(0x640));
  safe.insert(QChar(','),QChar(0x60C));
  safe.insert(QChar('-'),QChar(0x0AD));
  safe.insert(QChar(';'),QChar(0x61b));
  safe.insert(QChar('?'),QChar(0x60f));
  safe.insert(QChar('P'),QChar(0x67E));
  safe.insert(QChar('J'),QChar(0x686));
  safe.insert(QChar('B'),QChar(0x6A4));
  safe.insert(QChar('G'),QChar(0x6AF));

  QString t("fataHa");
  QString ot;
  for(int i=0;i < t.length();i++) {
    if (safe.contains(t.at(i))) {
      ot.append(safe.value(t.at(i)));
    }
    else {
      ot.append(t.at(i));
    }
  }
  qDebug() << ot;

  if (fileName.length() == 0) {
    fileName = "test_node.html";
  }
  QString css;
  QFile f("test.css");
  if (f.open(QIODevice::ReadOnly)) {
    QTextStream in(&f);
    css = in.readAll();
    f.close();
  }
  f.setFileName(fileName);
  if (f.open(QIODevice::ReadOnly)) {
    QTextStream in(&f);
    QString txt = in.readAll();
    qDebug() << css << "\n" << txt;
    QTextDocument doc;
    if (! css.isEmpty()) {
      //      doc.setDefaultStyleSheet(css);
    }
    //    QFont font("Amiri");
    //    doc.setDefaultFont(font);
    doc.setHtml(txt);
  }
}
int main(int argc, char *argv[])
{


  bool verbose;
  QString settingsFile("Resources/themes/default/settings.ini");
  QCommandLineParser parser;
  parser.addHelpOption();
  QCommandLineOption nodeOption(QStringList() <<"n" << "node",QObject::tr("Node or comma delimited list of nodes to be searched"),"node");
  parser.addOption(nodeOption);

  parser.addHelpOption();

  QCommandLineOption patternOption(QStringList() <<"p" << "pattern",QObject::tr("Search pattern"),"pattern");
  parser.addOption(patternOption);

  QCommandLineOption outputOption(QStringList() <<"o" << "output",QObject::tr("Output file"),"output");
  parser.addOption(outputOption);

  QCommandLineOption separatorOption(QStringList() <<"s" << "separator",QObject::tr("Output file field separator"),"separator");
  separatorOption.setDefaultValue("|");
  parser.addOption(separatorOption);

  QCommandLineOption verboseOption(QStringList() <<"v" << "verbose",QObject::tr("Show matching entries"));
  parser.addOption(verboseOption);
  QCommandLineOption headOption(QStringList() <<"r" << "root",QObject::tr("Show root/headword for matching entries"));
  parser.addOption(headOption);

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

  QCommandLineOption dbOption(QStringList() <<"f" << "file",QObject::tr("Database path relative to current directory"));
  parser.addOption(dbOption);

  QCommandLineOption sizeOption(QStringList() <<"z" << "size",QObject::tr("Fragment padding"),"padding");
  parser.addOption(sizeOption);

  QCommandLineOption buckOption(QStringList() <<"b" << "buckwalter-input",QObject::tr("Buckwalter safe input"));
  parser.addOption(buckOption);

  QApplication app(argc,argv);
  parser.process(app);
  QStringList posargs = parser.positionalArguments();

  if (parser.isSet(testOption)) {
    QString fileName;
    if (posargs.size() > 0) {
      fileName = posargs[0];
    }
    test(fileName);
    return 0;
  }
  verbose = parser.isSet(verboseOption);
  QFile dbfile;
  QSqlDatabase db;
  int nodeCount = 0; // number of nodes read
  int wordCount = 0; // total number of pattern found
  QString dbname;
  //  QSettings settings(settingsFile,QSettings::IniFormat);
  //  settings.beginGroup("System");
  //  dbname = settings.value("Database",QString()).toString();
  //  if (dbname.length() == 0) {
  //    qDebug() << "Using hard-code dbname";
  if (! parser.isSet(dbOption)) {
    dbname = "Resources/lexicon.sqlite";
  }
  else {
    dbname = parser.value(dbOption);
  }
  QFileInfo fi(dbname);
  if (! fi.exists()) {
    dbname = "Resources/lexicon.sqlite";
    fi = QFileInfo(dbname);
  }
  if (! fi.exists()) {
    qDebug() << "Database not found:" << dbname;
  }


  QFile f(dbname);
  if (! f.exists()) {
    qDebug() << QString("Cannot find database: %1").arg(dbname);
    return 0;
  }


  dbfile.setFileName(fi.absoluteFilePath());

  if (! dbfile.exists() ) {
    QLOG_WARN() << QString(QObject::tr("Cannot find database : %1")).arg(dbname);
    return false;
  }
  if (db.isOpen()) {
    db.close();
  }

  bool showHead = parser.isSet(headOption);
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
    return 0;
  }
  TextSearch searcher;
  if (parser.isSet(buckOption) && ! parser.isSet(regexOption)) {
    QString t(pattern);
    pattern = searcher.fromSafe(pattern);
    qDebug() << QString("%1 converted to %2").arg(t).arg(pattern);
  }
  searcher.setVerbose(verbose);
  searcher.m_separator = parser.value(separatorOption);
  searcher.m_pattern = pattern;
  if (parser.isSet(sizeOption)) {
    bool ok = true;
    QString v = parser.value(sizeOption);
    int sz = v.toInt(&ok);
    if (ok) {
      searcher.setPadding(sz);
    }
  }
  searcher.setSearch(pattern,
                     parser.isSet(regexOption),
                     parser.isSet(caseOption),
                     parser.isSet(wholeOption),
                     parser.isSet(diacriticsOption));

  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(dbname);
  db.open();
  if (! db.isOpen()) {
    std::cerr  << qPrintable(QString("Could not open database:%1").arg(dbname)) << std::endl;
    return 0;
  }
  QStringList nodes;
  QSqlQuery query;
  SearchRunner * r = new SearchRunner;
  searcher.setNode(parser.value(nodeOption));
  if (verbose) {
    QObject::connect(&searcher,SIGNAL(recordsRead(int)),r,SLOT(recordsRead(int)));
  }
  searcher.search();
  delete r;
  /*
  QString root;
  QString headword;
  QString node;
  int maxFinds = 0;
  int maxReads = 0;
  int readCount = 0;
  int findCount = 0;
  bool finished = false;

  if (rx.pattern().length() > 0) {
    qDebug() << "Regular expression search" << rx.pattern();
  }
  else {
    qDebug() << "Text search" << pattern;
  }
  searcher.m_results.clear();
  QMap<int,QString> ret;
  QTime t;
  t.start();
  if (nodes.size() == 0) {
    if (! query.exec()) {
      qDebug() << "Error search all nodes" << query.executedQuery() << query.lastError();
    }
    while (query.next() && ! finished) {
      readCount++;
      QString xml = query.value("xml").toString();
      QString node = query.value("nodeid").toString();
      if (! node.startsWith("j")) {
        nodeCount++;
        if (maxReads && (nodeCount > maxReads)) {
          return 0;
        }
        ret = searcher.searchEntry(pattern,xml);
        if (ret.size() > 0) {
          SearchResult r;
          r.node = query.value("nodeid").toString();
          r.root = query.value("root").toString().toUtf8();
          r.head = query.value("word").toString().toUtf8();
          r.fragments = ret;
          searcher.m_results << r;
          findCount++;
          wordCount += ret.size();
        }
      }
      if (maxFinds && (findCount > maxFinds)) {
        finished = true;
      }
      if (maxReads && (readCount > maxReads)) {
        finished = true;
      }
    }
  }
  else {
    query.prepare(SQL_FIND_ENTRY_DETAILS);
    qDebug() << "nodes" << nodes;
    for(int i=0;(i < nodes.size()) && ! finished ;i++) {
      //query.prepare(SQL_FIND_ENTRY_DETAILS);
      query.bindValue(0,nodes[i]);
      if (! query.exec()) {
        qDebug() << "node query" << query.lastError();
        finished = true;
      }
      else {
        if (query.next()) {
          readCount++;
          QString xml = query.value("xml").toString();
          QString node = query.value("nodeid").toString();
          if (! node.startsWith("j")) {
            nodeCount++;
            if (maxReads && (nodeCount > maxReads)) {
              return 0;
            }
            ret = searcher.searchEntry(pattern,xml,rx);
            if (ret.size() > 0) {
              SearchResult r;
              r.node = query.value("nodeid").toString();
              r.root = query.value("root").toString().toUtf8();
              r.head = query.value("word").toString().toUtf8();
              r.fragments = ret;
              findCount++;
              wordCount += ret.size();
              searcher.m_results << r;
            }

            if (maxFinds && (findCount > maxFinds)) {
              finished = true;
            }
            if (maxReads && (readCount > maxReads)) {
              finished = true;
            }
          }
        }
      }
    }

  }
  searcher.m_time = t.elapsed();
  */
  dbfile.close();
  db.close();

  bool fileOutput = false;
  QFile of;
  if (parser.isSet(outputOption)) {
    searcher.toFile(parser.value(outputOption));
  }
  else {
    searcher.toFile();
  }

    /*
    of.setFileName(parser.value(outputOption));
    if (! of.open(QIODevice::WriteOnly)) {
      std::cerr << "Cannot open output file for writing: ";
      std::cerr << qPrintable(of.errorString()) << std::endl;
    }
    else {
      fileOutput = true;
    }
  }
  QTextStream out(&of);
  out.setCodec("UTF-8");
  for(int i=0;i < results.size();i++) {
    QMapIterator<int,QString> iter(results[i].fragments);
    while(iter.hasNext()) {
      iter.next();
      QStringList o;
      o << results[i].node << results[i].root << results[i].head  << QString("%1").arg(iter.key()) << iter.value();
      if (fileOutput) {
        out << o.join(parser.value(separatorOption));
        out << "\n";
      }
      else {
        std::cout << qPrintable(o.join(parser.value(separatorOption))) << std::endl;
      }
    }
  }
 QString summary =  QString("Search %1 : found in %2 entr%3, total count %4 (time %5ms)").arg(pattern).arg(findCount).arg(findCount > 1 ? "ies" : "y").arg(wordCount).arg(t.elapsed());
  of.close();
  std::cerr << qPrintable(summary) << std::endl;
    */
  return 0;
}
