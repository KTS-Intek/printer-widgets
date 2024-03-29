#ifndef PRINTNIQRCODEWDGT_H
#define PRINTNIQRCODEWDGT_H


///[!] widgets-shared
#include "gui-src/referencewidgetclass.h"


///[!] printer-widgets
#include "print-pgs/previewimagewdgt.h"


///[!] printer-shared
#include "src/printer/qrcodegeneratortypes.h"


namespace Ui {
class PrintNiQrCodeWdgt;
}

class PrintNiQrCodeWdgt : public ReferenceWidgetClass
{
    Q_OBJECT

public:
    explicit PrintNiQrCodeWdgt(GuiHelper *gHelper, QWidget *parent = 0);
    ~PrintNiQrCodeWdgt();
    PrintImageHelper::PrintSettCache printSett;
    QrCodeGenSett lastQrGenSett;

    QVariantMap getThisDevPrinterSett() const;

signals:
    void setMainImage(QPixmap p, QVariantMap map, QString lblTxt);

    void clearMainImage();

    void delCloseShortCut();

    void setThisDevPrinterSett(QVariantMap map);

public slots:
    void initPage();

    void clearPage();

    void settSaved(QVariantMap deviceMap);
    void sendMeSett();

    void updatePrintSett();

    void updateNameOfPrinter();

    void saveNameOfPrinter();

    void createCloseShortCut();


private slots:
    void on_tableView_customContextMenuRequested(const QPoint &pos);

    void on_pbSettings_clicked();

    void on_pbPrint_clicked();

    void onEscape();

    void restoreModelFromQrGenSett();

    void onTableView_activated(const QModelIndex &index);

private:
    Ui::PrintNiQrCodeWdgt *ui;
    PreviewImageWdgt *previewMain;


    QrList validlist;



};

#endif // PRINTNIQRCODEWDGT_H
