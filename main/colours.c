#include<stdio.h>
#include <gtk/gtk.h>

void button_clicked(GtkWidget *box, gpointer data) {
    
  g_print("clicked\n");
}

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

  g_signal_connect(G_OBJECT(window), "destroy",
        G_CALLBACK(gtk_main_quit), G_OBJECT(window));

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
