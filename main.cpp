#include "mainwindow.h"
#include <QApplication>
#include <qjsondocument>
#include <qjsonobject>
#include <qdebug>
#include <QCommandLineOption>
#include <QCommandLineParser>

struct UserInfo
{
    QString strUserName;  // 用户名
    QString strPassword;  // 密码
};

void parseArguments(UserInfo& g_userInfo)
{
    // 获取命令行参数
    QStringList arguments = QCoreApplication::arguments();

    qDebug() << "Arguments : " << arguments;

    if (arguments.count() < 2)
        return;

    QString strJson = arguments.at(1);

    qDebug() << "Json : " << strJson;

    // 解析Json对象
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(strJson.toLocal8Bit(), &jsonError);
    if (jsonError.error != QJsonParseError::NoError)
        return;

    if (doucment.isObject())
    {
        QJsonObject obj = doucment.object();
        QJsonValue value;
        if (obj.contains("UserName"))
        {
            value = obj.take("UserName");
            if (value.isString())
                g_userInfo.strUserName = value.toString();
        }
        if (obj.contains("Password"))
        {
            value = obj.take("Password");
            if (value.isString())
                g_userInfo.strPassword = value.toString();
        }
    }
}

//! 命令行下加入如下参数可看到输出(需在pro文件中加入CONFIG +=console)
//! {\"UserName\":\"Qter\",\"Password\":\"123456\"}
//! 当然也能自定义响应 -dir E:/Projects -h等参数了
/*
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    UserInfo g_userInfo;
    // 解析命令行参数
    parseArguments(g_userInfo);

    // 测试输出
    qDebug() << "UserName : " << g_userInfo.strUserName;
    qDebug() << "Password : " << g_userInfo.strPassword;

    MainWindow w;
    w.show();

    return a.exec();
}
*/

///第二个例子
// -a --BBB -c -D=Jack --Age -m=Hello -m "World"
int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  QCommandLineOption op1("a");// 短名称，无参数
  QCommandLineOption op2("BBB");// 长名称，无参数
  QCommandLineOption op3(QStringList() << "c" << "CCC");// 多个名称，无参数
  QCommandLineOption op4("D", "", "Name");// 短名称，有参数，无默认值
  QCommandLineOption op5("Age", "", "", "18");// 长名称，有参数，有默认值
  QCommandLineOption op6("m", "", "."); // 短名称，有参数，无默认值

  QCommandLineParser parser;
  parser.addOption(op1);
  parser.addOption(op2);
  parser.addOption(op3);
  parser.addOption(op4);
  parser.addOption(op5);
  parser.addOption(op6);
  parser.process(a);

  qDebug() << parser.isSet(op1); // true
  qDebug() << parser.isSet("BBB"); // true
  qDebug() << parser.isSet("CCC"); // true
  qDebug() << parser.value(op4); // "Jack"
  qDebug() << parser.value(op5); // "18"
  qDebug() << parser.values(op6);// ("Hello", "World")

  qDebug() << parser.helpText();
  MainWindow w;
  w.show();

  return a.exec();
}
