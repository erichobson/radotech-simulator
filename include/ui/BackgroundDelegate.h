/**
 * @file BackgroundDelegate.h
 * @brief
 */

#ifndef BACKGROUNDDELEGATE_H
#define BACKGROUNDDELEGATE_H

#include <QStyledItemDelegate>

namespace {
/**
 * @brief Style definitions for UI components
 *
 * These constants define the CSS-style formatting for various UI elements
 * including buttons, input fields, and combo boxes.
 */
const QString BUTTON_STYLE = R"(
        QPushButton {
            background-color: #FF7009;
            color: white;
            border-radius: 10px;
            padding: 5px 20px;
            font-size: 16px;
        }
        QPushButton:hover {
            background-color: #E66008;
        }
        QPushButton:pressed {
            background-color: #CC5A07;
        }
    )";

const QString INPUT_STYLE =
    "QDateEdit, QTimeEdit, QDoubleSpinBox, QSpinBox {"
    "    background-color: white;"
    "    color: #333333;"
    "    border: 2px solid #E0E0E0;"
    "    border-radius: 6px;"
    "    padding: 8px;"
    "    font-size: 14px;"
    "    min-width: 100px;"
    "}"
    "QDateEdit:hover, QTimeEdit:hover, QDoubleSpinBox:hover, QSpinBox:hover {"
    "    border-color: #FF7009;"
    "}"
    "QDateEdit::up-button, QDateEdit::down-button,"
    "QTimeEdit::up-button, QTimeEdit::down-button,"
    "QDoubleSpinBox::up-button, QDoubleSpinBox::down-button,"
    "QSpinBox::up-button, QSpinBox::down-button {"
    "    width: 0px;"
    "    height: 0px;"
    "    border: none;"
    "    image: none;"
    "}";

const QString COMBOBOX_STYLE =
    "QComboBox {"
    "    background-color: white;"
    "    border: 2px solid #E0E0E0;"
    "    border-radius: 6px;"
    "    padding: 8px;"
    "    font-size: 14px;"
    "    min-width: 200px;"
    "    color: #333333;"
    "}"
    "QComboBox:hover {"
    "    border-color: #FF7009;"
    "}"
    "QComboBox::drop-down {"
    "    border: none;"
    "    background: none;"
    "}"
    "QComboBox::down-arrow {"
    "    image: none;"
    "}"
    "QComboBox QAbstractItemView {"
    "    background-color: white;"
    "    border: 1px solid #E0E0E0;"
    "    selection-background-color: #FF7009;"
    "    selection-color: white;"
    "}";
}  // namespace

/**
 * @brief Custom delegate for styling background in combo boxes
 */
class BackgroundDelegate : public QStyledItemDelegate {
    Q_OBJECT

   public:
    explicit BackgroundDelegate(QObject* parent = nullptr);
    void paint(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;
};

#endif  // BACKGROUNDDELEGATE_H
