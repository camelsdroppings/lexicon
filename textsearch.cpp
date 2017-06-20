#include <QTime>
#include <QLocale>
#include <QSettings>
#include <QSqlError>
#include "QsLog.h"
#include "QsLogDest.h"
#include "definedsettings.h"
#include "definedsql.h"
#include "xsltsupport.h"
#include <iostream>
#include "textsearch.h"
#include "place.h"
#include "lanesupport.h"
extern LaneSupport * getSupport();
#ifndef LANE
SearchRunner::SearchRunner() {
}
void SearchRunner::recordsRead(int r) {
  double d = 100 * (r/47919.0) ;
  CERR << qPrintable(QString("..%1%..").arg(d,0,'g',4));
}
#endif
QDataStream &operator<<(QDataStream &s, const SearchResult& obj)
{
  QMapIterator<int,QString> iter(obj.fragments);
  while(iter.hasNext()) {
    iter.next();
    s << obj.root << obj.node << obj.head << iter.key() << iter.value() << "\n";
  }

 return s;
}

QDebug operator<<(QDebug debug, const SearchResult& obj)
{
  QMapIterator<int,QString> iter(obj.fragments);
  while(iter.hasNext()) {
    iter.next();
    debug.nospace() << obj.root << "   "
                    << obj.node << "   "
                    << obj.head << "   "
                    << iter.key() << "    "
                    << iter.value() ;
  }

  return debug.space();
}
QDebug operator<<(QDebug debug, const SearchHit& obj)
{
    debug.nospace() << obj.root << "   "
                    << obj.node << "   "
                    << obj.head << "   "
                    << obj.ix << "    "
                    << obj.fragment << "\n";

  return debug.space();
}

QString TextSearch::fixHtml(const QString & t) {
  QString html = t;
  QRegularExpression rxInsert("<!--insert{([^}]+)}-->",QRegularExpression::MultilineOption);
  QRegularExpressionMatchIterator iter = rxInsert.globalMatch(html);
  while(iter.hasNext()) {
    QRegularExpressionMatch m = iter.next();
    html.replace(m.captured(0),m.captured(1));
  }
  html.remove(QChar(0x2029));
  return html;
}

