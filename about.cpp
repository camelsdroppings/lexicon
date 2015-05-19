#include "about.h"
#include "version.h"
#include "definedsql.h"
AboutDialog::AboutDialog(QWidget * parent) : QDialog(parent) {
  QVBoxLayout * layout = new QVBoxLayout;
  QTabWidget * tabs = new QTabWidget(this);
  QWidget * tab;

  tab = this->buildTechTab();
  if (tab) {
    tabs->addTab(tab,tr("Tech"));
  }
  QDialogButtonBox *   buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);

  layout->addWidget(tabs);
  layout->addWidget(buttonBox);
  setLayout(layout);
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
}
QSize AboutDialog::sizeHint() const {
  return QSize(600,400);
}

QWidget * AboutDialog::buildTechTab() {
  QString sql(SQL_GET_INFO);

  QSqlQuery query;
  if (! query.prepare(sql)) {
    QLOG_WARN() << "SQL error preparing about info" << query.lastError().text();
    return NULL;
  }
  query.exec();
  if (! query.first()) {
    QLOG_WARN() << "SQL error retrieving about info" << query.lastError().text();
    return NULL;
  }
  QWidget * w = new QWidget(this);
  QFormLayout * layout = new QFormLayout;
  QLineEdit * edit;

  edit = new QLineEdit;
  edit->setText(query.value(1).toString());
  edit->setReadOnly(true);
  layout->addRow(tr("Name"),edit);

  edit = new QLineEdit;
  edit->setText(query.value(2).toString());
  edit->setReadOnly(true);
  layout->addRow(tr("Parser version"),edit);

  edit = new QLineEdit;
  edit->setText(query.value(3).toString());
  edit->setReadOnly(true);
  layout->addRow(tr("Db creation date"),edit);

  edit = new QLineEdit;
  edit->setText(query.value(4).toString());
  edit->setReadOnly(true);
  layout->addRow(tr("XML version"),edit);

  edit = new QLineEdit;
  edit->setText(query.value(5).toString());
  edit->setReadOnly(true);
  layout->addRow(tr("Db ID"),edit);

  edit = new QLineEdit;
  edit->setText(buildVersion());
  edit->setReadOnly(true);
  layout->addRow(tr("Build version"),edit);

   w->setLayout(layout);
  return w;
}
