#include "outputdelegate.h"
#include "errors.h"
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include "identificatoredit.h"

/*!\func ODelegate::ODelegate
 * конструктор
 * \param нет
 * \return нет
 */
ODelegate::ODelegate(QObject *)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
}
/*!\func ODelegate::createEditor
 * создает редактор ячейки
 * \param нет
 * \return нет
 */
QWidget *ODelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex & index) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    switch(index.column()) {
        case 0:
        {
            IdentificatorEdit *editor = new IdentificatorEdit(parent);
            return editor;
        }
        case 1:
        {
            QSpinBox *editor = new QSpinBox(parent);
            editor->setMinimum(1);
            editor->setMaximum(100);
            return editor;
        }
        case 2:
        {
            QComboBox *editor = new QComboBox(parent);
            editor->addItem("reg");
			editor->addItem("wire");
			editor->setCurrentIndex(0);
            return editor;
        }
        case 3:
        {
            QLineEdit *editor = new QLineEdit(parent);
			//editor->setInputMask("<N>HHHHHHHHH");
            return editor;
        }
    }
    return 0;
}
/*!\func ODelegate::createEditor
 * установить данные в виджет
 * \param нет
 * \return нет
 */
void ODelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    switch(index.column()) {
        case 0:
        {
             LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
             QString value = index.model()->data(index, Qt::EditRole).toString();
             IdentificatorEdit *spinBox = static_cast<IdentificatorEdit*>(editor);
             if(spinBox)
                 spinBox->setText(value);
        }
             break;
        case 1:
        {
             LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
             int value = index.model()->data(index, Qt::EditRole).toInt();
             QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
             if(spinBox)
                spinBox->setValue(value);
        }
             break;
        case 2:
        {
             LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
             QString value = index.model()->data(index, Qt::EditRole).toString();
             QComboBox *spinBox = static_cast<QComboBox*>(editor);
             if(spinBox)
             {
                 if(value == "reg")
                    spinBox->setCurrentIndex(0);
                 else
                    spinBox->setCurrentIndex(1);
             }
             break;
        }
        case 3:
        {
             LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
             QString value = index.model()->data(index, Qt::EditRole).toString();
             QLineEdit *spinBox = static_cast<QLineEdit*>(editor);
             if(spinBox)
                 spinBox->setText(value);
        }
             break;
    }
}
/*!\func ODelegate::setModelData
 * установить данные в модель
 * \param нет
 * \return нет
 */
void ODelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    switch(index.column()) {
        case 0:
        {
            IdentificatorEdit *spinBox = static_cast<IdentificatorEdit*>(editor);
            QString value = spinBox->text();
            model->setData(index, value, Qt::EditRole);
            break;
        }
        case 1:
        {
            QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
            if(spinBox)
            {
                spinBox->interpretText();
                model->setData(index, spinBox->value(), Qt::EditRole);
            }
            break;
        }
        case 2:
        {
            LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
            QComboBox *spinBox = static_cast<QComboBox*>(editor);
            if(spinBox)
            {
                QString value = spinBox->currentText();
                model->setData(index, value, Qt::EditRole);
            }
            break;
        }
        case 3:
        {
            QLineEdit *spinBox = static_cast<QLineEdit*>(editor);
            if(spinBox)
            {
                model->setData(index, spinBox->text(), Qt::EditRole);
            }
            break;
        }
    }
}
/*!\func ODelegate::updateEditorGeometry
 * изменить геометрию
 * \param нет
 * \return нет
 */
void ODelegate::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
     LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
     editor->setGeometry(option.rect);
}
