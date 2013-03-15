#include "enterinputs.h"
#include "errors.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTimer>
#include <QMouseEvent>
#include <QFileDialog>
#include "ui_enterinputs.h"

extern QString program_dir;

/*!\func
 * конструктор
 * \param no
 * \return no
 */
EnterInputs::EnterInputs(const QString&fn, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::EnterInputs),
    data(),
    modelInputs(0),
    modelOutputs(0),
    modelStates(0),
    modelControl(0),
    settings(program_dir + "/../etc/config_fsm.ini", QSettings::IniFormat, parent),
    project(0)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    ui->setupUi(this);
    //load params
    fileOutput = fn;
    settings.beginGroup("projects");
    if(fn.isEmpty())
	    project_file_name = settings.value("project").toString();
    else
	    project_file_name = fn;
    settings.endGroup();
    settings.beginGroup("window");
    setGeometry(settings.value("x").toInt(),settings.value("y").toInt(),
		settings.value("width").toInt(),settings.value("height").toInt()	);
    settings.endGroup();
    loadProject(project_file_name);
	if(data.getInputs().isEmpty())
	{
		data.addInput("CLK", 1);
		data.addInput("RST", 1);
	}
    //link models
    GraphBody *gr = static_cast<GraphBody*>(ui->wGraph);
    gr->setData(&data);
    /**********************
    qint32 i = data.addState("1");
    qint32 b = data.addState("2");
    qint32 i2 = data.addState("3");
    qint32 b2 = data.addState("4");
    data.addRelation(i, b, "1-2");
    data.addRelation(i, b2, "1-4");
    data.addRelation(i, i2, "1-3");
    data.addRelation(i2, i, "3-1");
    data.addInput("input1", 1);
    data.addInput("input2", 2);
    data.addInput("input3", 3);
    qint32 out1 = data.addOutput("out1", 1, "wire", "01");
    data.addOutput("out2", 2, "wire", "02");
    qint32 out3 = data.addOutput("out3", 3, "reg", "03");
    data.addOutput("out4", 4, "wire", "04");
    data.addControl(i,out1,"cond1", "val1");
    data.addControl(i,out3,"cond13", "val3");
    data.addControl(i2,out3,"cond13", "val3");
    **********************/
    modelStates = QSharedPointer<StateModel>(new StateModel(&data, QStringList()<<tr("State")<<tr("Condition")<<tr("Next state")));
    ui->tvStates->setModel(modelStates.data());
    modelControl = QSharedPointer<ControlModel>(new ControlModel(&data, QStringList()<<tr("State")<<tr("Output")<<tr("Condition")<<tr("Value")));
    ui->tvControls->setModel(modelControl.data());
    modelInputs = QSharedPointer<InputModel>(new InputModel(&data, QStringList()<<tr("Input")<<tr("Size")<<tr("Type")));
    ui->tvInputs->setModel(modelInputs.data());
    modelOutputs = QSharedPointer<OutputModel>(new OutputModel(&data, QStringList()<<tr("Input")<<tr("Size")<<tr("Type")<<tr("Value")));
    ui->tvOutputs->setModel(modelOutputs.data());
    ui->tvStates->setItemDelegate(&spinDelegate);
    ui->tvInputs->setItemDelegate(&iDelegate);
    ui->tvOutputs->setItemDelegate(&oDelegate);
    ui->tvControls->setItemDelegate(&controlDelegate);
    //menues
    menuStates.addAction(ui->actionAddState);
    menuStates.addAction(ui->actionAddRelation);
    menuStates.addAction(ui->actionDelete_state);
    menuInputs.addAction(ui->actionAddInput);
    menuInputs.addAction(ui->actionRemove_input);
    menuOutputs.addAction(ui->actionAddOutput);
    menuOutputs.addAction(ui->actionRemove_output);
    menuControls.addAction(ui->actionRemove_control);
    currentWizardState = WizardState::WIZARD_STATE_INPUTS;
    currentWizardStateChange = true;
    //set geometry
    ui->tvOutputs->setColumnWidth(0, 70);
    ui->tvOutputs->setColumnWidth(1, 70);
    ui->tvOutputs->setColumnWidth(2, 40);
    ui->tvOutputs->setColumnWidth(3, 170);
    ui->tvInputs->setColumnWidth(0, 70);
    ui->tvInputs->setColumnWidth(1, 70);
    ui->tvInputs->setColumnWidth(2, 40);
    ui->tvStates->setColumnWidth(0, 80);
    ui->tvStates->setColumnWidth(1, 80);
    ui->tvStates->setColumnWidth(2, 80);
    ui->tvControls->setColumnWidth(0, 80);
    ui->tvControls->setColumnWidth(1, 150);
    ui->tvControls->setColumnWidth(2, 80);
    //connect(&data, SIGNAL(sEditCommonData()), this, SLOT(onEditCommonData()));
}
/*!\func
 *
 * \param no
 * \return no
 */
