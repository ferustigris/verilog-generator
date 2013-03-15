#include "tdialogadditem.h"
#include "ui_tdialogadditem.h"
#include <QFileDialog>
#include <QMessageBox>

TDialogAddItem::TDialogAddItem(Data*data, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::TDialogAddItem) {
    m_ui->setupUi(this);
    this->data = data;
}

TDialogAddItem::~TDialogAddItem() {
    delete m_ui;
}

void TDialogAddItem::changeEvent(QEvent *e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
	m_ui->retranslateUi(this);
	break;
    default:
	break;
    }
}

void TDialogAddItem::on_pbCancel_pressed() {
    reject();
}

void TDialogAddItem::on_pbOk_pressed() {
    accept();
}

void TDialogAddItem::showModal(const qint32 id)
{
	if(!data)return;
	setModal(true);
	show();
	m_ui->tabWidget->setCurrentIndex(0);
	m_ui->leName->setText(data->getState(id).getName());
	m_ui->shortDescr->setPlainText(data->getState(id).getDescription());
	if(!exec())return;
	data->setStateName(id, m_ui->leName->text());
	data->setStateDescription(id, m_ui->shortDescr->toPlainText());
}

