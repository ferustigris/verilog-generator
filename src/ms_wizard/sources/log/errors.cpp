#include "errors.h"
#include <QDir>
#include <QApplication>

/*!\func LOG
 * ���������� ��������� � ������
 * \param
 * - level - ��������� ���������
 * - msg - ���������
 * - _maxlevel - ������� ��������������
 * - log_file - ���� �������
 * \return ���
 */
void LOG(quint8 level, QString msg, quint8 _maxlevel, QString log_file) {
    static quint8 maxlevel = 255;
    if(_maxlevel != 255) maxlevel = _maxlevel;
    if(level > maxlevel)return;
    static QDir dir;
    QString path(qApp->applicationDirPath() + "/../var/log/");
    dir.mkpath(path);
    static QString log_name = path + "/main.log";
    if(log_file != "")
        log_name = log_file;
    QFile file(log_name);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream out(&file);
    out.seek(file.size());
    out<<msg<<"\n";
    file.close();
}
/*!\func LOG
 * ������ �����
 * \param
 * - _name - ��� ������, ��������
 * \return ���
 */
ECommonError::ECommonError(const QString & _name) {
    name = _name;
    //LOG(LOG_DEBUG, name);
}
/*!\func LOG
 * ������ ���������� �� �������, �� ��������� ��� ����������
 * \param
 * - _name - ��� ������, ��������
 * \return ���
 */
ELibraryNotFound::ELibraryNotFound(const QString &name):ECommonError(name){
    LOG(LOG_APPER, name);
}
