#include <QDebug>
#include "test_data.h"
#include "data.h"
#include "statemodel.h"
#include "inputmodel.h"
#include "outputmodel.h"
#include "controlstatemodel.h"
#include "statedelegate.h"
#include <QLineEdit>
#include <QComboBox>
#include <QTreeView>
#include <QSpinBox>
#include "inputdelegate.h"
#include "outputdelegate.h"
#include "controldelegate.h"
#include "xmlproject.h"
#include "testgenerator.h"

Test_Data::Test_Data(QObject *parent) :
    QObject(parent)
{
}
/*!\func
 * data for states test
 * \param no
 * \return no
 */
void Test_Data::states_data()
{
	QTest::addColumn<QString>("name");
	QTest::addColumn<QString>("desc");
	QTest::newRow("state test 1")<<"test1"<<"d1";
        QTest::newRow("state test 2")<<"a123"<<"d2";
	QTest::newRow("state test 3")<<"asd"<<"sdfgfdghdfsgjeiopr";
}
/*!\func
 * states test
 * \param no
 * \return no
 */
void Test_Data::states()
{
	Data data;
	QFETCH(QString, name);
	QFETCH(QString, desc);
	qint32 result = data.addState(name);
	QCOMPARE(data.getState(result).getName(), name);
	data.setStateDescription(result, desc);
	QCOMPARE(data.getState(result).getDescription(), desc);
	//QVERIFY()
}
/*!\func
 * data for edges test
 * \param no
 * \return no
 */
void Test_Data::edges_data()
{
	QTest::addColumn<QString>("name1");
	QTest::addColumn<QString>("name2");
	QTest::addColumn<QString>("condition1");
	QTest::addColumn<QString>("condition2");
	QTest::newRow("relation test 1")<<"test1"<<"test2"<<"test11"<<"test22";
	QTest::newRow("relation test 2")<<"test4"<<"test12"<<"test1"<<"test2";
}
/*!\func
 * edges(relation) test
 * \param no
 * \return no
 */
void Test_Data::edges()
{
	Data data;
	QFETCH(QString, name1);
	QFETCH(QString, name2);
	QFETCH(QString, condition1);
	QFETCH(QString, condition2);
	qint32 ids1 = data.addState(name1);
	qint32 ids2 = data.addState(name2);
	qint32 rel1 = data.addRelation(ids1, ids2, condition1);
	qint32 rel2 = data.addRelation(ids2, ids1, condition2);
	QCOMPARE(data.getState(ids1), data.getRelation(rel1).getSource());
	QCOMPARE(data.getState(ids2), data.getRelation(rel1).getDest());
	QCOMPARE(data.getState(ids2), data.getRelation(rel2).getSource());
	QCOMPARE(data.getState(ids1), data.getRelation(rel2).getDest());
	//QVERIFY()
}
/*!\func
 * data for test work modelstates with data
 * \param no
 * \return no
 */
void Test_Data::model_states_data()
{
    QTest::addColumn<QString>("name1");
    QTest::addColumn<QString>("name2");
    QTest::addColumn<QString>("condition1");
    QTest::addColumn<QString>("condition2");
    QTest::newRow("relation test 1")<<"test1"<<"test2"<<"condition1"<<"condition2";
    QTest::newRow("relation test 2")<<"test4"<<"test12"<<"condition3"<<"condition4";
}
/*!\func
 * test work modelstates with data
 * \param no
 * \return no
 */
void Test_Data::model_states()
{
	Data data;
	StateModel model(&data, QStringList()<<"STATE"<<"NEXT"<<"CONDITION");
	QFETCH(QString, name1);
	QFETCH(QString, name2);
	QFETCH(QString, condition1);
	QFETCH(QString, condition2);
	qint32 ids1 = data.addState(name1);
	qint32 ids2 = data.addState(name2);
	data.addRelation(ids1,ids2,condition1);

	{
	    QList<State> list = data.getStates();
	    QVERIFY(!list.isEmpty());
	    QString name = list.takeFirst().getName();
	    QCOMPARE(name, name1);
	}
	{
	    QList<Relation> list = data.getState(ids1).getRelations();
	    QVERIFY(!list.isEmpty());
	    QString name = list.takeFirst().getCondition();
	    QCOMPARE(name, condition1);
	}
	QModelIndex index = model.index(0,0);
	QCOMPARE(index.row(), 0);
	QCOMPARE(index.column(), 0);
	QString name = model.data(index, Qt::DisplayRole).toString();
	QCOMPARE(data.getState(ids1).getName(), name);

}
/*!\func
 * data for test work modelstates with data: edges&states
 * \param no
 * \return no
 */
void Test_Data::model_states_vs_edges_data()
{
	QTest::addColumn<QString>("name1");
	QTest::addColumn<QString>("name2");
	QTest::addColumn<QString>("condition1");
	QTest::addColumn<QString>("condition2");
	QTest::newRow("relation test 1")<<"test1"<<"condition1"<<""<<"condition2";
	QTest::newRow("relation test 2")<<"test4"<<"test12"<<"condition11"<<"condition12";
}
/*!\func
 * test work modelstates with data: edges&states
 * \param no
 * \return no
 */
void Test_Data::model_states_vs_edges()
{
	Data data;
	StateModel model(&data, QStringList()<<"STATE"<<"NEXT"<<"CONDITION");
	QFETCH(QString, name1);
	QFETCH(QString, name2);
	QFETCH(QString, condition1);
	QFETCH(QString, condition2);
	qint32 ids1 = data.addState(name1);
	qint32 ids2 = data.addState(name2);
	qint32 rel1 = data.addRelation(ids1, ids2, condition1);
	qint32 rel2 = data.addRelation(ids2, ids1, condition2);

	QModelIndex parent = model.index(0,0);
	QModelIndex index = model.index(0,1, parent);
	QCOMPARE(index.row(), 0);
	QCOMPARE(index.column(), 1);
	QString name = model.data(index, Qt::DisplayRole).toString();
	QCOMPARE(data.getRelation(rel1).getCondition(), name);

	parent = model.index(1,0);
	index = model.index(0,1, parent);
	QCOMPARE(index.row(), 0);
	QCOMPARE(index.column(), 1);

	name = model.data(index, Qt::DisplayRole).toString();
	QCOMPARE(data.getRelation(rel2).getCondition(), name);

	index = model.index(0,2, parent);
	QVERIFY(index.isValid());

	name = model.data(index, Qt::DisplayRole).toString();
	QCOMPARE(data.getRelation(rel2).getDest().getName(), name);


	QCOMPARE(parent.row(), model.parent(index).row());
	QCOMPARE(parent.column(), model.parent(index).column());
	QCOMPARE(parent.internalId(), model.parent(index).internalId());
	index = model.index(0,0,index);
	QVERIFY(!index.isValid());
}
/*!\func
 * data for test modelcontrols readonly
 * \param no
 * \return no
 */
void Test_Data::model_inputs_data()
{
	QTest::addColumn<QString>("input1");
	QTest::addColumn<QString>("input2");
	QTest::addColumn<qint32>("size1");
	QTest::addColumn<qint32>("size2");
	QTest::newRow("inputs test 1")<<"input1"<<"input2"<<1<<13;
	QTest::newRow("inputs test 2")<<"input111"<<"input22"<<13<<1;
}
/*!\func
 * test work controlmodel read only
 * \param no
 * \return no
 */
