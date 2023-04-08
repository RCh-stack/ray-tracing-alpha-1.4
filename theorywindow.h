#ifndef THEORYWINDOW_H
#define THEORYWINDOW_H

#include <QDialog>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "sql_requests.h"

namespace Ui {
class TheoryWindow;
}

class TheoryWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TheoryWindow(QWidget *parent = 0);
    ~TheoryWindow();

    void set_window_options();
    void set_enabled_button(int id_page);
    void set_list_themes();

    void open_file_by_code(int row_index);
    void open_file_by_name(QString name);
    void output_table_of_contents(QString path);

private slots:
    void on_button_prev_page_clicked();

    void on_button_next_page_clicked();

    void on_list_of_contents_itemClicked(QTreeWidgetItem *item);

private:
    Ui::TheoryWindow *ui;
    QSqlDatabase db;
    int pages_read;
};

#endif // THEORYWINDOW_H
