#include "Bank.h"
#include "handlers.h"
#include <gtk/gtk.h>

// Определение глобального объекта bank
Bank bank("/Users/macbook/deposits.db");

// Функция для создания главного окна
void create_main_window() {
    GtkWidget *window, *grid;
    GtkWidget *btn_list, *btn_add, *btn_edit, *btn_delete, *btn_export, *btn_exit;

    // Инициализация GTK
    gtk_init(NULL, NULL);

    // Создание главного окна
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Управление депозитами");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // Создание сетки для размещения кнопок
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Создание кнопок
    btn_list = gtk_button_new_with_label("Список депозитов");
    btn_add = gtk_button_new_with_label("Добавить запись");
    btn_edit = gtk_button_new_with_label("Изменить запись");
    btn_delete = gtk_button_new_with_label("Удалить запись");
    btn_export = gtk_button_new_with_label("Экспорт информации");
    btn_exit = gtk_button_new_with_label("Выход");

    // Размещение кнопок в сетке
    gtk_grid_attach(GTK_GRID(grid), btn_list, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_add, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_edit, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_delete, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_export, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_exit, 0, 5, 1, 1);

    // Привязка сигналов к обработчикам событий
    g_signal_connect(btn_list, "clicked", G_CALLBACK(on_list_clicked), NULL);
    g_signal_connect(btn_add, "clicked", G_CALLBACK(on_add_clicked), NULL);
    g_signal_connect(btn_edit, "clicked", G_CALLBACK(on_edit_clicked), NULL);
    g_signal_connect(btn_delete, "clicked", G_CALLBACK(on_delete_clicked), NULL);
    g_signal_connect(btn_export, "clicked", G_CALLBACK(on_export_clicked), NULL);
    g_signal_connect(btn_exit, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Отображение всех элементов окна
    gtk_widget_show_all(window);

    // Запуск основного цикла GTK
    gtk_main();
}

int main(int argc, char *argv[]) {
    create_main_window();
    return 0;
}
