#include "findoptions.h"
#include "definedsettings.h"
#include "QsLog.h"
#include "popupoptions.h"
#ifndef STANDALONE
#include "application.h"
#include "externs.h"
#endif
/**
 * Not done:
 *
 * @param theme
 * @param parent
 */
FindOptions::FindOptions(const QString & theme,QWidget * parent) : OptionsWidget(theme,parent) {
  setObjectName("findoptions");
  m_section = "FullSearch";
  QVBoxLayout * layout = new QVBoxLayout;
  QGroupBox * fullbox = new QGroupBox(tr("Full text search"));

  m_fullDebug = new QCheckBox;
  m_fullFragmentSize = new QSpinBox;
  this->setControlSize(m_fullFragmentSize,LARGE_EDIT);
  m_fullIncludeHeads = new QCheckBox;
  m_fullOneRow = new QCheckBox;
  m_fullStep = new QSpinBox;

  this->setControlSize(m_fullFragmentSize,MEDIUM_EDIT);
  m_fullStep->setSingleStep(25);
  this->setControlSize(m_fullStep,MEDIUM_EDIT);
  m_contextStyle = new QLineEdit;
  m_contextStyleArabic = new QLineEdit;

  m_fullHeadColor = new QLineEdit;
  this->setControlSize(m_fullHeadColor,VLARGE_EDIT);
  m_fullHeadText = new QLineEdit;


  QFormLayout * fulllayout = new QFormLayout;
  QHBoxLayout * colorlayout = new QHBoxLayout;
  colorlayout->addWidget(m_fullHeadColor);
  QPushButton * colorbutton = new QPushButton(tr("Set colour"));
  connect(colorbutton,SIGNAL(clicked()),this,SLOT(onSetColor()));
  colorlayout->addWidget(colorbutton);
  colorlayout->addStretch();

  fulllayout->addRow(tr("Include head words in\nsearch results"),m_fullIncludeHeads);
  fulllayout->addRow(tr("Fragment size"),m_fullFragmentSize);
  fulllayout->addRow(tr("Main context style"),m_contextStyle);
  fulllayout->addRow(tr("Arabic context style"),m_contextStyleArabic);
  fulllayout->addRow(tr("Head word background colour"),colorlayout);
  fulllayout->addRow(tr("Text for head word results"),m_fullHeadText);
  fulllayout->addRow(tr("One row for each entry"),m_fullOneRow);
  fulllayout->addRow(tr("Progress interval"),m_fullStep);

  QPushButton * fullbtn = new QPushButton(tr("Set"));
  this->setControlSize(fullbtn,MEDIUM_EDIT);
  QHBoxLayout * setlayout1 = new QHBoxLayout;
  setlayout1->addWidget(fullbtn);
  //  setlayout1->addStretch();
  fulllayout->addRow(tr("Search dialog options"),setlayout1);
  fulllayout->addRow(tr("Debug"),m_fullDebug);

  connect(fullbtn,SIGNAL(clicked()),this,SLOT(onFullDialog()));

  fullbox->setLayout(fulllayout);



  /// Head word search
  QGroupBox * headbox = new QGroupBox(tr("Head word search"));
  m_headDebug = new QCheckBox;
  m_headStep = new QSpinBox;
  this->setControlSize(m_headStep,MEDIUM_EDIT);
  m_headVertical = new QCheckBox;
  m_headFocusTable = new QCheckBox;
  this->setControlSize(m_headStep,MEDIUM_EDIT);
  m_headStep->setSingleStep(25);
  m_headSingleClick = new QCheckBox;
  /*
  m_headNewTab = new QCheckBox;
  m_headGoTab = new QCheckBox;
  m_headWholeWord = new QCheckBox;
  m_headDiacritics = new QCheckBox;
  m_headRegex  = new QCheckBox;
  */
  QFormLayout * headlayout = new QFormLayout;
  headlayout->addRow(tr("Vertical layout"),m_headVertical);
  headlayout->addRow(tr("Step interval"),m_headStep);
  headlayout->addRow(tr("Initial focus on results"),m_headFocusTable);
  headlayout->addRow(tr("Single click to activate"),m_headSingleClick);
  QPushButton * headbtn = new QPushButton(tr("Set"));
  QHBoxLayout * setlayout2 = new QHBoxLayout;
  setlayout2->addWidget(headbtn);
  //  setlayout2->addStretch();
  headlayout->addRow(tr("Search dialog options"),setlayout2);
  connect(headbtn,SIGNAL(clicked()),this,SLOT(onHeadDialog()));

  headlayout->addRow(tr("Debug"),m_headDebug);
  headbox->setLayout(headlayout);

  /// Local search
  QGroupBox * localbox = new QGroupBox(tr("Local search"));

  m_localShowAll    = new QCheckBox;

  QFormLayout * locallayout = new QFormLayout;
  locallayout->addRow(tr("Show all results"),m_localShowAll);

  QPushButton * localbtn = new QPushButton(tr("Set"));
  QHBoxLayout * setlayout3 = new QHBoxLayout;
  setlayout3->addWidget(localbtn);
  //  setlayout3->addStretch();
  locallayout->addRow(tr("Search dialog options"),setlayout3);
  connect(localbtn,SIGNAL(clicked()),this,SLOT(onLocalDialog()));

  localbox->setLayout(locallayout);

  ///
  QGroupBox * otherbox = new QGroupBox(tr("Tab options for root,page and node search"));
  QGridLayout * gridlayout = new QGridLayout;

  m_nodeNew = new  QCheckBox;
  m_nodeGo  = new  QCheckBox;
  m_pageNew = new  QCheckBox;
  m_pageGo  = new  QCheckBox;
  m_rootNew = new  QCheckBox;
  m_rootGo  = new  QCheckBox;


  gridlayout->addWidget(new QLabel("Root"),0,1);
  gridlayout->addWidget(new QLabel("Page"),0,2);
  gridlayout->addWidget(new QLabel("Node"),0,3);

  gridlayout->addWidget(new QLabel("New tab"),1,0);
  gridlayout->addWidget(new QLabel("Activate tab"),2,0);

  gridlayout->addWidget(m_rootNew,1,1);
  gridlayout->addWidget(m_pageNew,1,2);
  gridlayout->addWidget(m_nodeNew,1,3);
  gridlayout->addWidget(m_rootGo,2,1);
  gridlayout->addWidget(m_pageGo,2,2);
  gridlayout->addWidget(m_nodeGo,2,3);
  otherbox->setLayout(gridlayout);


  layout->addWidget(fullbox);
  QHBoxLayout * hlayoutsearches = new QHBoxLayout;
  hlayoutsearches->addWidget(headbox);
  hlayoutsearches->addWidget(localbox);

  layout->addLayout(hlayoutsearches);//Widget(headbox);
  //  layout->addWidget(localbox);
  layout->addWidget(otherbox);
  layout->addStretch();
  setLayout(layout);
  addButtons();
  readSettings();
  setupConnections();
  //  getLexicon()->setCursorPosition(this);
}

