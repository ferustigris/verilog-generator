#include "generator.h"
#include "ui_generator.h"
#include "errors.h"
#include <QFile>
#include <QFileDialog>
#include <QProcess>
#include <node.h>
#include <nodefsm.h>
#include "project.h"
#include <QMessageBox>

/*!\func
 *
 * \param нет
 * \return нет
 */
Generator::Generator(INode *_node, const QString& ms_wizard_path, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::Generator)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    node = _node;
    m_ui->setupUi(this);
    this->ms_wizard_path = ms_wizard_path;
}
/*!\func
 *
 * \param нет
 * \return нет
 */
Generator::~Generator()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    delete m_ui;
}
/*!\func
 *
 * \param нет
 * \return нет
 */
void Generator::closeEvent ( QCloseEvent * )
{
	m_ui->pbNext->setText(tr("Next"));
	m_ui->pbCancel->setText(tr("Cancel"));
}
/*!\func
 *
 * \param нет
 * \return нет
 */
void Generator::changeEvent(QEvent *e)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
	m_ui->retranslateUi(this);
	break;
    default:
	break;
    }
}
/*!\func
 * нажюатие на кнопку отмена
 * \param нет
 * \return нет
 */
void Generator::on_pbCancel_clicked()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    close();
}
/*!\func
 * добавить сообщение в лог
 * \param
 * - msg - сообщение
 * \return нет
 */
void Generator::addLog(const QString& msg)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    m_ui->lwLog->addItem(msg);
}
/*!\func
 * добавить хорошее сообщение в лог
 * \param
 * - msg - сообщение
 * \return нет
 */
void Generator::addOk(const QString& msg)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    QListWidgetItem *item = new QListWidgetItem(QIcon(":/icons/ok"), msg, m_ui->lwLog);
    m_ui->lwLog->addItem(item);
}
/*!\func
 * добавить плохое сообщение в лог
 * \param
 * - msg - сообщение
 * \return нет
 */
void Generator::addWarning(const QString& msg)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    QListWidgetItem *item = new QListWidgetItem(QIcon(":/icons/bad"), msg, m_ui->lwLog);
    m_ui->lwLog->addItem(item);
}
/*!\func
 * добавить сообщение в лог
 * \param
 * - msg - сообщение
 * \return нет
 */
void Generator::on_pbNext_clicked()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    static bool closeble = false;
    if(closeble)
    {
	closeble = false;
	close();
	return;
    }
    generate();
    m_ui->pbNext->setText(tr("Close"));
    m_ui->pbCancel->setEnabled(false);
    closeble = true;
}
/*!\func
 * генерация кода для одной ноды, итеративная функция
 * \param
 * - node - текущая вершина
 * \return no
 */
