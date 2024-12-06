/**
 * @file
 * @brief
 */

#include "BackgroundDelegate.h"

#include <QPainter>

/**
 * @brief BackgroundDelegate implementation for custom item styling
 */
BackgroundDelegate::BackgroundDelegate(QObject* parent)
    : QStyledItemDelegate(parent) {}

/**
 * @brief Custom paint implementation for combo box items
 *
 * Handles the painting of combo box items with custom colors for
 * hover and selection states.
 *
 * @param painter The painter to use for drawing
 * @param option The style options for the item
 * @param index The model index of the item being painted
 */
void BackgroundDelegate::paint(QPainter* painter,
                               const QStyleOptionViewItem& option,
                               const QModelIndex& index) const {
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    if (opt.state & QStyle::State_MouseOver ||
        opt.state & QStyle::State_Selected) {
        opt.backgroundBrush = QBrush(QColor("#FF7009"));
        opt.palette.setColor(QPalette::Text, QColor("white"));
    } else {
        opt.backgroundBrush = QBrush(QColor("white"));
        opt.palette.setColor(QPalette::Text, QColor("#333333"));
    }

    QStyledItemDelegate::paint(painter, opt, index);
}
