#include <QtGui>
#include "errors.h"
#include "controlstatemodel.h"
/*!\func
 *
 * \param no
 * \return no
 */
ControlModel::ControlModel(Data *data, const QStringList &headers)
    : QAbstractItemModel(0)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    this->headers = headers;
    shared_data = data;
    if(!shared_data)return;
    connect(shared_data, SIGNAL(sAddControl(const qint32, const qint32)), this, SLOT(onAddControl(const qint32, const qint32)));
    connect(shared_data, SIGNAL(sEditControl(const qint32)), this, SLOT(onEditControl(const qint32)));
    connect(shared_data, SIGNAL(sAddState(const qint32)), this, SLOT(onAddState(const qint32)));
    connect(shared_data, SIGNAL(sEditState(const qint32)), this, SLOT(onEditState(const qint32)));
    connect(shared_data, SIGNAL(sRemoveState(const qint32)), this, SLOT(onRemoveState(const qint32)));
    connect(shared_data, SIGNAL(sRemoveControl(qint32)), this, SLOT(onRemoveControl(qint32)));
    connect(shared_data, SIGNAL(sEditOutput(const qint32)), this, SLOT(onEditOutput(const qint32)));
    QList<State> list = shared_data->getStates();
    foreach(State state, list)
    {
	ids.append(Statestruct(state.getId()));
	foreach(Control rel, shared_data->getControls(state.getId()))
	    ids[ids.count()-1].relations.append(rel.getId());
    }
}
/*!\func
 * slot data changed
 * \param no
 * \return no
 */
void ControlModel::onAddControl (const qint32 id_state, const qint32 id )
{
	qint32 i = -1;
	for(int k = 0; k < ids.count(); k++)
	    if(ids[k].id == id_state)
	    {
		i = k;
		break;
	    }
	if(!shared_data)return;
	if(i < 0)return;
	qint32 count = ids[i].relations.count() - 1;
	if(count < 0)count = 0;
	beginInsertRows(index(i, 0),count,count);
	ids[i].relations.append(id);
	endInsertRows();
}
/*!\func
 * slot data changed
 * \param no
 * \return no
 */
void ControlModel::onAddState (const qint32 id )
{
    qint32 row = 0;
    if(ids.count() > 0)row = ids.count()-1;
    beginInsertRows(QModelIndex(),row,row);
    ids.append(Statestruct(id));
    endInsertRows();
}
/*!\func
 * slot data remove
 * \param no
 * \return no
 */
void ControlModel::onRemoveState (const qint32 id )
{
    qint32 row = -1;
    for(int k = 0; k < ids.count(); k++)
	if(ids[k].id == id)
	{
	    row = k;
	    break;
	}
    if(row < 0)return;
    beginRemoveRows(QModelIndex(), row, row);
    ids.removeAt(row);
    endRemoveRows();
}
/*!\func
 * slot data changed
 * \param no
 * \return no
 */
void ControlModel::onEditState (const qint32 id )
{
    qint32 row = -1;
    for(int k = 0; k < ids.count(); k++)
	if(ids[k].id == id)
	{
	    row = k;
	    break;
	}
    if(row < 0)return;
    QModelIndex i = index(row, 0);
    emit dataChanged(i, i);
}
/*!\func
 * slot data changed
 * \param no
 * \return no
 */
void ControlModel::onEditControl (const qint32 id )
{
    if(!shared_data)
	return;
    qint32 row = -1, state = -1, state_id = shared_data->getControl(id).getState().getId();
    for(int k = 0; k < ids.count(); k++)
	if(ids[k].id == state_id)
	{
	    state = k;
	    break;
	}
    if(state < 0)return;
    foreach(qint32 r, ids[state].relations)
	if(r == id)
	{
	    row = r;
	    break;
	}
    if(row < 0)return;
    QModelIndex p = index(state, 0);
    QModelIndex i1 = index(row,0,p);
    QModelIndex i2 = index(row,3,p);
    emit dataChanged(i1, i2);
}
/*!\func
 * slot data changed
 * \param no
 * \return no
 */
void ControlModel::onRemoveControl (const qint32 id )
{
    if(!shared_data)
	return;
    qint32 row = -1, state = -1, state_id = shared_data->getControl(id).getState().getId();
    for(int k = 0; k < ids.count(); k++)
	if(ids[k].id == state_id)
	{
	    state = k;
	    break;
	}
    if(state < 0)return;
    for(qint32 r = 0; r < ids[state].relations.count(); r++)
	if(ids[state].relations[r] == id)
	{
	    row = r;
	    break;
	}
    if(row < 0)return;
    QModelIndex p = index(state, 0);
    beginRemoveRows(p, row, row);
    ids[state].relations.removeAt(row);
    endRemoveRows();
}
/*!\func
 * slot data changed
 * \param no
 * \return no
 */
void ControlModel::onEditOutput (const qint32 id )
{
    if(!shared_data)
	return;
    qint32 row = -1, state = -1, state_id = shared_data->getControl(id).getState().getId();
    for(int k = 0; k < ids.count(); k++)
	if(ids[k].id == state_id)
	{
	    state = k;
	    break;
	}
    if(state < 0)return;
    foreach(qint32 r, ids[state].relations)
	if(r == id)
	{
	    row = r;
	    break;
	}
    if(row < 0)return;
    QModelIndex p = index(state, 0);
    QModelIndex i1 = index(row,0,p);
    QModelIndex i2 = index(row,3,p);
    emit dataChanged(i1, i2);
}
/*!\func
 *
 * \param no
 * \return no
 */
