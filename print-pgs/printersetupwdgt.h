#ifndef PRINTERSETUPWDGT_H
#define PRINTERSETUPWDGT_H

#include <QWidget>
#include "print-pgs/previewimagewdgt.h"

namespace Ui {
class PrinterSetupWdgt;
}

class PrinterSetupWdgt : public QWidget
{
    Q_OBJECT

public:
    explicit PrinterSetupWdgt(QWidget *parent = 0);
    ~PrinterSetupWdgt();

    QPixmap getPagePix();

    int getRotateDeg();

signals:
    void stopTmrUpdate();

    void settSaved(QVariantMap map);

    void sendMeSett();


    void youCanCloseMe();

    void setImage(QPixmap p, QVariantMap map, QString lblTxt);

public slots:
    void loadThisSett(QVariantMap map);

private slots:
    void initObjects();

    void on_pbReload_clicked();

    void on_pbSave_clicked();

    void on_pbUpdateHtml_clicked();

    void on_pbPrint_clicked();

    void on_pushButton_clicked();

    void on_pbDefault_clicked();

private:
    Ui::PrinterSetupWdgt *ui;
    PreviewImageWdgt *previewOptions;

};

#endif // PRINTERSETUPWDGT_H
