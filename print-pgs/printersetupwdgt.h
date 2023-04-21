#ifndef PRINTERSETUPWDGT_H
#define PRINTERSETUPWDGT_H

#include <QWidget>


///[!] printer-widgets
#include "print-pgs/previewimagewdgt.h"


namespace Ui {
class PrinterSetupWdgt;
}

class PrinterSetupWdgt : public QWidget
{
    Q_OBJECT

public:
    explicit PrinterSetupWdgt(const QString &defaultimagepath, const bool &generateQrFoced, const bool &verboseMode, QWidget *parent = nullptr);
    ~PrinterSetupWdgt();

    PrintImageHelper::PrintSettCache printSettDefault;

    QPixmap getPagePix();

    int getRotateDeg();

    QString defaultimagepath;
    bool generateQrFoced;
    bool verboseMode;

    PrintImageHelper::PrintSettCache getPagePrintSett();

    struct PreviewTemplateText
    {
        QString qrTextMessage;
        QString bottomMessage;

        QString euiline;
        QString euilinekeys;
        QString euilineslitsymb;

        PreviewTemplateText() :
            qrTextMessage("Hi,\nHow are you?"), bottomMessage("Hi,\nHow are you?") ,
        euiline("1234567890abcdef routerni MYDEV"), euilinekeys("$eui64 $ni $model"), euilineslitsymb(" ")
        {}
    } prevTxt;


signals:
    void stopTmrUpdate();

    void settSaved(QVariantMap map);

    void sendMeSett();


    void youCanCloseMe();

    void setReadyImage(QPixmap p, QString lblTxt);

public slots:
    void loadThisSett(QVariantMap map);





private slots:
    void initObjects();

    void on_pbReload_clicked();

    void on_pbSave_clicked();

    void onpbUpdateHtml_clicked();

    void on_pbPrint_clicked();

    void on_pushButton_clicked();

    void on_pbDefault_clicked();

    void on_toolButton_clicked();


    void on_groupBox_4_clicked();

private:
    Ui::PrinterSetupWdgt *ui;
    PreviewImageWdgt *previewOptions;

};

#endif // PRINTERSETUPWDGT_H