EnterInputs::~EnterInputs()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    delete ui;
}
/*!\func
 *
 * \param no
 * \return no
 */
void EnterInputs::onEditCommonData()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if(ui->leModule->text() != data.getModuleName())
	    ui->leModule->setText(data.getModuleName());
    if(ui->tbDescription->toPlainText() != data.getModuleDescription())
	    ui->tbDescription->setPlainText(data.getModuleDescription());
}
/*!\func
 *
 * \param no
 * \return no
 */
void EnterInputs::on_actionAddInput_triggered()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    static qint32 id = 0;
    if(!id)
    {
        foreach(Input item, data.getInputs())
            if(id < item.getId())
                id = item.getId();
    }
    id = data.addInput("INPUT" + QString::number(id), 1)+1;
}
/*!\func
 *
 * \param no
 * \return no
 */
void EnterInputs::on_actionAddOutput_triggered()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    static qint32 id = 0;
    if(!id)
    {
        foreach(Output item, data.getOutputs())
            if(id < item.getId())
                id = item.getId();
    }
    id = data.addOutput("OUTPUT" + QString::number(id),1, "reg", "0")+1;
}
/*!\func
 * нажатие на кнопку добавить вершину
 * \param no
 * \return no
 */
void EnterInputs::on_actionAddState_triggered()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    static qint32 id = 0;
    if(!id)
    {
        foreach(State item, data.getStates())
            if(id < item.getId())
                id = item.getId();
    }
    id = data.addState("STATE" + QString::number(id))+1;
}
/*!\func
 * генераци€ кода
 * \param no
 * \return no
 */
void EnterInputs::on_actionGeneration_triggered()
{
	LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");

	if(fileOutput.isEmpty())
	{
		static Generator generator(&data);
		generator.setModal(true);
		generator.show();
		generator.generate();
	}
	else
	{
		on_actionSave_project_triggered();
	}
}
/*!\func
 * выход
 * \param no
 * \return no
 */
void EnterInputs::on_actionExit_triggered()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    close();
}
/*!\func
 * готово, перейти к графу
 * \param no
 * \return no
 */
void EnterInputs::on_pbFinish_clicked()
{
	close ();
}
/*!\func
 * перейти к след. состо€нию
 * \param no
 * \return no
 */
void EnterInputs::on_pbNext_clicked()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    switch(currentWizardState)
    {
	case WizardState::WIZARD_STATE_INPUTS:
		currentWizardState++;
		break;
	case WizardState::WIZARD_STATE_GRAPH:
		currentWizardState++;
		break;
    case WizardState::WIZARD_STATE_OTHERS:
		on_actionGeneration_triggered();
		currentWizardState++;
		break;
    case WizardState::WIZARD_STATE_GEN:
		on_actionGeneration_triggered();
		break;
	default:
	    break;
    }
    currentWizardStateChange = true;
    repaint();
    return;
}
/*!\func
 * обработчик нажати€ на кнопку "добавить св€зь"
 * \param no
 * \return no
 */
void EnterInputs::on_actionAddRelation_triggered()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    static qint32 id = 0;
    QModelIndex index = ui->tvStates->selectionModel()->currentIndex();
    if(!index.isValid())return;
    qint32 state = modelStates->getStateIdByIndex(index);
    id = data.addRelation(state,state,"1");
}
/*!\func
 *
 * \param no
 * \return no
 */
void EnterInputs::on_actionDelete_state_triggered()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    QModelIndex index = ui->tvStates->selectionModel()->currentIndex();
    if(!index.isValid())return;
    modelStates->removeRow(index.row(), index.parent());
}
/*!\func
 *
 * \param no
 * \return no
 */
