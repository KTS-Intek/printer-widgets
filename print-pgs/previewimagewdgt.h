#ifndef PREVIEWIMAGEWDGT_H
#define PREVIEWIMAGEWDGT_H

#include <QWidget>
#include <QPixmap>
#include "src/printer/printimagehelper.h"
#include <QVariantMap>
#include <QWheelEvent>
#include <QKeyEvent>

namespace Ui {
class PreviewImageWdgt;
}

class PreviewImageWdgt : public QWidget
{
    Q_OBJECT

public:
    explicit PreviewImageWdgt(const bool &hideRotationSett, QWidget *parent = 0);
    ~PreviewImageWdgt();
    PrintImageHelper::PrintSettCache printSett;

    QPixmap getLastPixmap() const;

    QPixmap getLastPixmapTxt() const;


signals:
    void onPageSettChanged(bool previewMode, qreal scale);

    void startTmrUpdateImg();

public slots:
    void setLastPageSett(bool previewMode, int zoom);

    void updateImage();

    void setImage(QPixmap p, QVariantMap map, QString lblTxt);

    void setImageData(QVariantMap map);

    void setPrintSett(const PrintImageHelper::PrintSettCache &printSett);

    void clearImage();

    void updatePrintImage();


protected:
//    void wheelEvent(QWheelEvent *event);

//    void keyPressEvent(QKeyEvent *event);

//    void keyReleaseEvent(QKeyEvent *event);

private slots:
    void on_toolButton_clicked();

    void on_tbSaveAs_clicked();

private:
    Ui::PreviewImageWdgt *ui;


    QPixmap lastPix, lastPixTxt;
    QVariantMap lastAbout;
    QString lastLblTxt;

    bool isControlPressed;

};

#endif // PREVIEWIMAGEWDGT_H
