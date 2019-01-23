#include "printersetupwdgt.h"
#include "ui_printersetupwdgt.h"

#include <QFile>
#include <QPainter>
#include <QTextItem>
#include <QDebug>
#include <QPrintDialog>
#include <QPrinter>
#include <QDesktopServices>
#include <QFileDialog>
#include <QStandardPaths>
#include <QTimer>
#include <QDebug>
#include <QShortcut>

///[!] printer-shared
#include "src/printer/printimagehelper.h"
#include "src/printer/qrcodegenerator.h"

#define QR_TEXT_MESSAGE "Hi,\nHow are you?"

PrinterSetupWdgt::PrinterSetupWdgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrinterSetupWdgt)
{
    ui->setupUi(this);

    initObjects();
}

PrinterSetupWdgt::~PrinterSetupWdgt()
{
    delete ui;
}

QPixmap PrinterSetupWdgt::getPagePix()
{
    return QrCodeGenerator::getPagePix(ui->sbWidth->value(), ui->sbHeight->value(), ui->sbxResolDpi->value(), ui->cbxQrCodeCorrLevel->currentText(), QR_TEXT_MESSAGE);

}

int PrinterSetupWdgt::getRotateDeg()
{
    int rotateDeg = 0;
    if(ui->rbRotate90->isChecked())
        rotateDeg = 90;
    if(ui->rbRotate180->isChecked())
        rotateDeg = 180;
    if(ui->rbRotate270->isChecked())
        rotateDeg = 270;
    return rotateDeg;
}

void PrinterSetupWdgt::loadThisSett(QVariantMap map)
{
    PrintImageHelper::PrintSettCache printSett = PrintImageHelper::variantMap2printSett(map);

    ui->sbWidth->setValue(printSett.widthMM);
    ui->sbHeight->setValue(printSett.heightMM);
    ui->sbMargTop->setValue(printSett.top);
    ui->sbMargR->setValue(printSett.right);
    ui->sbMargB->setValue(printSett.bottom);
    ui->sbMargL->setValue(printSett.left);

    ui->rbPortrait->setChecked(printSett.isPortrait);
    ui->rbLandscape->setChecked(!ui->rbPortrait->isChecked());

    ui->sbxResolDpi->setValue(printSett.resolutionDpi);

    ui->pteUserField->setPlainText(printSett.userData);
    ui->sbImgFontSize->setValue(printSett.fontPpt);
    ui->sbTextTopMargin->setValue(printSett.textTopMargin);
    ui->sbTextLeftMargin->setValue(printSett.textLeftMargin);

    ui->sbBrightness->setValue(printSett.brightness);
    ui->sbContrast->setValue(printSett.contrast);

    switch(printSett.rotateDeg){
    case 90: ui->rbRotate90->setChecked(true); break;
    case 180: ui->rbRotate180->setChecked(true); break;
    case 270: ui->rbRotate270->setChecked(true); break;
    default: ui->rbNoRotation->setChecked(true); break;
    }

    ui->cbxImgFormat->setCurrentIndex(printSett.formatIndx);

    ui->cbxImgColor->setCurrentIndex(printSett.colorIndx);

    ui->cbxDateMask->setCurrentIndex(qMax(0, ui->cbxDateMask->findText(printSett.dateMask)));

    ui->sbRotateText->setValue(printSett.textRotateDeg);
    ui->cbxQrCodeCorrLevel->setCurrentIndex(qMax(0, ui->cbxQrCodeCorrLevel->findText(printSett.qrCorrLetter)));
}

