#ifndef PLUGINVIEW_H
#define PLUGINVIEW_H

#include "binstruct.h"
#include <vector>
#include <QString>

typedef void (*plugin_get_info_func)(char **Name, char **Author, char **ShortHelp, char **LongHelp);
typedef int (*plugin_init_func)(Binary *binary);
typedef void (*plugin_draw_func)();

typedef struct {
    plugin_init_func plugin_init;
    plugin_get_info_func plugin_get_info;
    plugin_draw_func plugin_draw;
} SPluginView;

class PluginView
{
public:
    PluginView();
    void LoadFrom(QString path);
    int getCount();
    vector <SPluginView> plugins;
};

#endif // PLUGINVIEW_H
