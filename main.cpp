#include <QtGui/QApplication>
#include <string>
#include "mainwindow.h"

MainWindow *wi;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    wi=&w;
    w.show();
    return a.exec();
}

void log(std::string txt) {
if (wi)
        wi->log(QString(txt.c_str()));

}
