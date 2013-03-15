#include "generator.h"
#include "ui_generator.h"
#include "errors.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

/*!\func
 *
 * \param нет
 * \return нет
 */
Generator::Generator(Data*data, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::Generator)
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    m_ui->setupUi(this);
    this->data = data;
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
    generateCode(fileName);
    m_ui->pbNext->setText(tr("Close"));
    m_ui->pbCancel->setEnabled(false);
    closeble = true;
}
/*!\func
 * непосредственно, генерация
 * \param нет
 * \return нет
 */
void Generator::generateCode(const QString &tmpfile)
{
	LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
	bool ok = true;
	addLog(tr("Generate..."));
	QString fn(tmpfile);
	if(tmpfile.isEmpty())
	{
		fn = QFileDialog::getSaveFileName(this, tr("Save File..."), QString(), tr("Verilog (*.v);;All Files (*)"));
	}
	if(!data)
	{
		addWarning("No data!");
		addWarning("Fail!");
		return;
	}
	if(fn.isEmpty())
	{
		addWarning("Fail!");
		return;
	}
	if(!fn.endsWith(".v"))fn += ".v";
	QFile file(fn);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		addWarning("Can't open file!");
		addWarning("Fail!");
		return;
	}
	QTextStream out(&file);
	//module definition
	QStringList html = data->getModuleDescription().split('\n');
	out<<"//module: "<<data->getModuleName()<<"\n";
	foreach(QString tmp, html)
		out<<"//"<<tmp<<"\n";
	out << "module " << data->getModuleName() << "(";
	QString tmp("");
	foreach(Input m, data->getInputs())
	{
		tmp += " " + m.getName() + ",";
	}
	//calc size var for states
	int statesCount = data->getStates().count();
	int sizeStatesVariable = 1, count2 = 2;
	while(count2 < statesCount)
	{
		count2 *= 2;
		sizeStatesVariable++;
	}
	foreach(Output m, data->getOutputs())
	{
		tmp += " " + m.getName() + ",";
	}
	if(!tmp.isEmpty())tmp = tmp.left(tmp.length() - 1);
	out<<tmp<<");\n";
	out<< "//constants\n";
	int index = 0;
	foreach(State state, data->getStates())
	{
		foreach(QString str, state.getDescription().split("\n"))
			out<< "\t//" <<str<< "\n";
		out<< QString("\tparameter ") + state.getName() + " = " + QString::number(sizeStatesVariable) +"'b" + QString::number(index++, 2) + ";\n";
	}
	//inputs&outputs
	out<< "//inputs\n";
	foreach(Input m, data->getInputs())
	{
		out<< "\tinput " + m.getName() + ";\n";
		QString size("");
		if(m.getSize() > 1)
		{
			size = "[" + QString::number(m.getSize() - 1) + ":0]";
		}
		out<< "\t" + m.getType() + size + " " + m.getName() + ";\n";
	}
	out<< "//outputs\n";
	foreach(Output m, data->getOutputs())
	{
		out<< "\toutput " + m.getName() + ";\n";
		QString size("");
		if(m.getSize() > 1)
		{
			size = "[" + QString::number(m.getSize() - 1) + ":0]";
		}
		out<< "\t" + m.getType() + size + " " + m.getName() + ";\n";
	}
	out<< "//temporary variables\n";
	foreach(Output m, data->getOutputs())
	{
		QString size("");
		if(m.getSize() > 1)
		{
			size = "[" + QString::number(m.getSize() - 1) + ":0]";
		}
		out<< "\treg" + size + " _" + m.getName() + ";\n";
	}
	out<<"\n\treg["<<QString::number(sizeStatesVariable-1)<<":0] "<<"state"<<";\n";
	//init
	out<<"\n\t"<<"//Inicialization\n";
	out<<"\t"<<"initial\n";
	out<<"\t"<<"begin\n";
	foreach(Output m, data->getOutputs())
	{
                out<<"\t\t_" << m.getName() << " <= "<<m.getValue() << ";\n";
	}
	if(!data->getStates().isEmpty())
		out<<"\t\t"<<"state <= "<<data->getStates().first().getName()<<";\n";
	out<<"\t\t"<<"$monitor(\"Current state: state=%h\", state)"<<";\n";
	out<<"\t"<<"end\n\n";
	//states
	out<<"\n\t"<<"//Enter by clock\n";
		out<<"\t"<<"always @(posedge CLK)\n";
	/*foreach(Input m, data->getInputs())
	{
	    out<<" or "<<m.getName();
	}
	out<<")\n";*/
	out<<"\t"<<"begin"<<"\n";
	out<<"\t\t"<<"if(RST)"<<"\n";
	out<<"\t\t"<<"begin"<<"\n";
	foreach(State state, data->getStates())
	{
		out<<"\t\t\t"<<"state = "<<state.getName()<<";\n";
		break;
	}
	out<<"\t\t"<<"end else"<<"\n";
	out<<"\t\t"<<"case(state)"<<"\n";
	foreach(State state, data->getStates())
	{
		foreach(QString tmp, state.getDescription().split("\n"))
			out<<"\t\t\t//"<<tmp<<"\n";
		out<<"\t\t\t"<<state.getName()<<":\n";
		out<<"\t\t\t"<<"begin"<<"\n";
		out<<"\t\t\t\t"<<"$display(\""<<data->getModuleName()<<": Enter to state="<<state.getName()<<"\");\n";
		out<<"\t\t\t\t//TODO: some code...\n";
		out<<"\t\t\t\t//transfer to next state\n";
		foreach(Relation relation, data->getRelations(state.getId()))
		{
			if(relation.getCondition().isEmpty())
			{
				out<<"\t\t\t\t"<<"$display(\""<<data->getModuleName()<<": transfer to next state="<<relation.getDest().getName()<<"\");\n";
				out<<"\t\t\t\t"<<"state = "<<relation.getDest().getName()<<";\n";
			}
			else
			{
				out<<"\t\t\t\t"<<"if("<<relation.getCondition()<<")\n";
				out<<"\t\t\t\t"<<"begin"<<"\n";
				out<<"\t\t\t\t\t"<<"$display(\""<<data->getModuleName()<<": transfer to next state="<<relation.getDest().getName()<<"\");\n";
				out<<"\t\t\t\t\t"<<"state = "<<relation.getDest().getName()<<";\n";
				out<<"\t\t\t\t"<<"end"<<"\n";
			}
		}
		out<<"\t\t\t\t//control by outputs\n";
		foreach(Control ctr, data->getControls(state.getId()))
		{
			if(ctr.getCondition().isEmpty())
			{
				out<<"\t\t\t\t"<<"$display(\""<<data->getModuleName()<<": control by outputs: "<<ctr.getOutput().getName()<<"="<<ctr.getValue()<<"\");\n";
				out<<"\t\t\t\t"<<"_"<<ctr.getOutput().getName()<<" = "<<ctr.getValue()<<";\n";
			}
			else
			{
				out<<"\t\t\t\t"<<"if("<<ctr.getCondition()<<")\n";
				out<<"\t\t\t\t"<<"begin"<<"\n";
				out<<"\t\t\t\t\t"<<"$display(\""<<data->getModuleName()<<": control by outputs: "<<ctr.getOutput().getName()<<"="<<ctr.getValue()<<"\");\n";
				out<<"\t\t\t\t\t"<<"_"<<ctr.getOutput().getName()<<" = "<<ctr.getValue()<<";\n";
				out<<"\t\t\t\t"<<"end"<<"\n";
			}
		}
		out<<"\t\t\t"<<"end"<<"\n";
	}
	out<<"\t\t"<<"endcase"<<"\n";
	out<<"\t"<<"end"<<"\n\n";
	//assign outputs temp vars
	foreach(Output m, data->getOutputs())
	{
		out<<"\tassign "<<m.getName()<<" = _"<<m.getName()<<";\n";
	}
	out<<"endmodule"<<"\n";

    file.close();
    if(ok)
	    addOk(tr("Complite."));
    else
	    addWarning(tr("Fail!"));
}
/*!\func
 * проверка валидности
 * \param нет
 * \return нет
 */
