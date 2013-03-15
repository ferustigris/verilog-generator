#include "controldelegate.h"
#include "errors.h"
#include <QLineEdit>
/*!\func ControlDelegate::ControlDelegate
 * �����������
 * \param ���
 * \return ���
 */
ControlDelegate::ControlDelegate(QObject *)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
}
/*!\func ControlDelegate::createEditor
 * ������� �������� ������
 * \param ���
 * \return ���
 */
QWidget *ControlDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex & index) const
 {
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if(!index.isValid())return 0;
    if(!index.parent().isValid())
    {
        return 0;
    }
    else
    {
        switch(index.column())
        {
            case 2:
            case 3:
            {//value
                QLineEdit *editor = new QLineEdit(parent);
                return editor;
            }
            default:
                return 0;
        }
    }
    return 0;
 }
/*!\func ControlDelegate::createEditor
 * ���������� ������ � ������
 * \param ���
 * \return ���
 */
void ControlDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if(!index.isValid())return;
    if(!index.parent().isValid())
        return;
    switch(index.column())
    {
        case 2:
        case 3:
        {//Value
            QString value = index.model()->data(index, Qt::DisplayRole).toString();
            QLineEdit *edit = static_cast<QLineEdit*>(editor);
            if(edit)
                edit->setText(value);
            break;
        }
        default:
            break;
    }
}
/*!\func ControlDelegate::setModelData
 * ���������� ������ � ������
 * \param ���
 * \return ���
 */
void ControlDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if(!index.isValid())return;
    if(!index.parent().isValid())
    {
        return;
    }
    else
    {
        switch(index.column())
        {
            case 2:
            {//VBalue
                QLineEdit *edit = static_cast<QLineEdit*>(editor);
                model->setData(index, edit->text(), Qt::EditRole);
                break;
            }
            case 3:
            {//Condition
                QLineEdit *edit = static_cast<QLineEdit*>(editor);
                model->setData(index, edit->text(), Qt::EditRole);
                break;
            }
            default:
                return;
        }
    }
}
/*!\func ControlDelegate::updateEditorGeometry
 * �������� ���������
 * \param ���
 * \return ���
 */
void ControlDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    editor->setGeometry(option.rect);
}

