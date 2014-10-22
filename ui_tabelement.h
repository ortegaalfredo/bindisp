/********************************************************************************
** Form generated from reading UI file 'tabelement.ui'
**
** Created: Wed Jan 25 04:04:12 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TABELEMENT_H
#define UI_TABELEMENT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TabElement
{
public:
    QGridLayout *gridLayout;
    QGraphicsView *graphicsView;
    QTextEdit *textEdit;
    QListWidget *listWidget;

    void setupUi(QWidget *TabElement)
    {
        if (TabElement->objectName().isEmpty())
            TabElement->setObjectName(QString::fromUtf8("TabElement"));
        TabElement->resize(606, 505);
        gridLayout = new QGridLayout(TabElement);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        graphicsView = new QGraphicsView(TabElement);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(3);
        sizePolicy.setVerticalStretch(3);
        sizePolicy.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy);
        graphicsView->setLayoutDirection(Qt::LeftToRight);

        gridLayout->addWidget(graphicsView, 0, 0, 2, 1);

        textEdit = new QTextEdit(TabElement);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        QFont font;
        font.setPointSize(6);
        textEdit->setFont(font);
        textEdit->setReadOnly(true);

        gridLayout->addWidget(textEdit, 2, 0, 1, 1);

        listWidget = new QListWidget(TabElement);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(3);
        sizePolicy1.setHeightForWidth(listWidget->sizePolicy().hasHeightForWidth());
        listWidget->setSizePolicy(sizePolicy1);
        listWidget->setMovement(QListView::Free);
        listWidget->setSortingEnabled(true);

        gridLayout->addWidget(listWidget, 0, 1, 3, 1);


        retranslateUi(TabElement);
        QObject::connect(listWidget, SIGNAL(itemSelectionChanged()), TabElement, SLOT(SubSelected()));

        QMetaObject::connectSlotsByName(TabElement);
    } // setupUi

    void retranslateUi(QWidget *TabElement)
    {
        TabElement->setWindowTitle(QApplication::translate("TabElement", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TabElement: public Ui_TabElement {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TABELEMENT_H
