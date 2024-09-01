#ifndef DUALICONDELEGATE_H
#define DUALICONDELEGATE_H

#include <QStyledItemDelegate>
#include <QIcon>

class DualIconDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    DualIconDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:
    void icon1Clicked(const QModelIndex &index);
    void icon2Clicked(const QModelIndex &index);
};

#endif // DUALICONDELEGATE_H
