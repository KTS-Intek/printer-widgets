#include "previewimagewdgt.h"
#include "ui_previewimagewdgt.h"
#include <QTimer>
#include <QPainter>

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>


///[!] guisett-shared
#include "gui-src/settloader.h"


PreviewImageWdgt::PreviewImageWdgt(const bool &hideRotationSett, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviewImageWdgt)
{
    ui->setupUi(this);

    isControlPressed = false;

    if(hideRotationSett){
//        ui->label_3->setText(tr("Preview"));
        ui->cbxIgnoreRotation->hide();
    }


    if(true){
        QTimer *t = new QTimer(this);
        t->setSingleShot(true);
        t->setInterval(111);
        connect(t, SIGNAL(timeout()), this, SLOT(updatePrintImage()));
        connect(this, SIGNAL(startTmrUpdateImg()), t, SLOT(start()) );

        connect(ui->cbxIgnoreRotation, SIGNAL(clicked(bool)), t, SLOT(start()));
        connect(ui->hsZoom, SIGNAL(valueChanged(int)), t, SLOT(start()));

    }
}

PreviewImageWdgt::~PreviewImageWdgt()
{
    delete ui;
}


QPixmap PreviewImageWdgt::getLastPixmap() const
{
    return lastPix;
}

QPixmap PreviewImageWdgt::getLastPixmapTxt() const
{
    return lastPixTxt;
}

void PreviewImageWdgt::setLastPageSett(bool previewMode, int zoom)
{
    ui->cbxIgnoreRotation->setChecked(previewMode);
    ui->hsZoom->setValue(zoom);
}

void PreviewImageWdgt::updateImage()
{
    emit startTmrUpdateImg();
}

void PreviewImageWdgt::setImage(QPixmap p, QVariantMap map, QString lblTxt)
{
    lastLblTxt = lblTxt;
    lastPix = p;
    lastAbout = map;
    updateImage();
}

void PreviewImageWdgt::setImageData(QVariantMap map)
{
    lastAbout = map;
    updateImage();
}

void PreviewImageWdgt::setPrintSett(const PrintImageHelper::PrintSettCache &printSett)
{
    this->printSett = printSett;
    updateImage();
}

void PreviewImageWdgt::clearImage()
{
    lastLblTxt.clear();
    lastPix = QPixmap(":/katynko/qrhi.png");
    lastAbout.clear();
    updateImage();
}

void PreviewImageWdgt::updatePrintImage()
{
    if(lastPix.isNull()){
        clearImage();
        return;
    }


    QPixmap p = PrintImageHelper::getPixmapWithUserData(printSett.userData, printSett.dateMask, printSett.fontPpt, printSett.textTopMargin,
                                                        printSett.textLeftMargin, lastAbout, printSett.textRotateDeg, lastPix);


    p = PrintImageHelper::convertImage(printSett.formatIndx, printSett.colorIndx,
                                                          ui->cbxIgnoreRotation->isChecked() ? p : PrintImageHelper::rotateImage(printSett.rotateDeg, p), printSett.contrast, printSett.brightness) ;
    p.setDevicePixelRatio(PrintImageHelper::calculateDevicePixelRatio(ui->hsZoom->value(), ui->hsZoom->minimum(), ui->hsZoom->maximum()));// ui->dsbScale->value());
    ui->lblImg->setPixmap(p);
    lastPixTxt = p;
    if(lastLblTxt.isEmpty()){
        if(!ui->lblTxt->isHidden())
            ui->lblTxt->hide();
        return;
    }
    if(ui->lblTxt->isHidden())
        ui->lblTxt->show();
    ui->lblTxt->setText(lastLblTxt);
}

//void PreviewImageWdgt::wheelEvent(QWheelEvent *event)
//{
//    const QPoint numPixels = event->pixelDelta();
//    const QPoint numDegrees = event->angleDelta() / 8;
//    const int step = ui->hsZoom->singleStep();
//    if (!numPixels.isNull()) {
//        ui->hsZoom->setValue( ui->hsZoom->value() + ((numPixels.x() < 0) ? -1 * step : step));
//    } else if (!numDegrees.isNull()) {
//        const QPoint numSteps = numDegrees / 15;
//        ui->hsZoom->setValue( ui->hsZoom->value() + ((numSteps.x() < 0) ? -1 * step : step));

//    }
//    event->accept();
//}

//void PreviewImageWdgt::keyPressEvent(QKeyEvent *event)
//{
//    if(event->key() == Qt::Key_Control){
//        isControlPressed = true;
//    }
//    event->accept();
//}

//void PreviewImageWdgt::keyReleaseEvent(QKeyEvent *event)
//{
//    if(event->key() == Qt::Key_Control){
//        isControlPressed = false;
//    }
//    event->accept();
//}

void PreviewImageWdgt::on_toolButton_clicked()
{
    ui->hsZoom->setValue((ui->hsZoom->minimum() + ui->hsZoom->maximum()) / 2);
}

void PreviewImageWdgt::on_tbSaveAs_clicked()
{
    QString fileNam = QFileDialog::getSaveFileName(this, tr("Save as"), SettLoader::getValidLastDir()
                                                   , tr("Images(*.png *.jpg *.bmp *.ppm *.xbm *.xpm)")) ; //,tr("Readable Files(*.ods  *.csv);;ODS Files(*.ods);;CSV Files(*.csv);;All Files(*.*)"));
    if(!fileNam.isEmpty()){
        //        QFileInfo fi(fileNam);
        //        emit onSlahDoPapkyChngd(fi.absolutePath());

        QString extn = fileNam.right(4).toLower();
        if(!QString(".bmp .jpg .png .ppm .xbm .xpm").split(" ").contains(extn))
            fileNam.append(".png");

        //        int h = pain

        const bool r = lastPixTxt.save(fileNam);
        QMessageBox::information(this, tr("Image"), r ? tr("Done)") :
                                                        tr("Can't save the image"));

        qDebug() << "export to file " << fileNam << r;
    }
}
