#ifndef OUTPUT_H
#define OUTPUT_H
#include "input.h"

class Data;
class Output : public Input
{
public:
    Output(Data* data = 0, const qint32 id = -1, const QString&name="", const qint32 size = 1, const QString&type = "wire", const QString&value="");
    virtual QString getValue() const;
    virtual void setValue(const QString&newValue);
private:
    QString value;
};

#endif // OUTPUT_H
