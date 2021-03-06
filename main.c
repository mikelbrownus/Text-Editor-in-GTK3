#include <gtk/gtk.h>
#include <string.h>

typedef struct
{
    GtkWidget *text;
    GtkWidget *address;
    GtkTextBuffer *buff;
    gboolean saved;
} PgID;

const int page_limit = 10;

PgID *book;

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
    free(book);
    gtk_main_quit();
}

void save_file(char *file_address)
{
    g_print("%s\n", file_address);
}

void save_as_dialog()
{
    GtkWidget *save_dialog;
    int result;
    save_dialog = gtk_file_chooser_dialog_new("Save As",
                                                NULL,
                                                GTK_FILE_CHOOSER_ACTION_SAVE,
                                                "Cancel",
                                                GTK_RESPONSE_CANCEL,
                                                "Save",
                                                GTK_RESPONSE_ACCEPT,
                                                NULL);
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(save_dialog);
    gtk_file_chooser_set_do_overwrite_confirmation(chooser, TRUE);
    result = gtk_dialog_run(GTK_DIALOG(save_dialog));
    if(result == GTK_RESPONSE_ACCEPT)
    {
        char *file_address;
        
        file_address = gtk_file_chooser_get_filename(chooser);
        save_file(file_address);
        free(file_address);
        gtk_widget_destroy(save_dialog);
    } else if(result == GTK_RESPONSE_CANCEL) {
        gtk_widget_destroy(save_dialog);
    }
}

void close_tab(GtkWidget *button, gpointer data)
{
    int page_number = gtk_notebook_page_num(GTK_NOTEBOOK(notebook), data);
    g_print("closing page: %d\n", page_number);
    int limit = gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook));
    if(!gtk_text_buffer_get_modified(book[page_number].buff)) 
    {
        gtk_notebook_remove_page(GTK_NOTEBOOK(notebook), page_number);
        for(int i = page_number; i < limit; ++i){
            book[i] = book[i+1];
        }
    } else {
        save_as_dialog();
    }
    
}

char* name_from_address(char* address)
{
    return strrchr(address, '/');
}

void add_tab(char *name)
{
    int number_of_pages = gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook));
    if(number_of_pages == page_limit){
        return;
    }
    GtkWidget *label = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *icon = gtk_image_new_from_file("close.png");
    GtkWidget *button = gtk_button_new();
    gtk_button_set_image(GTK_BUTTON(button), icon);
    gtk_widget_set_tooltip_text(button, "Close tab");

    GtkWidget *textview = gtk_text_view_new();
    book[number_of_pages].text = gtk_label_new(name_from_address(name));
    book[number_of_pages].address = gtk_label_new(name);
    book[number_of_pages].buff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
     
    
    gtk_box_pack_start(GTK_BOX(label), book[number_of_pages].text, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(label), book[number_of_pages].address, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(label), button, FALSE, FALSE, 0);
    GtkWidget *scrollwindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrollwindow), textview);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), scrollwindow, label);
    g_signal_connect(GTK_WIDGET(button), "clicked", G_CALLBACK(close_tab),
                     scrollwindow);
    gtk_widget_show(button);
    gtk_widget_show(book[number_of_pages].text);
    gtk_widget_show_all(scrollwindow);
}

void open_file(char *file_address)
{
    g_print("%s\n", file_address);
}

void open_file_dialog()
{
    GtkWidget *open_dialog;
    int result;
    open_dialog = gtk_file_chooser_dialog_new("Open File",
                                                NULL,
                                                GTK_FILE_CHOOSER_ACTION_OPEN,
                                                "Cancel",
                                                GTK_RESPONSE_CANCEL,
                                                "Open",
                                                GTK_RESPONSE_ACCEPT,
                                                NULL);
    result = gtk_dialog_run(GTK_DIALOG(open_dialog));
    if(result == GTK_RESPONSE_ACCEPT)
    {
        char *file_address;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(open_dialog);
        file_address = gtk_file_chooser_get_filename(chooser);
        open_file(file_address);
        free(file_address);
        gtk_widget_destroy(open_dialog);
    } else if(result == GTK_RESPONSE_CANCEL) {
        gtk_widget_destroy(open_dialog);
    }
}

void button_click(GtkWidget *button, gpointer data)
{
    char *btn = (char*)data;
    if(strcmp(btn, "New") == 0){
        add_tab("/New");
    } else if(strcmp(btn, "Open") == 0) {
        open_file_dialog();
    } else if(strcmp(btn, "Save As") == 0) {
        save_as_dialog();
    } else if(strcmp(btn, "Save") == 0) {
        save_file(NULL);
    }
}

void make_notebook(GtkWidget *vbox)
{
    notebook = gtk_notebook_new();
    gtk_box_pack_start(GTK_BOX(vbox), notebook, TRUE, TRUE, 0);
    add_tab("/untitled");
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
    book = (PgID*)malloc(sizeof(PgID) * (page_limit + 1));
    gtk_init(&argc, &argv);
    make_window();
    gtk_main();
    return 0;
}