#include "laneslexicon.h"
LanesLexicon::LanesLexicon(QWidget *parent) :
    QMainWindow(parent)

{

  QSplitter * w = new QSplitter;
  m_tree = new ContentsWidget(this);
  m_tabs = new QTabWidget(this);
  GraphicsEntry * entry = new GraphicsEntry(this);
  m_notes = new NotesWidget(this);
  w->addWidget(m_tree);
  w->addWidget(m_tabs);
  m_tabs->addTab(entry,tr(""));

  m_tabs->addTab(m_notes,"Notes");
  setCentralWidget(w);
  createActions();
  createToolBar();
  createMenus();
  createStatusBar();

  if (openDatabase("lexicon.sqlite")) {
    statusBar()->showMessage(tr("Ready"));
    m_tree->loadContents();
    entry->prepareQueries();
  }
  else {
    statusBar()->showMessage(tr("Failed to open database"));
  }
  connect(m_tree,SIGNAL(itemClicked(QTreeWidgetItem *,int)),this,SLOT(rootClicked(QTreeWidgetItem *,int)));
  connect(entry,SIGNAL(focusItemChanged(QGraphicsItem *, QGraphicsItem *, Qt::FocusReason)),
          this,SLOT(focusItemChanged(QGraphicsItem *, QGraphicsItem *, Qt::FocusReason)));

  connect(this,SIGNAL(nodeActivated(const QString & ,const QString & )),
          m_notes,SLOT(setActiveNode(const QString & ,const QString & )));

  connect(m_notesBtn,SIGNAL(clicked()),this,SLOT(onNotesClicked()));
 }

LanesLexicon::~LanesLexicon()
{
  qDebug() << "main window destructor";
}
void LanesLexicon::createActions() {
  m_exitAction = new QAction(tr("Exit"),this);
  connect(m_exitAction,SIGNAL(triggered()),this,SLOT(on_actionExit()));
}
void LanesLexicon::createToolBar() {
  m_fileToolBar = addToolBar(tr("&File"));
  m_fileToolBar->addAction(m_exitAction);
}
void LanesLexicon::createMenus() {
  m_fileMenu = menuBar()->addMenu(tr("&File"));
  m_fileMenu->addAction(m_exitAction);
}
void LanesLexicon::createStatusBar() {
  m_notesBtn = new QPushButton(tr("Notes"));
  m_notesBtn->setEnabled(false);
  statusBar()->insertPermanentWidget(0,m_notesBtn,0);
  statusBar()->showMessage(tr("Ready"));
}
QSize LanesLexicon::sizeHint() const {
  return QSize(800,600);
}
void LanesLexicon::on_actionExit()
{
  if (m_db.isOpen()) {
    m_db.close();
  }
  close();
}
void LanesLexicon::onNotesClicked() {
  m_tabs->setCurrentWidget(m_notes);
}
bool LanesLexicon::openDatabase(const QString & dbname) {
  QFile dbfile(dbname);
  if (! dbfile.exists()) {
    return false;
  }
  if (m_db.isOpen()) {
    m_db.close();
  }
  bool ok;
  m_db = QSqlDatabase::addDatabase("QSQLITE");
  m_db.setDatabaseName(dbname);
  ok = m_db.open();
  return ok;
}
void LanesLexicon::rootClicked(QTreeWidgetItem * item,int /* column */) {
  QString root = item->text(0);
  if (QApplication::keyboardModifiers() & Qt::ShiftModifier) {
    GraphicsEntry * w = new GraphicsEntry(this);
    w->prepareQueries();
    m_tabs->insertTab(m_tabs->currentIndex()+1,w,root);
    w->getXmlForRoot(root);
    connect(w,SIGNAL(focusItemChanged(QGraphicsItem *, QGraphicsItem *, Qt::FocusReason)),
          this,SLOT(focusItemChanged(QGraphicsItem *, QGraphicsItem *, Qt::FocusReason)));

  }
  else {
    GraphicsEntry * w = dynamic_cast<GraphicsEntry *>(m_tabs->widget(0));
    w->getXmlForRoot(root);
    m_tabs->setTabText(0,root);
  }
  qDebug() << "Get root" << QApplication::keyboardModifiers() << root;
}
void LanesLexicon::focusItemChanged(QGraphicsItem * newFocus, QGraphicsItem * oldFocus, Qt::FocusReason reason) {
  EntryItem * item = dynamic_cast<EntryItem *>(newFocus);
  if (item) {
    statusBar()->showMessage(item->getNode());
    m_notesBtn->setEnabled(true);
    emit(nodeActivated(item->getNode(),item->getWord()));
  }

}
