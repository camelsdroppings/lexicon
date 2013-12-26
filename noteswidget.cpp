#include "noteswidget.h"
EditableSqlModel::EditableSqlModel(QObject *parent)
    : QSqlQueryModel(parent)
{
  m_baseQuery  = "select id,nodeid,word,note,tag from notes";
  createConnection();

}

Qt::ItemFlags EditableSqlModel::flags(
        const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    if (index.column() == 1 || index.column() == 2)
        flags |= Qt::ItemIsEditable;
    return flags;
}

bool EditableSqlModel::setData(const QModelIndex &index, const QVariant &value, int /* role */)
{
  if (index.column() == 3 || index.column() == 4) {
  }
  else {
    return false;
  }

  QModelIndex primaryKeyIndex = QSqlQueryModel::index(index.row(), 0);
  int id = data(primaryKeyIndex).toInt();

  clear();
  qDebug() << "setData" << index.row() << index.column() << value;
  bool ok;
  if (index.column() == 3) {
    ok = setNote(id, value.toString());
  }
  else if (index.column() == 4 ){
    ok = setTag(id, value.toString());
  }
  refresh();
  return ok;
}

void EditableSqlModel::refresh()
{
  QString query;
  if (m_searchItem.isEmpty()) {
      query = m_baseQuery +  " order by nodeId";
  }
  else if (m_searchItem.startsWith("n")) {
    query = QString("%1 where nodeId = '%2'").arg(m_baseQuery).arg(m_searchItem);
  }
  else {
    query = QString("%1 where word = '%2'").arg(m_baseQuery).arg(m_searchItem);
  }
  setQuery(query,m_db);
  if (lastError().isValid()) {
    qDebug() << "Last error" << lastError().text();
  }
  setHeaderData(Notes_Id,Qt::Horizontal,tr("Id"));
  setHeaderData(Notes_NodeId,Qt::Horizontal,tr("Node"));
  setHeaderData(Notes_Word,Qt::Horizontal,tr("Word"));
  setHeaderData(Notes_Note,Qt::Horizontal,tr("Note"));
  setHeaderData(Notes_Tag,Qt::Horizontal,tr("Tag"));
}

bool EditableSqlModel::setNote(int id, const QString & note)
{
  QSqlQuery query(m_db);
  query.prepare("update notes set note = ? where id = ?");
    query.addBindValue(note);
    query.addBindValue(id);
    return query.exec();
}

bool EditableSqlModel::setTag(int id, const QString & tag)
{
  QSqlQuery query(m_db);
    query.prepare("update notes set tag = ? where id = ?");
    query.addBindValue(tag);
    query.addBindValue(id);
    return query.exec();
}
bool EditableSqlModel::createConnection() {
  bool dbInitRequired = false;
  QString dbname = "notes.sqlite";

  QFile dbfile(dbname);
  if (! dbfile.exists()) {
    dbInitRequired = true;
  }
  m_db = QSqlDatabase::addDatabase("QSQLITE","notesdb");
  m_db.setDatabaseName(dbname);
  if (! m_db.open()) {
    QMessageBox::critical(0,QObject::tr("Database Error"),
                          m_db.lastError().text());
    return false;
  }
  return true;
}

/**
 * A vertically split widget
 * left search functions
 *
 * @param parent
 */

