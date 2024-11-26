/**
 * @file DeviceImageWidget.h
 * @brief Declaration of the DeviceImageWidget class.
 */

#ifndef DEVICEIMAGEWIDGET_H
#define DEVICEIMAGEWIDGET_H

#include <QLabel>
#include <QPixmap>

class DeviceImageWidget : public QLabel {
    Q_OBJECT
   public:
    explicit DeviceImageWidget(const QPixmap &pixmap,
                               QWidget *parent = nullptr);

   protected:
    void resizeEvent(QResizeEvent *event) override;

   private:
    QPixmap originalPixmap;
};

#endif  // DEVICEIMAGEWIDGET_H
