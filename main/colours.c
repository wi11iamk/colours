#include <stdio.h>
#include <gtk/gtk.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct node {
    char * str;
    struct node * next;
};

typedef struct node node;

struct list {
    node * head;
    int size;
};

typedef struct list list;

node * newNode(char * path, node * next);
list * newList();
void addNode(list * l, char * path);

#define MAX_PATH_LEN 265
#define MAX_COMMAND_LEN MAX_PATH_LEN + 150
#define EXECUTED_FILE "widget"

/**
 * consider whether the current file is a regular file 
 * @param path
 * @return 1 or 0
 */
int isRegularFile(const char *path);

/**
 * consider whether the current file is a file that has C extension 
 * @param name
 * @return 
 */
int isSourceFile(char * name);

/**
 * get all source file in the path
 * @param names list of file
 * @param paths list of file with path
 * @param path path to the consider folder
 */
void collectSourceFile(list * names, list * paths, const char *path);

/**
 * create a command from the input path file
 * @param path
 * @return a command line
 */
char * createCompilingCommand(const char * path);

/**
 * fire when button of the sub menu is click
 * @param widget the button
 * @param data path to the executed file
 */
void button_action(GtkWidget *widget, gpointer data);

/**
 * process the sub pane
 * @param names list of file
 * @param paths list of file to be compiled
 * @param title name of the folder
 */
void paneProcessing(list * names, list * paths, const char * title);


/**
 * process a path. get the file , compile it, execute it
 * @param path path to the file 
 */
void processing(const char *path);

/**
 * consider whether the current file is a directory 
 * @param path
 * @return 1 or 0
 */
int isDirectory(const char *path);


void button_clicked(GtkWidget *widget, gpointer data);

int main(int argc, char *argv[]) {

    GtkWidget *window;
    GtkWidget *box;

    GtkWidget *ln;
    GtkWidget *fx;
    GtkWidget *di;
    GtkWidget *ax;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 230, 500);
    gtk_window_set_title(GTK_WINDOW(window), "colourS");
    gtk_container_set_border_width(GTK_CONTAINER(window), 5);

    box = gtk_box_new(TRUE, 1);
    gtk_container_add(GTK_CONTAINER(window), box);

    ln = gtk_button_new_with_label("Violet");
    fx = gtk_button_new_with_label("Blue");
    di = gtk_button_new_with_label("Green");
    ax = gtk_button_new_with_label("Red");

    gtk_box_pack_start(GTK_BOX(box), ln, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), fx, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), di, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), ax, TRUE, TRUE, 0);

    g_signal_connect(G_OBJECT(ln), "clicked",
            G_CALLBACK(button_clicked), "Violet");

    g_signal_connect(G_OBJECT(fx), "clicked",
            G_CALLBACK(button_clicked), "Blue");

    g_signal_connect(G_OBJECT(di), "clicked",
            G_CALLBACK(button_clicked), "Green");

    g_signal_connect(G_OBJECT(ax), "clicked",
            G_CALLBACK(button_clicked), "Red");

    g_signal_connect(G_OBJECT(window), "destroy",
            G_CALLBACK(gtk_main_quit), G_OBJECT(window));

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

void button_clicked(GtkWidget *widget, gpointer data) {
    g_print("%d\n", isDirectory(data));
    if (isDirectory(data)) {
        g_print("Valid directory %s\n", (char *) data);
        processing(data);

    } else {
        g_print("Invalid directory %s\n", (char *) data);
    }
}


int isDirectory(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}


void processing(const char *path) {
    list * names = newList();
    list * paths = newList();

    collectSourceFile(names, paths, path);
    if (names->size > 0) {
        paneProcessing(names, paths, path);
    } else {
        g_print("%s is empty\n", path);
    }

}