ControlModel::~ControlModel()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
}
/*!\func
 *
 * \param no
 * \return no
 */
int ControlModel::columnCount(const QModelIndex & ) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    return headers.count();
}
/*!\func
 *
 * \param no
 * \return no
 */
QVariant ControlModel::data(const QModelIndex &index, int role) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (!index.isValid())
	return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
	return QVariant();
    switch(index.column()){
    case 0:
	{
	    if(!index.internalId())
	    {
		if(ids.count() > index.row())
		    return shared_data->getState(ids[index.row()].id).getName();
	    }
	}
	break;
    case 1:
	{
	    if(index.internalId())
	    {
		if(ids.count() > index.internalId()-1)
		{
		    if(ids[index.internalId()-1].relations.count() > index.row())
				return shared_data->getControl(ids[index.internalId()-1].relations[index.row()]).getOutput().getName();
		}
	    }
	}
	break;
    case 2:
	{
	    if(index.internalId())
	    {
		if(ids.count() > index.internalId()-1)
		{
		    if(ids[index.internalId()-1].relations.count() > index.row())
				return  shared_data->getControl(ids[index.internalId()-1].relations[index.row()]).getCondition();
		}
	    }
	}
	break;
    case 3:
	{
	    if(index.internalId())
	    {
		if(ids.count() > index.internalId()-1)
		{
		    if(ids[index.internalId()-1].relations.count() > index.row())
				return shared_data->getControl(ids[index.internalId()-1].relations[index.row()]).getValue();
		}
	    }
	}
	break;
    default:
	return QVariant();
    }
    return QVariant();

}
/*!\func
 *
 * \param no
 * \return no
 */
Qt::ItemFlags ControlModel::flags(const QModelIndex &index) const
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
QVariant ControlModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	return headers[section];
    return QVariant();
}
/*!\func
 * create index
 * \param no
 * \return no
 */
QModelIndex ControlModel::index(int row, int column, const QModelIndex &parent) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (parent.isValid() && !parent.parent().isValid())
	    return createIndex(row, column, parent.row() + 1);
    else if(!parent.isValid())
	    return createIndex(row, column, 0);
    return QModelIndex();
}
/*!\func
 *
 * \param no
 * \return no
 */
QModelIndex ControlModel::parent(const QModelIndex &index) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (!index.isValid())
	return QModelIndex();
    if(index.internalId())
	return createIndex(index.internalId()-1,0,0);
    return QModelIndex();
}
/*!\func
 * count childs elements
 * \param no
 * \return no
 */
int ControlModel::rowCount(const QModelIndex &parent) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if(parent.isValid()&&!parent.internalId()&&!parent.parent().isValid())
    {
	    if(shared_data)
	    {
		if(parent.row() < ids.count())
		    return ids[parent.row()].relations.count();
	    }
	    return 0;
    }
    if(!parent.isValid()&&!parent.parent().isValid())
	    return ids.count();
    return 0;
}
/*!\func
 *
 * \param no
 * \return no
 */
bool ControlModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (role != Qt::EditRole)
	return false;
    if(!index.parent().isValid())
	return false;
    switch(index.column())
    {
    case 2:
	{
	    if(index.row() < ids[index.parent().row()].relations.count())
	    {
		shared_data->setControlCondition(ids[index.parent().row()].relations[index.row()], value.toString());
		emit dataChanged(index, index);
		return true;
	    }
	}
	break;
    case 3:
	{
	    if(index.row() < ids[index.parent().row()].relations.count())
	    {
		shared_data->setControlValue(ids[index.parent().row()].relations[index.row()], value.toString());
		emit dataChanged(index, index);
		return true;
	    }
	}
	break;
    default:
	return false;
    }
    return true;
}
/*!\func
 * типы поддерживаемые драг и дропом
 * \param нет
 * \return нет
 */
QStringList ControlModel::mimeTypes() const
{
    QStringList types;
    types << "text/plain";
    return types;
}
/*!\func
 * обработчик события вставки дропом
 * \param нет
 * \return нет
 */
bool ControlModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
	Q_UNUSED(row);
	Q_UNUSED(column);
    LOG(LOG_ERROR, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (action == Qt::IgnoreAction)
	return true;
    if(parent.row() < 0)return false;
    if(parent.column() < 0)return false;
    qint32 index = 0;
    if(parent.parent().isValid())
    {
	if(parent.parent().row() < ids.count())
	    index = ids[parent.parent().row()].id;//this->index(parent.parent().row(), 0);
    }
    else
	if(ids.count() > parent.row())
	    index = ids[parent.row()].id;
    if (data->hasText())
		shared_data->addControl(index,data->text().toInt(),"", "0");
    return true;
}
/*!\func
 * for remove data by model
 * \param
 * - row - row of item
 * - parent - not used
 * \return no
 */
bool ControlModel::removeRow ( int row, const QModelIndex & parent)
{
    Q_UNUSED(parent);
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    bool result = false;
    if(shared_data)
    {
	if(parent.isValid())
	{
		if(ids.count() > parent.row())
		{
		    if(ids[parent.row()].relations.count() > row)
		    {
			shared_data->removeControl(ids[parent.row()].relations[row]);
			result = true;
		    }
		}
	}
    }
    return result;
}
