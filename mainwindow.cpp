#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_prefDialog.h"
#include <qfiledialog.h>
#include <iostream>
#include <QSettings>
#include <QDockWidget>
#include <QTextEdit>

using namespace std;

// Settings
QSettings *settings;

// Log Text edit
QTextEdit *qte = NULL;

void guilog(const char *format, ...) {
    char buf[512];
    va_list ap;
    // render the instruction and add it to the string
    va_start(ap,format);
    vsnprintf(buf,sizeof(buf),format,ap);
    va_end(ap);

 if (qte)
        qte->append(buf);
 else printf("%s\n",buf);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
 ui->setupUi(this);
 settings = new QSettings( "Groundworks", "BinVis_Mainwindow");

 // Restore geometry and state
 QByteArray qbag=this->saveGeometry();
 QByteArray qbas=this->saveState();


 appinfo = new AppInfo;
 appinfo->plugins=&this->pluginBag;
 appinfo->mainwindow=this;
 appinfo->Log = guilog;

 this->LoadPlugins();


 // Create Document viewer
 if ( appinfo->plugins->pluginGeneric.defaultDocViewPlugin.plugin_display_widget )
    {
    documentDock = appinfo->plugins->pluginGeneric.defaultDocViewPlugin.plugin_display_widget();
    if (documentDock!=NULL) {
          appinfo->plugins->pluginGeneric.defaultDocViewPlugin.plugin_init(appinfo);
          QDockWidget *dockWidget = new QDockWidget("Document Viewer");
          dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
          dockWidget->setMinimumWidth(200);
          dockWidget->setFeatures(QDockWidget::AllDockWidgetFeatures );
          dockWidget->setAcceptDrops(true);
          dockWidget->setAccessibleDescription("Document Viewer");
          dockWidget->setWidget(documentDock);
          addDockWidget(Qt::LeftDockWidgetArea, dockWidget);
          dockWidget->setObjectName("DockWidget");
        }
    }
 // Create Log viewer
 if ( appinfo->plugins->pluginGeneric.defaultLogViewPlugin.plugin_display_widget )
    {
    LogDock = appinfo->plugins->pluginGeneric.defaultLogViewPlugin.plugin_display_widget();
    if (LogDock!=NULL) {
          appinfo->plugins->pluginGeneric.defaultLogViewPlugin.plugin_init(appinfo);
          QDockWidget *dockWidgetL = new QDockWidget("Log Viewer");
          dockWidgetL->setAllowedAreas(Qt::AllDockWidgetAreas );
          //dockWidgetL->setMinimumWidth(200);
          dockWidgetL->setFeatures(QDockWidget::AllDockWidgetFeatures );
          dockWidgetL->setAcceptDrops(true);
          dockWidgetL->setAccessibleDescription("Log Viewer");
          dockWidgetL->setWidget(LogDock);
          addDockWidget(Qt::BottomDockWidgetArea, dockWidgetL);
          dockWidgetL->setObjectName("LogWidget");
          qte = (QTextEdit *)LogDock;
        }
    }

 this->restoreGeometry(settings->value("geometry",qbag ).toByteArray());
 this->restoreState(settings->value("windowsState",qbas).toByteArray());
 QVariant qv;
 qv.setValue(this->ui->tabWidget);
 this->setProperty("TabWidget",qv);
}

MainWindow::~MainWindow()
{
    delete appinfo;
    delete ui;
}

void MainWindow::log(QString str) {
    string stri(str.toAscii());
    std::cout<<stri<<std::endl;
}


void MainWindow::closeEvent(QCloseEvent *event)
 {
     settings->setValue("geometry", saveGeometry());
     settings->setValue("windowState", saveState());
     QMainWindow::closeEvent(event);
 }