void FindOptions::readSettings() {
  QSettings settings(m_settingsFileName,QSettings::IniFormat);
  settings.setIniCodec("UTF-8");
  settings.beginGroup("FullSearch");

  m_fullDebug->setChecked(settings.value(SID_FULLSEARCH_DEBUG,false).toBool());
  m_fullFragmentSize->setValue(settings.value(SID_FULLSEARCH_FRAGMENT_SIZE,50).toInt());
  m_fullIncludeHeads->setChecked(settings.value(SID_FULLSEARCH_INCLUDE_HEADS,true).toBool());
  m_fullOneRow->setChecked(settings.value(SID_FULLSEARCH_ONE_ROW,true).toBool());
  m_fullStep->setValue(settings.value(SID_FULLSEARCH_STEP,50).toInt());
  m_fullHeadColor->setText(settings.value(SID_FULLSEARCH_HEAD_BACKGROUND).toString());
  m_contextStyle->setText(settings.value(SID_FULLSEARCH_MAIN_CONTEXT).toString());
  m_contextStyleArabic->setText(settings.value(SID_FULLSEARCH_ARABIC_CONTEXT).toString());

  m_fullNewTab     = settings.value(SID_FULLSEARCH_NEW_TAB,true).toBool();
  m_fullGoTab      = settings.value(SID_FULLSEARCH_GO_TAB,true).toBool();
  m_fullWholeWord  = settings.value(SID_FULLSEARCH_WHOLE_WORD,true).toBool();
  m_fullDiacritics = settings.value(SID_FULLSEARCH_DIACRITICS,true).toBool();
  m_fullRegex      = settings.value(SID_FULLSEARCH_TYPE_REGEX,true).toBool();
  m_fullForce      = settings.value(SID_FULLSEARCH_FORCE,true).toBool();

  settings.endGroup();
  settings.beginGroup("HeadSearch");
  // head word search

  m_headDebug->setChecked(settings.value(SID_HEADSEARCH_DEBUG,false).toBool());
  m_headSingleClick->setChecked(settings.value(SID_HEADSEARCH_SINGLE_CLICK,true).toBool());
  m_headVertical->setChecked(settings.value(SID_HEADSEARCH_VERTICAL_LAYOUT,true).toBool());
  m_headFocusTable->setChecked(settings.value(SID_HEADSEARCH_FOCUS_TABLE,true).toBool());
  m_headStep->setValue(settings.value(SID_HEADSEARCH_STEP,50).toInt());

  m_headNewTab     = settings.value(SID_HEADSEARCH_NEW_TAB,true).toBool();
  m_headGoTab      = settings.value(SID_HEADSEARCH_GO_TAB,true).toBool();
  m_headWholeWord  = settings.value(SID_HEADSEARCH_WHOLE_WORD,true).toBool();
  m_headDiacritics = settings.value(SID_HEADSEARCH_DIACRITICS,true).toBool();
  m_headRegex      = settings.value(SID_HEADSEARCH_TYPE_REGEX,true).toBool();
  m_headForce      = settings.value(SID_HEADSEARCH_FORCE,true).toBool();


  //  m_viewerSize = settings.value(SID_FULLSEARCH_VIEWER_SIZE,QSize(600,400)).toSize();

  /*

  m_fullWholeWord->setChecked(settings.value(SID_FULLSEARCH_WHOLE_WORD,true).toBool());
 m_fullXslt;
 m_fullHeadColor;
 m_fullHeadText;

  */
  settings.endGroup();
  settings.beginGroup("LocalSearch");
  m_localWholeWord  = settings.value(SID_LOCALSEARCH_WHOLE_WORD,true).toBool();
  m_localDiacritics = settings.value(SID_LOCALSEARCH_DIACRITICS,true).toBool();
  m_localRegex      = settings.value(SID_LOCALSEARCH_TYPE_REGEX,true).toBool();
  m_localForce      = settings.value(SID_LOCALSEARCH_FORCE,true).toBool();


  m_localShowAll->setChecked(settings.value(SID_LOCALSEARCH_SHOW_ALL,true).toBool());

  settings.endGroup();
  settings.beginGroup("Search");

  m_nodeNew->setChecked(settings.value(SID_NODESEARCH_NEW_TAB,true).toBool());
  m_nodeGo->setChecked(settings.value(SID_NODESEARCH_GO_TAB,true).toBool());
  m_pageNew->setChecked(settings.value(SID_PAGESEARCH_NEW_TAB,true).toBool());
  m_pageGo->setChecked(settings.value(SID_PAGESEARCH_GO_TAB,true).toBool());
  m_rootNew->setChecked(settings.value(SID_ROOTSEARCH_NEW_TAB,true).toBool());
  m_rootGo->setChecked(settings.value(SID_ROOTSEARCH_GO_TAB,true).toBool());

  m_dirty = false;
}
void FindOptions::writeSettings(const QString & fileName) {
  QString f = m_settingsFileName;
  if (!fileName.isEmpty()) {
    f = fileName;
  }

  QSettings settings(f,QSettings::IniFormat);
  settings.setIniCodec("UTF-8");
  settings.beginGroup("FullSearch");

  settings.setValue(SID_FULLSEARCH_DEBUG,m_fullDebug->isChecked());
  settings.setValue(SID_FULLSEARCH_FRAGMENT_SIZE,m_fullFragmentSize->value());
  settings.setValue(SID_FULLSEARCH_INCLUDE_HEADS,m_fullIncludeHeads->isChecked());
  settings.setValue(SID_FULLSEARCH_ONE_ROW,m_fullOneRow->isChecked());
  settings.setValue(SID_FULLSEARCH_STEP,m_fullStep->value());
  settings.setValue(SID_FULLSEARCH_HEAD_BACKGROUND,m_fullHeadColor->text());
  settings.setValue(SID_FULLSEARCH_MAIN_CONTEXT,m_contextStyle->text());
  settings.setValue(SID_FULLSEARCH_ARABIC_CONTEXT,m_contextStyleArabic->text());


  settings.setValue(SID_FULLSEARCH_NEW_TAB,m_fullNewTab);
  settings.setValue(SID_FULLSEARCH_GO_TAB,m_fullGoTab);
  settings.setValue(SID_FULLSEARCH_WHOLE_WORD,m_fullWholeWord);
  settings.setValue(SID_FULLSEARCH_DIACRITICS,m_fullDiacritics);
  settings.setValue(SID_FULLSEARCH_TYPE_REGEX,m_fullRegex);
  settings.setValue(SID_FULLSEARCH_FORCE,m_fullForce);
  settings.endGroup();
  settings.beginGroup("HeadSearch");

  settings.setValue(SID_HEADSEARCH_DEBUG,m_headDebug->isChecked());
  settings.setValue(SID_HEADSEARCH_VERTICAL_LAYOUT,m_headVertical->isChecked());
  settings.setValue(SID_HEADSEARCH_FOCUS_TABLE,m_headFocusTable->isChecked());
  settings.setValue(SID_HEADSEARCH_STEP,m_headStep->value());
  settings.setValue(SID_HEADSEARCH_SINGLE_CLICK,m_headSingleClick->isChecked());

  settings.setValue(SID_HEADSEARCH_NEW_TAB,m_headNewTab);
  settings.setValue(SID_HEADSEARCH_GO_TAB,m_headGoTab);
  settings.setValue(SID_HEADSEARCH_WHOLE_WORD,m_headWholeWord );
  settings.setValue(SID_HEADSEARCH_DIACRITICS,m_headDiacritics);
  settings.setValue(SID_HEADSEARCH_TYPE_REGEX,m_headRegex);
  settings.setValue(SID_HEADSEARCH_FORCE,m_headForce);


  settings.endGroup();
  settings.beginGroup("LocalSearch");

   settings.setValue(SID_LOCALSEARCH_WHOLE_WORD,m_localWholeWord);
   settings.setValue(SID_LOCALSEARCH_DIACRITICS,m_localDiacritics);
   settings.setValue(SID_LOCALSEARCH_TYPE_REGEX,m_localRegex);
   settings.setValue(SID_LOCALSEARCH_FORCE,m_localForce);


  settings.value(SID_LOCALSEARCH_SHOW_ALL,m_localShowAll->isChecked());

  settings.endGroup();
  settings.beginGroup("Search");

  settings.setValue(SID_NODESEARCH_NEW_TAB,m_nodeNew->isChecked());
  settings.setValue(SID_NODESEARCH_GO_TAB,m_nodeGo->isChecked());
  settings.setValue(SID_PAGESEARCH_NEW_TAB,m_pageNew->isChecked());
  settings.setValue(SID_PAGESEARCH_GO_TAB,m_pageGo->isChecked());
  settings.setValue(SID_ROOTSEARCH_NEW_TAB,m_rootNew->isChecked());
  settings.setValue(SID_ROOTSEARCH_GO_TAB,m_rootGo->isChecked());

  settings.sync();
  settings.endGroup();
  emit(modified(false));
}
/**
 * TODO not complete
 *
 *
 * @return
 */