void Test_Data::model_inputs()
{
    QFETCH(QString, input1);
    QFETCH(QString, input2);
    QFETCH(qint32, size1);
    QFETCH(qint32, size2);
    Data data;

    qint32 inp1 = data.addInput(input1, size1);
    InputModel model(&data, QStringList()<<"INPUT"<<"SIZE"<<"TYPE");
    QCOMPARE(model.rowCount(QModelIndex()), 1);

    qint32 inp2 = data.addInput(input2, size2);
    //valid rowCount func
    QCOMPARE(model.rowCount(QModelIndex()), 2);
    QCOMPARE(model.columnCount(QModelIndex()), 3);
    //valid colCount func
    //valid index func, data
    QModelIndex index = model.index(0, 0);
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 0);
    QCOMPARE(index.column(), 0);
    QString name = model.data(index, Qt::DisplayRole).toString();
    QCOMPARE(data.getInput(inp1).getName(), name);
    //valid index func, data
    index = model.index(1, 1);
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 1);
    QCOMPARE(index.column(), 1);
    name = model.data(index, Qt::DisplayRole).toString();
    QCOMPARE(data.getInput(inp2).getSize(), name.toInt());
    /*valid index func
    QModelIndex parent = model.index(0,0);
    index = model.index(0,1, parent);
    QVERIFY(!index.isValid());
    //valid parent func
    index = model.index(0,15);
    name = model.data(index, Qt::DisplayRole).toString();
    QCOMPARE(QString(""), name);
    */
}
/*!\func
 * data for test outputmodel readonly
 * \param no
 * \return no
 */
void Test_Data::model_outputs_data()
{
	QTest::addColumn<QString>("output1");
	QTest::addColumn<QString>("output2");
	QTest::addColumn<qint32>("size1");
	QTest::addColumn<qint32>("size2");
	QTest::addColumn<QString>("init1");
	QTest::addColumn<QString>("init2");
	QTest::newRow("inputs test 1")<<"input1"<<"input2"<<1<<13<<"1"<<"2";
	QTest::newRow("inputs test 2")<<"input111"<<"input22"<<13<<1<<"4"<<"5";
}
/*!\func
 * test work outputmodel read only
 * \param no
 * \return no
 */
void Test_Data::model_outputs()
{
    QFETCH(QString, output1);
    QFETCH(QString, output2);
    QFETCH(qint32, size1);
    QFETCH(qint32, size2);
    QFETCH(QString, init1);
    QFETCH(QString, init2);
    Data data;

    qint32 inp1 = data.addOutput(output1, size1, "wire", init1);
    OutputModel model(&data, QStringList()<<"INPUT"<<"SIZE"<<"TYPE"<<"INIT");
    QCOMPARE(model.rowCount(QModelIndex()), 1);

    qint32 inp2 = data.addOutput(output2, size2, "wire", init2);
    //valid rowCount func
    QCOMPARE(model.rowCount(QModelIndex()), 2);
    QCOMPARE(model.columnCount(QModelIndex()), 4);
    //valid colCount func
    //valid index func, data
    QModelIndex index = model.index(0, 0);
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 0);
    QCOMPARE(index.column(), 0);
    QString name = model.data(index, Qt::DisplayRole).toString();
    QCOMPARE(data.getOutput(inp1).getName(), name);
    //valid index func, data
    index = model.index(1, 1);
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 1);
    QCOMPARE(index.column(), 1);
    name = model.data(index, Qt::DisplayRole).toString();
    QCOMPARE(data.getOutput(inp2).getSize(), name.toInt());
    //valid index func, data type
    index = model.index(1, 2);
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 1);
    QCOMPARE(index.column(), 2);
    name = model.data(index, Qt::DisplayRole).toString();
    QCOMPARE(data.getOutput(inp2).getType(), name);
    //valid index func, data init
    index = model.index(1, 3);
    QVERIFY(index.isValid());
    QCOMPARE(index.row(), 1);
    QCOMPARE(index.column(), 3);
    name = model.data(index, Qt::DisplayRole).toString();
    QCOMPARE(data.getOutput(inp2).getValue(), name);
    /*valid index func
    QModelIndex parent = model.index(0,0);
    index = model.index(0,1, parent);
    QVERIFY(!index.isValid());
    //valid parent func
    index = model.index(0,15);
    name = model.data(index, Qt::DisplayRole).toString();
    QCOMPARE(QString(""), name);
    */
}
/*!\func
 * data for test modelcontrols readonly
 * \param no
 * \return no
 */
void Test_Data::model_controls_data()
{
	QTest::addColumn<QString>("name1");
	QTest::addColumn<QString>("name2");
	QTest::addColumn<QString>("condition1");
	QTest::addColumn<QString>("condition2");
	QTest::addColumn<QString>("output1");
	QTest::addColumn<QString>("output2");
	QTest::addColumn<QString>("value1");
	QTest::addColumn<QString>("value2");
	QTest::newRow("control test 1")<<"test1"<<"test2"<<"condition1"<<"condition2"<<"out1"<<"out2"<<"1"<<"2";
	QTest::newRow("control test 2")<<"test12"<<"test3332"<<"1"<<"2"<<"1"<<"t2"<<"3321"<<"2222";
}/*!\func
 * test work controlmodel read only
 * \param no
 * \return no
 */
void Test_Data::model_controls()
{
    Data data;
    QFETCH(QString, name1);
    QFETCH(QString, name2);
    QFETCH(QString, condition1);
    QFETCH(QString, condition2);
    QFETCH(QString, output1);
    QFETCH(QString, output2);
    QFETCH(QString, value1);
    QFETCH(QString, value2);
    qint32 ids1 = data.addState(name1);
    ControlModel model(&data, QStringList()<<"STATE"<<"OUT"<<"CONDITION"<<"VALUE");
    QCOMPARE(model.columnCount(), 4);
    QCOMPARE(model.rowCount(), 1);
    qint32 ids2 = data.addState(name2);
    QCOMPARE(model.rowCount(), 2);
    qint32 inp1 = data.addOutput(output1, 1, "wire", value1);
    qint32 inp2 = data.addOutput(output2, 1, "wire", value2);
    //row cont & dinamic added
    QModelIndex parent = model.index(0,0);
    QCOMPARE(model.rowCount(parent), 0);
    qint32 ctr1 = data.addControl(ids1, inp1, value1, condition1);
    QCOMPARE(data.getControl(ctr1).getState().getId(), ids1);
    QCOMPARE(model.rowCount(parent), 1);
    data.addControl(ids2, inp2, value2, condition2);
    //data state
    QString name = model.data(parent, Qt::DisplayRole).toString();
    QCOMPARE(data.getState(ids1).getName(), name);
    //data cotrol
    QModelIndex index = model.index(0,1, parent);
    QVERIFY(index.isValid());
    name = model.data(index, Qt::DisplayRole).toString();
    QCOMPARE(data.getOutput(inp1).getName(), name);

    index = model.index(0,1, parent);
    QVERIFY(!model.rowCount(index) > 0);
    QVERIFY(index.isValid());
    name = model.data(index, Qt::DisplayRole).toString();
    QCOMPARE(data.getOutput(inp1).getName(), name);
    //index func
    index = model.index(0,0,index);
    QVERIFY(!index.isValid());
    //data: condition
    index = model.index(0,2, parent);
    QVERIFY(index.isValid());
    name = model.data(index, Qt::DisplayRole).toString();
    QCOMPARE(data.getControl(ctr1).getCondition(), name);
    //data: value
    index = model.index(0,3, parent);
    QVERIFY(index.isValid());
    name = model.data(index, Qt::DisplayRole).toString();
    QCOMPARE(data.getControl(ctr1).getValue(), name);
    //parent func
//    index = model.parent(index);
  //  QVERIFY(index.row(), parent.row());
}
/*!\func
 * data for test edit data in modelstates
 * \param no
 * \return no
 */
void Test_Data::editmodel_states_data()
{
    QTest::addColumn<QString>("name1");
    QTest::addColumn<QString>("name2");
    QTest::addColumn<QString>("name3");
    QTest::addColumn<QString>("condition1");
    QTest::addColumn<QString>("condition2");
    QTest::newRow("relation test 1")<<"test1"<<"test2"<<"newName1"<<"condition1"<<"condition2";
    QTest::newRow("relation test 2")<<"test4"<<"test12"<<"newName2"<<"condition3"<<"condition4";
}
/*!\func
 * test edit data in modelstates
 * \param no
 * \return no
 */