TextSearch::TextSearch() {
  m_padding = 30;
  m_verbose = false;
  m_caseSensitive = false;
  m_wholeWord = false;
  m_regex = false;
  m_diacritics = true;

  QMap<QChar,QChar> safe;
  m_safe.insert(QChar('C'),QChar(0x621));
  m_safe.insert(QChar('M'),QChar(0x622));
  m_safe.insert(QChar('O'),QChar(0x623));
  m_safe.insert(QChar('W'),QChar(0x624));
  m_safe.insert(QChar('I'),QChar(0x625));
  m_safe.insert(QChar('Q'),QChar(0x626));
  m_safe.insert(QChar('A'),QChar(0x627));
  m_safe.insert(QChar('b'),QChar(0x628));
  m_safe.insert(QChar('p'),QChar(0x629));
  m_safe.insert(QChar('t'),QChar(0x62A));
  m_safe.insert(QChar('v'),QChar(0x62B));
  m_safe.insert(QChar('j'),QChar(0x62C));
  m_safe.insert(QChar('H'),QChar(0x62D));
  m_safe.insert(QChar('x'),QChar(0x62E));
  m_safe.insert(QChar('d'),QChar(0x62F));
  m_safe.insert(QChar('V'),QChar(0x630));
  m_safe.insert(QChar('r'),QChar(0x631));
  m_safe.insert(QChar('z'),QChar(0x632));
  m_safe.insert(QChar('s'),QChar(0x633));
  m_safe.insert(QChar('c'),QChar(0x634));
  m_safe.insert(QChar('S'),QChar(0x635));
  m_safe.insert(QChar('D'),QChar(0x636));
  m_safe.insert(QChar('T'),QChar(0x637));
  m_safe.insert(QChar('Z'),QChar(0x638));
  m_safe.insert(QChar('E'),QChar(0x639));
  m_safe.insert(QChar('g'),QChar(0x63A));
  m_safe.insert(QChar('f'),QChar(0x641));
  m_safe.insert(QChar('q'),QChar(0x642));
  m_safe.insert(QChar('k'),QChar(0x643));
  m_safe.insert(QChar('l'),QChar(0x644));
  m_safe.insert(QChar('m'),QChar(0x645));
  m_safe.insert(QChar('n'),QChar(0x646));
  m_safe.insert(QChar('h'),QChar(0x647));
  m_safe.insert(QChar('w'),QChar(0x648));
  m_safe.insert(QChar('Y'),QChar(0x649));
  m_safe.insert(QChar('y'),QChar(0x64A));
  m_safe.insert(QChar('F'),QChar(0x64B));
  m_safe.insert(QChar('N'),QChar(0x64C));
  m_safe.insert(QChar('K'),QChar(0x64D));
  m_safe.insert(QChar('a'),QChar(0x64E));
  m_safe.insert(QChar('u'),QChar(0x64F));
  m_safe.insert(QChar('i'),QChar(0x650));
  m_safe.insert(QChar('~'),QChar(0x651));
  m_safe.insert(QChar('e'),QChar(0x670));
  m_safe.insert(QChar('L'),QChar(0x671));
  m_safe.insert(QChar('_'),QChar(0x640));
  m_safe.insert(QChar(','),QChar(0x60C));
  m_safe.insert(QChar('-'),QChar(0x0AD));
  m_safe.insert(QChar(';'),QChar(0x61b));
  m_safe.insert(QChar('?'),QChar(0x60f));
  m_safe.insert(QChar('P'),QChar(0x67E));
  m_safe.insert(QChar('J'),QChar(0x686));
  m_safe.insert(QChar('B'),QChar(0x6A4));
  m_safe.insert(QChar('G'),QChar(0x6AF));


}
QString TextSearch::buckwalterCharacters() {
  QString v;
  QList<QChar> k = m_safe.keys();
  for(int i=0;i < k.size();i++) {
    v.append(k[i]);
  }
  return v;
}
void TextSearch::setVerbose(bool v) {
  m_verbose = v;
}
void TextSearch::setFields(const QString & v) {
  m_fields = v;
}
void TextSearch::setXsltFileName(const QString & v) {
  m_xsltFile = v;
}
void TextSearch::setDbFileName(const QString & v) {
  m_dbFile = v;
}
QString TextSearch::dbFile() const {
  return m_dbFile;
}
void TextSearch::setSettingsPath(const QString & p) {
  m_iniFileName = p;
}

