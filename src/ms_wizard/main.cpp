#include <QtGui/QApplication>
#include <QTranslator>
#include "enterinputs.h"
#include "errors.h"
#include "xmlproject.h"
#include "generator.h"

QString program_dir("");

/*!\func
 * точка входа в программу
 * \param
 * - кол-во арг-ов ком. строки
 * - аргументы ком. строки
 * \return no
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    program_dir = qApp->applicationDirPath();
    QString output_file(""), input_file("");
    int loglevel = LOG_ERROR;//LOG_DEBUG;
    foreach(QString str, a.arguments())
    {
	if(str.contains("loglevel"))
	{
	    bool ok;
	    int tmp_val;
	    QStringList tmp_list = str.split("=");
	    if(tmp_list.count() > 1)
	    {
		tmp_val = tmp_list.at(1).toInt(&ok);
		if(ok)
		    loglevel = tmp_val;
	    }
	}
        //output file.v
	if(str.contains("of"))
	{
	    QStringList tmp_list = str.split("=");
	    if(tmp_list.count() > 1)
		output_file = tmp_list.at(1);
	}
        //input file.xml
	if(str.contains("if"))
	{
	    QStringList tmp_list = str.split("=");
	    if(tmp_list.count() > 1)
		input_file = tmp_list.at(1);
	}
	LOG(LOG_DEBUG, str);
    }
    LOG(LOG_DEBUG, "start " + qApp->applicationName(), loglevel);
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    QTranslator translator;
    translator.load(":/tr/" + QLocale::system().name());
    a.installTranslator(&translator);
    //generate code in invisible mode
    if((!input_file.isEmpty())&&(!output_file.isEmpty()))
    {
	LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
	Data data;
	XmlProject project(&data, input_file);
	project.load();
	Generator generator(&data);
        generator.generateCode(output_file);
	return 0;
    }
    EnterInputs w(input_file);
    w.show();
    return a.exec();
}