void Test_Data::editmodel_states()
{
	Data data;
	StateModel model(&data, QStringList()<<"STATE"<<"NEXT"<<"CONDITION");
	StateDelegate delegate(0);
	QFETCH(QString, name1);
	QFETCH(QString, name2);
	QFETCH(QString, name3);
	QFETCH(QString, condition1);
	QFETCH(QString, condition2);
	qint32 ids1 = data.addState(name1);
	qint32 ids2 = data.addState(name2);
	qint32 rel1 = data.addRelation(ids2, ids1, condition1);
	qint32 rel2 = data.addRelation(ids2, ids1, condition2);
	//edit state
	QModelIndex index = model.index(0,0);
	QVERIFY(index.isValid());
	//before edit
	QString name = model.data(index, Qt::DisplayRole).toString();
	QCOMPARE(data.getState(ids1).getName(), name);
	//edit
	QLineEdit*w = static_cast<QLineEdit*>(delegate.createEditor(0,QStyleOptionViewItem (),index ));
	delegate.setEditorData(w, index);
	QCOMPARE(name1, w->text());
	//edit finish
	w->setText(name3);
	delegate.setModelData(w, &model, index);
	QCOMPARE(data.getState(ids1).getName(), name3);
	//post edit
	name = model.data(index, Qt::DisplayRole).toString();
	QCOMPARE(data.getState(ids1).getName(), name);
//edit relation1
	QModelIndex parent = model.index(1,0);
	index = model.index(0,1, parent);
	QVERIFY(index.isValid());
	//before edit
	name = model.data(index, Qt::DisplayRole).toString();
	QCOMPARE(data.getRelation(rel1).getCondition(), name);
	//edit
	w = static_cast<QLineEdit*>(delegate.createEditor(0,QStyleOptionViewItem (),index ));
	delegate.setEditorData(w, index);
	QCOMPARE(condition1, w->text());
	//edit finish
	w->setText(condition2);
	delegate.setModelData(w, &model, index);
	QCOMPARE(data.getRelation(rel1).getCondition(), condition2);
	//post edit
	name = model.data(index, Qt::DisplayRole).toString();
	QCOMPARE(data.getRelation(rel1).getCondition(), name);
//edit relation2
	index = model.index(1,2, parent);
	QVERIFY(index.isValid());
	//before edit
	name = model.data(index, Qt::DisplayRole).toString();
	QCOMPARE(data.getRelation(rel2).getDest().getName(), name);
	//edit
	QComboBox* b = static_cast<QComboBox*>(delegate.createEditor(0,QStyleOptionViewItem (),index ));
	delegate.setEditorData(b, index);
	QCOMPARE(name3, b->currentText());
	QCOMPARE(b->count(), 2);
	QCOMPARE(b->itemText(0), name3);
	QCOMPARE(b->itemText(1), name2);
	QCOMPARE(b->count(), 2);
	//edit finish
	b->setCurrentIndex(1);
	delegate.setModelData(b, &model, index);
	//post edit
	name = model.data(index, Qt::DisplayRole).toString();
	QCOMPARE(data.getRelation(rel2).getDest().getName(), name);
	QCOMPARE(name2, name);
}
/*!\func
 * data for test edit data in modelInputs
 * \param no
 * \return no
 */
void Test_Data::editmodel_inputs_data()
{
    QTest::addColumn<QString>("name1");
    QTest::addColumn<QString>("name2");
    QTest::addColumn<QString>("name3");
    QTest::addColumn<qint32>("size1");
    QTest::addColumn<qint32>("size2");
    QTest::addColumn<qint32>("size3");
    QTest::newRow("inputs edit valid 1")<<"test1"<<"test2"<<"newName1"<<1<<2<<3;
    QTest::newRow("inputs edit valid 2")<<"test4"<<"test12"<<"newName2"<<3<<14<<13;
}
/*!\func
 * test edit data in modelstates
 * \param no
 * \return no
 */
void Test_Data::editmodel_inputs()
{
	Data data;
	InputModel model(&data, QStringList()<<"STATE"<<"NEXT"<<"CONDITION");
	IDelegate delegate(&data);
	QFETCH(QString, name1);
	QFETCH(QString, name2);
	QFETCH(QString, name3);
	QFETCH(qint32, size1);
	QFETCH(qint32, size2);
	QFETCH(qint32, size3);
	qint32 in1 = data.addInput(name1, size1);
	Q_UNUSED(in1);
	qint32 in2 = data.addInput(name2, size2);
//edit name
	QModelIndex index = model.index(1,0);
	QVERIFY(index.isValid());
	//before edit
	QString name = model.data(index, Qt::DisplayRole).toString();
	QCOMPARE(data.getInput(in2).getName(), name);
	//edit
	QLineEdit*w = static_cast<QLineEdit*>(delegate.createEditor(0,QStyleOptionViewItem (),index ));
	delegate.setEditorData(w, index);
	QCOMPARE(name2, w->text());
	//edit finish
	w->setText(name3);
	delegate.setModelData(w, &model, index);
	QCOMPARE(data.getInput(in2).getName(), name3);
	//post edit
	name = model.data(index, Qt::DisplayRole).toString();
	QCOMPARE(data.getInput(in2).getName(), name);
//edit size
	index = model.index(1,1);
	QVERIFY(index.isValid());
	//before edit
	qint32 size = model.data(index, Qt::DisplayRole).toInt();
	QCOMPARE(data.getInput(in2).getSize(), size);
	//edit
	QSpinBox*sb = static_cast<QSpinBox*>(delegate.createEditor(0,QStyleOptionViewItem (),index ));
	delegate.setEditorData(sb, index);
	QCOMPARE(size2, sb->value());
	//edit finish
	sb->setValue(size3);
	delegate.setModelData(sb, &model, index);
	QCOMPARE(data.getInput(in2).getSize(), size3);
	//post edit
	size = model.data(index, Qt::DisplayRole).toInt();
	QCOMPARE(data.getInput(in2).getSize(), size);
}
/*!\func
 * data for test edit data in modelInputs
 * \param no
 * \return no
 */
void Test_Data::editmodel_outputs_data()
{
    QTest::addColumn<QString>("name1");
    QTest::addColumn<QString>("name2");
    QTest::addColumn<QString>("name3");
    QTest::addColumn<qint32>("size1");
    QTest::addColumn<qint32>("size2");
    QTest::addColumn<qint32>("size3");
    QTest::addColumn<qint32>("tpIndex");
    QTest::addColumn<QString>("type1");
    QTest::addColumn<QString>("type2");
    QTest::addColumn<QString>("type3");
    QTest::addColumn<QString>("value1");
    QTest::addColumn<QString>("value2");
    QTest::addColumn<QString>("value3");
    QTest::newRow("inputs edit valid 1")<<"test1"<<"test2"<<"newName1"
	    <<1<<2<<3
            <<0<<"t1"<<"reg"<<"reg"
	    <<"1"<<"11"<<"111";
    QTest::newRow("inputs edit valid 2")<<"test4"<<"test12"<<"newName2"
	    <<3<<14<<13
            <<0<<"t5"<<"reg"<<"reg"
	    <<"33"<<"44"<<"55";
}
/*!\func
 * test edit data in modelstates
 * \param no
 * \return no
 */