void paneProcessing(list * names, list * paths, const char * title) {

    GtkWidget * window;
    GtkWidget * box;
    GtkWidget * buttons[names->size];
    int i;
    node * name;
    node * path;
    
    gtk_init(NULL, NULL);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 230, 500);
    gtk_window_set_title(GTK_WINDOW(window), title);
    gtk_container_set_border_width(GTK_CONTAINER(window), 5);

    box = gtk_box_new(TRUE, 1);
    gtk_container_add(GTK_CONTAINER(window), box);

    name = names->head;
    path = paths->head;
    i = 0;
    while (name != NULL &&
            path != NULL) {
        buttons[i] = gtk_button_new_with_label(name->str);
        gtk_box_pack_start(GTK_BOX(box), buttons[i], TRUE, TRUE, 0);
        g_signal_connect(G_OBJECT(buttons[i]), "clicked",
                G_CALLBACK(button_action), path->str);
        name = name->next;
        path = path->next;
        i++;
    }

    g_signal_connect(G_OBJECT(window), "destroy",
            G_CALLBACK(gtk_main_quit), G_OBJECT(window));

    gtk_widget_show_all(window);


    gtk_main();
}


void button_action(GtkWidget *widget, gpointer data) {
    char * command = createCompilingCommand(data);
    int len;
    FILE * fp = popen(command, "w");
    if (fp == NULL) {
        fprintf(stderr, "cannot execute command : %s\n", command);
        free(command);

    } else {
        free(command);
        pclose(fp);

        len = strlen("./") + strlen(EXECUTED_FILE) + 2;
        command = (char *) calloc(len, sizeof (char));
        snprintf(command, len, "./%s", EXECUTED_FILE);

        fp = popen(command, "w");
        if (fp == NULL) {
            fprintf(stderr, "cannot execute command : %s\n", command);
            free(command);
        } else {
            free(command);
            pclose(fp);
            len = strlen("rm ") + strlen(EXECUTED_FILE) + 2;
            command = (char *) calloc(len, sizeof (char));
            command = (char *) calloc(len, sizeof (char));
            snprintf(command, len, "rm %s", EXECUTED_FILE);
            fp = popen(command, "w");
            if (fp == NULL) {
                fprintf(stderr, "cannot execute command : %s\n", command);
            } else {
                pclose(fp);
            }
            free(command);
        }
    }
}


char * createCompilingCommand(const char * path) {

    char * command = (char *) malloc((MAX_COMMAND_LEN) * sizeof (char));
    snprintf(command, MAX_COMMAND_LEN,
            "gcc -I Include/ `pkg-config --cflags glfw3` -o %s %s `pkg-config --static --libs glfw3` `pkg-config --libs freealut`",
            EXECUTED_FILE, path);

    return command;
}


void collectSourceFile(list * names, list * paths, const char *path) {
    DIR *dp;
    struct dirent *ep;
    char file[MAX_PATH_LEN];
    dp = opendir(path);
    if (dp != NULL) {
        while (ep = readdir(dp)) {
            strcpy(file, path);
            strcat(file, "/");
            strcat(file, ep->d_name);
            if (isSourceFile(ep->d_name) &&
                    isRegularFile(file)) {
                addNode(names, ep->d_name);
                addNode(paths, file);
            }
        }

        (void) closedir(dp);
    } else
        perror("Couldn't open the directory");
}


int isSourceFile(char * name) {
    char * dot = strrchr(name, '.');
    if (dot && !strcmp(dot, ".c")) {
        return 1;
    }

    return 0;
}


int isRegularFile(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;

    return S_ISREG(statbuf.st_mode);
}


node * newNode(char * path, node * next){
    node * n = (node *) malloc(sizeof(node));
    n->next = next;
    n->str = strdup(path);
    return n;
}


list * newList(){
    list * l = (list *) malloc(sizeof(list));
    l->head = NULL;
    l->size = 0 ;
    return l;
}


void addNode(list * l, char * path){
    if (l != NULL) {
        l->head = newNode(path, l->head);
        l->size++;
    }
}
