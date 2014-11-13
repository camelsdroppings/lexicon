#include "contentswidget.h"
#include "QsLog.h"
#include "place.h"
#include "application.h"
#include "definedsettings.h"
#define ROOT_COLUMN 0
#define WORD_COLUMN 1
#define HEAD_SUPPLEMENT_COLUMN 2
#define NODE_COLUMN 3
ContentsWidget::ContentsWidget(QWidget * parent) : QTreeWidget(parent) {
  setObjectName("treeroots");
  readSettings();
  setColumnCount(4);
  setHeaderLabels(
                   QStringList() << tr("Letter/Root") << tr("Head") << tr("") << tr("Node"));
  setSelectionMode(QAbstractItemView::SingleSelection);
  header()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
  header()->setSectionResizeMode(HEAD_SUPPLEMENT_COLUMN,QHeaderView::ResizeToContents);
  this->setStyleSheet(QString("selection-background-color : %1").arg(m_backgroundColor));

  connect(this,SIGNAL(itemExpanded(QTreeWidgetItem *)),this,SLOT(nodeExpanded(QTreeWidgetItem *)));
  connect(this,SIGNAL(itemCollapsed(QTreeWidgetItem *)),this,SLOT(nodeCollapsed(QTreeWidgetItem *)));
  this->setExpandsOnDoubleClick(false);

  if (! m_debug)
    this->hideColumn(3);

}
ContentsWidget::~ContentsWidget() {

}
/**
 * The default font is through the stylesheet entry for QTreeWidget#treeRoots
 * but we load it from QSettings anyway. The stylesheet entry will override it
 * if both are set.
 * For non-Arabic columns (itype,node) set via QSettings
 *
 */