void Test_Data::editmodel_outputs()
{
	Data data;
	OutputModel model(&data, QStringList()<<"STATE"<<"NEXT"<<"CONDITION"<<"");
	ODelegate delegate(&data);
	QFETCH(QString, name1);
	QFETCH(QString, name2);
	QFETCH(QString, name3);
	QFETCH(qint32, size1);
	QFETCH(qint32, size2);
	QFETCH(qint32, size3);
	QFETCH(QString, type1);
	QFETCH(QString, type2);
	QFETCH(QString, type3);
	QFETCH(qint32, tpIndex);
	QFETCH(QString, value1);
	QFETCH(QString, value2);
	QFETCH(QString, value3);
	qint32 in1 = data.addOutput(name1, size1, type1, value1);
	Q_UNUSED(in1);
	qint32 in2 = data.addOutput(name2, size2, type2, value2);
//edit name
	QModelIndex index = model.index(1,0);
	QVERIFY(index.isValid());
	//before edit
	QString name = model.data(index, Qt::DisplayRole).toString();
	QCOMPARE(data.getOutput(in2).getName(), name);
	//edit
	QLineEdit*w = static_cast<QLineEdit*>(delegate.createEditor(0,QStyleOptionViewItem (),index ));
	delegate.setEditorData(w, index);
	QCOMPARE(name2, w->text());
	//edit finish
	w->setText(name3);
	delegate.setModelData(w, &model, index);
	QCOMPARE(data.getOutput(in2).getName(), name3);
	//post edit
	name = model.data(index, Qt::DisplayRole).toString();
	QCOMPARE(data.getOutput(in2).getName(), name);
//edit size
	index = model.index(1,1);
	QVERIFY(index.isValid());
	//before edit
	qint32 size = model.data(index, Qt::DisplayRole).toInt();
	QCOMPARE(data.getOutput(in2).getSize(), size);
	//edit
	QSpinBox*sb = static_cast<QSpinBox*>(delegate.createEditor(0,QStyleOptionViewItem (),index ));
	delegate.setEditorData(sb, index);
	QCOMPARE(size2, sb->value());
	//edit finish
	sb->setValue(size3);
	delegate.setModelData(sb, &model, index);
	QCOMPARE(data.getOutput(in2).getSize(), size3);
	//post edit
	size = model.data(index, Qt::DisplayRole).toInt();
	QCOMPARE(data.getOutput(in2).getSize(), size);
//edit type
	index = model.index(1,2);
	QVERIFY(index.isValid());
	//before edit
	QString type = model.data(index, Qt::DisplayRole).toString();
	QCOMPARE(data.getOutput(in2).getType(), type);
	//edit
	QComboBox*cb = static_cast<QComboBox*>(delegate.createEditor(0,QStyleOptionViewItem (),index ));
	QVERIFY(cb);
	delegate.setEditorData(cb, index);
	QCOMPARE(type2, cb->currentText());
	//edit finish
	cb->setCurrentIndex(tpIndex);
	delegate.setModelData(cb, &model, index);
	QCOMPARE(data.getOutput(in2).getType(), type3);
	//post edit
	type = model.data(index, Qt::DisplayRole).toString();
	QCOMPARE(data.getOutput(in2).getType(), type);
//edit value
	index = model.index(1,3);
	QVERIFY(index.isValid());
	//before edit
	QString value = model.data(index, Qt::DisplayRole).toString();
	QCOMPARE(data.getOutput(in2).getValue(), value);
	//edit
	w = static_cast<QLineEdit*>(delegate.createEditor(0,QStyleOptionViewItem (),index ));
	QVERIFY(w);
	delegate.setEditorData(w, index);
	QCOMPARE(value2, w->text());
	//edit finish
	w->setText(value3);
	delegate.setModelData(w, &model, index);
	QCOMPARE(data.getOutput(in2).getValue(), value3);
	//post edit
	value = model.data(index, Qt::DisplayRole).toString();
	QCOMPARE(data.getOutput(in2).getValue(), value);
}
/*!\func
 * data for test edit data in modelInputs
 * \param no
 * \return no
 */
void Test_Data::dragndrop_outputs_data()
{
    QTest::addColumn<QString>("name1");
    QTest::addColumn<QString>("name2");
    QTest::addColumn<QString>("name3");
    QTest::addColumn<qint32>("size1");
    QTest::addColumn<qint32>("size2");
    QTest::addColumn<qint32>("size3");
    QTest::addColumn<QString>("type1");
    QTest::addColumn<QString>("type2");
    QTest::addColumn<QString>("type3");
    QTest::addColumn<QString>("value1");
    QTest::addColumn<QString>("value2");
    QTest::addColumn<QString>("value3");
    QTest::newRow("inputs edit valid 1")<<"test1"<<"test2"<<"newName1"
	    <<1<<2<<3
	    <<"t1"<<"reg"<<"wire"
	    <<"1"<<"11"<<"111";
    QTest::newRow("inputs edit valid 2")<<"test4"<<"test12"<<"newName2"
	    <<3<<14<<13
	    <<"t5"<<"wire"<<"reg"
	    <<"33"<<"44"<<"55";
}
/*!\func
 * test edit data in modelstates
 * \param no
 * \return no
 */
void Test_Data::dragndrop_outputs()
{
	Data data;
	OutputModel model(&data, QStringList()<<"STATE"<<"NEXT"<<"CONDITION"<<"");
	ControlModel control_model(&data, QStringList()<<"STATE"<<"NEXT"<<"CONDITION"<<"");
	ODelegate delegate(&data);
	QFETCH(QString, name1);
	QFETCH(QString, name2);
	QFETCH(QString, name3);
	QFETCH(qint32, size1);
	QFETCH(qint32, size2);
	QFETCH(qint32, size3);
	Q_UNUSED(size3);
	QFETCH(QString, type1);
	QFETCH(QString, type2);
	QFETCH(QString, type3);
	QFETCH(QString, value1);
	QFETCH(QString, value2);
	QFETCH(QString, value3);
	qint32 in1 = data.addOutput(name1, size1, type1, value1);
	qint32 in2 = data.addOutput(name2, size2, type2, value2);
	QModelIndex index = model.index(1,0);
	QVERIFY(index.isValid());
	QMimeData *d = model.mimeData(QModelIndexList()<<index);
	QVERIFY(d);
	QCOMPARE(in2, d->text().toInt());
	index = model.index(0,0);
	QVERIFY(index.isValid());
	d = model.mimeData(QModelIndexList()<<index);
	QVERIFY(d);
	QCOMPARE(in1, d->text().toInt());
	//drop
	data.addState(name1);
	data.addState(name2);
	index = control_model.index(1,0);
	QVERIFY(index.isValid());
	control_model.dropMimeData(d, Qt::CopyAction, 0,0,index);
	QCOMPARE(data.getControls().count(), 1);
}
/*!\func
 * data for test edit data in ControlInputs
 * \param no
 * \return no
 */
void Test_Data::editmodel_controls_data()
{
    QTest::addColumn<QString>("statename1");
    QTest::addColumn<QString>("statename2");
    QTest::addColumn<QString>("statename3");
    QTest::addColumn<QString>("name1");
    QTest::addColumn<QString>("name2");
    QTest::addColumn<QString>("name3");
    QTest::addColumn<qint32>("size1");
    QTest::addColumn<qint32>("size2");
    QTest::addColumn<qint32>("size3");
    QTest::addColumn<QString>("type1");
    QTest::addColumn<QString>("type2");
    QTest::addColumn<QString>("type3");
    QTest::addColumn<QString>("condition1");
    QTest::addColumn<QString>("condition2");
    QTest::addColumn<QString>("condition3");
    QTest::addColumn<QString>("value1");
    QTest::addColumn<QString>("value2");
    QTest::addColumn<QString>("value3");
    QTest::newRow("controls edit valid 1")
	    <<"state1"<<"state2"<<"state3"
	    <<"test1"<<"test2"<<"newName1"
	    <<1<<2<<3
	    <<"t1"<<"reg"<<"wire"
	    <<"c1"<<"c2"<<"c3"
	    <<"1"<<"11"<<"111";
    QTest::newRow("controls edit valid 2")
	    <<"state11"<<"state22"<<"state33"
	    <<"test4"<<"test12"<<"newName2"
	    <<3<<14<<13
	    <<"t5"<<"wire"<<"reg"
	    <<"true"<<"c2"<<"false"
	    <<"33"<<"44"<<"55";
}
/*!\func
 * test edit data in modelstates
 * \param no
 * \return no
 */
