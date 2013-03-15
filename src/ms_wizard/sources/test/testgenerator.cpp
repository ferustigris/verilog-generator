#include "testgenerator.h"

/*!\func
 * Конструктор класса, тестирующего генерацию кода verilog
 * \params
 * \result
 */
TestGenerator::TestGenerator(Data*data) : Generator(data)
{
}
/*!\func
 * заглужка добавления в лог
 * \params
 * \result
 */
void TestGenerator::addLog(const QString& msg)
{
	log += msg +"\n";
}
/*!\func
 * заглушка добавления положительного сообщения
 * \params
 * \result
 */
void TestGenerator::addOk(const QString& msg)
{
	log += "1"+msg +"\n";
}
/*!\func
 *заглушка добавления отрицательного сообщения
 * \params
 * \result
 */
void TestGenerator::addWarning(const QString& msg)
{
	log += "2"+msg +"\n";
}
/*!\func
 * получить весь лог
 * \params
 * \result
 */
QString TestGenerator::getLog() const
{
	return log;
}
/*!\func
 * генерация кода
 * \params
 * \result
 */
void TestGenerator::generate(const QString &tmpFile)
{
	generateCode(tmpFile);
}
/*!\func
 * проверка
 * \params
 * \result
 */
void TestGenerator::check()
{
	checkValidate();
}
