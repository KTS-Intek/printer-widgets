#ifndef QREXPORTWDGT_H
#define QREXPORTWDGT_H


///[!] widgets-shared
#include "gui-src/matildaconfwidget.h"


///[!] printer-widgets
#include "print-pgs/previewimagewdgt.h"


///[!] printer-shared
#include "src/printer/qrcodegeneratortypes.h"


namespace Ui {
class QrExportWdgt;
}

class QrExportWdgt : public MatildaConfWidget
{
    Q_OBJECT

public:
    explicit QrExportWdgt(GuiHelper *gHelper, QWidget *parent = 0);
    ~QrExportWdgt();
    QrCodeGenSett lastQrGenSett;
    PrintImageHelper::PrintSettCache printSett;

    QVariantMap getThisDevPrinterSett() const;


signals:
    void setMainImage(QPixmap p, QVariantMap map, QString lblTxt);

    void clearMainImage();


public slots:
    void setQrs(QStringList list);

    void initPage();

    void clearPage();

private slots:
    void on_pbFirst_clicked();

    void on_pbPrev_clicked();

    void on_pbNext_clicked();

    void on_pbEnd_clicked();

private:
    Ui::QrExportWdgt *ui;

    void showThisItem(int pos);

    PreviewImageWdgt *previewMain;

    QStringList list2print;
    int lastPosition, lastSize;


};

#endif // QREXPORTWDGT_H
