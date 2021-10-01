#ifndef MYPRINTPREVIEWDLG_H
#define MYPRINTPREVIEWDLG_H

#include <QPrintPreviewDialog>
#include <QTextDocument>
#include <QPrinter>

class MyPrintPreviewDlg : public QPrintPreviewDialog
{
    Q_OBJECT
public:
    explicit MyPrintPreviewDlg(QPrinter *printer, QWidget *parent = nullptr);

    QString lastprintername;
    QTextDocument document;

signals:

public slots:
    void onPaintRequest(QPrinter *printer);
};

#endif // MYPRINTPREVIEWDLG_H
