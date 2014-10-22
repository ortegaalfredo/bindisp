#include "plugin.h"
#include <QDir>
#include <QLibrary>
#include <QPluginLoader>
#include <iostream>

#define DEFAULT_PLUGIN "2D Viewer"
#define DEFAULT_PLUGIN_MAP "Default Map viewer"

extern void (*Log)(const char *buf);
PluginView::PluginView()
{
}

void PluginView::LoadFrom(char *path)
{

int nodefault=1;
// For all files in the plugin path...
QDir *dir = new QDir(path);
dir->setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
dir->setSorting(QDir::Size | QDir::Reversed);
QFileInfoList list = dir->entryInfoList();
std::cout << "Loading Viewer plugins...." << std::endl;


for (int i = 0; i < list.size(); ++i) { // HACK: Preload all libs
        QFileInfo fileInfo = list.at(i);
        const char *data;
        // Try to load the library
//        QLibrary myLib(fileInfo.absoluteFilePath());
        QPluginLoader myLib(fileInfo.absoluteFilePath());
        myLib.setLoadHints(QLibrary::ResolveAllSymbolsHint);
        data = fileInfo.absoluteFilePath().toStdString().c_str();
        bool loaded;
        if (fileInfo.absoluteFilePath().indexOf("DefaultView.so")>0)
            {
            loaded = myLib.load();
            std::cout << fileInfo.absoluteFilePath().toStdString();
            if (loaded) std::cout << " Loaded\n";
            else        std::cout <<  " Not Loaded WARNING\n";
            }
        }


for (int i = 0; i < list.size(); ++i) {
         QFileInfo fileInfo = list.at(i);
         // Try to load the library
         QLibrary myLib(fileInfo.absoluteFilePath());
         myLib.setLoadHints(QLibrary::ResolveAllSymbolsHint);
         myLib.load();
         if (myLib.isLoaded())  {
             // Try to resolve the *correct* functions in the library
             SPluginView spv;
             spv.plugin_get_info = (plugin_get_info_func) myLib.resolve("plugin_get_info");
             spv.plugin_init = (plugin_init_func) myLib.resolve("plugin_init");
             spv.plugin_update = (plugin_update_func) myLib.resolve("plugin_update");
             spv.plugin_get_actions = (plugin_get_actions_func) myLib.resolve("plugin_get_actions");
             spv.plugin_display_widget = (plugin_display_widget_func) myLib.resolve("plugin_display_widget");

             spv.plugin_event_keypress = (plugin_event_keypress_func) myLib.resolve("plugin_event_keypress");
             spv.plugin_event_mousemove = (plugin_event_mousemove_func) myLib.resolve("plugin_event_mousemove");
             spv.plugin_event_mousepress = (plugin_event_mousepress_func) myLib.resolve("plugin_event_mousepress");
             spv.plugin_event_mouserelease = (plugin_event_mouserelease_func) myLib.resolve("plugin_event_mouserelease");
             spv.plugin_event_mousedoubleclick = (plugin_event_mousedoubleclick_func) myLib.resolve("plugin_event_mousedoubleclick");
             spv.plugin_event_wheel = (plugin_event_wheel_func) myLib.resolve("plugin_event_wheel");

             if ((spv.plugin_update !=0 ) && (spv.plugin_init !=0) && (spv.plugin_get_info!=0)  )
                {
                 // Correct symbols found, add to the vector
                 this->plugins.push_back(spv);
                 const char *Name,*Author,*ShortHelp,*LongHelp;
                 spv.plugin_get_info(&Name,&Author,&ShortHelp,&LongHelp);
                 Log (qPrintable(QString("\tLoaded %1 (%2) from %3").arg(fileInfo.fileName(), 10).arg(Name).arg(Author) ));
                 // First plugin is the default
                 if (nodefault) {
                    this->defaultPlugin = spv;
                    nodefault=0;
                 }
                 // Look for a proper default plugins
                 if (!strncmp(DEFAULT_PLUGIN,Name,strlen(DEFAULT_PLUGIN))) {
                    this->defaultPlugin = spv;
                     std::cout<< "Selected default viewer plugin: "<<Name<<std::endl;
                    }
                 if (!strncmp(DEFAULT_PLUGIN_MAP,Name,strlen(DEFAULT_PLUGIN_MAP))) {
                    this->defaultPluginMap = spv;
                     std::cout<< "Selected default MAP viewer plugin: "<<Name<<std::endl;
                    }

                }
            }

     }
 delete dir;
std::cout << "Loaded "<<this->getCount()<<" plugins...." << std::endl;
if (this->getCount() == 0) std::cout << "WARNING! no viewer plugins found! Check installation." << std::endl;
}

int PluginView::getCount()
{
    return plugins.size();
}
