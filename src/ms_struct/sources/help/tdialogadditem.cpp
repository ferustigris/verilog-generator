#include "tdialogadditem.h"
#include "ui_tdialogadditem.h"
#include <QFileDialog>
#include <QMessageBox>

/*!\func
 * constructor
 * \params
 * - mi - changes node
 * - dbPath - project work dir
 * - parent - parent widget
 * \return no
 */
TDialogAddItem::TDialogAddItem(INode *mi, const QString &dbPath, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::TDialogAddItem) {
    m_ui->setupUi(this);
	n = mi;
	oldName = n->getName();
	setModal(true);
	m_ui->pbOk->setEnabled(true);
	show();
	m_ui->tabWidget->setCurrentIndex(0);
	m_ui->textEdit->setCurrentFileName(dbPath);
	m_ui->textEdit->fileOpen(mi->getHelpFile());
	m_ui->leName->setText(mi->getName());
	m_ui->shortDescr->setPlainText(mi->getShortDesc());
}
/*!\func
 * destructor
 * \params no
 * \return no
 */
TDialogAddItem::~TDialogAddItem() {
    delete m_ui;
}
/*!\func
 * change
 * \params
 * e - event
 * \return no
 */
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
/*!\func
 * cancel slot
 * \params no
 * \return no
 */
void TDialogAddItem::on_pbCancel_pressed() {
    reject();
}
/*!\func
 * ok slot
 * \params no
 * \return no
 */
void TDialogAddItem::on_pbOk_pressed() {
	QString name(m_ui->leName->text());
	n->setHelpFile(m_ui->textEdit->getCurrentFileName());
	QString short_desc(m_ui->shortDescr->toPlainText());
	n->setHelp(m_ui->textEdit->fileSave(), m_ui->textEdit->getAddedImages());
	n->setShortDesc(short_desc);
	n->setName(name);
	accept();
}
/*!\func
 * are name is present in project?
 * \params
 * - name - new name
 * - n - node
 * \return no
 */
bool isNamePresent(QString name, INode *n)
{
    if(!n)return false;
    foreach(INode *child, n->getNodes())
    {
        if(isNamePresent(name, child))return true;
    }
    if(name == n->getName())return true;
    return false;
}
/*!\func
 * text changed
 * \params
 * - text - new text
 * \return no
 */
void TDialogAddItem::on_leName_textChanged(QString text)
{
    //QMessageBox::information(0, text, text);
    if(oldName == text)
    {
        m_ui->pbOk->setEnabled(true);
        return;
    }
    else
    if((n)&&(!text.isEmpty()))
    {
        INode*p = n;
        while(p->getParent())
            p = p->getParent();
        if(!isNamePresent(text, p))
        {
            m_ui->pbOk->setEnabled(true);
            return;
        }
    }
    m_ui->pbOk->setEnabled(false);
}
