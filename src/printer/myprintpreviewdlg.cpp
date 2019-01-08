#include "myprintpreviewdlg.h"

MyPrintPreviewDlg::MyPrintPreviewDlg(QPrinter *printer, QWidget *parent) : QPrintPreviewDialog(printer, parent)
{
    connect(this, SIGNAL(paintRequested(QPrinter*)), this, SLOT(onPaintRequest(QPrinter*)) );
}

void MyPrintPreviewDlg::onPaintRequest(QPrinter *printer)
{
    document.print(printer);

}
