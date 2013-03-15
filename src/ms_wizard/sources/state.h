#ifndef STATE_H
#define STATE_H
#include <QString>
#include <QPoint>
class Data;
class Relation;

class State
{
public:
    State(Data*data = 0, const qint32 id = -1, const QString&nm = "");
    ~State();
    QString getName() const;
    QPoint getPosition() const;
    void setName(const QString& newValue);
    QString getDescription() const;
    void setDescription(const QString& newValue);
    void setPosition(const QPoint& value);
    QList<Relation> getRelations() const;
    qint32 getId() const;
    bool operator==(const State&) const;
private:
    QString name, description;
    qint32 id;
    Data *data;
    QPoint pos;
};

#endif // STATE_H
