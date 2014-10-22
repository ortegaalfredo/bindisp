#include "plugin.h"
#include <QDir>
#include <QLibrary>
#include <iostream>

#define DEFAULT_DOCVIEW_PLUGIN "Document Viewer"
#define DEFAULT_LOGVIEW_PLUGIN "Log Viewer"

extern void (*Log)(const char *buf);

PluginGeneric::PluginGeneric()
{
    this->defaultDocViewPlugin.plugin_display_widget=NULL;
    this->defaultLogViewPlugin.plugin_display_widget=NULL;
}

void PluginGeneric::LoadFrom(char *path)
{
    // For all files in the plugin path...
QDir *dir = new QDir(path);
dir->setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
dir->setSorting(QDir::Size | QDir::Reversed);
QFileInfoList list = dir->entryInfoList();
std::cout << "Loading Generic plugins...." << std::endl;
 for (int i = 0; i < list.size(); ++i) {
         QFileInfo fileInfo = list.at(i);
         // Try to load the library
         QLibrary myLib(fileInfo.absoluteFilePath());
         myLib.load();
         if (myLib.isLoaded())  {
             // Try to resolve the *correct* functions in the library
             SPluginGeneric spv;
             spv.plugin_get_info = (plugin_get_info_func) myLib.resolve("plugin_get_info");
             spv.plugin_init = (plugin_init_func) myLib.resolve("plugin_init");
             spv.plugin_exec = (plugin_exec_func) myLib.resolve("plugin_exec");
             spv.plugin_display_widget = (plugin_display_widget_func) myLib.resolve("plugin_display_widget");

            if ((spv.plugin_exec !=0 ) && (spv.plugin_init !=0) )
                {
                 // Correct symbols found, add to the vector
                 this->plugins.push_back(spv);
                 const char *Name,*Author,*ShortHelp,*LongHelp;
                 spv.plugin_get_info(&Name,&Author,&ShortHelp,&LongHelp);
                 Log (qPrintable(QString("\tLoaded %1 (%2) from %3").arg(fileInfo.fileName(), 10).arg(Name).arg(Author) ));
                 // Look for a proper default plugins
                 if (!strncmp(DEFAULT_DOCVIEW_PLUGIN,Name,strlen(DEFAULT_DOCVIEW_PLUGIN))) {
                    this->defaultDocViewPlugin = spv;
                     std::cout<< "Selected default document viewer plugin: "<<Name<<std::endl;
                    }
                 if (!strncmp(DEFAULT_LOGVIEW_PLUGIN,Name,strlen(DEFAULT_LOGVIEW_PLUGIN))) {
                    this->defaultLogViewPlugin = spv;
                     std::cout<< "Selected default Log viewer plugin: "<<Name<<std::endl;
                    }

                }
         } else std::cout<< "WARNING! Failed to load " << fileInfo.canonicalFilePath().toStdString() << std::endl;

     }
 delete dir;
std::cout << "Loaded "<<this->getCount()<<" plugins...." << std::endl;
if (this->getCount() == 0) std::cout << "WARNING! no generic plugins found! Check installation." << std::endl;
}


int PluginGeneric::getCount()
{
    return plugins.size();
}