QString TextSearch::fromSafe(const QString & v) {
  QString ot;
  for(int i=0;i < v.length();i++) {
    if (m_safe.contains(v.at(i))) {
      ot.append(m_safe.value(v.at(i)));
    }
    else {
      ot.append(v.at(i));
    }
  }
  return ot;
}
void TextSearch::setNode(const QString & node) {
    QRegularExpression nrx("^\\d+$");
    QStringList n = node.split(",");

    for(int i=0;i < n.size();i++) {
      if (nrx.match(n[i]).hasMatch()) {
       m_nodes <<  "n" + n[i];
      }
    }
}
void TextSearch::setPadding(int sz) {
  m_padding = sz;
}
void TextSearch::setCaseSensitive(bool v) {
  m_caseSensitive = v;
}
void TextSearch::setWholeWord(bool v) {
  m_wholeWord = v;
}
QString TextSearch::transform(int type,const QString & xsl,const QString & xml) {
  int ok;
  if (xsl.isEmpty()) {
    std::cerr << qPrintable("No XSLT filename supplied") << std::endl;
    return QString();
  }
  if (type == ENTRY_XSLT && ! QFileInfo::exists(xsl)) {
    std::cerr << qPrintable(QString("Cannot find XSLT file: %1").arg(QDir::current().relativeFilePath(xsl))) << std::endl;
    return QString();
  }

  ok = compileStylesheet(type,xsl);
  if (ok == 0) {
    QString html = xsltTransform(type,xml);
    if (! html.isEmpty()) {

      return fixHtml(html);
    }
    else {
      QLOG_WARN() << "Transform returned no HTML";
      return QString();
    }
  }
  else {
    std::cerr << "XSLT compile errors" << std::endl;
  }
  QStringList errors = getParseErrors();
  if (errors.size() > 0) {
    std::cerr << qPrintable(QString("%1  XSLT compile errors").arg(errors.size())) << std::endl;
    /// TODO fix this
    //  errors.prepend("Errors when processing entry styesheet:");
    //  QMessageBox msgBox;
    //  msgBox.setText(errors.join("\n"));
    //  msgBox.exec();
    clearParseErrors();
  }
  return QString();

}
QList<QPair<QString,QString> > TextSearch::splitText(const QString & txt) {
  QList<QPair<QString,QString>> texts;
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
  QStringList css;
  int pos = 0;
  for(int i=1;i < p.length();i++) {
    if (p.at(i) != p.at(i-1)) {
      words << txt.mid(pos, i - pos);
      if (p.at(i-1) == 'r') {
        css << "ar";
      }
      else {
        css << "en";
      }
      texts << qMakePair(words[words.size() - 1],css[css.size() - 1]);
      pos = i;
    }

  }
  if (pos < p.length()) {
    words << txt.mid(pos);
    if (p.at(pos) == 'r') {
      css << "ar";
    }
    else {
      css << "en";
    }
    texts << qMakePair(words[words.size() - 1],css[css.size() - 1]);
  }
  return texts;
}

