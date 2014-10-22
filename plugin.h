#ifndef PLUGIN_H
#define PLUGIN_H

#include <QObject>
#include <QVariant>
#include "binstruct.h"


using namespace std;


// Forward declarations
class Binary;
class BagOfPlugins;
class QMainWindow;
class QAction;
class QWidget;
class QTabWidget;

Q_DECLARE_METATYPE(QTabWidget*)
Q_DECLARE_METATYPE(Sub*)
Q_DECLARE_METATYPE(Section*)
Q_DECLARE_METATYPE(Binary*)

typedef QWidget *PQWidget;
// This struct contains all application info

typedef struct {
  QMainWindow *mainwindow;
  vector <PBinary> binarys;
  BagOfPlugins *plugins;
  // Logging function
  void (*Log)(const char *format, ...);
} AppInfo;


//------------------ Plugin common functions
typedef void (*plugin_get_info_func)(const char **Name, const char **Author, const char **ShortHelp, const char **LongHelp);
typedef int (*plugin_init_func)(AppInfo *appinfo);
typedef void (*plugin_get_actions_func)(void *actions);

//------------------ Optional events
typedef void (*plugin_event_keypress_func)(char key);
typedef void (*plugin_event_mousepress_func)(int x,int y,int buttons);
typedef void (*plugin_event_mousemove_func)(int x,int y, int buttons);
typedef void (*plugin_event_mouserelease_func)(int x,int y, int buttons);
typedef void (*plugin_event_mousedoubleclick_func)(int x,int y, int buttons);
typedef void (*plugin_event_wheel_func)(int x,int y, int buttons);

//------------------- View plugins --------------------------
typedef void (*plugin_update_func)(PQWidget widget, PSub newSub);
typedef PQWidget (*plugin_display_widget_func)(void);

// View Plugin interface
typedef struct {
    plugin_init_func plugin_init;
    plugin_get_info_func plugin_get_info;
    plugin_get_actions_func plugin_get_actions;
    plugin_update_func plugin_update;
    plugin_display_widget_func plugin_display_widget;
    // Optional events
    plugin_event_keypress_func  plugin_event_keypress;
    plugin_event_mousemove_func plugin_event_mousemove;
    plugin_event_mousepress_func plugin_event_mousepress;
    plugin_event_mouserelease_func plugin_event_mouserelease;
    plugin_event_mousedoubleclick_func plugin_event_mousedoubleclick;
    plugin_event_wheel_func plugin_event_wheel;
    // Properties
} SPluginView;

// View plugin loader and container class
class PluginView
{
public:
    PluginView();
    // Loads all plugins from a path
    void LoadFrom(char *path);
    int getCount();
    // Container of all view plugins
    vector <SPluginView> plugins;
    SPluginView defaultPlugin;
    SPluginView defaultPluginMap;
};
//--------------------- Disasm plugins -----------------------------

// Return extension an file description
typedef void (*plugin_get_file_types_func)(const char **Extension, const char **Description);
// Process the file
typedef PBinary (*plugin_disasm_func)(char *FileName);

// Disasm Plugin interface
typedef struct {
    plugin_init_func plugin_init;
    plugin_get_info_func plugin_get_info;
    plugin_disasm_func plugin_disasm;
    plugin_get_file_types_func plugin_get_file_types;
} SPluginDisasm;

// View plugin loader and container class

class PluginDisasm
{
public:
    PluginDisasm();
    // Loads all plugins from a path
    void LoadFrom(char *path);
    int getCount();
    // Container of all view plugins
    vector <SPluginDisasm> plugins;
};

//------------------------- Generic plugins -----------------------------

typedef void (*plugin_exec_func)(PQWidget dockArea);

// Generic Plugin interface
typedef struct {
    plugin_init_func plugin_init;
    plugin_get_info_func plugin_get_info;
    plugin_exec_func plugin_exec;
    plugin_display_widget_func plugin_display_widget;
} SPluginGeneric;


// Generic plugin loader and container class

class PluginGeneric
{
public:
    PluginGeneric();
    // Loads all plugins from a path
    void LoadFrom(char *path);
    int getCount();
    // Container of all view plugins
    vector <SPluginGeneric> plugins;
    // Default document viewer plugin (it's a generic plugin)
    SPluginGeneric defaultDocViewPlugin;
    // Default log viewer plugin (it's a generic plugin)
    SPluginGeneric defaultLogViewPlugin;
};

// Contanier class for all plugins
class BagOfPlugins {
    public:
    PluginView pluginView;
    PluginDisasm pluginDisasm;
    PluginGeneric pluginGeneric;
};



#endif // PLUGIN_H
