#ifndef CONTROLDELEGATE_H
#define CONTROLDELEGATE_H
#include <QItemDelegate>

class ControlDelegate : public QItemDelegate
{
public:
    ControlDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                       const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor,
         const QStyleOptionViewItem &option, const QModelIndex &index) const;
};
#endif // CONTROLDELEGATE_H
