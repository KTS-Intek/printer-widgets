#ifndef UCPRINTLABEL_H
#define UCPRINTLABEL_H


///[!] widgets-shared
#include "gui-src/referencewidgetclass.h"


///[!] printer-widgets
#include "print-pgs/previewimagewdgt.h"


///[!] printer-shared
#include "src/printer/qrcodegeneratortypes.h"


namespace Ui {
class UcPrintLabel;
}

class UcPrintLabel : public MatildaConfWidget
{
    Q_OBJECT

public:
    explicit UcPrintLabel(GuiHelper *gHelper, QWidget *parent = 0);
    ~UcPrintLabel();

    PrintImageHelper::PrintSettCache printSett;
//    QrCodeGenSett lastQrGenSett;

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
    void on_pbSave_clicked();

    void on_pbSettings_clicked();


    void onEscape();

    void restoreModelFromQrGenSett();

    void onCbxModel_currentIndexChanged(int index);

    void onInputDataChanged();

    void on_pbHtml_clicked();

private:
    Ui::UcPrintLabel *ui;

    PreviewImageWdgt *previewMain;


    QrList validlist;
};

#endif // UCPRINTLABEL_H
