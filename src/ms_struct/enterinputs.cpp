#include "enterinputs.h"
#include "errors.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileDialog>
#include "inode.h"
#include "constants.h"
#include "ui_enterinputs.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QTime>

extern QString program_dir;
extern QString config_path;
/*!\func
 *
 * \param no
 * \return no
 */
EnterInputs::EnterInputs(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::EnterInputs),
    settings(config_path + "config_struct.ini", QSettings::IniFormat, parent)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    ui->setupUi(this);
    settings.beginGroup(Params::group__window);
    setGeometry(settings.value("x").toInt(),settings.value("y").toInt(),
                settings.value("width").toInt(),settings.value("height").toInt());
    settings.endGroup();
    ui->statusBar->addWidget(&pathLevels);
    settings.beginGroup(Params::group_project);
    ms_wizard_path = settings.value(Params::programs_ms_wizard).toString();
    QString curprj = settings.value(Params::current_project).toString();
    settings.endGroup();
    window_title = windowTitle ();
    QDir dir;
    change(false);
    if(!dir.exists(curprj))
        curprj = "";
    graphBody = new GraphBody(this, curprj, ms_wizard_path);
    setWindowTitle(window_title + " - " + graphBody->getProjectName());
    ui->mainLayout->addWidget(graphBody);
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
void EnterInputs::on_actionAddState_triggered()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    graphBody->addTop(TOP_SIMPLE);
}
/*!\func
 *
 * \param no
 * \return no
 */
void EnterInputs::on_actionGeneration_triggered()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    static Generator generator(graphBody->getParentNode(), ms_wizard_path);
    generator.setModal(true);
    generator.show();
    generator.checkValidate();
}
/*!\func
 *
 * \param no
 * \return no
 */
void EnterInputs::on_actionExit_triggered()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    close();
}
/*!\func
 *
 * \param no
 * \return no
 */
void EnterInputs::paintEvent ( QPaintEvent *)
{
}

void EnterInputs::on_actionLevel_up_triggered()
{
    ui->actionLevel_up->setEnabled(graphBody->levelUp());
}
/*!\func
 * save current project
 * \param no
 * \return no
 */
void EnterInputs::on_actionSave_project_triggered()
{
    if(graphBody->isTmpProject())
    {
        QString tmp = QFileDialog::getSaveFileName(0, tr("Save project..."), QDir::homePath());
        if(tmp.isEmpty())return;
        graphBody->setProjectName(tmp);
    }
    graphBody->save();
    setWindowTitle(window_title + " - "  + graphBody->getProjectName());
}
//! событие закрыти формы
void EnterInputs::closeEvent ( QCloseEvent * event )
{
    Q_UNUSED(event)
    if(hasChanges)
    switch(QMessageBox::question(0, tr("Saving project..."),
                             tr("Are you want save project before close it?"),
                             QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel))
    {
    case QMessageBox::Yes:
        on_actionSave_project_triggered();
        break;
    case QMessageBox::Cancel:
        event->ignore();
		return;
    default:
        ;;
    }
    settings.beginGroup(Params::group_project);
    settings.setValue(Params::current_project, graphBody->getProjectName());
    settings.endGroup();
    event->accept();
    settings.beginGroup(Params::group__window);
    settings.setValue("x", geometry().x());
    settings.setValue("y", geometry().y());
    settings.setValue("width", geometry().width());
    settings.setValue("height", geometry().height());
    settings.endGroup();
}
//! open existing project
void EnterInputs::on_actionOpen_project_triggered()
{
    if(hasChanges)
        if(QMessageBox::question(0, tr("Saving project..."), tr("Are you want save project before close it?"), QMessageBox::Yes, QMessageBox::No, 0) == QMessageBox::Yes)
            on_actionSave_project_triggered();
        QString project_path = QFileDialog::getOpenFileName(0, tr("Open project..."), QDir::homePath(), tr("Projects (") + FileNames::project_main_file + ");;");
	if(project_path.isEmpty())return;
	project_path.replace(FileNames::project_main_file, "");
	project_path.replace(".xml", "");
	QDir dir;
	dir.mkpath(project_path);
    if (graphBody)
        delete graphBody;
    graphBody = new GraphBody(this, project_path, ms_wizard_path);
        ui->mainLayout->addWidget(graphBody);
        setWindowTitle(window_title + " - "  + project_path);
}
//! new open project
void EnterInputs::on_actionCreate_project_triggered()
{
    if(hasChanges)
        if(QMessageBox::question(0, tr("Saving project..."), tr("Are you want save project before close it?"), QMessageBox::Yes, QMessageBox::No, 0) == QMessageBox::Yes)
            graphBody->save();
    newProject();
}
//! new open project
bool EnterInputs::newProject ()
{
	//QMessageBox::information(0,ms_wizard_path,settings.fileName());
        //ui->mainLayout->removeWidget(graphBody.data());
    if (graphBody)
        delete graphBody;
    graphBody = new GraphBody(this, "", ms_wizard_path);
        ui->mainLayout->addWidget(graphBody);
        setWindowTitle(window_title + " - "  + graphBody->getProjectName());
	return true;
}
/*!\func
 * add device in the project
 * \params no
 * \return no
 */
void EnterInputs::on_actionAdd_device_triggered()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    graphBody->addTop(TOP_DEVICE);
}
/*!\func
 * enable or disable button "levelup"
 * \params
 * - enable - enable or disable
 * \return no
 */
void EnterInputs::setEnableUpLevel(const bool enable)
{
    ui->actionLevel_up->setEnabled(enable);
}
/*!\func
 * set level path in status bar
 * \params
 * - path - path
 * \return no
 */
void EnterInputs::setLevelPath(const QString& path)
{
    pathLevels.setText(path);
}
/*!\func
 * set changes flag
 * \params
 * - changes - are changes present?
 * \return no
 */
void EnterInputs::change(const bool changes)
{
    hasChanges = changes;
    ui->actionSave_project->setEnabled(changes);
}
/*!\func
 * about programm
 * \params no
 * \return no
 */
void EnterInputs::on_actionAbout_triggered()
{
	QMessageBox::about(0, tr("About code generator %1").arg("0.0.0.4"), tr("Program for visual struct hard electronic schemes\nand generate it in verilog code\n\n")
														 +tr("Developed by Kuznecov A.U., NPO RIT, Russia\ne-mail:a-slash2003@mail.ru"));
}