bool FindOptions::isModified()  {
  m_dirty = false;
  QSettings settings(m_settingsFileName,QSettings::IniFormat);
  settings.setIniCodec("UTF-8");
  settings.beginGroup("FullSearch");

  if (compare(&settings,SID_FULLSEARCH_DEBUG,m_fullDebug))  {
    m_dirty = true;
  }


  if (compare(&settings,SID_FULLSEARCH_FRAGMENT_SIZE,m_fullFragmentSize)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_FULLSEARCH_MAIN_CONTEXT,m_contextStyle)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_FULLSEARCH_ARABIC_CONTEXT,m_contextStyleArabic)) {
    m_dirty = true;
  }


  if (compare(&settings,SID_FULLSEARCH_INCLUDE_HEADS,m_fullIncludeHeads)) {
    m_dirty = true;
  }


  if (compare(&settings,SID_FULLSEARCH_ONE_ROW,m_fullOneRow)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_FULLSEARCH_STEP,m_fullStep)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_FULLSEARCH_HEAD_BACKGROUND,m_fullHeadColor)) {
    m_dirty = true;
  }

  if (m_fullNewTab     != settings.value(SID_FULLSEARCH_NEW_TAB,true).toBool()) {
    m_dirty = true;
  }

  if (m_fullGoTab      != settings.value(SID_FULLSEARCH_GO_TAB,true).toBool()) {
    m_dirty = true;
  }

  if (m_fullWholeWord  != settings.value(SID_FULLSEARCH_WHOLE_WORD,true).toBool()) {
    m_dirty = true;
  }

  if (m_fullDiacritics != settings.value(SID_FULLSEARCH_DIACRITICS,true).toBool()) {
    m_dirty = true;
  }

  if (m_fullRegex      != settings.value(SID_FULLSEARCH_TYPE_REGEX,true).toBool()) {
    m_dirty = true;
  }

  if (m_fullForce      != settings.value(SID_FULLSEARCH_FORCE,true).toBool()) {
    m_dirty = true;
  }


  settings.endGroup();
  settings.beginGroup("HeadSearch");

  if (compare(&settings,SID_HEADSEARCH_DEBUG,m_headDebug)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_HEADSEARCH_SINGLE_CLICK,m_headSingleClick)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_HEADSEARCH_VERTICAL_LAYOUT,m_headVertical)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_HEADSEARCH_FOCUS_TABLE,m_headFocusTable)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_HEADSEARCH_STEP,m_headStep)) {
    m_dirty = true;
  }

  if (m_headNewTab     != settings.value(SID_HEADSEARCH_NEW_TAB,true).toBool()) {
    m_dirty = true;
  }

  if (m_headGoTab      != settings.value(SID_HEADSEARCH_GO_TAB,true).toBool()) {
    m_dirty = true;
  }

  if (m_headWholeWord  != settings.value(SID_HEADSEARCH_WHOLE_WORD,true).toBool()) {
    m_dirty = true;
  }

  if (m_headDiacritics != settings.value(SID_HEADSEARCH_DIACRITICS,true).toBool()) {
    m_dirty = true;
  }

  if (m_headRegex      != settings.value(SID_HEADSEARCH_TYPE_REGEX,true).toBool()) {
    m_dirty = true;
  }

  if (m_headForce      != settings.value(SID_HEADSEARCH_FORCE,true).toBool()) {
    m_dirty = true;
  }


  settings.endGroup();
  settings.beginGroup("LocalSearch");

  if (compare(&settings,SID_LOCALSEARCH_SHOW_ALL,m_localShowAll)) {
    m_dirty = true;
  }

  if (m_localWholeWord  != settings.value(SID_LOCALSEARCH_WHOLE_WORD,true).toBool()) {
    m_dirty = true;
  }

  if (m_localDiacritics != settings.value(SID_LOCALSEARCH_DIACRITICS,true).toBool()) {
    m_dirty = true;
  }

  if (m_localRegex      != settings.value(SID_LOCALSEARCH_TYPE_REGEX,true).toBool()) {
    m_dirty = true;
  }

  if (m_localForce      != settings.value(SID_LOCALSEARCH_FORCE,true).toBool()) {
    m_dirty = true;
  }
  settings.endGroup();
  settings.beginGroup("Search");


  if (compare(&settings,SID_NODESEARCH_NEW_TAB,m_nodeNew)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_NODESEARCH_GO_TAB,m_nodeGo)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_PAGESEARCH_NEW_TAB,m_pageNew)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_PAGESEARCH_GO_TAB,m_pageGo)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_ROOTSEARCH_NEW_TAB,m_rootNew)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_ROOTSEARCH_GO_TAB,m_rootGo)) {
    m_dirty = true;
  }

  return m_dirty;
}
void FindOptions::onSetFont() {
  /*
  bool arabic = false;
  QPushButton * button = qobject_cast<QPushButton *>(QObject::sender());
  QFont f;
  if (button == m_standardButton) {
    f.fromString(m_standardFont->text());
  }
  else {
    arabic = true;
    f.fromString(m_arabicFont->text());
  }
  if (arabic) {
    QFontDialog * d = new QFontDialog(f);
    QList<QComboBox *> boxes = d->findChildren<QComboBox *>();
    if (boxes.size() == 1) {
      int ix = boxes[0]->findText("Arabic");
      if (ix >= 0) {
        boxes[0]->setCurrentText("Arabic");
        boxes[0]->activated(ix);
      }
    }
    d->setCurrentFont(f);
    if (d->exec() == QDialog::Accepted) {
      QFont font = d->currentFont();
      m_arabicFont->setText(font.toString());
    }
    delete d;
    return;
  }
  bool ok;
  QFont font = QFontDialog::getFont(&ok, f,  this);
  if (!ok) {
    return;
  }
  m_standardFont->setText(font.toString());
  */
}
void FindOptions::onSetColor() {
  QColor color;
  color.setNamedColor(m_fullHeadColor->text());
  QColorDialog d(color);
  if (d.exec() != QDialog::Accepted) {
    return;
  }
  int r,g,b;
  color = d.currentColor();
  color.getRgb(&r,&g,&b);
  QString str = QString("%1,%2,%3").arg(r).arg(g).arg(b);
  QSettings settings(m_settingsFileName,QSettings::IniFormat);
  settings.setIniCodec("UTF-8");
  settings.beginGroup("Colors");
  QStringList keys = settings.allKeys();
  QStringList v;
  for(int i=0;i < keys.size();i++) {
    v = settings.value(keys[i]).toStringList();
    if (v.join(",") == str) {
      m_fullHeadColor->setText(keys[i]);
      return;
    }
  }
  m_fullHeadColor->setText(d.currentColor().name());
}
void FindOptions::onFullDialog() {
  DialogOptions d;
  d.setWindowTitle(tr("Search Options"));
  d.setChecked(DialogOptions::Tab,m_fullNewTab);
  d.setChecked(DialogOptions::Go,m_fullGoTab);
  d.setChecked(DialogOptions::Whole,m_fullWholeWord);
  d.setChecked(DialogOptions::Diacritics,m_fullDiacritics);
  d.setChecked(DialogOptions::Regex,m_fullRegex);
  d.setChecked(DialogOptions::Force,m_fullForce);
  if (d.exec() == QDialog::Accepted) {
    m_fullNewTab =  d.isChecked(DialogOptions::Tab);
    m_fullGoTab = d.isChecked(DialogOptions::Go);
    m_fullWholeWord = d.isChecked(DialogOptions::Whole);
    m_fullDiacritics = d.isChecked(DialogOptions::Diacritics);
    m_fullRegex = d.isChecked(DialogOptions::Regex);
    m_fullForce = d.isChecked(DialogOptions::Force);
    bool v = isModified();
    setButtons(v);
    emit(modified(v));
  }
}
void FindOptions::onHeadDialog() {
  DialogOptions d;
  d.setWindowTitle(tr("Search Options"));
  d.setChecked(DialogOptions::Tab,m_headNewTab);
  d.setChecked(DialogOptions::Go,m_headGoTab);
  d.setChecked(DialogOptions::Whole,m_headWholeWord);
  d.setChecked(DialogOptions::Diacritics,m_headDiacritics);
  d.setChecked(DialogOptions::Regex,m_headRegex);
  d.setChecked(DialogOptions::Force,m_headForce);
  if (d.exec() == QDialog::Accepted) {
    m_headNewTab =  d.isChecked(DialogOptions::Tab);
    m_headGoTab = d.isChecked(DialogOptions::Go);
    m_headWholeWord = d.isChecked(DialogOptions::Whole);
    m_headDiacritics = d.isChecked(DialogOptions::Diacritics);
    m_headRegex = d.isChecked(DialogOptions::Regex);
    m_headForce = d.isChecked(DialogOptions::Force);
    bool v = isModified();
    setButtons(v);
    emit(modified(v));
  }
}
void FindOptions::onLocalDialog() {
  DialogOptions d;
  d.setWindowTitle(tr("Search Options"));
  d.enableOption(DialogOptions::Tab,false);
  d.enableOption(DialogOptions::Go,false);
  d.setChecked(DialogOptions::Whole,m_localWholeWord);
  d.setChecked(DialogOptions::Diacritics,m_localDiacritics);
  d.setChecked(DialogOptions::Regex,m_localRegex);
  d.setChecked(DialogOptions::Force,m_localForce);
  if (d.exec() == QDialog::Accepted) {
    m_localWholeWord = d.isChecked(DialogOptions::Whole);
    m_localDiacritics = d.isChecked(DialogOptions::Diacritics);
    m_localRegex = d.isChecked(DialogOptions::Regex);
    m_localForce = d.isChecked(DialogOptions::Force);
    emit(modified(true));
    bool v = isModified();
    setButtons(v);
    emit(modified(v));
  }
}