NotesWidget::NotesWidget(QWidget * parent) : QWidget(parent) {
  // if (! createConnection()) {
  //   return;
  // }
  QVBoxLayout * layout = new QVBoxLayout;
  QSplitter * splitter = new QSplitter;
  splitter->addWidget(createQueryWidget());
  splitter->addWidget(createEditWidget());
  layout->addWidget(splitter);
  setLayout(layout);

}
QWidget * NotesWidget::createQueryWidget() {
  m_baseQuery  = "select id,nodeid,word,note,tag from notes";
  //  m_model = new QSqlQueryModel;
  m_model = new EditableSqlModel;
  // m_model->setQuery(m_baseQuery + " order by nodeId",m_db);
  // m_model->setHeaderData(Notes_Id,Qt::Horizontal,tr("Id"));
  // m_model->setHeaderData(Notes_NodeId,Qt::Horizontal,tr("Node"));
  // m_model->setHeaderData(Notes_Word,Qt::Horizontal,tr("Word"));
  // m_model->setHeaderData(Notes_Note,Qt::Horizontal,tr("Note"));
  // m_model->setHeaderData(Notes_Tag,Qt::Horizontal,tr("Tag"));
  //  m_model->select();

  m_view = new QTableView;
  m_view->setModel(m_model);
  m_view->setSelectionMode(QAbstractItemView::SingleSelection);
  m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_view->setColumnHidden(Notes_Id,true);
  m_view->setColumnHidden(Notes_Tag,true);
  m_view->resizeColumnsToContents();
  m_view->horizontalHeader()->setStretchLastSection(true);
  m_view->show();
  QWidget * queryWidget = new QWidget;

  QVBoxLayout * layout = new QVBoxLayout;
  layout->addWidget(new QLabel(tr("Show notes for")));

  QHBoxLayout * hlayout1 = new QHBoxLayout;
  hlayout1->addWidget(new QLabel(tr("Word/Node")));
  m_show = new QLineEdit;
  hlayout1->addWidget(m_show);
  layout->addLayout(hlayout1);

  m_showBtn = new QPushButton(tr("Search"));

  connect(m_showBtn,SIGNAL(clicked()),this,SLOT(onShowClicked()));
  layout->addWidget(m_showBtn);
  layout->addWidget(m_view);
  queryWidget->setLayout(layout);

  //  connect(m_view->selectionModel(),SIGNAL(currentRowChanged(const QModelIndex &,const QModelIndex &)),
  //          this,SLOT(showSelectedNote()));
  connect(m_view->selectionModel(),SIGNAL(selectionChanged(const QItemSelection &,const QItemSelection &)),
            this,SLOT(showSelectedNote()));
  //  void 	selectionChanged(const QItemSelection & selected, const QItemSelection & deselected)

  m_model->refresh();
  return queryWidget;
}
QWidget * NotesWidget::createEditWidget() {
  QWidget * editWidget = new QWidget;

  QVBoxLayout * layout = new QVBoxLayout;
  m_note = new QTextEdit;
  m_tag = new QLineEdit;
  layout->addWidget(m_note);
  layout->addWidget(m_tag);
  editWidget->setLayout(layout);
  return editWidget;


}
NotesWidget::~NotesWidget() {
  if (m_db.isOpen()) {
    m_db.close();
  }
}
void NotesWidget::onShowClicked() {
  m_model->setSearch(m_show->text());
  m_model->refresh();
  // QString t = m_show->text();
  // if (t.isEmpty()) {
  //    m_model->setQuery(m_baseQuery +  " order by nodeId",m_db);
  // }
  // else if (t.startsWith("n")) {
  //   m_model->setQuery(QString("%1 where nodeId = '%2'").arg(m_baseQuery).arg(t),m_db);
  // }
  // else {
  //   m_model->setQuery(QString("%1 where word = '%2'").arg(m_baseQuery).arg(t),m_db);
  // }
  // if (m_model->lastError().isValid()) {
  //   qDebug() << "Last error" << m_model->lastError().text();
  // }
}
bool NotesWidget::createConnection() {
  bool dbInitRequired = false;
  QString dbname = "notes.sqlite";

  QFile dbfile(dbname);
  if (! dbfile.exists()) {
    dbInitRequired = true;
  }
  m_db = QSqlDatabase::addDatabase("QSQLITE","notesdb");
  m_db.setDatabaseName(dbname);
  if (! m_db.open()) {
    QMessageBox::critical(0,QObject::tr("Database Error"),
                          m_db.lastError().text());
    return false;
  }
  return true;
}
void NotesWidget::showSelectedNote() {
  if (m_view->selectionModel()->hasSelection()) {
    int row = m_view->selectionModel()->currentIndex().row();
    m_note->setText(m_model->data(m_model->index(row, Notes_Note)).toString());
    m_tag->setText(m_model->data(m_model->index(row, Notes_Tag)).toString());
  }
}
