#include "edgeparams.h"
#include "ui_edgeparams.h"
#include "inode.h"
/*!\func
 * конструктор
 * \param no
 * \return no
 */
EdgeParams::EdgeParams(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::EdgeParams)
{
    m_ui->setupUi(this);
    setWindowFlags(Qt::Tool);
    m_ui->wiresList->setModel(&wireModel);
    m_ui->wiresList->setItemDelegate(&wireDelegate);
    ppmenu.addAction(m_ui->actionAdd_wire);
    ppmenu.addAction(m_ui->actionRemove_wire);
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
void EdgeParams::closeEvent(QCloseEvent *e)
{
	Q_UNUSED(e);
        /*if(!wireModel.rowCount())
        {
            edge->sourceNode()->delEdge(edge);
            edge->destNode()->delEdge(edge);
            edge->Hide();
        }*/
        wireModel.removeRows(0, wireModel.rowCount(), QModelIndex());
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
	if(edge)
	{
		QStringList list;
		QAbstractItemModel *model = m_ui->wiresList->model();
		for(int row = 0; row < model->rowCount(); row++)
		{
                    QString name = model->data(model->index(row, 0), Qt::DisplayRole).toString();
                    if(name.isEmpty())continue;
                    QString data(name + "|"
				+ model->data(model->index(row, 1), Qt::EditRole).toString() + "|"
                                + model->data(model->index(row, 2), Qt::DisplayRole).toString() + "|"
                                + model->data(model->index(row, 3), Qt::DisplayRole).toString());
                    list<<data;

		}
		if(list.isEmpty())
                {
                    edge->sourceNode()->delEdge(edge);
                    edge->destNode()->delEdge(edge);
                    edge->Hide();
                }
		else
		{
                    edge->setWires(list);
		}
	}
	close();
}
/*!\func
 * показать
 * \param no
 * \return no
 */
void EdgeParams::showModal(Edge* e)
{
	edge = e;
	if(edge)
	{
		foreach(QString data, edge->getWires())
		{
			QStringList list = data.split("|");
                        if(list.count() == 4)
			{
				wireModel.insertRow(0);
				QModelIndex child = wireModel.index(0, 0);
				wireModel.setData(child, QVariant(list[0]), Qt::EditRole);
				child = wireModel.index(0, 1);
				wireModel.setData(child, list[1], Qt::EditRole);
                                child = wireModel.index(0, 2);
                                wireModel.setData(child, list[2], Qt::EditRole);
                                child = wireModel.index(0, 3);
                                wireModel.setData(child, list[3], Qt::EditRole);
                        }
		}
	}
	setWindowModality(Qt::ApplicationModal);
	show();
}
//! add wire
void EdgeParams::on_actionAdd_wire_triggered()
{
        static int id = 0;
	QModelIndex index = m_ui->wiresList->selectionModel()->currentIndex();
	wireModel.insertRow(index.row()+1);
	QModelIndex child = wireModel.index(index.row()+1, 0, index.parent());
        QString name("");
        do
        {
            name = "Wire" + QString::number(id++);
        } while(wireModel.present(name));
        wireModel.setData(child, QVariant(name), Qt::EditRole);
	child = wireModel.index(index.row()+1, 2, index.parent());
	wireModel.setData(child, "wire", Qt::EditRole);
}
//! del selected wire
void EdgeParams::on_actionRemove_wire_triggered()
{
    if (m_ui->wiresList->selectionModel()->selectedRows().isEmpty()) return;
    QModelIndex index = m_ui->wiresList->selectionModel()->selectedRows().first();
    if (!index.isValid())return;
    QAbstractItemModel *model = m_ui->wiresList->model();
    model->removeRows(index.row(), m_ui->wiresList->selectionModel()->selectedRows().count(), index.parent());
}
/*!\func
 * popup menu
 * \params
 * - index - model index
 * \return no
 */
void EdgeParams::on_wiresList_pressed(QModelIndex index)
{
    QCursor cursor;
    if(index.isValid()&&(qApp->mouseButtons() & Qt::RightButton))
        ppmenu.exec(cursor.pos());
}
/*!\func
 * close
 * \param no
 * \return no
 */
void EdgeParams::on_actionClose_triggered()
{
    close();
}
/*!\func
 * save all
 * \param no
 * \return no
 */
void EdgeParams::on_actionSave_triggered()
{
    on_pbOk_clicked();
}
