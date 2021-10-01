#include "printerhelper.h"
#include <QPrinterInfo>


//------------------------------------------------------------------------------------------------

PrinterHelper::PrinterHelper(QComboBox *cbx, QObject *parent) : QObject(parent)
{
    lastValidCbx = cbx;
}

int PrinterHelper::getCbxIndx(const QString &s, QComboBox *c)
{
    const int i = c->findText(s);
    c->setCurrentIndex( (i < 0 || s.isEmpty()) ? 0 : i);
    return c->currentIndex();
}


//------------------------------------------------------------------------------------------------

void PrinterHelper::request2update()
{
    if(lastValidCbx)
        onUpdateAvPrinters(lastValidCbx->currentText());
}

//------------------------------------------------------------------------------------------------

void PrinterHelper::onUpdateAvPrinters(QString lastPrinterName)
{
    if(lastPrinterName.isEmpty())
        lastPrinterName = lastValidCbx->currentText();
    if(lastPrinterName.isEmpty())
        lastPrinterName = QPrinterInfo::defaultPrinterName();

    QStringList l = QPrinterInfo::availablePrinterNames();
    int indx = l.indexOf(lastPrinterName) + 1;
    if(indx < 0 && !l.isEmpty())
        indx = 0;
    l.prepend(tr("Print to PDF"));
    lastValidCbx->clear();    
    lastValidCbx->addItems(l);
    lastValidCbx->setCurrentIndex(indx);
}

