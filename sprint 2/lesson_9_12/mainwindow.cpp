#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_hello_clicked()
{
    ui->lbl_greetings->setText("Button pressed!");

    // Параметры функции:
    // - родительское окно (this),
    // - заголовок окна ("Приветствие"),
    // - текст сообщения ("Привет, пользователь!").
    QMessageBox::information(this, "Приветствие", "Привет, пользователь!");

    // С помощью информационных сообщений
    // можно вести диалог с пользователем.
    auto result = QMessageBox::question(this, "Вопрос",
                                        "Вы любите тыквенный суп?");

    if (result == QMessageBox::StandardButton::Yes) {
        // Если пользователь нажал «Да».
        QMessageBox::information(this, "Ответ",
                                 "Отлично! Тыква очень полезна для здоровья.");
    } else if (result == QMessageBox::StandardButton::No) {
        // Если пользователь нажал «Нет».
        QMessageBox::information(this, "Ответ",
                                 "Тогда предлагаем вам попробовать ризотто.");
    } else {
        QMessageBox::information(this, "Ответ",
                                 "Кажется, вы просто закрыли окно.");
    }
}

