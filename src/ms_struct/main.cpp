#include <QtGui/QApplication>
#include <QTranslator>
#include "enterinputs.h"
#include <QDir>
#include "errors.h"
#include <QMessageBox>

QString program_dir("");
QString config_path("");

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
//QMessageBox::information(0, QString(),QString("asd1|asd2|asd3").section("|", 0, 0));
//exit(0);
    program_dir = qApp->applicationDirPath() + "/";
    config_path = program_dir + "/../etc/";
    QDir dir;
    dir.mkpath(config_path);
    int loglevel = LOG_ERROR;//LOG_DEBUG
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
	LOG(LOG_DEBUG, str);
    }
    LOG(LOG_DEBUG, "start " + qApp->applicationName(), loglevel);
    LOG(LOG_DEBUG, QString(__FUNCTION__) + " <" + QString::number(__LINE__) + ">");
    QTranslator translator;
    translator.load(":/tr/" + QLocale::system().name());
    a.installTranslator(&translator);
    EnterInputs w;
    w.show();
    return a.exec();
}
