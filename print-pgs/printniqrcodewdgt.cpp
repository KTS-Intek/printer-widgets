#include "printniqrcodewdgt.h"
#include "ui_printniqrcodewdgt.h"

#include <QShortcut>


///[!] printer-widgets
#include "print-pgs/printersetupwdgt.h"
#include "src/printer/printerhelper.h"
#include "src/printer/exportwdgt2qrcodegen.h"


///[!] guisett-shared-core
#include "src/nongui/settloader.h"


///[!] printer-shared
#include "src/printer/qrcodegenerator.h"
#include "src/printer/qrgenconverter.h"


#include "moji_defy.h"

PrintNiQrCodeWdgt::PrintNiQrCodeWdgt(LastDevInfo *lDevInfo, GuiHelper *gHelper, GuiSett4all *gSett4all, QWidget *parent) :
    ReferenceWidgetClass(lDevInfo, gHelper, gSett4all, parent),
    ui(new Ui::PrintNiQrCodeWdgt)
{
    ui->setupUi(this);
}

PrintNiQrCodeWdgt::~PrintNiQrCodeWdgt()
{
    delete ui;
}

QVariantMap PrintNiQrCodeWdgt::getThisDevPrinterSett() const
{
    return SettLoader::loadSett(SETT_PRINT_SETTMAP).toMap().value(QString::number(lastQrGenSett.qrDevType)).toMap();
}

void PrintNiQrCodeWdgt::initPage()
{

    setupObjects(ui->tableView, ui->tbFilter, ui->cbxFilter, ui->leFilter, SETT_FILTERS_QRPRINT_PWR);
    setupTbShowHide(ui->tbShowHide, ui->widget, false, false);

    connect(ui->pbCancel, SIGNAL(clicked(bool)), this, SLOT(deleteLater()) );

    previewMain = new PreviewImageWdgt(false, this);
    connect(this, SIGNAL(setMainImage(QPixmap,QVariantMap,QString)), previewMain, SLOT(setImage(QPixmap,QVariantMap,QString)));
    connect(this, SIGNAL(clearMainImage()), previewMain, SLOT(clearImage()) );
    ui->vl4previewMain->addWidget(previewMain);


    PrinterHelper *ph = new PrinterHelper(ui->cbxPrinter, this);
    connect(ui->tbUpdatePrint, SIGNAL(clicked(bool)), ph, SLOT(request2update()));
    ph->request2update();

    createCloseShortCut();
     clearPage();

     updatePrintSett();
     updateNameOfPrinter();

     ui->pbPrint->setEnabled(false);

     ui->tbFilter->hide();
     QTimer::singleShot(111, this, SLOT(restoreModelFromQrGenSett()) );
}

void PrintNiQrCodeWdgt::clearPage()
{
    model->clear();
    ui->stackedWidget->setCurrentIndex(0);

    emit clearMainImage();


}

void PrintNiQrCodeWdgt::settSaved(QVariantMap deviceMap)
{
    if(deviceMap.isEmpty())
        return;
    QVariantMap printersett = SettLoader::loadSett(SETT_PRINT_SETTMAP).toMap();
    printersett.insert(QString::number(lastQrGenSett.qrDevType), deviceMap);
    SettLoader::saveSett(SETT_PRINT_SETTMAP, printersett);
}

void PrintNiQrCodeWdgt::sendMeSett()
{
    emit setThisDevPrinterSett(getThisDevPrinterSett());
}

void PrintNiQrCodeWdgt::updatePrintSett()
{
    //QVM  <devType> <QVM sett 2 val>
//    const QVariantMap m = SettLoader::loadSett(SETT_PRINT_SETTMAP).toMap().value(QString::number(lastQrGenSett.qrDevType)).toMap();

    printSett = PrintImageHelper::variantMap2printSett(getThisDevPrinterSett());
    previewMain->setPrintSett(printSett);
}

void PrintNiQrCodeWdgt::updateNameOfPrinter()
{
    PrinterHelper::getCbxIndx(SettLoader::loadSett(SETT_PRINT_IMG_LASTPRINTER).toString(), ui->cbxPrinter);
    ui->sbCopies->setValue(SettLoader::loadSett(SETT_PRINT_IMG_LASTCOPIES, 1).toInt());
}

void PrintNiQrCodeWdgt::saveNameOfPrinter()
{
    SettLoader::saveSett(SETT_PRINT_IMG_LASTPRINTER, ui->cbxPrinter->currentText());
    SettLoader::saveSett(SETT_PRINT_IMG_LASTCOPIES, ui->sbCopies->value());
}

void PrintNiQrCodeWdgt::createCloseShortCut()
{
    connect(new QShortcut(QKeySequence(Qt::Key_Escape), this), SIGNAL(activated()), this, SLOT(onEscape()) );

}

void PrintNiQrCodeWdgt::on_tableView_customContextMenuRequested(const QPoint &pos)
{        
    gHelper->createCustomMenu(pos, ui->tableView, GuiHelper::ShowOnlyCopy|GuiHelper::ShowExport|GuiHelper::ShowReset, CLBRD_QR_PRINT, windowTitle());

}

