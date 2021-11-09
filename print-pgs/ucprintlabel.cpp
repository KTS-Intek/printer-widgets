#include "ucprintlabel.h"
#include "ui_ucprintlabel.h"

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


UcPrintLabel::UcPrintLabel(GuiHelper *gHelper, QWidget *parent) :
    MatildaConfWidget(gHelper, parent),
    ui(new Ui::UcPrintLabel)
{
    ui->setupUi(this);
}

UcPrintLabel::~UcPrintLabel()
{
    delete ui;
}

QVariantMap UcPrintLabel::getThisDevPrinterSett() const
{
    return SettLoader::loadSett(SETT_PRINT_SETTMAP).toMap().value(QString::number(QR_DEVTYPE_UC)).toMap();// lastQrGenSett.qrDevType)).toMap();

}

void UcPrintLabel::initPage()
{
    connect(ui->pbCancel, SIGNAL(clicked(bool)), this, SLOT(deleteLater()) );

    previewMain = new PreviewImageWdgt(false, gHelper->verboseMode, this);
    connect(this, SIGNAL(setMainImage(QPixmap,QVariantMap,QString)), previewMain, SLOT(setImage(QPixmap,QVariantMap,QString)));
    connect(this, SIGNAL(clearMainImage()), previewMain, SLOT(clearImage()) );
    ui->vl4previewMain->addWidget(previewMain);


//    PrinterHelper *ph = new PrinterHelper(ui->cbxPrinter, this);
//    connect(ui->tbUpdatePrint, SIGNAL(clicked(bool)), ph, SLOT(request2update()));
//    ph->request2update();

    createCloseShortCut();
     clearPage();

     updatePrintSett();
     updateNameOfPrinter();

     ui->cbxModel->clear();
     ui->cbxModel->setCurrentIndex(-1);
     ui->cbxModel->addItems(QString("KI-UC-BB-C-032 KI-UC-BB-HC-033 KI-UC-BB-ZC-034 KI-UC-BB-HZC-035 KI-UC-BB-I-036 KI-UC-BB-HI-037 KI-UC-BB-ZI-038 KI-UC-BB-HZI-039").split(" ", QString::SkipEmptyParts));

     connect(ui->cbxModel, SIGNAL(currentIndexChanged(int)), this, SLOT(onCbxModel_currentIndexChanged(int)) );
     ui->cbxModel->setCurrentIndex(1);

     ui->rbwidetemp->setEnabled(false);
     ui->rbroomtemp->setEnabled(false);

//     ui->pbPrint->setEnabled(false);

//     ui->tbFilter->hide();



//     QTimer::singleShot(111, this, SLOT(restoreModelFromQrGenSett()) );

     {
         QTimer *t = new QTimer(this);
         t->setSingleShot(true);
         t->setInterval(333);

         connect(ui->cbxModel, SIGNAL(currentIndexChanged(int)), t, SLOT(start()) );
         connect(ui->leeui64, SIGNAL(textChanged(QString)), t, SLOT(start()) );
         connect(ui->lesn, SIGNAL(textChanged(QString)), t, SLOT(start()) );
         connect(ui->lemac, SIGNAL(textChanged(QString)), t, SLOT(start()) );
         connect(ui->leimei, SIGNAL(textChanged(QString)), t, SLOT(start()) );
         connect(ui->rbwidetemp, SIGNAL(toggled(bool)), t, SLOT(start()) );
         connect(ui->rbroomtemp, SIGNAL(toggled(bool)), t, SLOT(start()) );
         connect(t, SIGNAL(timeout()), this, SLOT(onInputDataChanged()));
         t->start();
     }

     const QDateTime dt = QDateTime::currentDateTime();
     ui->lePartNumber->setText(QString("%1W%2-001").arg(dt.toString("yy")).arg(dt.date().weekNumber()));
}

void UcPrintLabel::clearPage()
{
    ui->lesn->clear();
    ui->lemac->clear();
    ui->leeui64->clear();
    ui->leimei->clear();
    ui->rbroomtemp->setChecked(true);
    ui->rbwidetemp->setChecked(false);

    ui->stackedWidget->setCurrentIndex(0);
    ui->cbxModel->setCurrentIndex(-1);

    emit clearMainImage();

}

void UcPrintLabel::settSaved(QVariantMap deviceMap)
{
    if(deviceMap.isEmpty())
        return;
    QVariantMap printersett = SettLoader::loadSett(SETT_PRINT_SETTMAP).toMap();
    printersett.insert(QString::number(QR_DEVTYPE_UC), deviceMap);//lastQrGenSett.qrDevType), deviceMap);
    SettLoader::saveSett(SETT_PRINT_SETTMAP, printersett);
}

void UcPrintLabel::sendMeSett()
{
    emit setThisDevPrinterSett(getThisDevPrinterSett());

}

void UcPrintLabel::updatePrintSett()
{
    //QVM  <devType> <QVM sett 2 val>
//    const QVariantMap m = SettLoader::loadSett(SETT_PRINT_SETTMAP).toMap().value(QString::number(lastQrGenSett.qrDevType)).toMap();

    printSett = PrintImageHelper::variantMap2printSett(getThisDevPrinterSett());
    previewMain->setPrintSett(printSett);
}

