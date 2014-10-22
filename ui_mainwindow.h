/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionExit;
    QAction *actionPreferences;
    QAction *actionTest;
    QAction *actionAbout;
    QAction *actionAbout_Plugins;
    QAction *actionFull_Screen;
    QWidget *centralWidget;
    QVBoxLayout *fdsf;
    QTabWidget *tabWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuView;
    QMenu *menu_Help;
    QMenu *menuWindow;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(646, 656);
        MainWindow->setDockOptions(QMainWindow::AllowNestedDocks|QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks|QMainWindow::ForceTabbedDocks);
        MainWindow->setUnifiedTitleAndToolBarOnMac(true);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionPreferences = new QAction(MainWindow);
        actionPreferences->setObjectName(QString::fromUtf8("actionPreferences"));
        actionTest = new QAction(MainWindow);
        actionTest->setObjectName(QString::fromUtf8("actionTest"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionAbout_Plugins = new QAction(MainWindow);
        actionAbout_Plugins->setObjectName(QString::fromUtf8("actionAbout_Plugins"));
        actionFull_Screen = new QAction(MainWindow);
        actionFull_Screen->setObjectName(QString::fromUtf8("actionFull_Screen"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        fdsf = new QVBoxLayout(centralWidget);
        fdsf->setSpacing(6);
        fdsf->setContentsMargins(11, 11, 11, 11);
        fdsf->setObjectName(QString::fromUtf8("fdsf"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));

        fdsf->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 646, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menu_Help = new QMenu(menuBar);
        menu_Help->setObjectName(QString::fromUtf8("menu_Help"));
        menuWindow = new QMenu(menuBar);
        menuWindow->setObjectName(QString::fromUtf8("menuWindow"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuWindow->menuAction());
        menuBar->addAction(menu_Help->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionPreferences);
        menuEdit->addAction(actionTest);
        menu_Help->addAction(actionAbout);
        menu_Help->addAction(actionAbout_Plugins);
        menuWindow->addAction(actionFull_Screen);

        retranslateUi(MainWindow);
        QObject::connect(actionOpen, SIGNAL(activated()), MainWindow, SLOT(OpenFile()));
        QObject::connect(actionPreferences, SIGNAL(activated()), MainWindow, SLOT(ShowPreferences()));
        QObject::connect(actionTest, SIGNAL(activated()), MainWindow, SLOT(ShowTest()));
        QObject::connect(actionExit, SIGNAL(activated()), MainWindow, SLOT(close()));

        tabWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        actionPreferences->setText(QApplication::translate("MainWindow", "Preferences", 0, QApplication::UnicodeUTF8));
        actionTest->setText(QApplication::translate("MainWindow", "Test", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("MainWindow", "About...", 0, QApplication::UnicodeUTF8));
        actionAbout_Plugins->setText(QApplication::translate("MainWindow", "About Plugins...", 0, QApplication::UnicodeUTF8));
        actionFull_Screen->setText(QApplication::translate("MainWindow", "Full Screen", 0, QApplication::UnicodeUTF8));
        actionFull_Screen->setShortcut(QApplication::translate("MainWindow", "F11", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("MainWindow", "&Edit", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("MainWindow", "&View", 0, QApplication::UnicodeUTF8));
        menu_Help->setTitle(QApplication::translate("MainWindow", "&Help", 0, QApplication::UnicodeUTF8));
        menuWindow->setTitle(QApplication::translate("MainWindow", "&Window", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
