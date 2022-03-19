#include <gtk/gtk.h>
#include <string.h>

typedef struct
{
    char *file_name;
    char *label;
    char *tooltip;
} TlButton;

TlButton toollist[] = 
{
    {"new.png", "New", "Create new document"},
    {"open.png", "Open", "Open document"},
    {"save.png", "Save", "Save document"},
    {"separator", "0", "0"},
    {"undo.png", "Undo", "Undo last action"},
    {"redo.png", "Redo", "Redo last action"},
    {"separator", "0", "0"},
    {"copy.png", "Copy", "Copy text"},
    {"cut.png", "Cut", "Cut text"},
    {"paste.png", "Paste", "Paste text"},
    {"separator", "0", "0"},
    {"search.png", "Search", "Search for text"},
    {"searchnreplace.png", "Search & Replace", "Search & replace text"}
};

typedef struct
{
    char *label;
    int sub_items;
    char sub_menu[6][15];
} MuButton;

MuButton menuList[] =
{
    {"File", 6, {"New", "Open", "Save", "Save As", "Quit", "Close"}},
    {"Edit", 4, {"Cut", "Copy", "Paste", "Delete"}},
    {"View", 4, {"Full Screen", "Larger Text", "Smaller Text", "Reset"}},
    {"Search", 4, {"Find", "Find Next", "Find Previous", "Replace"}},
    {"Tools", 1, {"Check Spelling"}},
    {"Documents", 2, {"Close All", "Save All"}},
    {"Help", 2, {"Contents", "About"}}
};

GtkWidget *notebook;
void close_window()
{
    // check tabs are saved
    gtk_main_quit();
}

void close_tab()
{
    
}

void add_tab(char *name)
{
    GtkWidget *textview = gtk_text_view_new();
    GtkWidget *text = gtk_label_new(name);
    GtkWidget *label = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *icon = gtk_image_new_from_file("close.png");
    GtkWidget *button = gtk_button_new();
    gtk_button_set_image(GTK_BUTTON(button), icon);
    gtk_widget_set_tooltip_text(button, "Close tab");
    gtk_box_pack_start(GTK_BOX(label), text, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(label), button, FALSE, FALSE, 0);
    g_signal_connect(GTK_WIDGET(button), "clicked", G_CALLBACK(close_tab),
                     NULL);
    GtkWidget *scrollwindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrollwindow), textview);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), scrollwindow, label);
    gtk_widget_show_all(label);
    gtk_widget_show_all(scrollwindow);
}

void button_click(GtkWidget *button, gpointer data)
{
    char *btn = (char*)data;
    if(strcmp(btn, "New") == 0){
        add_tab("New Tab");
    }
}

void make_notebook(GtkWidget *vbox)
{
    notebook = gtk_notebook_new();
    gtk_box_pack_start(GTK_BOX(vbox), notebook, TRUE, TRUE, 0);
    add_tab("untitled");
}

void make_menu(GtkWidget *vbox)
{
    
    int mLimit = sizeof(menuList) / sizeof(MuButton);
    GtkWidget *menubar = gtk_menu_bar_new();
    int i;
    for (i = 0; i < mLimit; ++i)
    {
        GtkWidget *item = gtk_menu_item_new_with_label(menuList[i].label);
        GtkWidget *item_menu = gtk_menu_new();
        gtk_menu_shell_append(GTK_MENU_SHELL(menubar), item);
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), item_menu);
        int loop;

        for (loop = 0; loop < menuList[i].sub_items; ++loop)
        {
            char *theLabel = menuList[i].sub_menu[loop];
            GtkWidget *subitem = gtk_menu_item_new_with_label(theLabel);
            g_signal_connect(GTK_WIDGET(subitem), "activate", G_CALLBACK(button_click), theLabel);
            gtk_menu_shell_append(GTK_MENU_SHELL(item_menu), subitem);
        }
    }
    gtk_container_add(GTK_CONTAINER(vbox), menubar);
}

void make_toolbar(GtkWidget *vbox)
{
    
    int tLimit = sizeof(toollist) / sizeof(TlButton);
    GtkWidget *toolbar = gtk_toolbar_new();
    GtkToolItem *item;
    int i;
    gtk_toolbar_set_icon_size(GTK_TOOLBAR(toolbar), GTK_ICON_SIZE_MENU);
    for (i = 0; i < tLimit; ++i)
    {
        if (strcmp(toollist[i].file_name, "separator") == 0)
        {
            item = gtk_separator_tool_item_new();
        }
        else
        {
            GtkWidget *icon = gtk_image_new_from_file(toollist[i].file_name);
            item = gtk_tool_button_new(NULL, NULL);
            gtk_tool_button_set_icon_widget(GTK_TOOL_BUTTON(item), icon);
            gtk_tool_item_set_tooltip_text(item, toollist[i].tooltip);
            g_signal_connect(GTK_TOOL_BUTTON(item), "clicked", G_CALLBACK(button_click),
                             toollist[i].label);
        }
        gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
    }
    gtk_container_add(GTK_CONTAINER(vbox), toolbar);
}

void make_window()
{
    GtkWidget *window;
    GtkWidget *vbox;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);
    gtk_window_set_title(GTK_WINDOW(window), "My Text Editor");
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(close_window), NULL);
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    make_menu(vbox);
    make_toolbar(vbox);
    make_notebook(vbox);
    gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);
    make_window();
    gtk_main();
    return 0;
}