void EnterInputs::on_pbCancel_clicked()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    switch(currentWizardState)
    {
	case WizardState::WIZARD_STATE_INPUTS:
	    break;
	case WizardState::WIZARD_STATE_GRAPH:
	    currentWizardState--;
	    break;
	case WizardState::WIZARD_STATE_OTHERS:
	    currentWizardState--;
	    break;
	case WizardState::WIZARD_STATE_GEN:
	    currentWizardState--;
	    currentWizardState--;
		break;
	default:
	    break;
    }
    currentWizardStateChange = true;
    repaint();
}
/*!\func
 * анализ генерируемого кода до генерации
 * \param no
 * \return no
 */
void EnterInputs::paintEvent ( QPaintEvent *)
{
    if(!currentWizardStateChange)return;
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    ui->sSplitAll->hide();
    ui->gbOther->hide();
    ui->wGraph->hide();
    ui->pbFinish->setVisible(false);
    ui->pbNext->setVisible(true);
    ui->wButtons->show();
    ui->pbNext->setEnabled(true);
    ui->pbCancel->setEnabled(true);
    ui->actionAddInput->setEnabled(false);
    ui->actionAddOutput->setEnabled(false);
    ui->actionAddRelation->setEnabled(false);
    ui->actionAddState->setEnabled(false);
    ui->actionDelete_state->setEnabled(false);
    switch(currentWizardState)
    {
	case WizardState::WIZARD_STATE_INPUTS:
	    ui->sSplitAll->show();
	    ui->actionAddInput->setEnabled(true);
	    ui->actionAddOutput->setEnabled(true);
	    ui->pbCancel->setEnabled(false);
	    ui->actionAddRelation->setEnabled(true);
	    ui->actionAddState->setEnabled(true);
	    ui->actionDelete_state->setEnabled(true);
	    break;
	case WizardState::WIZARD_STATE_GRAPH:
	    ui->wGraph->show();
	    ui->actionAddState->setEnabled(true);
	    break;
	case WizardState::WIZARD_STATE_GEN:
	    ui->pbNext->setVisible(false);
	    ui->pbFinish->setVisible(true);
	case WizardState::WIZARD_STATE_OTHERS:
	    ui->gbOther->show();
	    break;
	default:
	    break;
    }
    currentWizardStateChange = false;
}
/*!\func
 * конструктор
 * \param no
 * \return no
 */
WizardState::WizardState ()
{
    currentWizardState = 0;
}
/*!\func
 * оператор сравнени€
 * \param no
 * \return no
 */
bool WizardState::operator == (const WizardState& ws)
{
    return currentWizardState == ws.currentWizardState;
}
/*!\func
 * оператор инкремента
 * \param no
 * \return no
 */
WizardState& WizardState::operator ++ (int)
{
    currentWizardState++;
    return *this;
}
/*!\func
 * оператор декремента
 * \param no
 * \return no
 */
WizardState& WizardState::operator -- (int)
{
    currentWizardState--;
    return *this;
}
/*!\func
 * установить состо€ние
 * \param no
 * \return no
 */
WizardState& WizardState::operator =(const WizardStates newState)
{
    currentWizardState = newState;
    return *this;
}
/*!\func
 * save project
 * \param no
 * \return no
 */
void EnterInputs::on_actionSave_project_triggered()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if(project_file_name.isEmpty())
	project_file_name = QFileDialog::getSaveFileName(this, tr("Save project..."), QString(), tr("Projects (*.xml);;"));
    if(project_file_name.isEmpty())
	return;
    if(project.isNull())
	project = QSharedPointer<XmlProject>(new XmlProject(&data, project_file_name));
    project->save();
}
//! open project
void EnterInputs::on_actionOpen_project_triggered()
{
    if(project_file_name.isEmpty())
	project_file_name = QFileDialog::getOpenFileName(this, tr("Open project..."), QString(), tr("Verilog (*.xml);;Projects"));
    if(project_file_name.isEmpty())
	return;
    loadProject(project_file_name);
}
//! событие закрыти формы
void EnterInputs::closeEvent ( QCloseEvent * event )
{
    Q_UNUSED(event)
    if(!project_file_name.isEmpty())
    {
	settings.beginGroup("projects");
	if(fileOutput.isEmpty())
		settings.setValue("project", project_file_name);
	settings.endGroup();
    }
    settings.beginGroup("window");
    settings.setValue("x", geometry().x());
    settings.setValue("y", geometry().y());
    settings.setValue("width", geometry().width());
    settings.setValue("height", geometry().height());
    settings.endGroup();
}
/*!\func
 * clicked by states list
 * \param
 * - index - index, by who clicked
 * \return no
 */
