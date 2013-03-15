#include <QtGui>
#include <QComboBox>
#include "errors.h"
#include "includestatemodel.h"
/*!\func
 *
 * \param no
 * \return no
 */
IncludeModel::IncludeModel(const QStringList &headers, const QString &data,
		     QObject *parent)
    : QAbstractItemModel(parent)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    QVector<QVariant> rootData;
    foreach (QString header, headers)
	rootData << header;
    rootItem = new TreeItem(rootData);
    setupModelData(data.split(QString("\n")), rootItem);
}
/*!\func
 *
 * \param no
 * \return no
 */
IncludeModel::~IncludeModel()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    delete rootItem;
}
/*!\func
 *
 * \param no
 * \return no
 */
int IncludeModel::columnCount(const QModelIndex & ) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    return rootItem->columnCount();
}
/*!\func
 *
 * \param no
 * \return no
 */
QVariant IncludeModel::data(const QModelIndex &index, int role) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (!index.isValid())
	return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
	return QVariant();

    TreeItem *item = getItem(index);

    return item->data(index.column());
}
/*!\func
 *
 * \param no
 * \return no
 */
Qt::ItemFlags IncludeModel::flags(const QModelIndex &index) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (!index.isValid())
	return Qt::ItemIsDropEnabled;
    return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled;
}
/*!\func
 *
 * \param no
 * \return no
 */
TreeItem *IncludeModel::getItem(const QModelIndex &index) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (index.isValid()) {
	TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
	if (item) return item;
    }
    return rootItem;
}
/*!\func
 *
 * \param no
 * \return no
 */
QVariant IncludeModel::headerData(int section, Qt::Orientation orientation,
			       int role) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	return rootItem->data(section);

    return QVariant();
}
/*!\func
 *
 * \param no
 * \return no
 */
QModelIndex IncludeModel::index(int row, int column, const QModelIndex &parent) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (parent.isValid() && parent.column() != 0)
	return QModelIndex();
    TreeItem *parentItem = getItem(parent);

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
	return createIndex(row, column, childItem);
    else
	return QModelIndex();
}
/*!\func
 *
 * \param no
 * \return no
 */
bool IncludeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}
/*!\func
 *
 * \param no
 * \return no
 */
bool IncludeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    TreeItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}
/*!\func
 *
 * \param no
 * \return no
 */
QModelIndex IncludeModel::parent(const QModelIndex &index) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (!index.isValid())
	return QModelIndex();

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
	return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}
/*!\func
 *
 * \param no
 * \return no
 */
bool IncludeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
   LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
   bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
	removeRows(0, rowCount());

    return success;
}
/*!\func
 *
 * \param no
 * \return no
 */
bool IncludeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    TreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}
/*!\func
 *
 * \param no
 * \return no
 */
int IncludeModel::rowCount(const QModelIndex &parent) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    TreeItem *parentItem = getItem(parent);
    return parentItem->childCount();
}
/*!\func
 *
 * \param no
 * \return no
 */
bool IncludeModel::setData(const QModelIndex &index, const QVariant &value,
			int role)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (role != Qt::EditRole)
	return false;
    TreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
	emit dataChanged(index, index);

    return result;
}
/*!\func
 *
 * \param no
 * \return no
 */
bool IncludeModel::setHeaderData(int section, Qt::Orientation orientation,
			      const QVariant &value, int role)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
	return false;
    bool result = rootItem->setData(section, value);
    if (result)
	emit headerDataChanged(orientation, section, section);
    return result;
}
/*!\func
 *
 * \param no
 * \return no
 */
void IncludeModel::setupModelData(const QStringList &lines, TreeItem *parent)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    QList<TreeItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
	int position = 0;
	while (position < lines[number].length()) {
	    if (lines[number].mid(position, 1) != " ")
		break;
	    position++;
	}

	QString lineData = lines[number].mid(position).trimmed();

	if (!lineData.isEmpty()) {
	    // Read the column data from the rest of the line.
	    QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
	    QVector<QVariant> columnData;
	    for (int column = 0; column < columnStrings.count(); ++column)
		columnData << columnStrings[column];

	    if (position > indentations.last()) {
		// The last child of the current parent is now the new parent
		// unless the current parent has no children.

		if (parents.last()->childCount() > 0) {
		    parents << parents.last()->child(parents.last()->childCount()-1);
		    indentations << position;
		}
	    } else {
		while (position < indentations.last() && parents.count() > 0) {
		    parents.pop_back();
		    indentations.pop_back();
		}
	    }

	    // Append a new item to the current parent's list of children.
	    TreeItem *parent = parents.last();
	    parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());
	    for (int column = 0; column < columnData.size(); ++column)
		parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
	}

	number++;
    }
}
/*!\func
 * типы поддерживаемые драг и дропом
 * \param нет
 * \return нет
 */
