#include "wiremodel.h"
#include "QStringList"
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QApplication>
#include <errors.h>

/*!\func
 *
 * \param no
 * \return no
 */
TInputModel::TInputModel()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    rows = 0;
    columns = 4;
}
/*!\func
 *
 * \param no
 * \return no
 */
int TInputModel::rowCount ( const QModelIndex & ) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    return rows;
}
/*!\func
 *
 * \param no
 * \return no
 */
int TInputModel::columnCount ( const QModelIndex &) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    return columns;
}
/*!\func
 *
 * \param no
 * \return no
 */
QVariant TInputModel::data ( const QModelIndex &index, int role) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (!index.isValid())
	 return QVariant();
    if (role != Qt::DisplayRole && role != Qt::EditRole)
	 return QVariant();
    switch(index.column())
    {
	case 0:
	    return  inputs[index.row()].name;
	case 1:
	    if (role == Qt::DisplayRole)
		return  QString::number(inputs[index.row()].begin) + ":" + QString::number(inputs[index.row()].end);
	    else
		return  QString::number(inputs[index.row()].begin + 1);
        case 2:
            return  inputs[index.row()].type;
        case 3:
            return  inputs[index.row()].description;
    }
    return inputs[index.row()].name;
}
/*!\func
 * are wire present?
 * \param no
 * \return no
 */
bool TInputModel::present(const QString&name)const
{
    bool present = false;
    foreach(TInputItem item, inputs)
    {
        if(item.name == name)
        {
            present = true;
            break;
        }
    }
    return present;
}
/*!\func
 *
 * \param no
 * \return no
 */
bool TInputModel::insertRow ( int row, const QModelIndex & parent)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    beginInsertRows(parent,rows,rows);
    inputs.insert(row, TInputItem("",0,0));
    rows++;
    endInsertRows();
    return true;
}
/*!\func
 *
 * \param no
 * \return no
 */
bool TInputModel::setData ( const QModelIndex & index, const QVariant & value, int role)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (role != Qt::EditRole)
	 return false;
    bool result = false;
    switch(index.column())
    {
	case 0:
	    if(value.toString() == "")return false;
	    inputs[index.row()].name = value.toString();
	    result = true;
	    break;
	case 1:
	    inputs[index.row()].end = 0;
	    inputs[index.row()].begin = value.toInt() - 1;
	    result = true;
	    break;
	case 2:
	    if(value.toString() == "")return false;
	    inputs[index.row()].type = value.toString();
	    result = true;
	    break;
        case 3:
            inputs[index.row()].description = value.toString();
            result = true;
            break;
    }
    if (result)
	emit dataChanged(index, index);
    return result;
}
/*!\func
 *
 * \param no
 * \return no
 */
Qt::ItemFlags TInputModel::flags(const QModelIndex &index) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
     if (!index.isValid())
	 return 0;

     return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
/*!\func
 *
 * \param no
 * \return no
 */
QVariant TInputModel::headerData ( int section, Qt::Orientation orientation, int role) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (role != Qt::DisplayRole)
	 return QVariant();
    if (orientation == Qt::Vertical)
	 return QVariant();
    switch(section)
    {
	case 0:
	    return qApp->translate("EnterInputs", "Name");
	case 1:
	    return qApp->translate("EnterInputs", "Size");
        case 2:
            return qApp->translate("EnterInputs", "Type");
        case 3:
            return qApp->translate("EnterInputs", "Description");
    }
    return QVariant();
}
/*!\func
 *
 * \param no
 * \return no
 */
const QList<TInputItem>& TInputModel::getList()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    return inputs;
}
/*!\func
 *
 * \param no
 * \return no
 */
bool TInputModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    bool success = true;
    if (rows < 1)return false;
    beginRemoveRows(parent, position, position + rows - 1);
    for(int i = position; i < position + rows; i++)
		if(inputs.count() > position)
		{
			inputs.removeAt(position);
			this->rows--;
		}
    endRemoveRows();
    return success;
}

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
	    editor->addItem("wire");
		editor->addItem("reg");
	    return editor;
	}
        case 3:
        {
            QLineEdit *editor = new QLineEdit(parent);
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
	    break;
	    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
	    QString value = index.model()->data(index, Qt::EditRole).toString();
	    QComboBox *spinBox = static_cast<QComboBox*>(editor);
	    if(value == "reg")
	       spinBox->setCurrentIndex(1);
	    else
		   spinBox->setCurrentIndex(0);
	}
	break;
        case 3:
        {
             LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
             QString value = index.model()->data(index, Qt::EditRole).toString();
             QLineEdit *spinBox = static_cast<QLineEdit*>(editor);
             spinBox->setText(value);
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
            TInputModel *m = dynamic_cast<TInputModel*>(model);
            if(m)
            {
                if(!m->present(value))
                    model->setData(index, value, Qt::EditRole);
            }
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
        case 3:
        {
             LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
             QLineEdit *spinBox = static_cast<QLineEdit*>(editor);
             QString value = spinBox->text();
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

