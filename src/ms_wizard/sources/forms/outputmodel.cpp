#include "errors.h"
#include "outputmodel.h"
#include "QStringList"
#include "data.h"
#include <QApplication>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QMimeData>

/*!\func
 *
 * \param no
 * \return no
 */
OutputModel::OutputModel(Data*data, const QStringList&headers)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    this->headers = headers;
    this->shared_data = data;
    if(!shared_data)return;
    connect(shared_data, SIGNAL(sAddOutput(const qint32)), this, SLOT(onAddOutput(const qint32)));
    connect(shared_data, SIGNAL(sRemoveOutput(const qint32)), this, SLOT(onRemoveOutput(const qint32)));
    foreach(Output output, shared_data->getOutputs())
	ids.append(output.getId());
}
/*!\func
 * slot data changed
 * \param no
 * \return no
 */
void OutputModel::onAddOutput (const qint32 id )
{
    qint32 row = ids.count()?ids.count()-1:0;
    beginInsertRows(QModelIndex(),row,row);
    ids.append(id);
    endInsertRows();
}
/*!\func
 * slot data changed
 * \param no
 * \return no
 */
void OutputModel::onRemoveOutput (const qint32 id )
{
    qint32 row = 0;
    for(int i=0;i < ids.count(); i++)
	if(ids[i] == id)
	{
	    row = i;
	    break;
	}
    beginRemoveRows(QModelIndex(), row, row);
    ids.removeAt(row);
    endRemoveRows();
}
/*!\func
 *
 * \param no
 * \return no
 */
int OutputModel::rowCount ( const QModelIndex & ) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    return ids.count();
}
/*!\func
 *
 * \param no
 * \return no
 */
int OutputModel::columnCount ( const QModelIndex &) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    return headers.count();
}
/*!\func
 *
 * \param no
 * \return no
 */
QVariant OutputModel::data ( const QModelIndex &index, int role) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (!index.isValid())
	 return QVariant();
    if (role != Qt::DisplayRole && role != Qt::EditRole)
	 return QVariant();
    switch(index.column())
    {
	case 0:
	    if(index.row() < ids.count())
		return  shared_data->getOutput(ids[index.row()]).getName();
	    break;
	case 1:
	    if(index.row() < ids.count())
		return  shared_data->getOutput(ids[index.row()]).getSize();
	    break;
	case 2:
	    if(index.row() < ids.count())
		return  shared_data->getOutput(ids[index.row()]).getType();
	    break;
	case 3:
	    if(index.row() < ids.count())
		return  shared_data->getOutput(ids[index.row()]).getValue();
	    break;
    }
    return QVariant();
}
/*!\func
 *
 * \param no
 * \return no
 */
bool OutputModel::setData ( const QModelIndex & index, const QVariant & value, int role)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (role != Qt::EditRole)
	 return false;
    bool result = false;
    switch(index.column())
    {
	case 0:
	    if(shared_data)
		    if(ids.count() > index.row())
		    {
			shared_data->setOutputName(ids[index.row()], value.toString());
			result = true;
		    }
	    break;
	case 1:
	    if(shared_data)
		    if(ids.count() > index.row())
		    {
			shared_data->setOutputSize(ids[index.row()], value.toInt());
			result = true;
		    }
	    break;
	case 2:
	    if(shared_data)
		    if(ids.count() > index.row())
		    {
			shared_data->setOutputType(ids[index.row()], value.toString());
			result = true;
		    }
	    break;
	case 3:
	    if(shared_data)
		    if(ids.count() > index.row())
		    {
			shared_data->setOutputValue(ids[index.row()], value.toString());
			result = true;
		    }
	    break;
    }
    if (result)
    {
	    emit dataChanged(index, index);
    }
    return result;
}
/*!\func
 *
 * \param no
 * \return no
 */
Qt::ItemFlags OutputModel::flags(const QModelIndex &index) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
     if (!index.isValid())
	 return 0;

     if (index.isValid())
	 return Qt::ItemIsDragEnabled | Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
     else
	 return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
/*!\func
 *
 * \param no
 * \return no
 */
QVariant OutputModel::headerData ( int section, Qt::Orientation orientation, int role) const
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if (role != Qt::DisplayRole)
	 return QVariant();
    if (orientation == Qt::Vertical)
	 return QVariant();
    if(section < headers.count())
	return headers[section];
    return QVariant();
}
/*!\func
 *
 * \param no
 * \return no
 */
QMimeData * OutputModel::mimeData ( const QModelIndexList & indexes ) const
{
    QMimeData *mimeData = new QMimeData();
    foreach (QModelIndex index, indexes) {
	if (index.isValid()) {
	    mimeData->setText(QString::number(ids[index.row()]));
	}
    }
    return mimeData;
}
/*!\func
 * for remove data by model
 * \param
 * - row - row of item
 * - parent - not used
 * \return no
 */
bool OutputModel::removeRow ( int row, const QModelIndex & parent)
{
	Q_UNUSED(parent);
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    bool result = false;
    if(shared_data)
	    if(ids.count() > row)
	    {
		shared_data->removeOutput(ids[row]);
		result = true;
	    }
    return result;
}
