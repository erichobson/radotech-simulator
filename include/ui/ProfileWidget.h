/**
 * @file ProfileWidget.h
 * @brief Declaration of the ProfileWidget class.
 */

#ifndef PROFILEWIDGET_H
#define PROFILEWIDGET_H

#include <QWidget>

class QLabel;

class ProfileWidget : public QWidget {
    Q_OBJECT
   public:
    explicit ProfileWidget(const QString &imagePath, const QString &userName,
                           QWidget *parent = nullptr);

    void setUserName(const QString& username);

   private:
    QLabel *profilePicLabel;
    QLabel *userNameLabel;
};

#endif  // PROFILEWIDGET_H
