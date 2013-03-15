#include <QtGui>
#include "errors.h"
#include "statemodel.h"

/*!\func
 * struct for temp data
 * \param no
 * \return no
 */
Statestruct::Statestruct(const qint32 id)
{
    this->id = id;
}
/*!\func
 *
 * \param no
 * \return no
 */
StateModel::StateModel(Data*data_shared, const QStringList &headers, QObject *parent)
    : QAbstractItemModel(parent)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    this->headers = headers;
    shared_data = data_shared;
    if(shared_data)
    {
        foreach(State state, shared_data->getStates())
        {
            bool present = false;
            foreach(Statestruct st, ids)
                if(st.id == state.getId())
                    present = true;
            //if(present)continue;
            ids.append(state.getId());
            foreach(Relation rel, shared_data->getRelations(state.getId()))
                ids[ids.count()-1].relations.append(rel.getId());
        }
    }
    connect(shared_data, SIGNAL(sAddState(const qint32)), this, SLOT(onAddState(const qint32)));
    connect(shared_data, SIGNAL(sEditState(const qint32)), this, SLOT(onEditState(const qint32)));
    connect(shared_data, SIGNAL(sRemoveState(const qint32)), this, SLOT(onRemoveState(const qint32)));
    connect(shared_data, SIGNAL(sEditRelation(const qint32)), this, SLOT(onEditRelation(const qint32)));
    connect(shared_data, SIGNAL(sAddRelation(const qint32)), this, SLOT(onAddRelation(const qint32)));
    connect(shared_data, SIGNAL(sRemoveRelation(const qint32)), this, SLOT(onRemoveRelation(const qint32)));
}
/*!\func
 * slot data added
 * \param no
 * \return no
 */
void StateModel::onAddState (const qint32 id )
{
    qint32 row = 0;
    if(ids.count() > 0)row = ids.count()-1;
    if(row < 0)return;
    bool present = false;
    foreach(Statestruct st, ids)
        if(st.id == id)
            present = true;
    if(present)return;
    beginInsertRows(QModelIndex(),row,row);
    ids.append(Statestruct(id));
    endInsertRows();
}
/*!\func
 * slot data changed
 * \param no
 * \return no
 */
void StateModel::onEditState (const qint32 id )
{
    qint32 row = 0;
    for(int i=0;i < ids.count(); i++)
        if(ids[i].id == id)
        {
            row = i;
            break;
        }
    QModelIndex ind = index(row, 0);
    emit dataChanged(ind, ind);
}
/*!\func
 * slot data remove
 * \param no
 * \return no
 */
void StateModel::onRemoveState (const qint32 id )
{
    qint32 row = -1;
    for(int i=0;i < ids.count(); i++)
        if(ids[i].id == id)
        {
            row = i;
            break;
        }
    if(row >= 0)
    {
        beginRemoveRows(QModelIndex(), row, row);
        ids.removeAt(row);
        endRemoveRows();
    }
}
/*!\func
 * slot data remove
 * \param no
 * \return no
 */
void StateModel::onRemoveRelation(const qint32 id )
{
    qint32 row1 = -1, row2 = -1;
    for(int i=0; i < ids.count(); i++)
        for(int k=0; k < ids[i].relations.count(); k++)
            if(ids[i].relations[k] == id)
            {
                row1 = i;
                row2 = k;
                break;
            }
    if((row1 < 0)||(row2 < 0))return;
    QModelIndex parent = index(row1, 0);
    beginRemoveRows(parent, row2, row2);
    ids[row1].relations.removeAt(row2);
    endRemoveRows();
}
/*!\func
 * slot data changed
 * \param no
 * \return no
 */
void StateModel::onEditRelation (const qint32 id )
{
    qint32 row1 = -1, row2 = -1;
    qint32 state_id = shared_data->getRelation(id).getSource().getId();
    for(int i=0; i < ids.count(); i++)
        if(ids[i].id == state_id)
        {
            row1 = i;
            break;
        }
    if(row1 < 0)return;
    for(int i=0; i < ids[row1].relations.count(); i++)
        if(ids[row1].relations[i] == id)
        {
            row2 = i;
            break;
        }
    if(row2 < 0)return;
    QModelIndex parent = index(row1, 0);
    QModelIndex ind1 = index(row2, 1, parent);
    QModelIndex ind2 = index(row2, 2, parent);
    emit dataChanged(ind1, ind2);
}
/*!\func
 * slot data changed
 * \param no
 * \return no
 */
