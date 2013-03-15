#ifndef IDENTIFICATOREDIT_H
#define IDENTIFICATOREDIT_H

#include <QObject>
#include <QLineEdit>

class IdentificatorEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit IdentificatorEdit(QWidget *parent = 0);
public slots:
    void textEdited( const QString & text );
private:
    QString oldText;
};

#endif // IDENTIFICATOREDIT_H
