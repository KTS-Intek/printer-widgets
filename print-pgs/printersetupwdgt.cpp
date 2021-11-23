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

#include <QFileDialog>

///[!] printer-shared
#include "src/printer/printimagehelper.h"
#include "src/printer/qrcodegenerator.h"

///[!] guisett-shared-core
#include "src/nongui/settloader.h"


#define QR_TEXT_MESSAGE "Hi,\nHow are you?"

//----------------------------------------------------------

PrinterSetupWdgt::PrinterSetupWdgt(const QString &defaultimagepath, const bool &generateQrFoced, const bool &verboseMode, QWidget *parent) :
    QWidget(parent), defaultimagepath(defaultimagepath), generateQrFoced(generateQrFoced), verboseMode(verboseMode),
    ui(new Ui::PrinterSetupWdgt)
{
    ui->setupUi(this);

    initObjects();
}

//----------------------------------------------------------

PrinterSetupWdgt::~PrinterSetupWdgt()
{
    delete ui;
}

//----------------------------------------------------------

QPixmap PrinterSetupWdgt::getPagePix()
{

    QPixmap qr;
    PrintImageHelper::PrintSettCache printSett = previewOptions->printSett;
    if(printSett.genearateaqrcode){

        qr = printSett.enqrcustomposition ?
                    QrCodeGenerator::encode(QR_TEXT_MESSAGE, printSett.qrCorrLetter, printSett.qrwidthpx, printSett.qrheightpx) :
                    QrCodeGenerator::getPagePix(printSett, QR_TEXT_MESSAGE);
    }

    QPixmap p = PrintImageHelper::getPixmapWithUserDataExt(previewOptions->printSett, "1234567890abcdef routerni MYDEV", "$eui64 $ni $model", " ", qr);
//    const QVariantMap map = PrintImageHelper::getMapAboutDev(ui->leEUI64->text().simplified().trimmed(), "$eui64 $ni", " ");


    return p;// QrCodeGenerator::getPagePix(ui->sbWidth->value(), ui->sbHeight->value(), ui->sbxResolDpi->value(), ui->cbxQrCodeCorrLevel->currentText(), QR_TEXT_MESSAGE);

}

//----------------------------------------------------------

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

//----------------------------------------------------------

PrintImageHelper::PrintSettCache PrinterSetupWdgt::getPagePrintSett()
{
    PrintImageHelper::PrintSettCache printSett;// = getPagePrintSett();
//Page
    printSett.widthMM               = ui->sbWidth->value();
    printSett.heightMM              = ui->sbHeight->value();
    printSett.top                   = ui->sbMargTop->value();
    printSett.left                  = ui->sbMargL->value();
    printSett.isPortrait            = ui->rbPortrait->isChecked();
    printSett.resolutionDpi         = ui->sbxResolDpi->value();
    printSett.genearateaqrcode      = ui->cbxGenerateAqr->isChecked();


    //Template
    //Image
    printSett.rotateDeg             = getRotateDeg();
    printSett.colorIndx             = ui->cbxImgColor->currentIndex();
    printSett.formatIndx            = ui->cbxImgFormat->currentIndex();
    printSett.contrast              = ui->sbContrast->value();
    printSett.brightness            = ui->sbBrightness->value();
    printSett.backgroundimagepath   = ui->lePath2imgBackGround->text();
    printSett.defaultimagepath      = defaultimagepath;// "";
    printSett.usedefaultimage       = ui->cbxBackGround->isChecked();

//text
    printSett.dateMask              = ui->cbxDateMask->currentText();
    printSett.textRotateDeg         = ui->sbRotateText->value();
    printSett.fontPpt               = ui->sbImgFontSize->value();
    printSett.textTopMargin         = ui->sbTextTopMargin->value();
    printSett.textLeftMargin        = ui->sbTextLeftMargin->value();
    printSett.userData              = ui->pteUserField->toPlainText();

//Qr Code
    printSett.qrCorrLetter          = ui->cbxQrCodeCorrLevel->currentText();
    printSett.enqrcustomposition    = ui->groupBox_4->isChecked();
    printSett.qrtopmarginpx         = ui->sbQrTopMargin->value();
    printSett.qrleftmarginpx        = ui->sbQrLeftMargin->value();
    printSett.qrheightpx            = ui->sbQrHeight->value();
    printSett.qrwidthpx             = ui->sbQrHeight->value();//A QR code is a square




//    QString path2img;



    return printSett;
}

