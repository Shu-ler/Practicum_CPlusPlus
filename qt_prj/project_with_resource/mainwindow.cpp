#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString image_path = ":/images/images/i-love-qt.png";
    SetPixmap(ui->lbl_pixmap, image_path);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetPixmap(QLabel *label_pix, const QString path)
{
    QPixmap pix(path);
    Q_ASSERT(!pix.isNull());

    label_pix->setPixmap( pix );
    FitToImage(label_pix);
    label_pix->show();
}

void MainWindow::FitToImage(QLabel *label)
{
    // Получение изображения
    auto pict = label->pixmap();

    // Определение ширины и высоты изображения
    auto w = pict.size();

    // Сдвигание виджета в позицию 0,0
    QPoint left_top{0, 0};
    label->move(left_top);

    // Установка размеров виджета
    label->setFixedSize(w);

    // Установка размеров окна
    this->setFixedSize(w);
}