void Test_Data::editmodel_controls()
{
	Data data;
	ControlModel model(&data, QStringList()<<"STATE"<<"NEXT"<<"CONDITION"<<"");
	ControlDelegate delegate;
	QFETCH(QString, statename1);
	QFETCH(QString, statename2);
	QFETCH(QString, statename3);
	QFETCH(QString, name1);
	QFETCH(QString, name2);
	QFETCH(QString, name3);
	QFETCH(qint32, size1);
	QFETCH(qint32, size2);
	QFETCH(QString, type1);
	QFETCH(QString, type2);
	QFETCH(QString, type3);
	QFETCH(QString, condition1);
	QFETCH(QString, condition2);
	QFETCH(QString, condition3);
	QFETCH(QString, value1);
	QFETCH(QString, value2);
	QFETCH(QString, value3);
	qint32 st1 = data.addState(statename1);
	data.addState(statename2);
	qint32 in1 = data.addOutput(name1, size1, type1, value1);
	qint32 in2 = data.addOutput(name2, size2, type2, value2);
	qint32 c1 = data.addControl(st1,in1, condition1, value1);
	data.addControl(st1,in2, condition2, value2);
//edit condition
	QModelIndex parent = model.index(0,0);
	QVERIFY(parent.isValid());
	QModelIndex index = model.index(0,2, parent);
	QVERIFY(index.isValid());
	//before edit
	QString name = model.data(index, Qt::DisplayRole).toString();
	QCOMPARE(data.getControl(c1).getCondition(), name);
	//edit
	QLineEdit*w = static_cast<QLineEdit*>(delegate.createEditor(0,QStyleOptionViewItem (),index ));
	QVERIFY(w);
	delegate.setEditorData(w, index);
	QCOMPARE(condition1, w->text());
	//edit finish
	w->setText(condition3);
	delegate.setModelData(w, &model, index);
	QCOMPARE(data.getControl(c1).getCondition(), condition3);
	//post edit
	name = model.data(index, Qt::DisplayRole).toString();
	QCOMPARE(data.getControl(c1).getCondition(), name);
}
/*!\func
 * data for remove data
 * \param no
 * \return no
 */
void Test_Data::remove_data()
{
    QTest::addColumn<QString>("statename1");
    QTest::addColumn<QString>("statename2");
    QTest::addColumn<QString>("statename3");
    QTest::addColumn<QString>("statename4");
    QTest::addColumn<QString>("relationname1");
    QTest::addColumn<QString>("relationname2");
    QTest::addColumn<QString>("relationname3");
    QTest::addColumn<QString>("relationname4");
    QTest::addColumn<QString>("inputname1");
    QTest::addColumn<QString>("inputname2");
    QTest::addColumn<QString>("inputname3");
    QTest::addColumn<QString>("outputname1");
    QTest::addColumn<QString>("outputname2");
    QTest::addColumn<QString>("outputname3");
    QTest::newRow("controls edit valid 1")
	    <<"state1"<<"state2"<<"state3"<<"state4"
	    <<"rel1"<<"rel2"<<"rel3"<<"rel4"
	    <<"test1"<<"test2"<<"newName1"
	    <<"test1"<<"test2"<<"newName1";
    QTest::newRow("controls edit valid 2")
	    <<"state11"<<"state22"<<"state33"<<"state44"
	    <<"rel11"<<"rel22"<<"rel33"<<"rel44"
	    <<"test4"<<"test12"<<"newName2"
	    <<"test1"<<"test2"<<"newName1";
}
/*!\func
 * test remove data
 * \param no
 * \return no
 */
void Test_Data::remove()
{
	Data data;
	ControlModel cmodel(&data, QStringList()<<"STATE"<<"NEXT"<<"CONDITION"<<"");
	InputModel imodel(&data, QStringList()<<"STATE"<<"NEXT"<<"CONDITION"<<"");
	OutputModel omodel(&data, QStringList()<<"STATE"<<"NEXT"<<"CONDITION"<<"");
	StateModel smodel(&data, QStringList()<<"STATE"<<"NEXT"<<"CONDITION"<<"");
	QFETCH(QString, statename1);
	QFETCH(QString, statename2);
	QFETCH(QString, statename3);
	QFETCH(QString, statename4);
	QFETCH(QString, relationname1);
	QFETCH(QString, relationname2);
	QFETCH(QString, relationname3);
	QFETCH(QString, relationname4);
	QFETCH(QString, inputname1);
	QFETCH(QString, inputname2);
	QFETCH(QString, inputname3);
	QFETCH(QString, outputname1);
	QFETCH(QString, outputname2);
	QFETCH(QString, outputname3);
	qint32 st1 = data.addState(statename1);
	qint32 st2 = data.addState(statename2);
	qint32 st3 = data.addState(statename3);
	qint32 st4 = data.addState(statename4);
	data.addRelation(st1, st3, relationname1);
	data.addRelation(st2, st1, relationname2);
	qint32 rel3 = data.addRelation(st2, st3, relationname3);
	data.addRelation(st2, st4, relationname4);
	data.addInput(inputname1, 1);
	qint32 in2 = data.addInput(inputname2, 1);
	data.addInput(inputname3, 1);
	qint32 out1 = data.addOutput(outputname1, 1, "wire", "01");
	qint32 out2 = data.addOutput(outputname2, 1, "wire", "01");
	qint32 out3 = data.addOutput(outputname3, 1, "wire", "01");
	qint32 ctr1 = data.addControl(st1,out1,"cond1", "val1");
	qint32 ctr2 = data.addControl(st1,out2,"cond13", "val3");
	qint32 ctr3 = data.addControl(st3,out3,"cond13", "val3");
//edit condition
	data.removeState(st1);
	QCOMPARE(data.getStates().count(), 3);
	QCOMPARE(data.getControls().count(), 1);
	QCOMPARE(data.getRelations().count(), 2);
	data.removeOutput(out3);
	QCOMPARE(data.getOutputs().count(), 2);
	QCOMPARE(data.getControls().count(), 0);
	data.removeInput(in2);
	QCOMPARE(data.getInputs().count(), 2);
	ctr1 = data.addControl(st2,out1,"cond1", "val1");
	ctr2 = data.addControl(st2,out2,"cond13", "val3");
	ctr3 = data.addControl(st3,out1,"cond13", "val3");
	data.removeControl(ctr2);
	QCOMPARE(data.getControls().count(), 2);
	data.removeRelation(rel3);
	QCOMPARE(data.getRelations().count(), 1);
}
/*!\func
 * data for remove data
 * \param no
 * \return no
 */
void Test_Data::remove_in_models_data()
{
    QTest::addColumn<QString>("statename1");
    QTest::addColumn<QString>("statename2");
    QTest::addColumn<QString>("statename3");
    QTest::addColumn<QString>("statename4");
    QTest::addColumn<QString>("relationname1");
    QTest::addColumn<QString>("relationname2");
    QTest::addColumn<QString>("relationname3");
    QTest::addColumn<QString>("relationname4");
    QTest::addColumn<QString>("controlname1");
    QTest::addColumn<QString>("controlname2");
    QTest::addColumn<QString>("controlname3");
    QTest::addColumn<QString>("controlname4");
    QTest::addColumn<QString>("inputname1");
    QTest::addColumn<QString>("inputname2");
    QTest::addColumn<QString>("inputname3");
    QTest::addColumn<QString>("outputname1");
    QTest::addColumn<QString>("outputname2");
    QTest::addColumn<QString>("outputname3");
    QTest::newRow("remove data 1")
	    <<"state1"<<"state2"<<"state3"<<"state4"
	    <<"rel1"<<"rel2"<<"rel3"<<"rel4"
	    <<"relationname1"<<"relationname2"<<"relationname3"<<"relationname4"
	    <<"test1"<<"test2"<<"newName1"
	    <<"test1"<<"test2"<<"newName1";
    QTest::newRow("remove data 2")
	    <<"state11"<<"state22"<<"state33"<<"state44"
	    <<"rel11"<<"rel22"<<"rel33"<<"rel44"
	    <<"relationname12"<<"relationname23"<<"relationname34"<<"relationname45"
	    <<"test4"<<"test12"<<"newName2"
	    <<"test1"<<"test2"<<"newName1";
}
/*!\func
 * test remove data
 * \param no
 * \return no
 */