QStringList IncludeModel::mimeTypes() const
{
    QStringList types;
    types << "text/plain";
    return types;
}
/*!\func
 * обработчик событи€ вставки дропом
 * \param нет
 * \return нет
 */
bool IncludeModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int , const QModelIndex &parent)
{

	Q_UNUSED (parent);
	LOG(LOG_ERROR, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (action == Qt::IgnoreAction)
	return true;
    int beginRow = row < 0 ? 0 : row;
    beginInsertRows(QModelIndex(),0,rootItem->childCount());
    rootItem->insertChildren(beginRow,1,3);
    if (data->hasText())
    {
	TreeItem * item = rootItem->child(beginRow);
	if(!item)return false;
	item->setData(0,data->text());
	QRegExp reg1("\\(+");
	QRegExp reg2("\\)+");
	QString str = data->text();
	str = str.section(reg1,1);
	str = str.remove(")" + str.section(reg2,1));
	foreach(QString in, str.split(","))
	{
	    item->insertChildren(0,1,3);
	    item->child(0)->setData(1,in);
	}
    }
    endInsertRows();
    return true;
}

/*!\func MachinesIncludeDelegate::MachinesIncludeDelegate
 * конструктор
 * \param нет
 * \return нет
 */
MachinesIncludeDelegate::MachinesIncludeDelegate(QObject *)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
}
/*!\func MachinesIncludeDelegate::createEditor
 * создает редактор €чейки
 * \param нет
 * \return нет
 */
QWidget *MachinesIncludeDelegate::createEditor(QWidget *parent,
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
	    case 1:
	    {//next state
		QComboBox *editor = new QComboBox(parent);
		editor->addItem("test1");//<<"test2"<<"test3";
		//chooseMod->addItem(qApp->translate("EnterInputs", "Choose..."));
		//connect(chooseMod, SIGNAL(currentIndexChanged(int)), this, SLOT(on_currentIndexChanged(int)));
		return editor;
	    }
	    case 2:
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
/*!\func MachinesIncludeDelegate::createEditor
 * установить данные в виджет
 * \param нет
 * \return нет
 */
void MachinesIncludeDelegate::setEditorData(QWidget *editor,
				     const QModelIndex &index) const
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
	    case 1:
	    {//next state
		QString value = index.model()->data(index, Qt::EditRole).toString();
		QComboBox *spinBox = static_cast<QComboBox*>(editor);
		spinBox->setCurrentIndex(0);
		for(int i = 0; i < spinBox->count(); i++)
		    if(spinBox->itemText(i) == value)
			spinBox->setCurrentIndex(i);
		break;
	    }
	    case 2:
	    {//condition
		QString value = index.model()->data(index, Qt::EditRole).toString();
		QLineEdit *spinBox = static_cast<QLineEdit*>(editor);
		spinBox->setText(value);
		break;
	    }
	    default:
		return;
	}
    }
}
/*!\func MachinesIncludeDelegate::setModelData
 * установить данные в модель
 * \param нет
 * \return нет
 */
void MachinesIncludeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
				    const QModelIndex &index) const
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
	    case 1:
	    {//next state
		QComboBox *spinBox = static_cast<QComboBox*>(editor);
		model->setData(index, spinBox->currentText(), Qt::EditRole);
		break;
	    }
	    case 2:
	    {//condition
		QLineEdit *spinBox = static_cast<QLineEdit*>(editor);
		model->setData(index, spinBox->text(), Qt::EditRole);
		break;
	    }
	    default:
		return;
	}
    }
}
/*!\func MachinesIncludeDelegate::updateEditorGeometry
 * изменить геометрию
 * \param нет
 * \return нет
 */
void MachinesIncludeDelegate::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    editor->setGeometry(option.rect);
}
/*!\func
 * слот выбора модул€
 * \param нет
 * \return нет

void MachinesIncludeDelegate::on_currentIndexChanged ( int index )
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if(!chooseMod)return;
    if(index == chooseMod->count() - 1)
    {//выбран пункт ®выбрать®
	QString fn = QFileDialog::getOpenFileName(0, qApp->translate("EnterInputs", "Open module file..."), QString(), qApp->translate("EnterInputs", "Verilog (*.v);;All Files (*)"));
	chooseMod->insertItem(0, fn);
    }
}
*/
