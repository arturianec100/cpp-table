#include "tablemodel.h"

#include <algorithm>

QModelIndex TableModel::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column);
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    return table_.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    return table_.size() > 0 ? table_[0].size() : 0;
}

QModelIndex TableModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    if (index.row() >= rowCount()) {
        return QVariant();
    }
    if (index.column() >= columnCount()) {
        return QVariant();
    }
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        return table_[index.row()][index.column()];
    } else {
        return QVariant();
    }
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    /*
    if (orientation == Qt::Horizontal) {
        return QString::number(section);
    } else {
        return QStringLiteral("Row %1").arg(section);
    }
    */
    return QString::number(section);
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        table_[index.row()].replace(index.column(), value.toString());
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

bool TableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(QModelIndex(), row, row+count-1);

    StringRow rowToCopy(maxCols(), "");
    table_.insert(row, count, rowToCopy);

    endInsertRows();
    return true;
}

bool TableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), row, row+count-1);

    table_.remove(row, count);

    endRemoveRows();
    return true;
}

bool TableModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(QModelIndex(), column, column+count-1);

    for (auto &row: table_) {
        row.insert(column, count, "");
    }

    endInsertColumns();
    return true;
}

bool TableModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(QModelIndex(), column, column+count-1);

    for (auto &row: table_) {
        row.remove(column, count);
    }

    endRemoveColumns();
    return true;
}

const StringTable &TableModel::table() const
{
    return table_;
}

void TableModel::setTable(const StringTable &table)
{
    beginResetModel();
    table_ = table;
    resizeToMaxColumnCount();
    endResetModel();
}

int TableModel::maxCols() const
{
    int cols = table_.size() > 0 ? table_[0].size() : 0;
    for (auto &row : table_) {
        cols = std::max(cols, row.size());
    }
    return cols;
}

void TableModel::resizeToMaxColumnCount()
{
    int cols = maxCols();
    if (cols == 0) {
        return;
    }
    for (auto &row : table_) {
        while (row.size() < cols) {
            row.append("");
        }
    }
}