void Test_Data::remove_in_models()
{
	Data data;
	ControlModel cmodel(&data, QStringList()<<"STATE"<<"NEXT"<<"CONDITION"<<"");
	InputModel imodel(&data, QStringList()<<"STATE"<<"NEXT"<<"CONDITION"<<"");
	OutputModel omodel(&data, QStringList()<<"STATE"<<"NEXT"<<"CONDITION"<<"");
	StateModel smodel(&data, QStringList()<<"STATE"<<"NEXT"<<"CONDITION"<<"");
	QFETCH(QString, statename1);
	QFETCH(QString, statename2);
	QFETCH(QString, statename3);
	QFETCH(QString, statename4);
	QFETCH(QString, relationname1);
	QFETCH(QString, relationname2);
	QFETCH(QString, relationname3);
	QFETCH(QString, relationname4);
	QFETCH(QString, controlname1);
	QFETCH(QString, controlname2);
	QFETCH(QString, controlname3);
	QFETCH(QString, controlname4);
	QFETCH(QString, inputname1);
	QFETCH(QString, inputname2);
	QFETCH(QString, inputname3);
	QFETCH(QString, outputname1);
	QFETCH(QString, outputname2);
	QFETCH(QString, outputname3);
	qint32 st1 = data.addState(statename1);
	qint32 st2 = data.addState(statename2);
	qint32 st3 = data.addState(statename3);
	qint32 st4 = data.addState(statename4);
	qint32 rel1 = data.addRelation(st1, st3, relationname1);
	qint32 rel2 = data.addRelation(st2, st1, relationname2);
	qint32 rel3 = data.addRelation(st2, st3, relationname3);
	qint32 rel4 = data.addRelation(st2, st4, relationname4);
	data.addInput(inputname1, 1);
	data.addInput(inputname2, 1);
	data.addInput(inputname3, 1);
	qint32 out1 = data.addOutput(outputname1, 1, "wire", "01");
	qint32 out2 = data.addOutput(outputname2, 1, "wire", "01");
	qint32 out3 = data.addOutput(outputname3, 1, "wire", "01");
	data.addControl(st1,out1,controlname1, "val1");
	data.addControl(st1,out2,controlname2, "val3");
	qint32 ctr3 = data.addControl(st2,out3,controlname3, "val3");
	qint32 ctr4 = data.addControl(st2,out3,controlname4, "val3");
//remove state
	//state model
	data.removeState(st1);
	QModelIndex parent = smodel.index(0,0);
	QModelIndex index = smodel.index(0,1,parent);
	QVERIFY(parent.isValid());
	QString name = smodel.data(parent, Qt::DisplayRole).toString();
	QCOMPARE(data.getState(st2).getName(), name);
	name = smodel.data(index, Qt::DisplayRole).toString();
	QCOMPARE(data.getRelation(rel3).getCondition(), name);
	data.removeRelation(rel3);
	name = smodel.data(index, Qt::DisplayRole).toString();
	QCOMPARE(data.getRelation(rel4).getCondition(), name);
	QCOMPARE(smodel.rowCount(parent), 1);
	//control model
	parent = cmodel.index(0,0);
	index = cmodel.index(0,1,parent);
	QVERIFY(parent.isValid());
	name = cmodel.data(parent, Qt::DisplayRole).toString();
	QCOMPARE(data.getState(st2).getName(), name);
	name = cmodel.data(index, Qt::DisplayRole).toString();
	QCOMPARE(data.getControl(ctr3).getOutput().getName(), name);
	data.removeControl(ctr3);
	name = cmodel.data(index, Qt::DisplayRole).toString();
	QCOMPARE(data.getControl(ctr4).getOutput().getName(), name);
	QCOMPARE(cmodel.rowCount(parent), 1);
	//remove by model
	//inputs
	imodel.removeRow(0);
	QCOMPARE(data.getInputs().count(), 2);
	imodel.removeRow(10000);
	QCOMPARE(data.getInputs().count(), 2);
	imodel.removeRow(0);
	QCOMPARE(data.getInputs().count(), 1);
	//outputs
	omodel.removeRow(0);
	QCOMPARE(data.getOutputs().count(), 2);
	omodel.removeRow(10000);
	QCOMPARE(data.getOutputs().count(), 2);
	omodel.removeRow(0);
	QCOMPARE(data.getOutputs().count(), 1);
	omodel.removeRow(0);
	QCOMPARE(data.getOutputs().count(), 0);
	//states
	QCOMPARE(data.getStates().count(), 3);
	smodel.removeRow(0);
	QCOMPARE(data.getStates().count(), 2);
	smodel.removeRow(10000);
	QCOMPARE(data.getStates().count(), 2);
	smodel.removeRow(0);
	QCOMPARE(data.getStates().count(), 1);
	//relations
	parent = smodel.index(0,0);
	st1 = data.addState(statename1);
	st2 = data.addState(statename2);
	st3 = data.addState(statename3);
	rel1 = data.addRelation(st4, st1, relationname1);
	rel2 = data.addRelation(st4, st2, relationname2);
	rel3 = data.addRelation(st4, st3, relationname3);
	rel4 = data.addRelation(st4, st4, relationname4);
	QCOMPARE(data.getRelations(st4).count(), 4);
	smodel.removeRow(0, parent);
	QCOMPARE(data.getRelations(st4).count(), 3);
	//controls
	parent = cmodel.index(0,0);
	out1 = data.addOutput(outputname1, 1, "wire", "01");
	out2 = data.addOutput(outputname2, 1, "wire", "01");
	out3 = data.addOutput(outputname3, 1, "wire", "01");
	rel1 = data.addControl(st4, out1, controlname1, "");
	rel2 = data.addControl(st4, out2, controlname2, "");
	rel3 = data.addControl(st4, out3, controlname3, "");
	rel4 = data.addControl(st4, out1, controlname4, "");
	QCOMPARE(data.getControls(st4).count(), 4);
	cmodel.removeRow(0, parent);
	QCOMPARE(data.getControls(st4).count(), 3);
	cmodel.removeRow(10000, parent);
	QCOMPARE(data.getControls(st4).count(), 3);
	cmodel.removeRow(0, parent);
	QCOMPARE(data.getControls(st4).count(), 2);
}
/*!\func
 * data for remove data
 * \param no
 * \return no
 */
void Test_Data::xmlproject_data()
{
    QTest::addColumn<QString>("statename1");
    QTest::addColumn<QString>("statename2");
    QTest::addColumn<QString>("statename3");
    QTest::addColumn<QString>("statename4");
    QTest::addColumn<QString>("relationname1");
    QTest::addColumn<QString>("relationname2");
    QTest::addColumn<QString>("relationname3");
    QTest::addColumn<QString>("relationname4");
    QTest::addColumn<QString>("controlname1");
    QTest::addColumn<QString>("controlname2");
    QTest::addColumn<QString>("controlname3");
    QTest::addColumn<QString>("controlname4");
    QTest::addColumn<QString>("inputname1");
    QTest::addColumn<QString>("inputname2");
    QTest::addColumn<QString>("inputname3");
    QTest::addColumn<QString>("outputname1");
    QTest::addColumn<QString>("outputname2");
    QTest::addColumn<QString>("outputname3");
    QTest::newRow("remove data 1")
	    <<"state1"<<"state2"<<"state3"<<"state4"
	    <<"rel1"<<"rel2"<<"rel3"<<"rel4"
	    <<"relationname1"<<"relationname2"<<"relationname3"<<"relationname4"
	    <<"test1"<<"test2"<<"newName1"
	    <<"test1"<<"test2"<<"newName1";
    QTest::newRow("remove data 2")
	    <<"state11"<<"state22"<<"state33"<<"state44"
	    <<"rel11"<<"rel22"<<"rel33"<<"rel44"
	    <<"relationname12"<<"relationname23"<<"relationname34"<<"relationname45"
	    <<"test4"<<"test12"<<"newName2"
	    <<"test1"<<"test2"<<"newName1";
}
/*!\func
 * test remove data
 * \param no
 * \return no
 */
