#include <gtk/gtk.h>

int main(int argc, char *argv[]) {

  GtkWidget *window;
  GtkWidget *vbox;

  GtkWidget *line;
  GtkWidget *func;
  GtkWidget *di;
  GtkWidget *ax;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 230, 250);
  gtk_window_set_title(GTK_WINDOW(window), "colourS");
  gtk_container_set_border_width(GTK_CONTAINER(window), 5);

  vbox = gtk_vbox_new(TRUE, 1);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  ln = gtk_button_new_with_label("Line");
  fx = gtk_button_new_with_label("Sine");
  di = gtk_button_new_with_label("Dice");
  ax = gtk_button_new_with_label("Guitar");

  gtk_box_pack_start(GTK_BOX(vbox), ln, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), fx, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), di, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), ax, TRUE, TRUE, 0);

  g_signal_connect(G_OBJECT(window), "destroy",
        G_CALLBACK(gtk_main_quit), G_OBJECT(window));

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
