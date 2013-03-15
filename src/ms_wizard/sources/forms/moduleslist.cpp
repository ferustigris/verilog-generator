#include <QMimeData>
#include "moduleslist.h"

ModulesList::ModulesList(QSplitter*&s) : QListWidget(s)
{
}
/*!\func
 *
 * \param no
 * \return no
 */
QMimeData * ModulesList::mimeData (const QList<QListWidgetItem *> items) const
{
    QMimeData *mimeData = new QMimeData();
    foreach (QListWidgetItem *item, items) {
        mimeData->setText(item->text());
    }
    return mimeData;
}
/*!\func
 *
 * \param no
 * \return no
 */
QStringList ModulesList::mimeTypes () const
{
    QStringList types;
    types << "text/plain";
    return types;
}