void Test_Data::xmlproject()
{
	Data data;
	ControlModel cmodel(&data, QStringList()<<"STATE"<<"NEXT"<<"CONDITION"<<"");
	InputModel imodel(&data, QStringList()<<"STATE"<<"NEXT"<<"CONDITION"<<"");
	OutputModel omodel(&data, QStringList()<<"STATE"<<"NEXT"<<"CONDITION"<<"");
	StateModel smodel(&data, QStringList()<<"STATE"<<"NEXT"<<"CONDITION"<<"");
	QFETCH(QString, statename1);
	QFETCH(QString, statename2);
	QFETCH(QString, statename3);
	QFETCH(QString, statename4);
	QFETCH(QString, relationname1);
	QFETCH(QString, relationname2);
	QFETCH(QString, relationname3);
	QFETCH(QString, relationname4);
	QFETCH(QString, controlname1);
	QFETCH(QString, controlname2);
	QFETCH(QString, controlname3);
	QFETCH(QString, controlname4);
	QFETCH(QString, inputname1);
	QFETCH(QString, inputname2);
	QFETCH(QString, inputname3);
	QFETCH(QString, outputname1);
	QFETCH(QString, outputname2);
	QFETCH(QString, outputname3);
	qint32 st1 = data.addState(statename1);
	qint32 st2 = data.addState(statename2);
	qint32 st3 = data.addState(statename3);
	qint32 st4 = data.addState(statename4);
        data.addRelation(st1, st3, relationname1);
        data.addRelation(st2, st1, relationname2);
        data.addRelation(st2, st3, relationname3);
        data.addRelation(st2, st4, relationname4);
	data.addInput(inputname1, 1);
	data.addInput(inputname2, 1);
	data.addInput(inputname3, 1);
	qint32 out1 = data.addOutput(outputname1, 1, "wire", "01");
	qint32 out2 = data.addOutput(outputname2, 1, "wire", "01");
	qint32 out3 = data.addOutput(outputname3, 1, "wire", "01");
	data.addControl(st1,out1,controlname1, "val1");
	data.addControl(st1,out2,controlname2, "val3");
        data.addControl(st2,out3,controlname3, "val3");
        data.addControl(st2,out3,controlname4, "val3");
	{
		XmlProject proj1(&data, "file010203");
		data.setStateDescription(st1, statename1);
		data.setStateDescription(st2, statename2);
		data.setStateDescription(st3, statename3);
		data.setStateDescription(st4, statename4);
		proj1.save();
	}
	Data data2;
	XmlProject proj2(&data2, "file010203");
	proj2.load();
	QCOMPARE(data.getStates().count(), data2.getStates().count());
	QCOMPARE(data.getStates()[0].getName(), data2.getStates()[0].getName());
	QCOMPARE(data.getStates()[0].getDescription(), data2.getStates()[0].getDescription());
	QCOMPARE(data.getStates()[0].getRelations().count(), data2.getStates()[0].getRelations().count());
	QCOMPARE(data.getStates()[0].getRelations()[0].getDest().getName(),
		 data2.getStates()[0].getRelations()[0].getDest().getName());
	QCOMPARE(data.getStates()[0].getRelations()[0].getSource().getName(),
		 data2.getStates()[0].getRelations()[0].getSource().getName());
	QCOMPARE(data.getInputs().count(), data2.getInputs().count());
	QCOMPARE(data.getOutputs().count(), data2.getOutputs().count());
	QCOMPARE(data.getOutputs()[0].getValue(), data2.getOutputs()[0].getValue());
	QCOMPARE(data.getOutputs()[0].getSize(), data2.getOutputs()[0].getSize());
	QCOMPARE(data.getControls().count(), data2.getControls().count());
	QCOMPARE(data.getControls()[0].getValue(), data2.getControls()[0].getValue());
	QCOMPARE(data.getControls()[1].getCondition(), data2.getControls()[1].getCondition());
	QCOMPARE(data.getControls()[0].getState().getName(), data2.getControls()[0].getState().getName());
	QCOMPARE(data.getControls()[2].getOutput().getName(), data2.getControls()[2].getOutput().getName());
}
/*!\func
 * data for states positions
 * \param no
 * \return no
 */
void Test_Data::states_position_data()
{
	QTest::addColumn<qint32>("x");
	QTest::addColumn<qint32>("y");
	QTest::newRow("state pos 1")<<1<<2;
	QTest::newRow("state pos 2")<<11<<12;
	QTest::newRow("state pos 3")<<31<<2;
	QTest::newRow("state pos 4")<<411<<222;
}
/*!\func
 * position states
 * \param no
 * \return no
 */
void Test_Data::states_position()
{
	QFETCH(qint32, x);
	QFETCH(qint32, y);
	Data data, data2;
	qint32 i = data.addState("asd");
	data.setStatePosition(i, QPoint(x,y));
	{
		XmlProject proj1(&data, "file010203");
		proj1.save();
	}
	XmlProject proj2(&data2, "file010203");
	proj2.load();
	QCOMPARE(data.getStates()[0].getPosition().x(), data2.getStates()[0].getPosition().x());
	//QCOMPARE(data.getStates()[0].getPosition(), data2.getStates()[0].getPosition());
}
/*!\func
 * data for states test
 * \param no
 * \return no
 */
void Test_Data::project_common_data()
{
	QTest::addColumn<QString>("name");
	QTest::addColumn<QString>("desc");
	QTest::newRow("state test 1")<<"test1"<<"d1";
        QTest::newRow("state test 2")<<"n123"<<"d2";
	QTest::newRow("state test 3")<<"asd"<<"sdfgfdghdfsgjeiopr";
}
/*!\func
 * states test
 * \param no
 * \return no
 */
void Test_Data::project_common()
{
	Data data, data2;
	QFETCH(QString, name);
	QFETCH(QString, desc);
	data.setModuleName(name);
	data.setModuleDescription(desc);
	QCOMPARE(data.getModuleName(), name);
	QCOMPARE(data.getModuleDescription(), desc);
	{
		XmlProject proj1(&data, "file010203");
		proj1.save();
	}
	XmlProject proj2(&data2, "file010203");
	proj2.load();
	QCOMPARE(data2.getModuleName(), name);
	QCOMPARE(data2.getModuleDescription(), desc);
}
/*!\func
 *
 * \param no
 * \return no
 */
void Test_Data::gen_verilog_data()
{
	QTest::addColumn<QString>("statename1");
	QTest::addColumn<QString>("statename2");
	QTest::addColumn<QString>("statename3");
	QTest::addColumn<QString>("statename4");
	QTest::addColumn<QString>("statedesc1");
	QTest::addColumn<QString>("statedesc2");
	QTest::addColumn<QString>("statedesc3");
	QTest::addColumn<QString>("statedesc4");
	QTest::addColumn<QString>("relationname1");
	QTest::addColumn<QString>("relationname2");
	QTest::addColumn<QString>("relationname3");
	QTest::addColumn<QString>("relationname4");
	QTest::addColumn<QString>("controlname1");
	QTest::addColumn<QString>("controlname2");
	QTest::addColumn<QString>("controlname3");
	QTest::addColumn<QString>("controlname4");
	QTest::addColumn<QString>("controlvalue1");
	QTest::addColumn<QString>("controlvalue2");
	QTest::addColumn<QString>("controlvalue3");
	QTest::addColumn<QString>("controlvalue4");
	QTest::addColumn<QString>("inputname1");
	QTest::addColumn<QString>("inputname2");
	QTest::addColumn<QString>("inputname3");
	QTest::addColumn<QString>("outputname1");
	QTest::addColumn<QString>("outputname2");
	QTest::addColumn<QString>("outputname3");
	QTest::addColumn<QString>("moduleName");
	QTest::addColumn<QString>("moduleDesc");
	QTest::addColumn<QString>("log");
	QTest::newRow("gen verilog 1")
		<<"state1"<<"state2"<<"state3"<<"state4"
		<<"state1"<<"state2"<<"state3"<<"state4"
		<<"rel1"<<"rel2"<<"rel3"<<"rel4"
		<<"relationname1"<<"relationname2"<<"relationname3"<<"relationname4"
		<<"1"<<"2"<<"3"<<"4"
		<<"test1"<<"test2"<<"newName1"
                <<"test11"<<"test22"<<"newName13"<<"mod_name2"<<"mod desc2"<<
(QString("Check...\n")
+"1Complite.\n"
+"Generate...\n"
+"1Complite.\n");
}
/*!\func
 * test generate verilog
 * \param no
 * \return no
 */