QMap<int,QString> TextSearch::searchEntry(QString xml,QString /* head */,QString /* node */) {
  QMap<int,QString> results;
  //  QTextDocument doc(xml);
  //  int cnt = doc.characterCount();
  //  bool regex = (m_rx.pattern().length() > 0);

  xml = QString("<word>%1</word>").arg(xml);

  // TODO get this from somewhere
  // NODE_XSLT cause the html to include the headword(s) i.e. the value
  // of <entryType @key="<head word>"
  //
  QString html =
    transform(NODE_XSLT,m_xsltFile,xml);


  if (html.isEmpty()) {
    return results;
  }
  //qDebug() << node;
  html = fixHtml(html);
  html.replace("\n","");
  QTextDocument doc;
  //QTextDocument xdoc(xml);
  //doc.setHtml(QString(xml.toUtf8()));
  doc.setHtml(QString(html.toUtf8()));


  int fc = 0;
  QTextDocument::FindFlags f;
  if (m_caseSensitive) {
    f = QTextDocument::FindCaseSensitively;
  }
  if (m_wholeWord) {
  f |= QTextDocument::FindWholeWords;
  }
  if (m_verbose) {
    //    qDebug() << doc.toHtml();
  }
  QTextCursor c;
  if (m_regex) {
    c = doc.find(m_rx,0,f);
  }
  else {
    c = doc.find(m_pattern,0,f);
  }
  int start;
  int end;

  QString fragment;
  while (! c.isNull()) {
    fragment.clear();
    fc++;
    start = c.selectionStart() - m_padding;
    end = c.selectionEnd() + m_padding;
    if (start < 0) {
      start = 0;
    }

    if (end > doc.characterCount()) {
      end = doc.characterCount();
    }

    for(int i=start;i < end;i++) {
      fragment += QString(doc.characterAt(i));
    }
    fragment.remove(QChar(0x2029));
    fragment.remove(QChar(0x200e));

    results[c.selectionStart()] = fragment;
    if (m_regex) {
      c = doc.find(m_rx,c.selectionEnd(),f);
    }
    else {
      c = doc.find(m_pattern,c.selectionEnd(),f);
    }
  }
  return results;
}
/*
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
*/
/*
  Return the regex to make the diacritics optional and fill the passed array with the characters
  so the calling function can remove them from the search pattern
*/
QString TextSearch::getDiacritics(QList<QChar> & points) {
  if (m_iniFileName.isEmpty()) {
    CERR << qPrintable("Cannot locate settings.ini, diacritics option ignored") << ENDL;
    return QString();
  }
  QSettings settings(m_iniFileName,QSettings::IniFormat);
  settings.setIniCodec("UTF-8");
  settings.beginGroup("Diacritics");
  QStringList keys = settings.childKeys();
  QString v;
  QString rx("[");
  bool ok;
  for(int i=0;i < keys.size();i++) {
    if (keys[i].startsWith("Char")) {
      v = settings.value(keys[i],QString()).toString();
      QChar c = QChar(v.toInt(&ok,16));
      if (ok) {
        rx.append(c);
      }
      else {
        //        qDebug() << "Cannot convert" << v;
      }
      points << c;
    }
  }
  rx.append("]*");

  return rx;
}
QRegularExpression TextSearch::buildRx(QString target,bool ignorediacritics,bool wholeword,bool casesensitive) {
  QList<QChar> dc;
  QString diacritics = getDiacritics(dc);//("[0x6710x64c0x6500x651]*");
  QString pattern;
  //  qDebug() << Q_FUNC_INFO << diacritics << diacritics.length();
  // remove diacritics from the search pattern
  for(int i=0;i < dc.size();i++) {
    target.remove(dc[i]);
  }
  for(int i=0;i < target.size();i++) {
    QChar sp = target.at(i);
    pattern.append(sp);
    if ( sp.isLetter() ) {
      /// if it's in the Arabic block, append to allow for optional diacritics
      if (ignorediacritics && ((sp.unicode() >= 0x600) && (sp.unicode() <= 0x6ff))) {
        pattern.append(diacritics);
      }
    }
  }
  if (wholeword) {
    pattern = "\\b" + pattern + "\\b";
  }
  QRegularExpression rx(pattern);
  if (! casesensitive) {
    rx.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
  }
  m_rx = rx;
  //  qDebug() << Q_FUNC_INFO << rx.isValid() << rx.pattern();
  return rx;
}
void TextSearch::toFile(const QString & fileName) const {
  bool fileOutput = false;
  QFile of;

  if (fileName.length() > 0) {
  of.setFileName(fileName);
  if (! of.open(QIODevice::WriteOnly)) {
    CERR << "Cannot open output file for writing: ";
    CERR << qPrintable(of.errorString()) << ENDL;
  }
  else {
    fileOutput = true;
  }
  }
  QTextStream out(&of);
  out.setCodec("UTF-8");

  int findCount = 0;
  QStringList fields = m_fields.split("",QString::SkipEmptyParts);

  for(int i=0;i < m_results.size();i++) {
    QMapIterator<int,QString> iter(m_results[i].fragments);
    findCount += m_results[i].fragments.size();
    while(iter.hasNext()) {
      iter.next();
      QStringList o;
      for(int j=0;j < fields.size();j++) {
        if (fields[j] == "N") {
          o << m_results[i].node;
        }
        if (fields[j] == "R") {
          o << m_results[i].root;
        }
        if (fields[j] == "H") {
          o << m_results[i].head;
        }
        if (fields[j] == "V") {
          o << QString("%1").arg(m_results[i].vol);
        }
        if (fields[j] == "P") {
          o << QString("%1").arg(m_results[i].page);
        }
        if (fields[j] == "O") {
          o << QString("%1").arg(iter.key());
        }
        if (fields[j] == "T") {
          o << iter.value();
        }
      }
      if (fileOutput) {
        out << o.join(m_separator);
        out << "\n";
      }
      else {
        COUT << qPrintable(o.join(m_separator)) << ENDL;
      }
    }
  }
  // QString summary =  QString("Search for: %1 : found in %2 entr%3, total count %4 (time %5ms)").arg(m_pattern).arg(entryCount).arg(entryCount > 1 ? "ies" : "y").arg(findCount).arg(m_time);
  of.close();
  COUT << ENDL;
  CERR << qPrintable(this->summary()) << ENDL;
}
void TextSearch::searchAll() {
  QSqlQuery query;
  query.prepare(SQL_ALL_ENTRIES);
  if (! query.exec()) {
    QString err = query.lastError().text();
    CERR << qPrintable(QString("SQL error %1 , %2").arg(SQL_ALL_ENTRIES).arg(err)) << ENDL;
    return;
  }

  int readCount = 0;
  int findCount = 0;
  bool finished = false;
  QMap<int,QString> ret;
  while (query.next() && ! finished) {
    readCount++;
    QString xml = query.value("xml").toString();
    QString node = query.value("nodeid").toString();
    QString head = query.value("word").toString();

    if (! node.startsWith("j")) {
      ret = searchEntry(xml,head,node);
      if (ret.size() > 0) {
        SearchResult r;
        r.node = query.value("nodeid").toString();
        r.root = query.value("root").toString().toUtf8();
        r.head = query.value("headword").toString().toUtf8();
        Place p = Place::fromEntryRecord(query.record());
        r.vol = p.volume();
        r.page = p.page();
        r.fragments = ret;
        m_results << r;
        findCount++;
      }
    }
    /*
    if (maxFinds && (findCount > maxFinds)) {
      finished = true;
    }
    if (maxReads && (readCount > maxReads)) {
      finished = true;
    }
    */
    if ((readCount % 1000) == 0) {
      emit(recordsRead(readCount));
    }
  }
}
void TextSearch::searchNodes() {
  int readCount = 0;
  int findCount = 0;
  bool finished = false;
  QSqlQuery query;
  query.prepare(SQL_FIND_ENTRY_DETAILS);
  QMap<int,QString> ret;
  for(int i=0;(i < m_nodes.size()) && ! finished ;i++) {
    //query.prepare(SQL_FIND_ENTRY_DETAILS);
    query.bindValue(0,m_nodes[i]);
    if (! query.exec()) {
      CERR  << qPrintable("node query error:") << qPrintable(query.lastError().text());
      finished = true;
    }
    else {
      if (query.next()) {
        readCount++;
        QString xml = query.value("xml").toString();
        QString node = query.value("nodeid").toString();
        QString head = query.value("word").toString();
        if (! node.startsWith("j")) {
          ret = searchEntry(xml,head,node);
          if (ret.size() > 0) {
            SearchResult r;
            r.node = query.value("nodeid").toString();
            r.root = query.value("root").toString().toUtf8();
            r.head = query.value("headword").toString().toUtf8();
            r.fragments = ret;
            findCount++;
            m_results << r;
          }
          /*
          if (maxFinds && (findCount > maxFinds)) {
            finished = true;
          }
          if (maxReads && (readCount > maxReads)) {
            finished = true;
          }
          */
        }
      }
    }
  }

}