void Generator::genNodeCode(const QString&startDir, INode* n, const QString&parentDir, QTextStream &xise)
{
	LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
	QDir dir(parentDir);
	QString curDir = (parentDir + "/" + n->getName()).toLower();
	dir.mkpath(curDir);
	dir.setCurrent(curDir);
	QFile fn((n->getName() + ".v").toLower());
	fn.open(QIODevice::WriteOnly);
	//write to .xise file...
	QString fileName((curDir + "/" + fn.fileName()).toLower());
	xise<<"<file xil_pn:name=\""<<fileName.replace(startDir,QString("")).toLower()<<"\" xil_pn:type=\"FILE_VERILOG\">\n";
	xise<<"\t"<<"<association xil_pn:name=\"BehavioralSimulation\"/>\n";
	xise<<"\t"<<"<association xil_pn:name=\"Implementation\"/>\n";
	xise<<"</file>\n";
	QTextStream out(&fn);
	foreach(QString str, n->getShortDesc().split("\n"))
		out<<"// "<<str<<"\n";
	out<<"module "<<n->getName()<<" (";
	QString tmp("CLK, RST, ");
    QStringList usedNames;
    foreach(Edge*e, n->edgesIn())
        foreach(QString data, e->getWires())
        {
                QStringList list = data.split("|");
                if(list.count() == 4)
                {
					if(!usedNames.contains("I_" + list[0]))
                    {
						tmp += " I_" + list[0] + ",";
						usedNames<<("I_" + list[0]);
                    }
                }
        }
    foreach(Edge*e, n->edgesOut())
        foreach(QString data, e->getWires())
        {
                QStringList list = data.split("|");
                if(list.count() == 4)
                {
					if(!usedNames.contains("O_" + list[0]))
                    {
						tmp += " O_" + list[0] + ",";
						usedNames<<("O_" + list[0]);
                    }
                }
        }
    if(!tmp.isEmpty())tmp = tmp.left(tmp.length() - 1);
    out<<tmp<<");"<<"\n";
    usedNames.clear();
	out<<"\n//inputs\n";
	out<<"input CLK;\n";
	out<<"input RST;\n";
	foreach(Edge*e, n->edgesIn())
	    foreach(QString data, e->getWires())
	    {
		    QStringList list = data.split("|");
                    if(list.count() == 4)
		    {
						if(!usedNames.contains("I_"+list[0]))
                        {
							out<<"input "<<"I_"<<list[0]<<";//"<<list[3]<<"\n";
							usedNames<<("I_"+list[0]);
                        }
		    }
	    }
    out<<"\n//outputs\n";
    foreach(Edge*e, n->edgesOut())
	    foreach(QString data, e->getWires())
	    {
                    QStringList list = data.split("|");
                    if(list.count() == 4)
		    {
						if(!usedNames.contains("O_" + list[0]))
                        {
							out<<"output O_"<<list[0]<<";//"<<list[3]<<"\n";
							usedNames<<("O_" + list[0]);
                        }
		    }
	    }
    out<<"\n//inputs\n";
    usedNames.clear();
    foreach(Edge*e, n->edgesIn())
	    foreach(QString data, e->getWires())
	    {
		    QStringList list = data.split("|");
                    if(list.count() == 4)
		    {
						if(!usedNames.contains("I_"+list[0]))
                        {
                            out<<"wire ";
                            if(list[1].toInt() > 1)
                                    out<<"["<<list[1]<<":0] ";
							out<<"I_"<<list[0]<<";\n";
							usedNames<<("I_"+list[0]);
                        }
		    }
	    }
    out<<"\n//outputs\n";
    foreach(Edge*e, n->edgesOut())
            foreach(QString data, e->getWires())
            {
                    QStringList list = data.split("|");
                    if(list.count() == 4)
                    {
						if(!usedNames.contains("O_" + list[0]))
                        {
							out<<list[2]<<" ";
                            if(list[1].toInt() > 1)
                                    out<<"["<<list[1]<<":0] ";
							out<<"O_"<<list[0]<<";\n";
							usedNames<<("O_" + list[0]);
                        }
                    }
            }
    usedNames.clear();
    out<<"\n//templorary variables\n";
    //внутренние шины для связи вызываемых модулей
    foreach(INode*child, n->getNodes())
    {
	foreach(Edge*e, child->edgesOut())
	{
		foreach(QString data, e->getWires())
		{
			QStringList list = data.split("|");
                        if(list.count() == 4)
			{
                            if(!usedNames.contains(list[0]))
                            {
				out<<"reg ";
				if(list[1].toInt() > 1)
					out<<"["<<list[1]<<":0] ";
                                out<<"_" + list[0]<<";//"<<list[3]<<"\n";
                                usedNames<<list[0];
                            }
			}
		}
	}
    }
    //call FPS ms_wizard's
    if(NodeFSM *fsm = dynamic_cast<NodeFSM*>(n))
    {
        QStringList list = Project::loadInfo(fsm->getMS()).split("\n");
	if(list.count() > 1)
	{
	    //variables
	    QString moduleName = list[0];
	    QProcess proc;
		QString dirname(QDir::tempPath()), filename (dirname + "/tmp.xml"), verilog_file(dir.currentPath() + "/" + moduleName.toLower() + ".v");
	    QFile f(filename);
	    QDir dir;
	    dir.mkpath(dirname);
	    f.open(QIODevice::WriteOnly);
	    QTextStream text(&f);
            text<<fsm->getMS();
	    f.close();
			proc.execute(ms_wizard_path + " if=\"" + filename + "\" of=\"" + verilog_file.toLower() + "\"");
		xise<<"<file xil_pn:name=\""<<verilog_file.replace(startDir,QString("")).toLower()<<"\" xil_pn:type=\"FILE_VERILOG\">\n";
	    xise<<"\t"<<"<association xil_pn:name=\"BehavioralSimulation\"/>\n";
	    xise<<"\t"<<"<association xil_pn:name=\"Implementation\"/>\n";
	    xise<<"</file>\n";
            QStringList inputs = Project::loadInputs(fsm->getMS()).split("\n", QString::SkipEmptyParts);
            QStringList outputs = Project::loadOutputs(fsm->getMS()).split("\n", QString::SkipEmptyParts);
	    out<<"\t"<<"//Inputs for FPC\n";
	    foreach(QString item, inputs)
	    {
		QStringList m = item.split("\t");
		if(m.count() < 4)continue;
                out<<"\treg";//<<m[1];
                if(m[2] != "1")out<<"["<<QString::number(m[2].toInt() - 1)<<":0]";
                out<<" "<<moduleName<<"_"<<m[0]<<";\n";
	    }
	    out<<"\t"<<"//Outputs for FPC\n";
	    foreach(QString item, outputs)
	    {
		QStringList m = item.split("\t");
		if(m.count() < 4)continue;
                out<<"\twire";//<<m[1];
                if(m[2] != "1")out<<"["<<QString::number(m[2].toInt() - 1)<<":0]";
                out<<" "<<moduleName<<"_"<<m[0]<<";\n";
	    }
	    out<<"\n";
	}
    }
    out<<"\n//call modules\n";
    if(NodeFSM *fsm = dynamic_cast<NodeFSM*>(n))
    {
        QStringList list = Project::loadInfo(fsm->getMS()).split("\n");
        if(list.count() > 1)
        {
            QString moduleName = list[0];
			out<<moduleName<<" __"<<moduleName<<"__ (";

            QStringList inputs = Project::loadInputs(fsm->getMS()).split("\n", QString::SkipEmptyParts);
            QStringList outputs = Project::loadOutputs(fsm->getMS()).split("\n", QString::SkipEmptyParts);
			QString tmp("\n\t.CLK(CLK),\n\t.RST(RST),\n");
            foreach(QString item, inputs)
            {
                QStringList m = item.split("\t");
                if(m.count() < 1)continue;
				tmp += "\t." + m[0] + "(" + moduleName + "_" + m[0] + "),\n";
            }
            foreach(QString item, outputs)
            {
                QStringList m = item.split("\t");
                if(m.count() < 1)continue;
				tmp += "\t." + m[0] + "(" + moduleName + "_" + m[0] + "),\n";
            }
            if(!tmp.isEmpty())
                tmp = tmp.left(tmp.length() - 2);
            out<<tmp<<");\n";
        }

    }
    //вызываем модули
    foreach(INode*child, n->getNodes())
    {
	out<<child->getName()<<" __"<<child->getName()<<"__ (";
	QString tmp("\n\t.CLK(CLK),\n\t.RST(RST),\n");
        usedNames.clear();
	foreach(Edge*e, child->edgesIn())
		foreach(QString data, e->getWires())
		{
			QStringList list = data.split("|");
                        if(list.count() > 3)
			{
                            if(!usedNames.contains(list[0]))
                            {
								tmp += "\t.I_" + list[0] + "(" +  "_" + list[0] + "),\n";
								usedNames<<("I_"+list[0]);
                            }
			}
		}
	foreach(Edge*e, child->edgesOut())
	{
		foreach(QString data, e->getWires())
		{
			QStringList list = data.split("|");
                        if(list.count() > 3)
			{
                            if(!usedNames.contains(list[0]))
                            {
								tmp += "\t.O_" + list[0] + "(" +  "_" + list[0] + "),\n";
								usedNames<<("O_"+list[0]);
                            }
                        }
		}
	}
	if (!tmp.isEmpty())
		tmp = tmp.left(tmp.length() - 2);
	out<<tmp<<");"<<"\n";
	genNodeCode(startDir, child, curDir, xise);
    }

	out<<"\n\tinitial\n";
	out<<"\tbegin\n";
	out<<"\t\t//TODO: initial code...\n";
	out<<"\tend\n\n";


	out<<"\n\talways @(posedge CLK)\n";
    out<<"\tbegin\n";
    out<<"\t\t//TODO: some code...\n";
    out<<"\tend\n\n";


    out<<"endmodule\n";
    fn.close();
}
/*!\func
 * непосредственно, генерация
 * \param нет
 * \return нет
 */
