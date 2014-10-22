#include "plugin.h"
#include <QDir>
#include <QLibrary>
#include <iostream>


extern void (*Log)(const char *buf);

PluginDisasm::PluginDisasm()
{
}

void PluginDisasm::LoadFrom(char *path)
{
    // For all files in the plugin path...
QDir *dir = new QDir(path);
dir->setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
dir->setSorting(QDir::Size | QDir::Reversed);
QFileInfoList list = dir->entryInfoList();
std::cout << "Loading Disasm plugins...." << std::endl;
 for (int i = 0; i < list.size(); ++i) {
         QFileInfo fileInfo = list.at(i);
         // Try to load the library
         QLibrary myLib(fileInfo.absoluteFilePath());
         myLib.load();
         if (myLib.isLoaded())  {
             // Try to resolve the *correct* functions in the library
             SPluginDisasm spv;
             spv.plugin_get_info = (plugin_get_info_func) myLib.resolve("plugin_get_info");
             spv.plugin_init = (plugin_init_func) myLib.resolve("plugin_init");
             spv.plugin_disasm = (plugin_disasm_func) myLib.resolve("plugin_disasm");
             spv.plugin_get_file_types = (plugin_get_file_types_func) myLib.resolve("plugin_get_file_types");

            //printf("%08X,%08X\n\n\r",spv.plugin_disasm,spv.plugin_get_info);
            if ((spv.plugin_init !=0) && (spv.plugin_disasm !=0) && (spv.plugin_get_file_types !=0) )
                {
                 // Correct symbols found, add to the vector
                 this->plugins.push_back(spv);
                 const char *Name,*Author,*ShortHelp,*LongHelp;
                 spv.plugin_get_info(&Name,&Author,&ShortHelp,&LongHelp);
                 Log (qPrintable(QString("\tLoaded %1 (%2) from %3").arg(fileInfo.fileName(), 10).arg(Name).arg(Author) ));
                }
            }

     }
 delete dir;
std::cout << "Loaded "<<this->getCount()<<" plugins...." << std::endl;
if (this->getCount() == 0) std::cout << "WARNING! no dissasembly plugins found! Check installation." << std::endl;
}


int PluginDisasm::getCount()
{
    return plugins.size();
}