void ContentsWidget::readSettings() {
  Lexicon * app = qobject_cast<Lexicon *>(qApp);
  QSettings * settings = app->getSettings();
  settings->beginGroup("Roots");
  m_backgroundColor = settings->value(SID_CONTENTS_BACKGROUND,"lightgray").toString();
  QString fontString = settings->value(SID_CONTENTS_STANDARD_FONT,QString()).toString();
  if ( ! fontString.isEmpty()) {
    m_itypeFont.fromString(fontString);
  }
  fontString = settings->value(SID_CONTENTS_ARABIC_FONT,QString()).toString();
  if ( ! fontString.isEmpty()) {
    QFont f;
    f.fromString(fontString);
    setFont(f);
  }
  m_debug = settings->value(SID_CONTENTS_DEBUG,false).toBool();
  m_moveDown = settings->value(SID_CONTENTS_MOVE_DOWN,"s").toString();
  m_moveUp = settings->value(SID_CONTENTS_MOVE_UP,"w").toString();
  m_expand = settings->value(SID_CONTENTS_EXPAND," ").toString();
  delete settings;
}
void ContentsWidget::loadContents() {
  QSqlQuery letterQuery;


  if (! m_entryQuery.prepare("select word,itype,bword,nodeId,supplement from entry where datasource = 1 and root = ? order by nodenum asc")) {
    QLOG_WARN() << QString(tr("Entry SQL prepare failed:%1")).arg(m_entryQuery.lastError().text());
  }

  QString sql = "select distinct letter from root where datasource = 1 order by letter";
  if ( ! letterQuery.prepare(sql)) {
    QLOG_WARN() << QString(tr("Error preparing letter query SQL:%1")).arg(letterQuery.lastError().text());
    return;
  }
  letterQuery.exec();

  QSqlQuery rootQuery;
  sql = "select word,supplement from root where letter = ? and datasource = 1 order by word,supplement";
  if (! rootQuery.prepare(sql)) {
    QLOG_WARN() << QString(tr("Error preparing root query SQL:%1")).arg(rootQuery.lastError().text());
    return;
  }
  while(letterQuery.next()) {
    QString letter = letterQuery.value(0).toString();
    QTreeWidgetItem * item = new QTreeWidgetItem((QTreeWidget*)0, QStringList(letter));
    rootQuery.bindValue(0,letter);
    rootQuery.exec();
    QString supp;
    QString root;
    while(rootQuery.next()) {
      bool ok = true;
      // we can have root entries that are in the main text
      // and the supplement but we only show one entry
      if (root != rootQuery.value(0).toString()) {
        //      itype = rootQuery.value(2).toString();
        root = rootQuery.value(0).toString();
        if (rootQuery.value(1).toInt() == 1) {
          supp = "*";
        }
        else {
          supp = "";
        }
        /// skipping the letter entries from the supplement
        if ((supp == "*") && (root.size() == 1)) {
          ok = false;
        }
        if (ok) {
          QStringList cols;
          cols << root << supp;
          item->addChild(new QTreeWidgetItem(cols));
          //          rootCount++;
        }
      }
      addTopLevelItem(item);
    }
  }
  resizeColumnToContents(HEAD_SUPPLEMENT_COLUMN);
}
Place ContentsWidget::findNextPlace(const Place & p) {
  QTreeWidgetItem * currentItem = 0;
  int tc = topLevelItemCount();
  int topIndex = -1;
  int childIndex = -1;
  bool found = false;

  QString root = p.getRoot();
  int supp = p.getSupplement();
  QString suppTest;
  Place np;
  if (supp == 1) {
    suppTest = "*";
  }
  /// TODO replace by code using findItems
  QLOG_DEBUG() << Q_FUNC_INFO << root << supp;
  for(int i = 0;(i < tc) && ! found;i++) {
    QTreeWidgetItem * topItem = topLevelItem(i);
    int kidCount = topItem->childCount();
    for(int j=0;(j < kidCount) && ! found ;j++) {
      QTreeWidgetItem * child = topItem->child(j);
      //      QLOG_DEBUG() << child->text(0) << child->text(1);
      if ((child->text(0) == root) && (child->text(1) == suppTest)) {
        currentItem = child;
        topIndex = i;
        if (j == (kidCount - 1)) {
          topIndex++;
          childIndex = 0;
        }
        else {
          childIndex = j + 1;
        }
        found = true;
      }
    }
  }
  if ((topIndex == -1) || (childIndex == -1 )) {
    QLOG_WARN() << "Root not found" << root;
    return np;
  }
  if (topIndex == tc) {
    emit(atEnd());
    return np;
  }
  /// overkill, but would only matter if there were letters without any roots
  for(int i = topIndex;i < tc; i++) {
      QTreeWidgetItem * item = topLevelItem(i);
      int kidCount = item->childCount();
      if (childIndex < kidCount) {
        QTreeWidgetItem * nextItem = item->child(childIndex);
        currentItem->setSelected(false);
        nextItem->setSelected(true);
        if (nextItem->text(1) == "*") {
          supp = 1;
        }
        else {
          supp = 0;
        }
        np.setRoot(nextItem->text(0));
        np.setSupplement(supp);
        return np;
      }
    }
  return np;
}
/**
 * Called by LanesLexicon when it gets a next root signal
 *
 *
 * @param root start root
 *
 * @return the next root in sequence
 */
QString ContentsWidget::findNextRoot(const QString & root) {
  QList<QTreeWidgetItem *> items = this->findItems(root,Qt::MatchRecursive,ROOT_COLUMN);
  if (items.size() > 0) {
    QTreeWidgetItem * nextItem = this->itemBelow(items[0]);
    if (nextItem) {
      items[0]->setSelected(false);
      nextItem->setSelected(true);
      return nextItem->text(ROOT_COLUMN);
    }
    else {
      emit(atEnd());
    }
  }
  return QString();
}
/**
 * Called by LanesLexicon when it gets a next root signal
 *
 *
 * @param root start root
 *
 * @return the next root in sequence
 */
QString ContentsWidget::findPrevRoot(const QString & root) {
  QList<QTreeWidgetItem *> items = this->findItems(root,Qt::MatchRecursive,ROOT_COLUMN);
  if (items.size() > 0) {
    QTreeWidgetItem * nextItem = this->itemAbove(items[0]);
    if (nextItem) {
      items[0]->setSelected(false);
      nextItem->setSelected(true);
      return nextItem->text(ROOT_COLUMN);
    }
    else {
      emit(atStart());
    }
  }
  return QString();
}
/**
 * Called by LanesLexicon when it gets a next root signal
 *
 *
 * @param root start root
 *
 * @return the next root in sequence
 */