bool Generator::checkValidate()
{
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    bool ok = 1;
    addLog(tr("Check..."));
    if(!data)return false;
    QList<State>states = data->getStates();
    if(data->getInputs().isEmpty())
    {
	    addWarning(tr("FSM has't outputs!"));
	    ok = 0;
    }
    if(states.isEmpty())
    {
	    addWarning(tr("FSM has't states!"));
	    ok = 0;
    }
    //check relations
    foreach(State state, states)
    {
	    QList<Relation>rels = data->getRelations(state.getId());
	    int countIn = 0;
	    rels = data->getRelations();
	    foreach(Relation rel, rels)
	    {
		    if(rel.getDest().getId() == state.getId())
		    {
			    countIn++;
			    break;
		    }
	    }
	    if(!countIn)
	    {
		    addWarning(tr("State <") + state.getName() + tr("> not used!"));
		    ok = 0;
	    }
	    if(rels.isEmpty())
	    {
		    addWarning(tr("State <") + state.getName() + tr("> finish state?"));
		    ok = 0;
	    }
    }
    if(ok)
	addOk(tr("Complite."));
    else
	addWarning(tr("Fail!"));
    m_ui->pbNext->setEnabled(true);
    return ok;
}
/*!\func
 * непосредственно, генерация
 * \param нет
 * \return нет
 */
void Generator::generate(const QString &tmpFile)
{
	fileName = tmpFile;
	checkValidate();
}