void PrinterSetupWdgt::initObjects()
{
    previewOptions = new PreviewImageWdgt(true, this);
    connect(this, SIGNAL(setImage(QPixmap,QVariantMap,QString)), previewOptions, SLOT(setImage(QPixmap,QVariantMap,QString)) );
    ui->vl4preview->addWidget(previewOptions);

    ui->tabWidget->setCurrentIndex(0);

    ui->cbxImgColor->addItems(PrintImageHelper::getSupportedImageColorConv());
    ui->cbxImgFormat->addItems(PrintImageHelper::getSupportedImageFormat());
    ui->cbxDateMask->addItems(PrintImageHelper::getSupportedDateMask());

    ui->pbReload->animateClick();
    connect(ui->sbTextLeftMargin, SIGNAL(editingFinished()), this, SLOT(on_pbUpdateHtml_clicked()) );
    connect(ui->sbTextTopMargin , SIGNAL(editingFinished()), this, SLOT(on_pbUpdateHtml_clicked()) );
    connect(ui->sbImgFontSize   , SIGNAL(editingFinished()), this, SLOT(on_pbUpdateHtml_clicked()) );

    QTimer *t = new QTimer(this);
    t->setSingleShot(true);
    t->setInterval(222);
    connect(t, SIGNAL(timeout()), this, SLOT(on_pbUpdateHtml_clicked()) );
    connect(this, SIGNAL(stopTmrUpdate()), t, SLOT(stop()) );



    connect(ui->sbTextLeftMargin, SIGNAL(valueChanged(int)), t, SLOT(start()) );
    connect(ui->sbTextTopMargin , SIGNAL(valueChanged(int)), t, SLOT(start()) );
    connect(ui->sbImgFontSize   , SIGNAL(valueChanged(int)), t, SLOT(start()) );
    connect(ui->sbRotateText    , SIGNAL(valueChanged(int)), t, SLOT(start()) );

    connect(ui->cbxDateMask     , SIGNAL(currentIndexChanged(int)), t, SLOT(start()) );

    connect(ui->pteUserField, SIGNAL(textChanged()), t, SLOT(start()) );
    connect(ui->rbNoRotation, SIGNAL(clicked(bool)), t, SLOT(start()) );
    connect(ui->rbRotate90  , SIGNAL(clicked(bool)), t, SLOT(start()) );
    connect(ui->rbRotate180 , SIGNAL(clicked(bool)), t, SLOT(start()) );
    connect(ui->rbRotate270 , SIGNAL(clicked(bool)), t, SLOT(start()) );

    connect(ui->cbxImgColor , SIGNAL(currentIndexChanged(int)), t, SLOT(start()) );
    connect(ui->cbxImgFormat, SIGNAL(currentIndexChanged(int)), t, SLOT(start()) );

    connect(ui->tabWidget   , SIGNAL(currentChanged(int)), t, SLOT(start()) );



    connect(ui->sbContrast  , SIGNAL(valueChanged(int)), t, SLOT(start()) );
    connect(ui->sbBrightness, SIGNAL(valueChanged(int)), t, SLOT(start()) );

    connect(ui->sbRotateText, SIGNAL(valueChanged(int)), ui->hsRotateText, SLOT(setValue(int)) );
    connect(ui->hsRotateText, SIGNAL(valueChanged(int)), ui->sbRotateText, SLOT(setValue(int)) );

    connect(ui->pbCancel    , SIGNAL(clicked(bool)), this, SIGNAL(youCanCloseMe()));


    ui->cbxQrCodeCorrLevel->addItems(QrCodeGenerator::getCorrection());

}

void PrinterSetupWdgt::on_pbReload_clicked()
{
    emit sendMeSett();//request to update settings

}

void PrinterSetupWdgt::on_pbSave_clicked()
{
    PrintImageHelper::PrintSettCache printSett;

    printSett.widthMM = ui->sbWidth->value();
    printSett.heightMM = ui->sbHeight->value();

    printSett.top = ui->sbMargTop->value();
    printSett.right = ui->sbMargR->value();
    printSett.bottom = ui->sbMargB->value();
    printSett.left= ui->sbMargL->value();

    printSett.isPortrait = ui->rbPortrait->isChecked();
    printSett.resolutionDpi = ui->sbxResolDpi->value();

    printSett.userData = ui->pteUserField->toPlainText();
    printSett.fontPpt = ui->sbImgFontSize->value();
    printSett.textTopMargin = ui->sbTextTopMargin->value();
    printSett.textLeftMargin = ui->sbTextLeftMargin->value();
    printSett.dateMask = ui->cbxDateMask->currentIndex();

    printSett.rotateDeg = getRotateDeg();

    printSett.formatIndx = ui->cbxImgFormat->currentIndex();
    printSett.colorIndx = ui->cbxImgColor->currentIndex();


    printSett.brightness = ui->sbBrightness->value();
    printSett.contrast = ui->sbContrast->value();
    printSett.textRotateDeg = ui->sbRotateText->value();
    printSett.qrCorrLetter = ui->cbxQrCodeCorrLevel->currentText();

    emit settSaved(PrintImageHelper::printSett2variantMap(printSett));
    emit youCanCloseMe();
}

void PrinterSetupWdgt::on_pbUpdateHtml_clicked()
{
    QVariantMap map;
    map.insert("$model", "my_model");
    map.insert("$ni", "89abcdef");
    map.insert("$eui64", "0123456789abcdef");


    previewOptions->printSett.userData          = ui->pteUserField->toPlainText();
    previewOptions->printSett.dateMask          = ui->cbxDateMask->currentText();
    previewOptions->printSett.fontPpt           = ui->sbImgFontSize->value();
    previewOptions->printSett.textTopMargin     = ui->sbTextTopMargin->value();
    previewOptions->printSett.textLeftMargin    = ui->sbTextLeftMargin->value();
    previewOptions->printSett.textRotateDeg     = ui->sbRotateText->value();
    previewOptions->printSett.formatIndx        = ui->cbxImgFormat->currentIndex();
    previewOptions->printSett.colorIndx         = ui->cbxImgColor->currentIndex();
    previewOptions->printSett.rotateDeg         =  getRotateDeg();//ui->cbxIgnoreRotation->isChecked() ? 0 :
    previewOptions->printSett.contrast          = ui->sbContrast->value();
    previewOptions->printSett.brightness        = ui->sbBrightness->value();

    emit setImage(getPagePix(), map, QR_TEXT_MESSAGE);//00 0D 6F 00  00 7A 8E 23
}

