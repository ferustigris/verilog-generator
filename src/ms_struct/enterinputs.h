#ifndef ENTERINPUTS_H
#define ENTERINPUTS_H

#include <QtGui/QMainWindow>
#include <QMenu>
#include <QLabel>
#include <QSettings>
#include <QStringListModel>
#include "generator.h"
#include "graphbody.h"

namespace Ui
{
    class EnterInputs;
}

class EnterInputs : public QMainWindow
{
    Q_OBJECT
public:
    EnterInputs(QWidget *parent = 0);
    ~EnterInputs();
    void setEnableUpLevel(const bool enable);
    void setLevelPath(const QString& path);
    void change(const bool changes);
private:
    Ui::EnterInputs *ui;
    virtual void paintEvent ( QPaintEvent * event );
    QSettings settings;
    QString ms_wizard_path;
    QString window_title;
    QLabel pathLevels;
    GraphBody *graphBody;
    bool hasChanges;
protected:
    virtual void closeEvent ( QCloseEvent * event );
    bool newProject ();
private slots:
	void on_actionAbout_triggered();
	void on_actionAdd_device_triggered();
    void on_actionCreate_project_triggered();
    void on_actionOpen_project_triggered();
    void on_actionSave_project_triggered();
    void on_actionLevel_up_triggered();
    void on_actionExit_triggered();
    void on_actionGeneration_triggered();
    void on_actionAddState_triggered();
};
#endif // ENTERINPUTS_H