Place ContentsWidget::findPrevPlace(const Place & p) {
  QTreeWidgetItem * currentItem = 0;
  int tc = topLevelItemCount();
  int topIndex = -1;
  int childIndex = -1;
  bool found = false;
  QString root = p.getRoot();
  int supp = p.getSupplement();
  QString suppTest;
  Place np;
  if (supp == 1) {
    suppTest = "*";
  }
  /// TODO replace by code using findItems

  for(int i = 0;(i < tc) && ! found;i++) {
    QTreeWidgetItem * topItem = topLevelItem(i);
    int kidCount = topItem->childCount();
    for(int j=0;(j < kidCount) && ! found ;j++) {
      QTreeWidgetItem * child = topItem->child(j);
      if ((child->text(0) == root) && (child->text(1) == suppTest)) {
        currentItem = child;
        /// if first child, we want the last root of the prev letter
        topIndex = i;
        if (j == 0) {
          topIndex--;
          childIndex = -1;
        }
        else {
          childIndex = j - 1;
        }
        found = true;
      }
    }
  }
  if (topIndex == -1) {
    emit(atStart());
    return np;
  }
  /// overkill, but would only matter if there were letters without any roots
  for(int i = topIndex;i >= 0; i--) {
      QTreeWidgetItem * item = topLevelItem(i);
      int kidCount = item->childCount();
      if (kidCount > childIndex) {
        if (childIndex == -1) {
          childIndex = kidCount - 1;
        }
        QTreeWidgetItem * nextItem = item->child(childIndex);
        currentItem->setSelected(false);
        nextItem->setSelected(true);
        if (nextItem->text(1) == "*") {
          supp = 1;
        }
        else {
          supp = 0;
        }
        np.setRoot(nextItem->text(0));
        np.setSupplement(supp);
        return np;
      }
    }
  return np;
}
void ContentsWidget::keyPressEvent(QKeyEvent * event) {
  if (event->text() == m_moveUp) {
    QTreeWidgetItem * item = this->itemAbove(this->currentItem());
    if (item) {
      this->setCurrentItem(item);
      return;
    }
  }
  if (event->text() == m_moveDown) {
    QTreeWidgetItem * item = this->itemBelow(this->currentItem());
    if (item) {
      this->setCurrentItem(item);
      return;
    }
  }
  if (event->text() == m_expand) {
    this->toggleExpand();
    return;
  }
  switch (event->key()) {
  case Qt::Key_Return : {
    QTreeWidgetItem * item = this->currentItem();
    /// this catches
    if ((item->parent() != 0) && (item->parent()->parent() != 0)) {
      emit(itemActivated(item,0));
    }
    break;
  }
  case Qt::Key_Space: {
    toggleExpand();
    break;
  }
  default:
    QTreeWidget::keyPressEvent(event);
  }
}
void ContentsWidget::toggleExpand() {
    QTreeWidgetItem * item = this->currentItem();
    if (item) {
      /// if it is an entry item
      if ((item->parent() != 0)  && (item->parent()->parent() != 0)) {
        /// TODO make this configurable
        emit(itemActivated(item,0));
      }
      else {
      /// it is a top level item so expand it
        if (item->childCount() > 0) {
          if (item->isExpanded()) {
            collapseItem(item);
          }
          else {
            expandItem(item);
          }
        }
        else {
          QString root = item->text(ROOT_COLUMN);
          this->addEntries(root,item);
          expandItem(item);
        }
      }
    }
}
void ContentsWidget::ensurePlaceVisible(const Place & p, bool select) {
  QLOG_DEBUG() << Q_FUNC_INFO << p << select;
  QString root = p.getRoot();
  int supplement = p.getSupplement();
  QTreeWidgetItem * item;
  QString suppTest;
  if (supplement == 1) {
    suppTest = "*";
  }
  Place rootPlace;
  rootPlace.setRoot(p.getRoot());
  item = this->findPlace(rootPlace);
  if (! item ) {
    return;
  }
  /**
   * this is called when restoring tabs so we need to load
   * the head words for each root we are restoring
   */
  int c;
  if (item) {
    c = addEntries(p.getRoot(),item);
  }
  else {
    QLOG_WARN() << "Ensure visible error: could not find item" << p.getRoot() << p.getWord();
    return;
  }
  if (c == 0) {
    QLOG_WARN() << "Add entries did not add any entries for" << p;
    return;
  }
  item->setExpanded(true);
  QModelIndex index = indexFromItem(item);
  if (index.isValid()) {
    scrollTo(index,QAbstractItemView::PositionAtCenter);
  }
  if (select)
    setCurrentItem(item);
  item = this->findPlace(p);
  if (item) {
     index = indexFromItem(item);
     if (index.isValid()) {
       scrollTo(index,QAbstractItemView::PositionAtCenter);
       setCurrentItem(item);
     }
  }
}
QTreeWidgetItem * ContentsWidget::findPlace(const Place & p) const {
  QModelIndex ix;
  QTreeWidgetItem * item = 0;
  QString target;
  int column;
  if (p.isRoot()) {
    column = ROOT_COLUMN;
    target = p.getRoot();
  }
  else {
    column = WORD_COLUMN;
    target = p.getWord();
  }
  //  QLOG_DEBUG() << Q_FUNC_INFO << target;
  QList<QTreeWidgetItem *> items = this->findItems(target,Qt::MatchRecursive,column);
  /// TODO multiple items, for supplement ?
  if (items.size() > 0) {
    item = items[0];
  }
  return item;
}
void ContentsWidget::focusInEvent(QFocusEvent * event) {
  /// clearing the style sheets has the effect that the systems default
  /// will be used
  this->setStyleSheet("");
  //  QTreeWidgetItem * item = this->currentItem();
  QTreeWidget::focusInEvent(event);

}
void ContentsWidget::focusOutEvent(QFocusEvent * event) {
  this->setStyleSheet(QString("selection-background-color : %1").arg(m_backgroundColor));
  QTreeWidget::focusOutEvent(event);
}
/**
 * This loads the head words for the given root
 * For head words there are two columns, the itype and word
 *
 * @param root
 * @param parent
 */