void PrinterSetupWdgt::on_pbPrint_clicked()
{

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setResolution(ui->sbxResolDpi->value());
    printer.setPageSize(QPageSize(PrintImageHelper::getPageSize(ui->sbWidth->value(), ui->sbHeight->value(), ui->rbPortrait->isChecked()), QPageSize::Millimeter));
    printer.setOrientation( ui->rbPortrait->isChecked() ? QPrinter::Portrait : QPrinter::Landscape);
    printer.setPageOrientation(ui->rbPortrait->isChecked() ? QPageLayout::Portrait : QPageLayout::Landscape);
    printer.setPageMargins(QMargins(ui->sbMargL->value(), ui->sbMargTop->value(), ui->sbMargR->value(), ui->sbMargB->value()));
    printer.setFullPage(true);

    QPrintDialog *d = new QPrintDialog(&printer, this);
    if(d->exec() == QDialog::Accepted){
        d->printer()->setFullPage(true);
        printer.setResolution(ui->sbxResolDpi->value());
        printer.setPageSize(QPageSize(PrintImageHelper::getPageSize(ui->sbWidth->value(), ui->sbHeight->value(), ui->rbPortrait->isChecked()), QPageSize::Millimeter));
        printer.setPageMargins(QMargins(ui->sbMargL->value(), ui->sbMargTop->value(), ui->sbMargR->value(), ui->sbMargB->value()));

        printer.setOrientation( ui->rbPortrait->isChecked() ? QPrinter::Portrait : QPrinter::Landscape);
        printer.setPageOrientation(ui->rbPortrait->isChecked() ? QPageLayout::Portrait : QPageLayout::Landscape);

//        if(ui->cbxImgColor->currentIndex() == 2)
//            printer.setColorMode(QPrinter::GrayScale);
        qDebug() << "marginB " << printer.margins().left << printer.margins().top;
        qDebug() << "sizeB" << printer.pageSizeMM() << printer.pageSize();

        QPainter painter;

        painter.begin(d->printer());
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
        const QPixmap p = previewOptions->getLastPixmapTxt();/// previewOptions->getLastPixmap();// PrintImageHelper::getPixmapWithUserData(ui->pteUserField->toPlainText(), ui->cbxDateMask->currentText(), ui->sbImgFontSize->value(), ui->sbTextTopMargin->value(), ui->sbTextLeftMargin->value());

//        p = PrintImageHelper::convertImage(ui->cbxImgFormat->currentIndex(), ui->cbxImgColor->currentIndex(),
//                                           PrintImageHelper::rotateImage(getRotateDeg(), p) , ui->sbContrast->value(), ui->sbBrightness->value());

        d->printer()->paintEngine()->drawPixmap( p.rect(), p, p.rect() );
        painter.end();
    }
    d->deleteLater();
}

void PrinterSetupWdgt::on_pushButton_clicked()
{

    QString filename = QFileDialog::getSaveFileName(this, tr("Save as"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    if(filename.isEmpty())
        return;
    if(filename.right(4).toLower() != ".pdf")
        filename.append(".pdf");

    QPrinter printer(QPrinter::HighResolution);
    printer.setFullPage(true);
    printer.setPageSize(QPageSize(PrintImageHelper::getPageSize(ui->sbWidth->value(), ui->sbHeight->value(), ui->rbPortrait->isChecked()), QPageSize::Millimeter));
    printer.setPageMargins(QMargins(ui->sbMargL->value(), ui->sbMargTop->value(), ui->sbMargR->value(), ui->sbMargB->value()));

    printer.setResolution(ui->sbxResolDpi->value());
    printer.setOrientation( ui->rbPortrait->isChecked() ? QPrinter::Portrait : QPrinter::Landscape);
    printer.setPageOrientation(ui->rbPortrait->isChecked() ? QPageLayout::Portrait : QPageLayout::Landscape);
    printer.setOutputFormat(QPrinter::PdfFormat);


    printer.setOutputFileName(filename);

    QPainter painter;

    painter.begin(&printer);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);


    const QPixmap p = previewOptions->getLastPixmapTxt();// previewOptions->getLastPixmap();// PrintImageHelper::getPixmapWithUserData(ui->pteUserField->toPlainText(), ui->cbxDateMask->currentText(), ui->sbImgFontSize->value(), ui->sbTextTopMargin->value(), ui->sbTextLeftMargin->value());

//    p = PrintImageHelper::convertImage(ui->cbxImgFormat->currentIndex(), ui->cbxImgColor->currentIndex(),
//                                       PrintImageHelper::rotateImage(getRotateDeg(), p) , ui->sbContrast->value(), ui->sbBrightness->value());

    printer.paintEngine()->drawPixmap(p.rect(), p, p.rect() );
    painter.end();

#ifdef Q_OS_WIN

        QDesktopServices::openUrl(QUrl("file:///" + filename));

#else
        QDesktopServices::openUrl(QUrl("file://" + filename));
#endif
}

void PrinterSetupWdgt::on_pbDefault_clicked()
{
    loadThisSett(PrintImageHelper::printSett2variantMap(PrintImageHelper::defaultPrintSett()));
}
