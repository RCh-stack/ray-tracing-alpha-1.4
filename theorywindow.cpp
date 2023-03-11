#include "theorywindow.h"
#include "ui_theorywindow.h"

TheoryWindow::TheoryWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TheoryWindow)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QSQLITE");  // -- подключение базы данных --
    db.setDatabaseName("C:/Program Files (x86)/Qt Project/RayTracing/EducationSystem.sqlite"); // -- путь к базе данных --

    if (!db.open())
        QMessageBox::critical(NULL, QObject::tr("Ошибка!"), db.lastError().text()); // -- выводим ошибку --

    set_window_options();
    pages_read = 0;
    open_file_by_code(pages_read);
    set_enabled_button(pages_read);
}

TheoryWindow::~TheoryWindow()
{
    delete ui;
}

void TheoryWindow::set_window_options()
{
    QPixmap leftArrow (":/icons/images/prev-page.png");
    QPixmap rightArrow (":/icons/images/next-page.png");
    QPixmap help(":/icons/images/help-button.png");

    QIcon ButtonNextPage(rightArrow);
    QIcon ButtonPrevPage(leftArrow);
    QIcon ButtonInformation(help);

    ui->button_next_page->setIcon(ButtonNextPage);
    ui->button_prev_page->setIcon(ButtonPrevPage);
    ui->button_help->setIcon(ButtonInformation);

    QPixmap bkgnd(":/icons/images/mainwindow_background.jpg");
    bkgnd = bkgnd.scaled(size(), Qt::IgnoreAspectRatio);
    QPalette p = palette();
    p.setBrush(QPalette::Background, bkgnd);
    setPalette(p);
}

// 1.1
void TheoryWindow::set_enabled_button(int id_page)
{
    if(id_page == 0)
        ui->button_prev_page->setEnabled(0);
    else
        ui->button_prev_page->setEnabled(1);

    if(id_page == 8)
        ui->button_next_page->setEnabled(0);
    else
        ui->button_next_page->setEnabled(1);
}

// 1.1
void TheoryWindow::output_table_of_contents(QString path)
{
    QFile file(path);
    QTextStream html(&file);
    if ((file.exists())&&(file.open(QIODevice::ReadOnly | QIODevice::Text)))
    {
        QString str = html.readAll();
        ui->text_theory->setText(str);
        file.close();
    }
}

// 1.1
void TheoryWindow::on_button_prev_page_clicked()
{
    if(pages_read > 0 && pages_read <= 8)
        pages_read--;
    open_file_by_code(pages_read);
    set_enabled_button(pages_read);
}

// 1.1
void TheoryWindow::on_button_next_page_clicked()
{
    if(pages_read >= 0 && pages_read < 8)
        pages_read++;
    open_file_by_code(pages_read);
    set_enabled_button(pages_read);
}

// 1.1
void TheoryWindow::open_file_by_code(int row_index)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM TheoryPage WHERE ID_Page = :id");
    query.bindValue(":id",  row_index);
    query.exec();   // -- выполнение запроса к БД --
    if(!query.next())   // -- если запрос пустой --
        QMessageBox::warning(this, "Теоретический материал", "Страница не найдена!");
    else
        output_table_of_contents(query.value("Path").toString());
}

// 1.1
void TheoryWindow::open_file_by_name(QString name)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM TheoryPage WHERE Name_Theme = :name");
    query.bindValue(":name",  name);
    query.exec();   // -- выполнение запроса к БД --
    if(!query.next())   // -- если запрос пустой --
        QMessageBox::warning(this, "Теоретический материал", "Страница не найдена!");
    else
    {
        output_table_of_contents(query.value("Path").toString());
        pages_read = query.value("ID_Page").toInt();
        set_enabled_button(pages_read);
    }
}

// 1.1
void TheoryWindow::on_list_of_contents_itemClicked(QTreeWidgetItem *item)
{
    QString itemToAppendName = item->text(0);
    open_file_by_name(itemToAppendName);
}
