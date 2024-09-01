#include "DualIconDelegate.h"
#include <QPainter>
#include <QMouseEvent>

DualIconDelegate::DualIconDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

void DualIconDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    painter->save();

    if (opt.state & QStyle::State_Selected) {
        painter->fillRect(opt.rect, opt.palette.highlight());
    } else {
        painter->fillRect(opt.rect, opt.palette.base());
    }

    QVariantList icons = index.data(Qt::UserRole).toList();

    if (icons.size() >= 2) {
        QIcon icon1 = qvariant_cast<QIcon>(icons[0]);
        QIcon icon2 = qvariant_cast<QIcon>(icons[1]);

        QSize iconSize = opt.decorationSize;
        QRect iconRect = opt.rect;

        int x1 = iconRect.x() + (iconRect.width() / 4 - iconSize.width() / 2);
        int y1 = iconRect.y() + (iconRect.height() - iconSize.height()) / 2;
        QRect iconRect1(x1, y1, iconSize.width(), iconSize.height());
        icon1.paint(painter, iconRect1, Qt::AlignCenter);

        int x2 = iconRect.x() + (3 * iconRect.width() / 4 - iconSize.width() / 2);
        int y2 = iconRect.y() + (iconRect.height() - iconSize.height()) / 2;
        QRect iconRect2(x2, y2, iconSize.width(), iconSize.height());
        icon2.paint(painter, iconRect2, Qt::AlignCenter);
    }

    // Center text if present
    if (!index.data(Qt::DisplayRole).isNull()) {
        QString text = index.data(Qt::DisplayRole).toString();
        painter->drawText(opt.rect, Qt::AlignCenter, text);
    }

    painter->restore();
}

bool DualIconDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) {
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        QVariantList icons = index.data(Qt::UserRole).toList();

        if (icons.size() >= 2) {
            QSize iconSize = option.decorationSize;
            QRect iconRect = option.rect;

            QRect iconRect1(
                iconRect.x() + (iconRect.width() / 4 - iconSize.width() / 2),
                iconRect.y() + (iconRect.height() - iconSize.height()) / 2,
                iconSize.width(), iconSize.height()
                );

            QRect iconRect2(
                iconRect.x() + (3 * iconRect.width() / 4 - iconSize.width() / 2),
                iconRect.y() + (iconRect.height() - iconSize.height()) / 2,
                iconSize.width(), iconSize.height()
                );

            if (iconRect1.contains(mouseEvent->pos())) {
                emit icon1Clicked(index);
                return true;
            } else if (iconRect2.contains(mouseEvent->pos())) {
                emit icon2Clicked(index);
                return true;
            }
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