void PrintNiQrCodeWdgt::on_pbSettings_clicked()
{
    PrinterSetupWdgt *w = new PrinterSetupWdgt(this);
    connect(w, SIGNAL(youCanCloseMe())          , w, SLOT(deleteLater()) );
    connect(w, SIGNAL(settSaved(QVariantMap))   , this, SLOT(settSaved(QVariantMap)) );
    connect(w, SIGNAL(sendMeSett())             , this, SLOT(sendMeSett()) );
    connect(w, SIGNAL(destroyed(QObject*))      , this, SLOT(updatePrintSett()) );


    connect(this, SIGNAL(delCloseShortCut()), w, SLOT(deleteLater()) );
    connect(this, SIGNAL(setThisDevPrinterSett(QVariantMap)), w, SLOT(loadThisSett(QVariantMap)));

    ui->stackedWidget->addWidget(w);
    ui->stackedWidget->setCurrentWidget(w);

    sendMeSett();//
}

void PrintNiQrCodeWdgt::on_pbPrint_clicked()
{
    QList<QPixmap> lp;
    saveNameOfPrinter();

    const QStringList keys4dev = QrGenConverter::getKeys4dev(lastQrGenSett.qrDevType);
    const int keys4devLen = keys4dev.size();

    for(int i = 0, imax = proxy_model->rowCount(); i < imax; i++){

        const int row = proxy_model->mapToSource(proxy_model->index(i,0)).row();

        const QString s = QrGenConverter::getDeviceLine( validlist.at(row), lastQrGenSett.qrDevType, keys4dev, keys4devLen);
        if(s.isEmpty())
            continue;

        const QPixmap qr = QrCodeGenerator::getPagePix(printSett.widthMM, printSett.heightMM, printSett.resolutionDpi, printSett.qrCorrLetter, s);//only qr
        const QPixmap p = PrintImageHelper::getPixmapWithUserData(printSett.userData, printSett.dateMask, printSett.fontPpt, printSett.textTopMargin,
                                                            printSett.textLeftMargin, QrGenConverter::getMapAboutDev(validlist.at(row)), printSett.textRotateDeg, qr);


        lp.append(PrintImageHelper::convertImage(printSett.formatIndx, printSett.colorIndx, PrintImageHelper::rotateImage(printSett.rotateDeg, p), printSett.contrast, printSett.brightness) );
    }

    if(lp.isEmpty()){
        emit showMess(tr("Nothing to print"));
        return;
    }


    QString pdfFileName;
    if(ui->cbxPrinter->currentIndex() < 1){
        QString filename = QFileDialog::getSaveFileName(this, tr("Save as"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
        if(filename.isEmpty())
            return;
        if(filename.right(4).toLower() != ".pdf")
            filename.append(".pdf");
        pdfFileName = filename;
    }
    const QString mess = PrintImageHelper::printPixmaps(ui->cbxPrinter->currentIndex() < 1 ? "" : ui->cbxPrinter->currentText(), pdfFileName, ui->sbCopies->value(),
                                   printSett.resolutionDpi,
                                PrintImageHelper::getPageSize(printSett.widthMM, printSett.heightMM, printSett.isPortrait), printSett.isPortrait,
                                QMargins(printSett.left,printSett.top,printSett.right,printSett.bottom), lp);


    if(!mess.isEmpty())
        emit showMess(mess);



}

void PrintNiQrCodeWdgt::onEscape()
{
    if(ui->stackedWidget->count() > 1){
        emit delCloseShortCut();
        return;
    }
    ui->pbCancel->animateClick();
}

void PrintNiQrCodeWdgt::restoreModelFromQrGenSett()
{
    validlist.clear();
    const QStringList lk = QrGenConverter::getKeys4dev(lastQrGenSett.qrDevType);
    if(lk.isEmpty())
        return;

    StandardItemModelHelper::modelSetHorizontalHeaderItems(model, QrGenConverter::keys2human(lk));
    int rowsAdd = 0;
    for(int i = 0, imax = lastQrGenSett.list.size(), cmax = lk.size(); i < imax; i++){
        const QrTableMap m = lastQrGenSett.list.at(i);
        QList<QStandardItem*> li;
        for(int c = 0; c < cmax; c++)
            li.append(new QStandardItem(m.value(lk.at(c))));

        model->appendRow(li);
        validlist.append(m);
        rowsAdd++;
    }

    if(rowsAdd > 0){
        ui->pbPrint->setEnabled(true);

        connect(ui->tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(onTableView_activated(QModelIndex)) );
        ui->tableView->selectRow(0);
        onTableView_activated(ui->tableView->currentIndex());

    }


}

void PrintNiQrCodeWdgt::onTableView_activated(const QModelIndex &index)
{
    if(!index.isValid())
        return;

    const int pos = proxy_model->mapToSource(index).row();
    if(pos < 0)
        return;

    const QString s = QrGenConverter::getDeviceLine( validlist.at(pos) , lastQrGenSett.qrDevType);

    if(s.isEmpty())
        return;
    if(!ui->tbShowHide->isChecked())
        ui->tbShowHide->animateClick();
//    map.insert("NI",
//    getPagePix(const int &wmm, const int &hmm, const int &dpi, const QString &corrlvl, const QString &s);
    emit setMainImage(QrCodeGenerator::getPagePix(printSett.widthMM, printSett.heightMM, printSett.resolutionDpi, printSett.qrCorrLetter, s), QrGenConverter::getMapAboutDev(validlist.at(pos)), s);


}