void MainWindow::OpenFile(void)
{

const char *Description,*Extension;
unsigned int i;

// Carga extensiones de plugins
QString Extn,tmpstr="";
for(i=0;i<this->pluginBag.pluginDisasm.getCount();i++)
    {
    SPluginDisasm pd = pluginBag.pluginDisasm.plugins[i];
    pd.plugin_get_file_types(&Extension,&Description);
    if (i) Extn+=";;";
    tmpstr.sprintf("%s (%s)",Description,Extension);
    Extn+=tmpstr;
    }

// Open Dialog

QFileDialog *qfd = new QFileDialog(this,tr("Open Executable"),".",Extn);
qfd->exec();
QStringList fileNames = qfd->selectedFiles();
std::cout << qPrintable(qfd->selectedFilter()) << std::endl;

// Retrieve selected plugin
SPluginDisasm selectedPlugin;
for(i=0;i<this->pluginBag.pluginDisasm.getCount();i++)
    {
    selectedPlugin = pluginBag.pluginDisasm.plugins[i];
    selectedPlugin.plugin_get_file_types(&Extension,&Description);
    tmpstr.sprintf("%s (%s)",Description,Extension);
    if (qfd->selectedFilter().compare(tmpstr)==0) break;
    }


// Check filename
if (fileNames.size()<1) return;
QString fileName = fileNames[0];
delete qfd;

// Apply the disassembly plugin
selectedPlugin.plugin_init(this->appinfo);
Binary *binary = selectedPlugin.plugin_disasm(fileName.toAscii().data() );
if (!binary) return;

// Apply the default viewer plugin
appinfo->plugins->pluginView.defaultPlugin.plugin_init(appinfo);
QWidget *viewWidget = appinfo->plugins->pluginView.defaultPlugin.plugin_display_widget();
if (viewWidget) {
    binary->views.push_back(viewWidget);
    int tabindex=this->ui->tabWidget->addTab(viewWidget,fileName);
    this->ui->tabWidget->setCurrentWidget(viewWidget);
    this->ui->tabWidget->setTabsClosable(true);
    //this->ui->tabWidget->setTabPosition(QTabWidget::West);
    }

// Apply the default MAP viewer plugin
appinfo->plugins->pluginView.defaultPluginMap.plugin_init(appinfo);
QWidget *MAPWidget = appinfo->plugins->pluginView.defaultPluginMap.plugin_display_widget();
if (MAPWidget)  {
    binary->views.push_back(MAPWidget);
    appinfo->plugins->pluginView.defaultPluginMap.plugin_init(appinfo);
    QString mapTitle;
    mapTitle.sprintf("Map: %s",binary->name.c_str() );
    QDockWidget *dockWidgetL = new QDockWidget(mapTitle);
    dockWidgetL->setAllowedAreas(Qt::AllDockWidgetAreas );
    dockWidgetL->setFeatures(QDockWidget::AllDockWidgetFeatures );
    dockWidgetL->setAcceptDrops(true);
    dockWidgetL->setAccessibleDescription("Binary Map viewer");
    dockWidgetL->setWidget(MAPWidget);
    addDockWidget(Qt::TopDockWidgetArea, dockWidgetL);
    dockWidgetL->setObjectName("MapWidget");
    QVariant data;
    data.setValue(binary);
    MAPWidget->setProperty("Binary",data);
    viewWidget->setProperty("Binary",data);
   // this->setWindowState(Qt::WindowFullScreen);
    }

RebuildMenus();
// Refresh Document viewer
appinfo->plugins->pluginGeneric.defaultDocViewPlugin.plugin_exec(this->documentDock);
}
// ---------------------- Sets focus to an Object inside the tab widget
void MainWindow::SetTabbedWidget(QWidget *qw)
{
    this->ui->tabWidget->setCurrentWidget(qw);
}

// ----------------------- Create Plugin View Actions
void MainWindow::UpdateActions()
{
    const char *Name,*Author,*ShortHelp,*LongHelp;
    QAction *action;

    // Update Views
    SPluginView spv;
    vector <SPluginView>::iterator it;
    for(it=this->pluginBag.pluginView.plugins.begin();it<this->pluginBag.pluginView.plugins.end();it++)
        {
        spv = *it;
        spv.plugin_get_info(&Name,&Author,&ShortHelp,&LongHelp);
        action = new QAction(Name,this);
        this->ViewActions.append(action);
        }


    // Update generic plugins
    SPluginGeneric spt;
    vector <SPluginGeneric>::iterator itt;
    for(itt=this->pluginBag.pluginGeneric.plugins.begin();itt<this->pluginBag.pluginGeneric.plugins.end();itt++)
        {
        spt = *itt;
        spt.plugin_get_info(&Name,&Author,&ShortHelp,&LongHelp);
        action = new QAction(Name,this);
        this->TransformActions.append(action);
        }

    // Update Dissasembler
    SPluginDisasm spd;
    vector <SPluginDisasm>::iterator itd;
    for(itd=this->pluginBag.pluginDisasm.plugins.begin();itd<this->pluginBag.pluginDisasm.plugins.end();itd++)
        {
        spd = *itd;
        spd.plugin_get_info(&Name,&Author,&ShortHelp,&LongHelp);
        action = new QAction(Name,this);
        this->DisasmActions.append(action);
        }



}

// --------------------------- Rebuild menus based on default View selected
void MainWindow::RebuildMenus(void)
{
//TabElement *ud = (TabElement *)ui->tabWidget->currentWidget();
QList <QAction *> actionsList;
int size,i;
if (appinfo->plugins->pluginView.defaultPlugin.plugin_get_actions)
    appinfo->plugins->pluginView.defaultPlugin.plugin_get_actions((void *)&actionsList);
QMenuBar *qmb = menuBar();
QMenu *menuHer = qmb->findChild<QMenu *>( "menuView" );
if (menuHer) {
    menuHer->clear();
    menuHer->addActions(this->ViewActions);
    menuHer->addSeparator();
    menuHer->addActions(actionsList);
    }
}

void MainWindow::ShowPreferences(void)
{
QDialog qd;
Ui_Dialog *ud = new Ui_Dialog();
ud->setupUi(&qd);
qd.exec();
delete ud;
}

void MainWindow::ShowLocura(void)
{
printf("QWERQWER\n");
}

void MainWindow::ShowTest(void)
{
printf("QWERQWER\n");
}


void MainWindow::LoadPlugins(void)
{
this->pluginBag.pluginView.LoadFrom("plugins/view");
this->pluginBag.pluginDisasm.LoadFrom("plugins/disasm");
this->pluginBag.pluginGeneric.LoadFrom("plugins/generic");
this->UpdateActions();
}

void MainWindow::on_actionFull_Screen_activated()
{
    if  (this->windowState() == Qt::WindowFullScreen)
         this->setWindowState(Qt::WindowMaximized);
    else this->setWindowState(Qt::WindowFullScreen);
}
