#include "statedelegate.h"
#include "errors.h"
#include <QLineEdit>
#include <QComboBox>
#include "identificatoredit.h"

/*!\func StateDelegate::StateDelegate
 * конструктор класса для редактирования состояний и их связей
 * \param нет
 * \return нет
 */
StateDelegate::StateDelegate(QObject *parent) :
        QItemDelegate(parent)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
}
/*!\func StateDelegate::createEditor
 * создает редактор ячейки
 * \param нет
 * \return нет
 */
QWidget *StateDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex & index) const
 {
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if(!index.isValid())return 0;
    if(!index.internalId())
    {
        switch(index.column())
        {
            case 0:
            {
                    IdentificatorEdit *editor = new IdentificatorEdit(parent);
                    return editor;
            }
            default:
                return 0;
        }
    }
    else
    {
        switch(index.column())
        {
            case 2:
            {//next state
                QComboBox *editor = new QComboBox(parent);
                for(int i = 0; i < index.model()->rowCount(QModelIndex());i++)
                {
                    editor->addItem(index.model()->index(i,0).data().toString());
                }
                return editor;
            }
            case 1:
            {//condition
                QLineEdit *editor = new QLineEdit(parent);
                return editor;
            }
            default:
                return 0;
        }
    }
    return 0;
 }
/*!\func StateDelegate::createEditor
 * установить данные в виджет
 * \param нет
 * \return нет
 */
void StateDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if(!index.isValid())return;
    if(!index.internalId())
    {
        switch(index.column())
        {
            case 0:
            {
                QString value = index.model()->data(index, Qt::EditRole).toString();
                IdentificatorEdit *edit = static_cast<IdentificatorEdit*>(editor);
                edit->setText(value);
                break;
            }
            default:
                return;
        }
    }
    else
    {
        switch(index.column())
        {
            case 2:
            {//next state
                QString value = index.model()->data(index, Qt::EditRole).toString();
                QComboBox *spinBox = static_cast<QComboBox*>(editor);
                spinBox->setCurrentIndex(0);
                for(int i = 0; i < spinBox->count(); i++)
                    if(spinBox->itemText(i) == value)
                    {
                        spinBox->setCurrentIndex(i);
                        break;
                    }
                break;
            }
            case 1:
            {//condition
                QString value = index.model()->data(index, Qt::EditRole).toString();
                QLineEdit *edit = static_cast<QLineEdit*>(editor);
                edit->setText(value);
                break;
            }
            default:
                return;
        }
    }
}
/*!\func StateDelegate::setModelData
 * установить данные в модель
 * \param нет
 * \return нет
 */
void StateDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if(!index.isValid())return;
    if(!index.internalId())
    {
        switch(index.column())
        {
            case 0:
            {//имя состояния
                QLineEdit *edit = static_cast<QLineEdit*>(editor);
                model->setData(index, edit->text(), Qt::EditRole);
                break;
            }
            default:
                return;
        }
    }
    else
    {
        switch(index.column())
        {
            case 2:
            {//next state
                QComboBox *spinBox = static_cast<QComboBox*>(editor);
                model->setData(index, spinBox->currentIndex(), Qt::EditRole);
                break;
            }
            case 1:
            {//condition
                QLineEdit *edit = static_cast<QLineEdit*>(editor);
                model->setData(index, edit->text(), Qt::EditRole);
                break;
            }
            default:
                return;
        }
    }
}
/*!\func StateDelegate::updateEditorGeometry
 * изменить геометрию
 * \param нет
 * \return нет
 */
void StateDelegate::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    editor->setGeometry(option.rect);
}