int ContentsWidget::addEntries(const QString & root,QTreeWidgetItem * parent) {
  int c = 0;
  QString supplement;
  QString itype;
  QString word;
  QString node;
  QLOG_DEBUG() << Q_FUNC_INFO << root;
  /// if already expanding return
  if (parent->childCount() > 0) {
    return -1;
  }
  m_entryQuery.bindValue(0,root);
  m_entryQuery.exec();
  bool isSupplementRoot = false;
  if (parent->text(1) == "*") {
    isSupplementRoot = true;
  }
  while(m_entryQuery.next()) {
    //QLOG_DEBUG() << m_entryQuery->value("bword").toString() << m_entryQuery->value("nodeId").toString();
    supplement.clear();
    if (! isSupplementRoot && (m_entryQuery.value("supplement").toInt() == 1)) {
      supplement = "*";
    }
    itype = m_entryQuery.value("itype").toString();
    word = m_entryQuery.value("word").toString();
    node = m_entryQuery.value("nodeid").toString();
    QTreeWidgetItem * item = new QTreeWidgetItem(QStringList() << itype << word << supplement << node);
    item->setFont(0,m_itypeFont);
    item->setFont(NODE_COLUMN,m_itypeFont);
    item->setFont(HEAD_SUPPLEMENT_COLUMN,m_itypeFont);
    item->setData(0,Qt::UserRole,m_entryQuery.value("nodeId"));//.toString()
    parent->addChild(item);
    c++;
  }
  if ((c > 0) && ! parent->isExpanded()) {
    parent->setExpanded(true);
  }
  return c;
}
void ContentsWidget::nodeExpanded(QTreeWidgetItem * /*item */) {
  QLOG_DEBUG() << Q_FUNC_INFO;
}
void ContentsWidget::nodeCollapsed(QTreeWidgetItem * /*item */) {
  QLOG_DEBUG() << Q_FUNC_INFO;
}
Place ContentsWidget::getCurrentPlace() {
  Place p;
  QTreeWidgetItem * item = this->currentItem();
  if (!item) {
    return p;
  }
  /// at at letter
  if (item->parent() == 0) {
    return p;
  }
  QString root = item->parent()->text(ROOT_COLUMN);
  QString word;
  QString node;
  QString supplement;
  //  for(int i=0; i < item->columnCount();i++) {
  //      QLOG_DEBUG() << i << item->text(i);
  //  }
  if ((item->parent() != 0) && (item->parent()->parent() != 0)) {
    root = item->parent()->text(0);
    word = item->text(1);
    node = item->text(3);
    supplement = item->text(2);
    p.setNode(node);
  }
  else {
    root = item->text(0);
    supplement = item->text(1);
    p.setRoot(root);
  }
  if (supplement == "*") {
    p.setSupplement(1);
  }
  else {
    p.setSupplement(0);
  }
  return p;
}