void Test_Data::gen_verilog()
{
	Data data;
	QFETCH(QString, statename1);
	QFETCH(QString, statename2);
	QFETCH(QString, statename3);
	QFETCH(QString, statename4);
	QFETCH(QString, statedesc1);
	QFETCH(QString, statedesc2);
	QFETCH(QString, statedesc3);
	QFETCH(QString, statedesc4);
	QFETCH(QString, relationname1);
	QFETCH(QString, relationname2);
	QFETCH(QString, relationname3);
	QFETCH(QString, relationname4);
	QFETCH(QString, controlname1);
	QFETCH(QString, controlname2);
	QFETCH(QString, controlname3);
	QFETCH(QString, controlname4);
	QFETCH(QString, controlvalue1);
	QFETCH(QString, controlvalue2);
	QFETCH(QString, controlvalue3);
	QFETCH(QString, controlvalue4);
	QFETCH(QString, inputname1);
	QFETCH(QString, inputname2);
	QFETCH(QString, inputname3);
	QFETCH(QString, outputname1);
	QFETCH(QString, outputname2);
	QFETCH(QString, outputname3);
	QFETCH(QString, moduleName);
	QFETCH(QString, moduleDesc);
	QFETCH(QString, log);
	data.setModuleName(moduleName);
	data.setModuleDescription(moduleDesc);
	qint32 st1 = data.addState(statename1);
	qint32 st2 = data.addState(statename2);
	qint32 st3 = data.addState(statename3);
	qint32 st4 = data.addState(statename4);
	data.setStateDescription(st1, statedesc1);
	data.setStateDescription(st2, statedesc2);
	data.setStateDescription(st3, statedesc3);
	data.setStateDescription(st4, statedesc4);

        data.addRelation(st1, st2, relationname1);
        data.addRelation(st2, st3, relationname2);
        data.addRelation(st3, st4, relationname3);
        data.addRelation(st4, st1, relationname4);
	data.addInput(inputname1, 1);
	data.addInput(inputname2, 3);
	data.addInput(inputname3, 1);
	qint32 out1 = data.addOutput(outputname1, 1, "wire", "01");
	qint32 out2 = data.addOutput(outputname2, 1, "reg", "01");
        data.addOutput(outputname3, 1, "wire", "01");
	data.addControl(st1,out1,controlname1, controlvalue1);
	data.addControl(st1,out2,controlname2, controlvalue2);
	data.addControl(st2,out2,controlname3, controlvalue3);
	TestGenerator gen(&data);
	gen.generate("qwe123asd");
	QFile f("qwe123asd.v");
	f.open(QIODevice::ReadOnly);
	QTextStream text(&f);
	QString c = text.readAll();
	QString code("");
	QFile fout("etalon.v");
	fout.open(QIODevice::ReadOnly);
	QTextStream out(&fout);
		code = out.readAll();
	//c.replace("\r","");
	QCOMPARE(c, code);
}
/*!\func
 *
 * \param no
 * \return no
 */
void Test_Data::gen_log_data()
{
	//QTest::addColumn<QString>("log");
	//QTest::newRow("gen verilog 1")
}
/*!\func
 * test generate verilog
 * \param no
 * \return no
 */
void Test_Data::gen_log()
{
	Data data1;
	TestGenerator gen(&data1);
	gen.check();
	QString log("Check...\n2FSM has't states!\n2Fail!\n");
	log = "Check...\n";
	log+="2FSM has't outputs!\n";
	log+="2FSM has't states!\n";
	log+="2Fail!\n";
	QCOMPARE(gen.getLog(), log);
	TestGenerator gen2(&data1);
	data1.addState("st1");
	gen2.check();
	log = "Check...\n";
	log+="2FSM has't outputs!\n";
	log+="2State <st1> not used!\n";
	log+="2State <st1> finish state?\n";
	log+="2Fail!\n";
	QCOMPARE(gen2.getLog(), log);
}
/*!\func
 *
 * \param no
 * \return no
 */
void Test_Data::unique_state_in_out_names_data()
{
        QTest::addColumn<QString>("name1");
        QTest::newRow("unique values 1")<<"name1";
}
/*!\func
 * test unique name of states, outputs and inputs
 * \param no
 * \return no
 */
void Test_Data::unique_state_in_out_names()
{
    QFETCH(QString, name1);
    Data data;
    qint32 st1 = data.addState(name1);
    qint32 st2 = data.addState(name1);
    QVERIFY(data.getState(st1).getName() != data.getState(st2).getName());
    data.setStateName(st2, name1);
    QVERIFY(data.getState(st1).getName() != data.getState(st2).getName());

    st1 = data.addInput(name1, 1);
    st2 = data.addInput(name1, 1);
    QVERIFY(data.getInput(st1).getName() != data.getInput(st2).getName());
    data.setInputName(st2, name1);
    QVERIFY(data.getInput(st1).getName() != data.getInput(st2).getName());

    st1 = data.addOutput(name1, 1, "","");
    st2 = data.addOutput(name1, 1,"","");
    QVERIFY(data.getOutput(st1).getName() != data.getOutput(st2).getName());
    data.setOutputName(st2, name1);
    QVERIFY(data.getOutput(st1).getName() != data.getOutput(st2).getName());

}
/*!\func
 *
 * \param no
 * \return no
 */
void Test_Data::invalid_values_data()
{
    QTest::addColumn<QString>("name_invalid");
    QTest::addColumn<QString>("name_valid");
    QTest::addColumn<QString>("type_invalid");
    QTest::addColumn<QString>("type_valid");
    QTest::newRow("invalid values: rus sign")
        <<"naÔe1"<<"na_e1"
		<<"wire1"<<"wire";
    QTest::newRow("invalid values: start with  digit")
        <<"1name1"<<"_1name1"
		<<"wire1"<<"wire";
}
/*!\func
 * test invalid values of state, input and output names
 * \param no
 * \return no
 */
void Test_Data::invalid_values()
{
    QFETCH(QString, name_invalid);
    QFETCH(QString, name_valid);
    QFETCH(QString, type_invalid);
    QFETCH(QString, type_valid);
    Data data;
    qint32 st1 = data.addState(name_invalid);
    QCOMPARE(data.getState(st1).getName(), name_valid);
    data.setStateName(st1, name_invalid);
    QCOMPARE(data.getState(st1).getName(), name_valid);

    st1 = data.addInput(name_invalid, 1);
    QCOMPARE(data.getInput(st1).getName(), name_valid);
    data.setInputName(st1, name_invalid);
    QCOMPARE(data.getInput(st1).getName(), name_valid);

    st1 = data.addOutput(name_invalid, 1,type_invalid,"");
    QCOMPARE(data.getOutput(st1).getName(), name_valid);
    data.setOutputName(st1, name_invalid);
    QCOMPARE(data.getOutput(st1).getName(), name_valid);

    QCOMPARE(data.getOutput(st1).getType(), type_valid);
    data.setOutputType(st1, type_invalid);
    QCOMPARE(data.getOutput(st1).getType(), type_valid);

    data.setModuleName(name_invalid);
    QCOMPARE(data.getModuleName(), name_valid);
}
QTEST_MAIN(Test_Data)