void UcPrintLabel::updateNameOfPrinter()
{
//    PrinterHelper::getCbxIndx(SettLoader::loadSett(SETT_PRINT_IMG_LASTPRINTER).toString(), ui->cbxPrinter);
//    ui->sbCopies->setValue(SettLoader::loadSett(SETT_PRINT_IMG_LASTCOPIES, 1).toInt());
}

void UcPrintLabel::saveNameOfPrinter()
{
//    SettLoader::saveSett(SETT_PRINT_IMG_LASTPRINTER, ui->cbxPrinter->currentText());
//    SettLoader::saveSett(SETT_PRINT_IMG_LASTCOPIES, ui->sbCopies->value());
}

void UcPrintLabel::createCloseShortCut()
{
    connect(new QShortcut(QKeySequence(Qt::Key_Escape), this), SIGNAL(activated()), this, SLOT(onEscape()) );

}

void UcPrintLabel::on_pbSave_clicked()
{

}

void UcPrintLabel::on_pbSettings_clicked()
{
    PrinterSetupWdgt *w = new PrinterSetupWdgt("", false, gHelper->verboseMode, this);
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

void UcPrintLabel::onEscape()
{
    if(ui->stackedWidget->count() > 1){
        emit delCloseShortCut();
        return;
    }
    ui->pbCancel->animateClick();
}

void UcPrintLabel::restoreModelFromQrGenSett()
{
//    validlist.clear();
//    const QStringList lk = QrGenConverter::getKeys4dev(QR_DEVTYPE_UC);//lastQrGenSett.qrDevType);
//    if(lk.isEmpty())
//        return;




//    StandardItemModelHelper::setModelHorizontalHeaderItems(model, QrGenConverter::keys2human(lk));
//    int rowsAdd = 0;
//    for(int i = 0, imax = lastQrGenSett.list.size(), cmax = lk.size(); i < imax; i++){
//        const QrTableMap m = lastQrGenSett.list.at(i);
//        QList<QStandardItem*> li;
//        for(int c = 0; c < cmax; c++)
//            li.append(new QStandardItem(m.value(lk.at(c))));

//        model->appendRow(li);
//        validlist.append(m);
//        rowsAdd++;
//    }

//    if(rowsAdd > 0){
//        ui->pbPrint->setEnabled(true);

//        connect(ui->tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(onTableView_activated(QModelIndex)) );
//        ui->tableView->selectRow(0);
//        onTableView_activated(ui->tableView->currentIndex());

//    }

}

void UcPrintLabel::onCbxModel_currentIndexChanged(int index)
{
    bool haseui64 = false;
    bool hasimei = false;
    bool isWideRange = false;
    ////KI-UC-BB-C-032
    /// KI-UC-BB-HC-033
    /// KI-UC-BB-ZC-034
    /// KI-UC-BB-HZC-035
    /// KI-UC-BB-I-036
    /// KI-UC-BB-HI-037
    /// KI-UC-BB-ZI-038
    /// KI-UC-BB-HZI-039").split(" ", QString::SkipEmptyParts));

    switch(index){
    case 0: break;
    case 1: hasimei = true              ; break;
    case 2: haseui64 = true             ; break;
    case 3: hasimei = haseui64 = true   ; break;
    case 4: isWideRange = true                  ; break;
    case 5: hasimei = true  ; isWideRange = true; break;
    case 6: haseui64 = true ; isWideRange = true; break;
    case 7: hasimei = haseui64 = true; isWideRange = true; break;
    }

    ui->leeui64->setEnabled(haseui64);
    ui->leimei->setEnabled(hasimei);
    ui->rbwidetemp->setChecked(isWideRange);
    ui->rbroomtemp->setChecked(!isWideRange);
}

void UcPrintLabel::onInputDataChanged()
{
//    validlist.clear();
    const QStringList lk = QrGenConverter::getKeys4dev(QR_DEVTYPE_UC);//lastQrGenSett.qrDevType);
    if(lk.isEmpty())
        return;
    //model sn mac eui64 imei
    QrTableMap m;
    m.insert("model", ui->cbxModel->currentText());
    m.insert("sn"   , ui->lesn->text().simplified().trimmed());
    m.insert("mac"  , ui->lemac->text().simplified().trimmed().toLower());
    m.insert("eui64", ui->leeui64->isEnabled() ? ui->leeui64->text().simplified().trimmed().toUpper() : "-");
    m.insert("imei" , ui->leimei->isEnabled() ? ui->leimei->text().simplified().trimmed().toUpper() : "-");

    const QString s = QrGenConverter::getDeviceLine( m , QR_DEVTYPE_UC);// lastQrGenSett.qrDevType);

    if(s.isEmpty())
        return;
//    if(!ui->tbShowHide->isChecked())
//        ui->tbShowHide->animateClick();
//    map.insert("NI",
//    getPagePix(const int &wmm, const int &hmm, const int &dpi, const QString &corrlvl, const QString &s);
    emit setMainImage(QrCodeGenerator::getPagePix(printSett.widthMM, printSett.heightMM, printSett.resolutionDpi, printSett.qrCorrLetter, s), QrGenConverter::getMapAboutDev(m), s);


}

void UcPrintLabel::on_pbHtml_clicked()
{

}