//----------------------------------------------------------

void PrinterSetupWdgt::loadThisSett(QVariantMap map)
{
    PrintImageHelper::PrintSettCache printSett = PrintImageHelper::variantMap2printSett(map);
    //page
    ui->sbWidth->setValue(              printSett.widthMM               );
    ui->sbHeight->setValue(             printSett.heightMM              );
    ui->sbMargTop->setValue(            printSett.top                   );
    ui->sbMargL->setValue(              printSett.left                  );
    ui->rbPortrait->setChecked(         printSett.isPortrait            );
    ui->rbLandscape->setChecked(        !ui->rbPortrait->isChecked()    );
    ui->sbxResolDpi->setValue(          printSett.resolutionDpi         );

    if(ui->cbxGenerateAqr->isEnabled())
        ui->cbxGenerateAqr->setChecked(     printSett.genearateaqrcode      );

    //Template
    switch(printSett.rotateDeg){
    case 90: ui->rbRotate90->setChecked(true); break;
    case 180: ui->rbRotate180->setChecked(true); break;
    case 270: ui->rbRotate270->setChecked(true); break;
    default: ui->rbNoRotation->setChecked(true); break;
    }
    ui->cbxImgColor->setCurrentIndex(   printSett.colorIndx             );
    ui->cbxImgFormat->setCurrentIndex(  printSett.formatIndx            );
    ui->sbContrast->setValue(           printSett.contrast              );
    ui->sbBrightness->setValue(         printSett.brightness            );
    ui->lePath2imgBackGround->setText(  printSett.backgroundimagepath   );
    ui->cbxBackGround->setChecked(      printSett.usedefaultimage       );

    //text
    ui->cbxDateMask->setCurrentIndex(   qMax(0, ui->cbxDateMask->findText(printSett.dateMask)));
    ui->sbRotateText->setValue(         printSett.textRotateDeg         );
    ui->sbImgFontSize->setValue(        printSett.fontPpt               );
    ui->sbTextTopMargin->setValue(      printSett.textTopMargin         );
    ui->sbTextLeftMargin->setValue(     printSett.textLeftMargin        );
    ui->pteUserField->setPlainText(     printSett.userData              );

    //Qr Code
    ui->cbxQrCodeCorrLevel->setCurrentIndex(qMax(0, ui->cbxQrCodeCorrLevel->findText(printSett.qrCorrLetter)));
    ui->groupBox_4->setChecked(         printSett.enqrcustomposition    );
    ui->sbQrTopMargin->setValue(        printSett.qrtopmarginpx         );
    ui->sbQrLeftMargin->setValue(       printSett.qrleftmarginpx        );
    ui->sbQrHeight->setValue(           printSett.qrheightpx            );
    ui->sbQrWidth->setValue(            printSett.qrheightpx             );//QR has a square form
}


//----------------------------------------------------------

