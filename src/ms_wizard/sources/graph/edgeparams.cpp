#include "edgeparams.h"
#include "ui_edgeparams.h"
/*!\func
 * конструктор
 * \param no
 * \return no
 */
EdgeParams::EdgeParams(Data*data, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::EdgeParams)
{
    m_ui->setupUi(this);
    setWindowFlags(Qt::Tool);
    this->data = data;
    id = -1;
}
/*!\func
 * деструктор
 * \param no
 * \return no
 */
EdgeParams::~EdgeParams()
{
    delete m_ui;
}

void EdgeParams::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
/*!\func
 * отмена
 * \param no
 * \return no
 */
void EdgeParams::on_pbCancel_clicked()
{
    close();
}
/*!\func
 * сохранить
 * \param no
 * \return no
 */
void EdgeParams::on_pbOk_clicked()
{
    if(data)
        data->setRelationCondition(id, m_ui->leName->text());
    close();
}
/*!\func
 * показать
 * \param no
 * \return no
 */
void EdgeParams::showModal(const qint32 id)
{
    if(data)
    {
        m_ui->leName->setText(data->getRelation(id).getCondition());
    }
    this->id = id;
    setWindowModality(Qt::WindowModal);
    show();
}
