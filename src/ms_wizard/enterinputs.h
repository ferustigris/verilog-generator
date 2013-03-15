#ifndef ENTERINPUTS_H
#define ENTERINPUTS_H

#include <QtGui/QMainWindow>
#include <QMenu>
#include <QSharedPointer>
#include <QStringListModel>
#include "inputmodel.h"
#include "outputmodel.h"
#include "statemodel.h"
#include "inputdelegate.h"
#include "outputdelegate.h"
#include "statedelegate.h"
#include "controldelegate.h"
//#include "includestatemodel.h"
#include "controlstatemodel.h"
#include "generator.h"
#include "data.h"
#include "xmlproject.h"
#include <QSettings>

class WizardState
{
public:
    enum WizardStates
    {
	WIZARD_STATE_INPUTS = 0,
	WIZARD_STATE_GRAPH,
	WIZARD_STATE_OTHERS,
	WIZARD_STATE_GEN,
    };
    WizardState ();
    bool operator == (const WizardState&);
    WizardState& operator ++ (int);
    WizardState& operator -- (int);
    WizardState& operator = (const WizardStates newState);
private:
    int currentWizardState;//WizardStates
};

namespace Ui
{
    class EnterInputs;
};

class EnterInputs : public QMainWindow
{
    Q_OBJECT
public:
    EnterInputs(const QString&fn, QWidget *parent = 0);
    ~EnterInputs();
private:
    Ui::EnterInputs *ui;
    Data data;
    QSharedPointer<InputModel> modelInputs;
    QSharedPointer<OutputModel> modelOutputs;
    QSharedPointer<StateModel> modelStates;
    QSharedPointer<ControlModel> modelControl;
    StateDelegate spinDelegate;
    ControlDelegate controlDelegate;
    IDelegate iDelegate;
    ODelegate oDelegate;
    int currentWizardState;//WizardStates
    bool currentWizardStateChange;
    QSettings settings;
    QString project_file_name;
    QSharedPointer<XmlProject> project;
    //menu
    QMenu ppMenu;
    QMenu menuInputs;
    QMenu menuOutputs;
    QMenu menuStates;
    QMenu menuControls;
    QString fileOutput;
private:
    void loadProject(const QString&fileName);
    virtual void paintEvent ( QPaintEvent * event );
private slots:
    void on_leModule_textChanged(QString );
    void on_tbDescription_textChanged();
    void on_actionSave_as_triggered();
    void on_tvControls_pressed(QModelIndex index);
    void on_actionRemove_control_triggered();
    void on_tvOutputs_pressed(QModelIndex index);
    void on_tvInputs_pressed(QModelIndex index);
    void on_actionRemove_output_triggered();
    void on_actionRemove_input_triggered();
    void on_tvStates_pressed(QModelIndex index);
    void on_actionOpen_project_triggered();
    void on_actionSave_project_triggered();
    void on_pbFinish_clicked();
    void on_pbCancel_clicked();
    void on_actionDelete_state_triggered();
    void on_pbNext_clicked();
    void on_actionExit_triggered();
    void on_actionGeneration_triggered();
    void on_actionAddOutput_triggered();
    void on_actionAddInput_triggered();
    void on_actionAddState_triggered();
    void on_actionAddRelation_triggered();
    void onEditCommonData();
protected:
    virtual void closeEvent ( QCloseEvent * event );
};
#endif // ENTERINPUTS_H
