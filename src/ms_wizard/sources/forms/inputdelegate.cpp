#include "inputdelegate.h"
#include "errors.h"
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include "identificatoredit.h"

/*!\func IDelegate::IDelegate
 * конструктор
 * \param нет
 * \return нет
 */
IDelegate::IDelegate(QObject *)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
}
/*!\func IDelegate::createEditor
 * создает редактор ячейки
 * \param нет
 * \return нет
 */
QWidget *IDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex & index) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    switch(index.column()) {
        case 0:
        {
            IdentificatorEdit *editor = new IdentificatorEdit(parent);
            //editor->setInputMask("NNNNNNNNNNNNNNN");
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
            return 0;
            QComboBox *editor = new QComboBox(parent);
            editor->addItem("wire");
            editor->addItem("reg");
            return editor;
        }
    }
    return 0;
}
/*!\func IDelegate::createEditor
 * установить данные в виджет
 * \param нет
 * \return нет
 */
void IDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    switch(index.column()) {
        case 0:
        {
             LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
             QString value = index.model()->data(index, Qt::EditRole).toString();
             IdentificatorEdit *spinBox = static_cast<IdentificatorEdit*>(editor);
             spinBox->setText(value);
        }
        break;
        case 1:
        {
             LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
             int value = index.model()->data(index, Qt::EditRole).toInt();
             QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
             spinBox->setValue(value);
        }
        break;
        case 2:
        {
            LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
            QComboBox *spinBox = static_cast<QComboBox*>(editor);
            spinBox->setCurrentIndex(0);
        }
        break;
    }
}
/*!\func IDelegate::setModelData
 * установить данные в модель
 * \param нет
 * \return нет
 */
void IDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    switch(index.column()) {
        case 0:
        {
            IdentificatorEdit *spinBox = static_cast<IdentificatorEdit*>(editor);
            QString value = spinBox->text();
            model->setData(index, value, Qt::EditRole);
        }
        break;
        case 1:
        {
            QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
            spinBox->interpretText();
            model->setData(index, spinBox->value(), Qt::EditRole);
        }
        break;
        case 2:
        {
            break;
            QComboBox *spinBox = static_cast<QComboBox*>(editor);
            QString value = spinBox->currentText();
            model->setData(index, value, Qt::EditRole);
        }
        break;
    }
}
/*!\func IDelegate::updateEditorGeometry
 * изменить геометрию
 * \param нет
 * \return нет
 */
void IDelegate::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
     LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
     editor->setGeometry(option.rect);
}