void Generator::generate()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if(!node)return;
    QString fn = QFileDialog::getSaveFileName(this, tr("Save project..."), QString(), tr("All Files (*)"));
    if(fn.isEmpty())return;
    QDir dir;
    dir.mkpath(fn);
    dir.setCurrent(fn);
    QFile fxise("project.xise");
    fxise.open(QIODevice::WriteOnly);
    QTextStream xise(&fxise);
    xise<<"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n";
    xise<<"<project xmlns=\"http://www.xilinx.com/XMLSchema\" xmlns:xil_pn=\"http://www.xilinx.com/XMLSchema\">\n";
    xise<<"<header>\n";
    xise<<"\t<!-- ISE by MS struct.             -->\n";
    xise<<"\t<!-- Copyright (c) 2010 NPO RIT, Inc.  All rights reserved. -->\n";
    xise<<"</header>\n";
    xise<<"<version xil_pn:ise_version=\"11.1\" xil_pn:schema_version=\"2\"/>\n";
    xise<<"<files>\n";
    genNodeCode(dir.currentPath() + "/", node, dir.currentPath(), xise);
    xise<<"</files>\n";

    xise<<"<properties>\n";
    xise<<"\t"<<"<property xil_pn:name=\"Auto Implementation Top\" xil_pn:value=\"false\"/>\n";
    xise<<"\t"<<"<property xil_pn:name=\"Constraints Entry\" xil_pn:value=\"Constraints Editor\"/>\n";
    xise<<"\t"<<"<property xil_pn:name=\"Device\" xil_pn:value=\"xc3s250e\"/>\n";
    xise<<"\t"<<"<property xil_pn:name=\"Device Family\" xil_pn:value=\"Spartan3E\"/>\n";
    xise<<"\t"<<"<property xil_pn:name=\"Fitter Report Format\" xil_pn:value=\"HTML\"/>\n";
    //xise<<"\t"<<"<property xil_pn:name=\"Implementation Top\" xil_pn:value=\"Module|gold_code\"/>\n";
    //xise<<"\t"<<"<property xil_pn:name=\"Implementation Top Instance Path\" xil_pn:value=\"/gold_code\"/>\n";
    //xise<<"\t"<<"<property xil_pn:name=\"PROP_DesignName\" xil_pn:value=\"gold_code_ver_217\"/>\n";
    xise<<"\t"<<"<property xil_pn:name=\"Package\" xil_pn:value=\"ft256\"/>\n";
    xise<<"\t"<<"<property xil_pn:name=\"Preferred Language\" xil_pn:value=\"Verilog\"/>\n";
    xise<<"\t"<<"<property xil_pn:name=\"Simulation Run Time ISim\" xil_pn:value=\"10000 ns\"/>\n";
    xise<<"\t"<<"<property xil_pn:name=\"Simulation Run Time Par\" xil_pn:value=\"10000 ns\"/>\n";
    xise<<"\t"<<"<property xil_pn:name=\"Simulator\" xil_pn:value=\"ISim (VHDL/Verilog)\"/>\n";
    xise<<"\t"<<"<property xil_pn:name=\"Speed Grade\" xil_pn:value=\"-5\"/>\n";
    xise<<"\t"<<"<property xil_pn:name=\"Synthesis Tool\" xil_pn:value=\"XST (VHDL/Verilog)\"/>\n";
    xise<<"\t"<<"<property xil_pn:name=\"Top-Level Source Type\" xil_pn:value=\"HDL\"/>\n";
    xise<<"\t"<<"<property xil_pn:name=\"Verbose Property Persistence\" xil_pn:value=\"false\"/>\n";
    xise<<"</properties>\n";
    xise<<"<bindings/>\n";
    xise<<"<libraries/>\n";
    //xise<<"<partitions>\n";
    //xise<<"\t"<<"<partition xil_pn:name=\"\/root\"/>\n";
    //xise<<"</partitions>\n";
    xise<<"</project>\n";
    fxise.close();
    addOk(tr("Complite."));
}
/*!\func
 * проверка валидности
 * \param нет
 * \return нет
 */
void Generator::checkValidate()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    if(!node)
    {
	LOG(LOG_ERROR, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + "> - не установлена модель состояний!");
	return;
    }
    bool ok = true;
    addLog(tr("Check validate..."));
    if(ok)
	addOk(tr("All right!"));
    m_ui->pbNext->setEnabled(true);
}