void TextSearch::search() {
  m_results.clear();
  QTime t;
  t.start();
  if (m_nodes.size() == 0) {
    this->searchAll();
  }
  else {
    this->searchNodes();
  }
 m_time = t.elapsed();
}
QString TextSearch::summary() const {

  int entryCount = m_results.size();
  int findCount = 0;
  for(int i=0;i < m_results.size();i++) {
    findCount += m_results[i].fragments.size();
  }
  QString t = QString("%1 search for \"%2\" - %3 occurrence%4 in %5 entr%6")
    .arg(m_regex ? "Regex" : "Text")
    .arg(m_regex ? m_rx.pattern() : m_pattern)
    .arg(findCount)
    .arg(findCount == 1 ? "" : "s")
    .arg(entryCount).arg(entryCount == 1 ? "y" : "ies");
  QString options;
  if (! m_regex ) {
    options = QString("Case %1sensitive, \"whole word match\" is %2. ").arg(m_caseSensitive == 0 ? "in" : "" ).arg(m_wholeWord == 1 ? "on" : "off");
  }
  t = QString("%1 (%2Search took %3 ms.)").arg(t).arg(options).arg(m_time);
  return t;
}
void TextSearch::setSearch(const QString & p,bool regex,bool caseSensitive,bool wholeWord,bool diacritics) {
  QRegularExpression rx;
  m_caseSensitive = caseSensitive;
  m_wholeWord = wholeWord;
  m_regex = regex;
  m_diacritics = diacritics;
  QString pattern = p;

  if (! regex) {
    m_pattern = pattern;
    QRegularExpression rx("[\u0600-\u06ff]+");
    if (rx.match(pattern).hasMatch()) { // text contains arabic
      if (diacritics) {
        pattern = QRegularExpression::escape(pattern);
        m_rx = buildRx(pattern,diacritics,wholeWord,caseSensitive);
        if (m_verbose) {
          CERR << "Arabic regex search (forced by diacritics):" << ENDL;
        }
        m_regex = true;
      }
      else {
      }
    }
    else {
    }
  }
  else {    // doing a regex search so just set the pattern
    //    m_pattern = pattern;
    m_rx.setPattern(pattern);
  }
  if (m_regex && ! caseSensitive) {
    m_rx.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
  }
  if (! m_rx.isValid()) {
    CERR << qPrintable(m_rx.pattern()) << ENDL;
    CERR << qPrintable(QString("Invalid regular expression: %1").arg(m_rx.errorString())) << ENDL;
  }
  if (m_verbose) {
    CERR << qPrintable(QString("Regex search                   :%1").arg(m_regex)) << ENDL;
    CERR << qPrintable(QString("Case sensitive                 :%1").arg(m_caseSensitive)) << ENDL;
    CERR << qPrintable(QString("Whole word match               :%1").arg(m_wholeWord)) << ENDL;
    CERR << qPrintable(QString("Ignore diacritics              :%1").arg(m_diacritics)) << ENDL;
    CERR << qPrintable(QString("Pattern                        :%1").arg(m_pattern)) << ENDL;
    CERR << qPrintable(QString("Regex pattern                  :%1").arg(m_rx.pattern())) << ENDL;
    CERR << qPrintable(QString("Regex case insensitive  option :%1").arg(m_rx.patternOptions() && QRegularExpression::CaseInsensitiveOption)) << ENDL;


  }
}

