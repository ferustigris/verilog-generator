#ifndef DATA_H
#define DATA_H

#include <QString>
#include <QMap>
#include <QPointF>
#include "relation.h"
#include "state.h"
#include "input.h"
#include "control.h"
#include "output.h"

//! class, provided all data
/*! class, provided  all data
 * release pattern fasad
 */
class Data : public QObject
{
    Q_OBJECT
public:
    Data();
    ~Data();
    //common data
    bool setModuleDescription(const QString&value);
    bool setModuleName(const QString&value);
    QString getModuleDescription() const;
    QString getModuleName() const;
    //states
    State getState(const qint32 id) const;
    QList<State> getStates() const;
    qint32 addState(const QString&name);
    bool setStateName(const qint32 id, const QString&newValue);
    bool setStateDescription(const qint32 id, const QString&newValue);
    bool setStatePosition(const qint32 id, const QPoint&value);
    bool removeState(const qint32 id);
    //relations
    QList<Relation> getRelations(const qint32 ids = -1) const;
    Relation getRelation(const qint32 id) const;
    qint32 addRelation(const qint32, const qint32, const QString&condition);
    bool setRelationCondition(const qint32 id, const QString&newValue);
    bool setRelationDest(const qint32 id, const qint32 newState);
    bool removeRelation(const qint32 id);
    //inputs
    QList<Input> getInputs() const;
    Input getInput(const qint32 id) const;
    qint32 addInput(const QString&name, const qint32 size);
    bool setInputName(const qint32 id, const QString&newValue);
    bool setInputSize(const qint32 id, const qint32 newValue);
    bool removeInput(const qint32 id);
    //outputs
    QList<Output> getOutputs() const;
    Output getOutput(const qint32 id) const;
    qint32 addOutput(const QString&name, const qint32 size, const QString&type, const QString&value);
    bool setOutputName(const qint32 id, const QString&newValue);
    bool setOutputType(const qint32 id, const QString&newValue);
    bool setOutputValue(const qint32 id, const QString&newValue);
    bool setOutputSize(const qint32 id, const qint32 newValue);
    bool removeOutput(const qint32 id);
    //Control
    QList<Control> getControls(const qint32 state_id = -1) const;
    Control getControl(const qint32 id) const;
    qint32 addControl(const qint32 state, const qint32 output, const QString&condition, const QString&value);
    bool setControlValue(const qint32 id, const QString&newValue);
    bool setControlCondition(const qint32 id, const QString&newValue);
    bool removeControl(const qint32 id);
private:
    QMap< qint32, State> states;
    QMap< qint32, Control> controls;
    QMap< qint32, Input> inputs;
    QMap< qint32, Output> outputs;
    QMap< qint32, Relation> relations;
    QString description, name;
private:
    QString getUniqueStateName(const qint32 id, const QString&name) const;
    QString getUniqueInputName(const qint32 id, const QString&name) const;
    QString getUniqueOutputName(const qint32 id, const QString&name) const;
    QString getValidIdentifier(const QString&name) const;
    QString getValidType(const QString&name) const;
signals:
    void dataChange();
    void sAddInput(const qint32 id);
    void sEditInput(const qint32 id);
    void sRemoveInput(const qint32 id);
    void sAddState(const qint32 id);
    void sRemoveState(const qint32 id);
    void sEditState(const qint32 id);
    void sAddRelation(const qint32 id);
    void sEditRelation(const qint32 id);
    void sRemoveRelation(const qint32 id);
    void sAddOutput(const qint32 id);
    void sEditOutput(const qint32 id);
    void sRemoveOutput(const qint32 id);
    void sAddControl(const qint32 state_id, const qint32 id);
    void sEditControl(const qint32 id);
    void sRemoveControl(const qint32 id);
    void sEditCommonData();
};

#endif // DATA_H
