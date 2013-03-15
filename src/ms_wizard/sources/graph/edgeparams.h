#ifndef EDGEPARAMS_H
#define EDGEPARAMS_H

#include "data.h"
#include <QtGui/QWidget>

namespace Ui {
    class EdgeParams;
}

class EdgeParams : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(EdgeParams)
public:
    explicit EdgeParams(Data*data, QWidget *parent = 0);
    virtual ~EdgeParams();
    void showModal(const qint32 id);
protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::EdgeParams *m_ui;
    Data*data;
    qint32 id;
private slots:
    void on_pbOk_clicked();
    void on_pbCancel_clicked();
};

#endif // EDGEPARAMS_H
