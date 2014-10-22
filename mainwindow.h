#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

#include "plugin.h"

namespace Ui
{
    class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    void closeEvent(QCloseEvent *event);
    void log(QString str);
    ~MainWindow();
    BagOfPlugins pluginBag;
    QList <QAction *> ViewActions;
    QList <QAction *> DisasmActions;
    QList <QAction *> TransformActions;
    void UpdateActions();
    void RebuildMenus(void);
    void SetTabbedWidget(QWidget *qw);

public slots:
    void OpenFile(void);
    void ShowPreferences(void);
    void ShowLocura(void);
    void ShowTest(void);    

private slots:
    void on_actionFull_Screen_activated();

private:
    QWidget *documentDock,*LogDock;
    void LoadPlugins(void);
    AppInfo *appinfo;
    Ui::MainWindow *ui;


};

#endif // MAINWINDOW_H
