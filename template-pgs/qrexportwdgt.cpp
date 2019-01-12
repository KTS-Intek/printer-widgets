#include "qrexportwdgt.h"
#include "ui_qrexportwdgt.h"

#include <QShortcut>
#include "src/printer/qrcodegenerator.h"
#include "src/printer/qrgenconverter.h"
#include "gui-src/settloader.h"


QrExportWdgt::QrExportWdgt(LastDevInfo *lDevInfo, GuiHelper *gHelper, GuiSett4all *gSett4all, QWidget *parent) :
    MatildaConfWidget(lDevInfo, gHelper, gSett4all, parent),
    ui(new Ui::QrExportWdgt)
{
    ui->setupUi(this);
    lastQrGenSett.qrDevType = QR_DEVTYPE_UNK;
}

QrExportWdgt::~QrExportWdgt()
{
    delete ui;
}

QVariantMap QrExportWdgt::getThisDevPrinterSett() const
{
    return SettLoader::loadSett(SETT_PRINT_SETTMAP).toMap().value(QString::number(lastQrGenSett.qrDevType)).toMap();
}

void QrExportWdgt::setQrs(QStringList list)
{
    list2print = list;
    lastPosition = 0;
    lastSize = list.size();
}

void QrExportWdgt::initPage()
{
    connect(ui->pbCancel, SIGNAL(clicked(bool)), this, SLOT(deleteLater()) );

    QShortcut *s = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    connect(s, SIGNAL(activated()), this, SLOT(deleteLater())) ;

    previewMain = new PreviewImageWdgt(false, this);
    connect(this, SIGNAL(setMainImage(QPixmap,QVariantMap,QString)), previewMain, SLOT(setImage(QPixmap,QVariantMap,QString)));
    connect(this, SIGNAL(clearMainImage()), previewMain, SLOT(clearImage()) );
    ui->vl4wdgt->addWidget(previewMain);
     clearPage();

     printSett = PrintImageHelper::variantMap2printSett(getThisDevPrinterSett());
     printSett.widthMM += 50;
     printSett.userData.clear();
     previewMain->setPrintSett(printSett);

     QTimer::singleShot(111, this, SLOT(on_pbFirst_clicked()) );
}

void QrExportWdgt::clearPage()
{
    ui->plainTextEdit->clear();
}

void QrExportWdgt::on_pbFirst_clicked()
{
    showThisItem(0);
}

void QrExportWdgt::on_pbPrev_clicked()
{
    showThisItem(--lastPosition);

}

void QrExportWdgt::on_pbNext_clicked()
{
    showThisItem(++lastPosition);

}

void QrExportWdgt::on_pbEnd_clicked()
{
    showThisItem(lastSize);

}

void QrExportWdgt::showThisItem(int pos)
{
    if(list2print.isEmpty())
        return;

    if(pos >= lastSize || pos < 0)
        pos = (lastSize - 1);

    if(pos < 0)
        return;

    ui->pbFirst->setEnabled(pos > 0);
    ui->pbPrev->setEnabled(ui->pbFirst->isEnabled());
    ui->pbNext->setEnabled(pos < (lastSize  -1 ));
    ui->pbEnd->setEnabled(ui->pbNext->isEnabled());

    lastPosition = pos;
    const QString s = qCompress(list2print.at(pos).toUtf8(), 9).toBase64(QByteArray::OmitTrailingEquals);

    qDebug() << "len " << s.length() << list2print.at(pos).toUtf8().length();

    ui->lblPosition->setText(QString("%1/%2").arg(lastPosition + 1).arg(lastSize));
    ui->plainTextEdit->setPlainText(s);
    if(s.isEmpty())
        return;

//    map.insert("NI",
//    getPagePix(const int &wmm, const int &hmm, const int &dpi, const QString &corrlvl, const QString &s);
    emit setMainImage(QrCodeGenerator::getPagePix(printSett.widthMM, printSett.widthMM, printSett.resolutionDpi, printSett.qrCorrLetter, s), QVariantMap(), "");


}
