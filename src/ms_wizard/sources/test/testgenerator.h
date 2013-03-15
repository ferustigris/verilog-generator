#ifndef TESTGENERATOR_H
#define TESTGENERATOR_H
#include "generator.h"
#include "generator.h"

class TestGenerator : public Generator
{
public:
    TestGenerator(Data*data);
    void addLog(const QString& msg);
    void addOk(const QString& msg);
    void addWarning(const QString& msg);
    QString getLog() const;
    void generate(const QString &tmpFile = "");
    void check();
private:
    QString log;
};

#endif // TESTGENERATOR_H