void StateModel::onAddRelation (const qint32 id )
{
    qint32 row1 = -1, row2 = -1;
    qint32 state_id = shared_data->getRelation(id).getSource().getId();
    for(int i=0; i < ids.count(); i++)
        if(ids[i].id == state_id)
        {
            row1 = i;
            break;
        }
    if(row1 < 0)return;
    QModelIndex parent = index(row1, 0);
    ids[row1].relations.append(id);
    for(int i=0; i < ids[row1].relations.count(); i++)
        if(ids[row1].relations[i] == id)
        {
            row2 = i;
            break;
        }
    if(row2 < 0)return;
    beginInsertRows(parent, row2, row2);
    endInsertRows();
}
/*!\func
 *
 * \param no
 * \return no
 */
StateModel::~StateModel()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
}
/*!\func
 *
 * \param no
 * \return no
 */
int StateModel::columnCount(const QModelIndex & ) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    //return rootItem->columnCount();
    return headers.count();
}
/*!\func
 *
 * \param no
 * \return no
 */
QVariant StateModel::data(const QModelIndex &index, int role) const
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
                        return shared_data->getRelation(ids[index.internalId()-1].relations[index.row()]).getCondition();
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
                        return shared_data->getRelation(ids[index.internalId()-1].relations[index.row()]).getDest().getName();
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
Qt::ItemFlags StateModel::flags(const QModelIndex &index) const
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
QVariant StateModel::headerData(int section, Qt::Orientation orientation,
			       int role) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	return headers[section];
    return QVariant();
}
/*!\func
 *
 * \param no
 * \return no
 */
QModelIndex StateModel::index(int row, int column, const QModelIndex &parent) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if(parent.isValid()&&!parent.parent().isValid())
    {
        if(ids.count() > parent.row())
	{
            if(ids[parent.row()].relations.count() > row)
		return createIndex(row, column, parent.row() + 1);
	}
    }
    else if(!parent.isValid())
    {
        if(ids.count() > row)
	{
	    return createIndex(row, column, 0);
	}
    }
    return QModelIndex();
}
/*!\func
 *
 * \param no
 * \return no
 */
QModelIndex StateModel::parent(const QModelIndex &index) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (!index.isValid())
	return QModelIndex();
    if(index.internalId())
	return createIndex(index.internalId()-1,0,0);
    return QModelIndex();
}
/*!\func
 * row count
 * \param no
 * \return no
 */
int StateModel::rowCount(const QModelIndex &parent) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    //TreeItem *parentItem = getItem(parent);
    if(!parent.isValid())
	return ids.count();//parentItem->childCount();
    if(!parent.internalId())
    {
	if(ids.count() > parent.row())
            return ids[parent.row()].relations.count();
    }
    return 0;
}
/*!\func
 *
 * \param no
 * \return no
 */
bool StateModel::setData(const QModelIndex &index, const QVariant &value,
			int role)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (role != Qt::EditRole)
	return false;
    //TreeItem *item = getItem(index);
    bool result = false;//item->setData(index.column(), value)
    if(!index.isValid())return false;
    if(!index.internalId())
    {
        shared_data->setStateName(ids[index.row()].id, value.toString());
        result = true;
    }
    else
    {
        if(ids.count() > index.internalId()-1)
        {
            if(index.column() == 1)
            {
                shared_data->setRelationCondition(ids[index.internalId()-1].relations[index.row()], value.toString());
            } else
                shared_data->setRelationDest(ids[index.internalId()-1].relations[index.row()], ids[value.toInt()].id);
            result = true;
        }
    }
    if (result)
	emit dataChanged(index, index);

    return result;
}
/*!\func
 * for remove data by model
 * \param
 * - row - row of item
 * - parent - not used
 * \return no
 */
bool StateModel::removeRow ( int row, const QModelIndex & parent)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    bool result = false;
    if(shared_data)
    {
        if(!parent.isValid())
        {
            if(ids.count() > row)
            {
                shared_data->removeState(ids[row].id);
                result = true;
            }
        } else {
                if(ids.count() > parent.row())
                {
                    if(ids[parent.row()].relations.count() > row)
                    {
                        shared_data->removeRelation(ids[parent.row()].relations[row]);
                        result = true;
                    }
                }
        }
    }
    return result;
}
/*!\func
 * get id state by index
 * \param
 * - index - index in state model
 * \return no
 */
qint32 StateModel::getStateIdByIndex(const QModelIndex & index)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if(!shared_data)return -1;
    if(!index.isValid())return -1;
    QModelIndex parent = index.parent();
    if(!parent.isValid())
    {
        if(ids.count() > index.row())
            return ids[index.row()].id;
    } else {
            if(ids.count() > parent.row())
                    return ids[parent.row()].id;
    }
    return -1;
}
