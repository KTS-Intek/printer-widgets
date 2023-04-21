#ifndef PRINTERHELPER_H
#define PRINTERHELPER_H

#include <QObject>
#include <QComboBox>
#include <QPixmap>

class PrinterHelper : public QObject
{
    Q_OBJECT
public:
    explicit PrinterHelper(QComboBox *cbx, QObject *parent = nullptr);

    static int getCbxIndx(const QString &s, QComboBox *c);

signals:
    void onPrintersUpdated();

public slots:
    void request2update();

    void onUpdateAvPrinters(QString lastPrinterName);


private:
    QComboBox *lastValidCbx;
};

#endif // PRINTERHELPER_H