void EnterInputs::on_tvStates_pressed(QModelIndex index)
{
    Q_UNUSED(index);
    QCursor cursor;
    if(qApp->mouseButtons() & Qt::RightButton)
	menuStates.exec(cursor.pos());
}
/*!\func
 * remove input
 * \param no
 * \return no
 */
void EnterInputs::on_actionRemove_input_triggered()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    QModelIndex index = ui->tvInputs->selectionModel()->currentIndex();
    if(!index.isValid())return;
    modelInputs->removeRow(index.row());
}
/*!\func
 * remove output
 * \param no
 * \return no
 */
void EnterInputs::on_actionRemove_output_triggered()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    QModelIndex index = ui->tvOutputs->selectionModel()->currentIndex();
    if(!index.isValid())return;
    modelOutputs->removeRow(index.row());
}
/*!\func
 * pressed on inputs list
 * \param no
 * \return no
 */
void EnterInputs::on_tvInputs_pressed(QModelIndex index)
{
    Q_UNUSED(index);
    QCursor cursor;
    if(qApp->mouseButtons() & Qt::RightButton)
	menuInputs.exec(cursor.pos());
}
/*!\func
 * pressed on outputs list
 * \param no
 * \return no
 */
void EnterInputs::on_tvOutputs_pressed(QModelIndex index)
{
    Q_UNUSED(index);
    QCursor cursor;
    if(qApp->mouseButtons() & Qt::RightButton)
	menuOutputs.exec(cursor.pos());
}
/*!\func
 * удалить управление
 * \param no
 * \return no
 */
void EnterInputs::on_actionRemove_control_triggered()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    QModelIndex index = ui->tvControls->selectionModel()->currentIndex();
    if(!index.isValid())return;
    modelControl->removeRow(index.row(),index.parent());
}
/*!\func
 * pressed on controls list
 * \param no
 * \return no
 */
void EnterInputs::on_tvControls_pressed(QModelIndex index)
{
    Q_UNUSED(index);
    QCursor cursor;
    if((index.isValid())&&(index.parent().isValid())&&(qApp->mouseButtons() & Qt::RightButton))
	menuControls.exec(cursor.pos());
}
/*!\func
 * загрузить проект
 * \param
 * - fileName - им€ фалйа
 * \return no
 */
void EnterInputs::loadProject(const QString&fileName)
{
    if(fileName.isEmpty())return;
    if(!QFile::exists(fileName))return;
    if(project.isNull())
	project = QSharedPointer<XmlProject>(new XmlProject(&data, fileName));
    project->load();
    onEditCommonData();
}
/*!\func
 * загрузить проект
 * \param
 * - fileName - им€ файла
 * \return no
 */
void EnterInputs::on_actionSave_as_triggered()
{
	LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
	QString tmp_project_file_name = QFileDialog::getSaveFileName(this, tr("Save project..."), QString(), tr("Projects (*.xml);;"));
	if(tmp_project_file_name.isEmpty())return;
	project_file_name = tmp_project_file_name;
	if(project.isNull())
	    project = QSharedPointer<XmlProject>(new XmlProject(&data, project_file_name));
	project->save();
}
/*!\func
 * изменено описание модул€
 * \param
 * \return no
 */
void EnterInputs::on_tbDescription_textChanged()
{
    data.setModuleDescription(ui->tbDescription->toPlainText());
}
/*!\func
 * изменено им€ модул€
 * \param
 * \return no
 */
void EnterInputs::on_leModule_textChanged(QString text)
{
    data.setModuleName(text);
    if((data.getModuleName() != text)&&(!text.isEmpty()))
        ui->leModule->setText(data.getModuleName());
}
