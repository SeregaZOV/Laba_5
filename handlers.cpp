#include "handlers.h"
#include "Bank.h"
#include <gtk/gtk.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

extern Bank bank; // Глобальный объект bank для работы с БД



// Обработчик для изменения депозита
void on_edit_clicked(GtkWidget *widget, gpointer data) {
    vector<Deposit> deposits = bank.getAllDeposits();

    if (deposits.empty()) {
        GtkWidget *error_dialog = gtk_message_dialog_new(NULL,
                                                        GTK_DIALOG_MODAL,
                                                        GTK_MESSAGE_ERROR,
                                                        GTK_BUTTONS_OK,
                                                        "Нет доступных депозитов для изменения.");
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }

    // Создание диалогового окна для изменения депозита
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Изменить депозит",
                                                    NULL,
                                                    GTK_DIALOG_MODAL,
                                                    "Изменить", GTK_RESPONSE_OK,
                                                    "Отмена", GTK_RESPONSE_CANCEL,
                                                    NULL);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *combo = gtk_combo_box_text_new();

    // Заполняем выпадающий список депозитами
    for (const auto &dep : deposits) {
        stringstream ss;
        ss << "ID: " << dep.getId() << " | " << dep.getClientName();
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), ss.str().c_str());
    }

    gtk_container_add(GTK_CONTAINER(content_area), combo);

    // Поля для ввода новых значений
    GtkWidget *grid = gtk_grid_new();
    GtkWidget *name_entry = gtk_entry_new();
    GtkWidget *amount_entry = gtk_entry_new();
    GtkWidget *rate_entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Новое имя клиента:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Новая сумма:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), amount_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Новая ставка (%):"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), rate_entry, 1, 2, 1, 1);

    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    // Обрабатываем результат
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        int selected = gtk_combo_box_get_active(GTK_COMBO_BOX(combo));
        if (selected >= 0) {
            int id = deposits[selected].getId();
            string new_name = gtk_entry_get_text(GTK_ENTRY(name_entry));
            double new_amount = atof(gtk_entry_get_text(GTK_ENTRY(amount_entry)));
            double new_rate = atof(gtk_entry_get_text(GTK_ENTRY(rate_entry)));

            // Проверка значений
            if (new_amount > 0 && new_amount <= 100000 && new_rate > 0 && new_rate <= 100) {
                bank.editDeposit(id, Deposit(new_name, new_amount, new_rate));
                cout << "Депозит успешно изменён." << endl;
            } else {
                cerr << "Ошибка: Неверные значения суммы или ставки!" << endl;
            }
        }
    }

    gtk_widget_destroy(dialog);
}

// Обработчик для отображения списка депозитов
void on_list_clicked(GtkWidget *widget, gpointer data) {
    vector<Deposit> deposits = bank.getAllDeposits();

    // Создаём диалоговое окно для отображения списка
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Список депозитов",
                                                    NULL,
                                                    GTK_DIALOG_MODAL,
                                                    "OK", GTK_RESPONSE_OK,
                                                    NULL);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *text_view = gtk_text_view_new();
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    stringstream ss;
    if (deposits.empty()) {
        ss << "Список депозитов пуст.";
    } else {
        for (const auto &dep : deposits) {
            ss << "ID: " << dep.getId()
               << " | Имя: " << dep.getClientName()
               << " | Сумма: " << dep.getAmount()
               << " | Ставка: " << dep.getInterestRate() << "%\n";
        }
    }

    gtk_text_buffer_set_text(buffer, ss.str().c_str(), -1);
    gtk_container_add(GTK_CONTAINER(content_area), text_view);

    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Обработчик для добавления нового депозита
void on_add_clicked(GtkWidget *widget, gpointer data) {
    // Диалоговое окно для ввода данных
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Добавить депозит",
                                                    NULL,
                                                    GTK_DIALOG_MODAL,
                                                    "Добавить", GTK_RESPONSE_OK,
                                                    "Отмена", GTK_RESPONSE_CANCEL,
                                                    NULL);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(content_area), grid);

    GtkWidget *label_name = gtk_label_new("Имя клиента:");
    GtkWidget *entry_name = gtk_entry_new();
    GtkWidget *label_amount = gtk_label_new("Сумма:");
    GtkWidget *entry_amount = gtk_entry_new();
    GtkWidget *label_rate = gtk_label_new("Ставка (%):");
    GtkWidget *entry_rate = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), label_name, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_name, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_amount, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_amount, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_rate, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_rate, 1, 2, 1, 1);

    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        string name = gtk_entry_get_text(GTK_ENTRY(entry_name));
        double amount = atof(gtk_entry_get_text(GTK_ENTRY(entry_amount)));
        double rate = atof(gtk_entry_get_text(GTK_ENTRY(entry_rate)));

        // Проверка значений
        if (amount > 0 && amount <= 100000 && rate > 0 && rate <= 100) {
            bank.addDeposit(Deposit(name, amount, rate));
        } else {
            cerr << "Ошибка: Неверные значения суммы или ставки!" << endl;
        }
    }
    gtk_widget_destroy(dialog);
}

// Обработчик для удаления депозита
void on_delete_clicked(GtkWidget *widget, gpointer data) {
    vector<Deposit> deposits = bank.getAllDeposits();

    if (deposits.empty()) {
        GtkWidget *error = gtk_message_dialog_new(NULL,
                                                 GTK_DIALOG_MODAL,
                                                 GTK_MESSAGE_ERROR,
                                                 GTK_BUTTONS_OK,
                                                 "Нет доступных депозитов для удаления.");
        gtk_dialog_run(GTK_DIALOG(error));
        gtk_widget_destroy(error);
        return;
    }

    GtkWidget *dialog = gtk_dialog_new_with_buttons("Удалить депозит",
                                                    NULL,
                                                    GTK_DIALOG_MODAL,
                                                    "Удалить", GTK_RESPONSE_OK,
                                                    "Отмена", GTK_RESPONSE_CANCEL,
                                                    NULL);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *combo = gtk_combo_box_text_new();
    for (const auto &dep : deposits) {
        stringstream ss;
        ss << "ID: " << dep.getId() << " | " << dep.getClientName();
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), ss.str().c_str());
    }

    gtk_container_add(GTK_CONTAINER(content_area), combo);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        int selected = gtk_combo_box_get_active(GTK_COMBO_BOX(combo));
        if (selected >= 0) {
            bank.removeDeposit(deposits[selected].getId());
        }
    }
    gtk_widget_destroy(dialog);
}

// Обработчик для экспорта данных в текстовый файл
void on_export_clicked(GtkWidget *widget, gpointer data) {
    const string outputFile = "/Users/macbook/Desktop/прога/Laba_4/deposits.txt";

    ofstream file(outputFile);
    if (!file) {
        cerr << "Ошибка при создании файла: " << outputFile << endl;
        return;
    }

    vector<Deposit> deposits = bank.getAllDeposits();
    if (deposits.empty()) {
        file << "Список депозитов пуст." << endl;
    } else {
        file << "ID\tИмя клиента\tСумма\tСтавка (%)\n";
        for (const auto &dep : deposits) {
            file << dep.getId() << "\t"
                 << dep.getClientName() << "\t"
                 << dep.getAmount() << "\t"
                 << dep.getInterestRate() << "\n";
        }
    }

    file.close();
    cout << "Экспорт данных завершён. Файл сохранён как: " << outputFile << endl;
}
