#include <QApplication>
#include "mainwindow.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("CP-1251"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("CP-1251"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("CP-1251"));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