void PrinterSetupWdgt::initObjects()
{
    previewOptions = new PreviewImageWdgt(true, verboseMode, this);
    connect(this, SIGNAL(setReadyImage(QPixmap,QString)), previewOptions, SLOT(setReadyImage(QPixmap,QString)) );
    ui->vl4preview->addWidget(previewOptions);

    ui->tabWidget->setCurrentIndex(0);

    ui->cbxImgColor->addItems(PrintImageHelper::getSupportedImageColorConv());
    ui->cbxImgFormat->addItems(PrintImageHelper::getSupportedImageFormat());
    ui->cbxDateMask->addItems(PrintImageHelper::getSupportedDateMask());

    ui->pbReload->animateClick();
    connect(ui->sbTextLeftMargin, SIGNAL(editingFinished()), this, SLOT(onpbUpdateHtml_clicked()) );
    connect(ui->sbTextTopMargin , SIGNAL(editingFinished()), this, SLOT(onpbUpdateHtml_clicked()) );
    connect(ui->sbImgFontSize   , SIGNAL(editingFinished()), this, SLOT(onpbUpdateHtml_clicked()) );

    QTimer *t = new QTimer(this);
    t->setSingleShot(true);
    t->setInterval(222);
    connect(t, SIGNAL(timeout()), this, SLOT(onpbUpdateHtml_clicked()) );
    connect(this, SIGNAL(stopTmrUpdate()), t, SLOT(stop()) );

    ui->cbxQrCodeCorrLevel->addItems(QrCodeGenerator::getCorrection());

    if(generateQrFoced){
        ui->cbxGenerateAqr->setEnabled(false);
        ui->cbxGenerateAqr->setChecked(true);
    }
//Page
    connect(ui->sbWidth             , SIGNAL(valueChanged(int))         , t, SLOT(start()));
    connect(ui->sbHeight            , SIGNAL(valueChanged(int))         , t, SLOT(start()));
    connect(ui->sbxResolDpi         , SIGNAL(valueChanged(int))         , t, SLOT(start()));
    connect(ui->rbLandscape         , SIGNAL(clicked(bool))             , t, SLOT(start()));
    connect(ui->rbPortrait          , SIGNAL(clicked(bool))             , t, SLOT(start()));
    connect(ui->sbMargL             , SIGNAL(valueChanged(int))         , t, SLOT(start()));
    connect(ui->sbMargTop           , SIGNAL(valueChanged(int))         , t, SLOT(start()));
    connect(ui->cbxGenerateAqr      , SIGNAL(clicked(bool))             , t, SLOT(start()) );

    //Template
    //Image
    connect(ui->rbNoRotation        , SIGNAL(clicked(bool))             , t, SLOT(start()) );
    connect(ui->rbRotate90          , SIGNAL(clicked(bool))             , t, SLOT(start()) );
    connect(ui->rbRotate180         , SIGNAL(clicked(bool))             , t, SLOT(start()) );
    connect(ui->rbRotate270         , SIGNAL(clicked(bool))             , t, SLOT(start()) );
    connect(ui->cbxImgColor         , SIGNAL(currentIndexChanged(int))  , t, SLOT(start()) );
    connect(ui->cbxImgFormat        , SIGNAL(currentIndexChanged(int))  , t, SLOT(start()) );
    connect(ui->sbContrast          , SIGNAL(valueChanged(int))         , t, SLOT(start()) );
    connect(ui->sbBrightness        , SIGNAL(valueChanged(int))         , t, SLOT(start()) );
    connect(ui->lePath2imgBackGround, SIGNAL(textChanged(QString))      , t, SLOT(start()));
    connect(ui->cbxBackGround       , SIGNAL(clicked(bool))             , t, SLOT(start()));

    //Text
    connect(ui->cbxDateMask         , SIGNAL(currentIndexChanged(int))  , t, SLOT(start()) );
    connect(ui->sbRotateText        , SIGNAL(valueChanged(int))         , t, SLOT(start()) );
    connect(ui->sbImgFontSize       , SIGNAL(valueChanged(int))         , t, SLOT(start()) );
    connect(ui->sbTextLeftMargin    , SIGNAL(valueChanged(int))         , t, SLOT(start()) );
    connect(ui->sbTextTopMargin     , SIGNAL(valueChanged(int))         , t, SLOT(start()) );
    connect(ui->pteUserField        , SIGNAL(textChanged())             , t, SLOT(start()) );

//QR Code
    connect(ui->cbxQrCodeCorrLevel  , SIGNAL(currentIndexChanged(int))  , t, SLOT(start()));
    connect(ui->groupBox_4          , SIGNAL(clicked(bool))             , t, SLOT(start()));
    connect(ui->sbQrTopMargin       , SIGNAL(valueChanged(int))         , t, SLOT(start()));
    connect(ui->sbQrLeftMargin      , SIGNAL(valueChanged(int))         , t, SLOT(start()));
    connect(ui->sbQrHeight          , SIGNAL(valueChanged(int))         , t, SLOT(start()));
//    connect(ui->sbQrWidth           , SIGNAL(valueChanged(int))         , t, SLOT(start()));

    connect(ui->tabWidget   , SIGNAL(currentChanged(int)), t, SLOT(start()) );


    connect(ui->sbRotateText, SIGNAL(valueChanged(int)), ui->hsRotateText, SLOT(setValue(int)) );
    connect(ui->hsRotateText, SIGNAL(valueChanged(int)), ui->sbRotateText, SLOT(setValue(int)) );

    connect(ui->pbCancel    , SIGNAL(clicked(bool)), this, SIGNAL(youCanCloseMe()));

    connect(ui->sbQrHeight, SIGNAL(valueChanged(int)), ui->sbQrWidth, SLOT(setValue(int)));
    ui->sbQrWidth->setValue(ui->sbQrHeight->value());


}

//----------------------------------------------------------

void PrinterSetupWdgt::on_pbReload_clicked()
{
    emit sendMeSett();//request to update settings

}

//----------------------------------------------------------

