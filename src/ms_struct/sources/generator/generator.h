#ifndef GENERATOR_H
#define GENERATOR_H

#include <QtGui/QDialog>
#include "inode.h"
#include <QTextStream>

namespace Ui {
    class Generator;
}

class Generator : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(Generator)
public:
    explicit Generator(INode* _node, const QString& ms_wizard_path, QWidget *parent = 0);
    virtual ~Generator();
    void addLog(const QString& msg);
    void addOk(const QString& msg);
    void addWarning(const QString& msg);
    void generate();
    void checkValidate();
protected:
    virtual void changeEvent(QEvent *e);
    virtual void closeEvent ( QCloseEvent * );
private:
    Ui::Generator *m_ui;
    INode * node;
    QString ms_wizard_path;
    void genNodeCode(const QString&startDir, INode* n, const QString&parentDir, QTextStream &xise);
private slots:
    void on_pbNext_clicked();
    void on_pbCancel_clicked();
};

#endif // GENERATOR_H