QList<SearchHit> TextSearch::getHits(int offset,int pageSize,bool summary) const {
  QList<SearchHit> ret;

  if (summary) {
    for(int i = offset;(i < m_results.size()) && (ret.size() < pageSize);i++) {
      SearchHit h;
      h.root = m_results[i].root;
      h.head = m_results[i].head;
      h.node = m_results[i].node;
      h.page = m_results[i].page;
      h.vol = m_results[i].vol;
      h.ix = m_results[i].fragments.size();
      h.fragment = m_results[i].fragments.first();
      ret << h;
    }
    return ret;
  }
  int ix = 0;
  for(int i = 0;(i < m_results.size()) && (ret.size() < pageSize);i++) {
    QMapIterator<int,QString> iter(m_results[i].fragments);
    while(iter.hasNext()) {
      iter.next();
      if (ix >= offset) {
        SearchHit h;
        h.root = m_results[i].root;
        h.head = m_results[i].head;
        h.node = m_results[i].node;
        h.page = m_results[i].page;
        h.vol = m_results[i].vol;
        h.ix = iter.key();
        h.fragment = iter.value();
        ret << h;
      }
      ix++;
    }
  }
  return ret;
}
int TextSearch::rows(bool summary) const {
  if (summary) {
    return m_results.size();
  }
  int c = 0;
  for(int i = 0;i < m_results.size();i++) {
    c += m_results[i].fragments.size();
  }
  return c;
}
void TextSearch::setOffsets() {
  int c = 0;
  for(int i = 0;i < m_results.size();i++) {
    c += m_results[i].fragments.size();
  }
}
QPair<int,int> TextSearch::setPages(int sz) {
  m_pageSize = sz;
  m_summaryPages.clear();
  int page=1;
  for(int i = 0;i < m_results.size();i += sz) {
    //      m_summaryPages.insert(i,qMakePair0);
    if ((i + sz) > m_results.size()) {
      m_summaryPages.insert(page,qMakePair(i,m_results.size() % sz));
    }
    else {
      m_summaryPages.insert(page,qMakePair(i,sz));
    }
    page++;
  }



  m_fullPages.clear();
  int j=0;
  int currentEntry=0;
  int currentFragment=0;
  QString pp(" ");
  page=0;
  for(int i = 0;i < m_results.size();i++) {
    QMapIterator<int,QString> iter(m_results[i].fragments);
    int k = 0;
    while(iter.hasNext()) {
      iter.next();
      if ((j % m_pageSize) == 0) {
        page++;
        m_fullPages.insert(page,qMakePair(i,k));
        pp = "P";
        currentFragment = k;
        currentEntry = i;
      }
      //      qDebug() << QString("%1 [%2][%3:%4] %5").arg(pp).arg(j).arg(i).arg(k).arg(j % m_pageSize);
      pp = " ";
      k++;
      j++;
    }
  }
  return qMakePair(m_summaryPages.size(),m_fullPages.size());
}