void PrinterSetupWdgt::on_pbSave_clicked()
{
    const PrintImageHelper::PrintSettCache printSett = getPagePrintSett();


    emit settSaved(PrintImageHelper::printSett2variantMap(printSett));
    emit youCanCloseMe();
}

//----------------------------------------------------------

void PrinterSetupWdgt::onpbUpdateHtml_clicked()
{
    previewOptions->printSett = getPagePrintSett();
    emit setReadyImage(getPagePix(), QR_TEXT_MESSAGE);//00 0D 6F 00  00 7A 8E 23
}

//----------------------------------------------------------

void PrinterSetupWdgt::on_pbPrint_clicked()
{

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setResolution(ui->sbxResolDpi->value());
    printer.setPageSize(QPageSize(PrintImageHelper::getPageSize(ui->sbWidth->value(), ui->sbHeight->value(), ui->rbPortrait->isChecked()), QPageSize::Millimeter));
    printer.setOrientation( ui->rbPortrait->isChecked() ? QPrinter::Portrait : QPrinter::Landscape);
    printer.setPageOrientation(ui->rbPortrait->isChecked() ? QPageLayout::Portrait : QPageLayout::Landscape);
    printer.setPageMargins(QMargins(ui->sbMargL->value(), ui->sbMargTop->value(), 0, 0));// ui->sbMargR->value(), ui->sbMargB->value()));
    printer.setFullPage(true);

    QPrintDialog *d = new QPrintDialog(&printer, this);
    if(d->exec() == QDialog::Accepted){
        d->printer()->setFullPage(true);
        printer.setResolution(ui->sbxResolDpi->value());
        printer.setPageSize(QPageSize(PrintImageHelper::getPageSize(ui->sbWidth->value(), ui->sbHeight->value(), ui->rbPortrait->isChecked()), QPageSize::Millimeter));
        printer.setPageMargins(QMargins(ui->sbMargL->value(), ui->sbMargTop->value(), 0, 0));// ui->sbMargR->value(), ui->sbMargB->value()));

        printer.setOrientation( ui->rbPortrait->isChecked() ? QPrinter::Portrait : QPrinter::Landscape);
        printer.setPageOrientation(ui->rbPortrait->isChecked() ? QPageLayout::Portrait : QPageLayout::Landscape);

//        if(ui->cbxImgColor->currentIndex() == 2)
//            printer.setColorMode(QPrinter::GrayScale);
        if(verboseMode){
            qDebug() << "marginB " << printer.margins().left << printer.margins().top;
            qDebug() << "sizeB" << printer.pageSizeMM() << printer.pageSize();
        }

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

//----------------------------------------------------------

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
    printer.setPageMargins(QMargins(ui->sbMargL->value(), ui->sbMargTop->value(), 0, 0));// ui->sbMargR->value(), ui->sbMargB->value()));

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

//----------------------------------------------------------

void PrinterSetupWdgt::on_pbDefault_clicked()
{
    loadThisSett(PrintImageHelper::printSett2variantMap(PrintImageHelper::defaultPrintSett()));
}

//----------------------------------------------------------

void PrinterSetupWdgt::on_toolButton_clicked()
{
    //    tryToImportFile(QFileDialog::getOpenFileName(this, tr("Open a file"),  SettLoader::getValidLastDir(), tr("Plain text(*.txt);;All files (*)")), false);

    QStringList l;
    l.append(tr("PNG image(*.png)"));
    l.append(tr("JPG image(*.jpg *.jpeg)"));
    l.append(tr("BMP image(*.bmp)"));
    l.append(tr("PPM image(*.ppm)"));
    l.append(tr("XBM image(*.xbm)"));
    l.append(tr("XPM image(*.xpm)"));

    QFileInfo fi(ui->lePath2imgBackGround->text());

    const QString path2dir =  ui->lePath2imgBackGround->text().isEmpty() ?
                SettLoader::getValidLastDir() :
                fi.absoluteDir().absolutePath();

    const QString filename = QFileDialog::getOpenFileName(this, tr("Get a background image"),path2dir, l.join(";;"));

    if(filename.isEmpty())
        return;


    ui->lePath2imgBackGround->setText(filename);

//                                 QStandardPaths::writableLocation(QStandardPaths::HomeLocation),  tr("PNG Images (*.png);;All files (*)"))
}

//----------------------------------------------------------

void PrinterSetupWdgt::on_groupBox_4_clicked()
{
//    ui->groupBox_4->setChecked(!ui->groupBox_4->isChecked());
}
