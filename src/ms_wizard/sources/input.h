#ifndef INPUT_H
#define INPUT_H
#include <QString>

class Data;

class Input
{
public:
    Input(Data* data_meneger = 0, const qint32 id = -1, const QString&name="", const qint32 size = 1);
    virtual QString getName() const;
    virtual QString getType() const;
    virtual qint32 getSize() const;
    virtual qint32 getId() const;
    virtual void setType(const QString&newType);
    virtual void setName(const QString&newValue);
    virtual void setSize(const qint32 newValue);
private:
    qint32 size;
    qint32 id;
    Data* data;
    QString name;
    QString type;
};

#endif // INPUT_H
