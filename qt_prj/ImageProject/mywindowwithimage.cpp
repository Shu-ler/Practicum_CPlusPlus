#include "mywindowwithimage.h"
#include "ui_mywindowwithimage.h"

MyWindowWithImage::MyWindowWithImage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyWindowWithImage)
{
    ui->setupUi(this);

    // Сохраним путь в переменную.
    QString absolute_path_file = "D:/Practicum_CPlusPlus/i-love-qt.png";

    // Вызываем созданный ранее метод.
    SetPixmap(ui->lbl_pixmap, absolute_path_file);
}

MyWindowWithImage::~MyWindowWithImage()
{
    delete ui;
}

void MyWindowWithImage::SetPixmap(QLabel *label_pix, const QString path) {
    QPixmap pix{path};

    // Убедимся, что изображение загрузилось.
    Q_ASSERT(!pix.isNull());

    label_pix->setPixmap(pix);
}

