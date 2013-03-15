#ifndef TDIALOGADDITEM_H
#define TDIALOGADDITEM_H

#include <QtGui/QDialog>
#include "data.h"

namespace Ui {
    class TDialogAddItem;
}

class TDialogAddItem : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(TDialogAddItem)
public:
    explicit TDialogAddItem(Data*data, QWidget *parent = 0);
    void showModal(const qint32 id);
    virtual ~TDialogAddItem();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::TDialogAddItem *m_ui;
    Data*data;
private slots:
    void on_pbOk_pressed();
    void on_pbCancel_pressed();
};

#endif // TDIALOGADDITEM_H
