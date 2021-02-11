#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include "typedefs.h"

#include <QtCore>

class TableModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    TableModel(QObject *parent = nullptr)
        : QAbstractListModel(parent) {}
    ~TableModel() {}
    //Read-only api
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    //Write api
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    //Multi-item edit
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    StringTable table() const;
    void setTable(const StringTable table);

protected:
    size_t maxCols() const;
    void resizeToMaxColumnCount();

private:
    StringTable table_;
};

#endif // TABLEMODEL_H
