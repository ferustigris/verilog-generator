#ifndef GENERATOR_H
#define GENERATOR_H

#include <QtGui/QDialog>
#include <QSharedPointer>
#include "inputmodel.h"
#include "data.h"
#include "outputmodel.h"

namespace Ui {
    class Generator;
}

class Generator : public QDialog {
	Q_OBJECT
	Q_DISABLE_COPY(Generator)
public:
	explicit Generator(Data*data, QWidget *parent = 0);
	virtual ~Generator();
	void generate(const QString &tmpFile = "");
	virtual void generateCode(const QString &tmpfile);
protected:
	virtual void addLog(const QString& msg);
	virtual void addOk(const QString& msg);
	virtual void addWarning(const QString& msg);
	virtual bool checkValidate();
	virtual void changeEvent(QEvent *e);
	virtual void closeEvent ( QCloseEvent * );
private:
	Ui::Generator *m_ui;
	Data *data;
	QString fileName;
private slots:
	void on_pbNext_clicked();
	void on_pbCancel_clicked();
};

#endif // GENERATOR_H
