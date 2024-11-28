/**
 * @file ClickableLabel.h
 * @brief Declaration of the ClickableLabel class.
 *
 * This Widget is a QLabel with a mouse click signal.
 */

#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>

class ClickableLabel : public QLabel {
    Q_OBJECT

   public:
    explicit ClickableLabel(QWidget *parent = nullptr);

   signals:
    /**
     * @brief Signal emitted when the label is clicked.
     */
    void clicked();

   protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif  // CLICKABLELABEL_H