QList<SearchHit> TextSearch::getPage(int page,bool summary) const {
  QList<SearchHit> ret;
  if (summary) {
    if (! m_summaryPages.contains(page)) {
      return ret;
    }
    QPair<int,int> ix = m_summaryPages.value(page);
    int offset = ix.first;
    int pageSize = ix.second;
    int c = pageSize;
    for(int i = offset;c > 0;i++) {
      SearchHit h;
      h.root = m_results[i].root;
      h.head = m_results[i].head;
      h.node = m_results[i].node;
      h.page = m_results[i].page;
      h.vol = m_results[i].vol;
      h.ix = m_results[i].fragments.size();
      h.fragment = m_results[i].fragments.first();
      ret << h;
      c--;
    }
    return ret;
  }
  if (! m_fullPages.contains(page)) {
    return ret;
  }
  QPair<int,int> ix = m_fullPages.value(page);
  int entryIx  = ix.first;
  int fragmentIx = ix.second;

  bool ok = false;
  for(int i = entryIx;(i < m_results.size()) && (ret.size() < m_pageSize);i++) {
    int j = 0;
    QMapIterator<int,QString> iter(m_results[i].fragments);
    while(iter.hasNext() && (ret.size() < m_pageSize)) {
      iter.next();
      if ((i == entryIx) && (j == fragmentIx)) {
        ok = true;
      }
      if (ok) {
        SearchHit h;
        h.root = m_results[i].root;
        h.head = m_results[i].head;
        h.node = m_results[i].node;
        h.page = m_results[i].page;
        h.vol = m_results[i].vol;
        h.ix = iter.key();
        h.fragment = iter.value();
        ret << h;
      }
      j++;
    }
  }
  return ret;
}
void TextSearch::dumpPages(bool summary) {
  if (! summary) {
    qDebug() << "Pages (non-summary)" << m_fullPages.size();
    //    qDebug() << m_fullPages;
    int ix = 0;
    for(int i=0;i < m_fullPages.size();i++) {
      QList<SearchHit> hits = getPage(i+1,summary);
      for(int j=0;j < hits.size();j++) {
        qDebug() << QString("[%1] Page %2-%3").arg(ix,4).arg(i+1).arg(j,2,10,QChar('0')) << hits[j];
        ix++;
      }
      qDebug() << "--------------------------------------------";
    }
  }
}