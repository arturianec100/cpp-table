#ifndef TABLETOOLS_H
#define TABLETOOLS_H

#include "tablemodel.h"

#include <QtWidgets>

namespace Ui {
class TableTools;
}

class TableTools : public QWidget
{
    Q_OBJECT

public:
    explicit TableTools(TableModel *tableModel, QWidget *parent = nullptr);
    ~TableTools();

    //void showError(const QString &message);

    QTableView *view() const;
    void setView(QTableView *view);

public slots:
    void removeSelectedRow() const;
    void addRowNearSelected() const;
    void addRowTop() const;
    void addRowBottom() const;

    void removeSelectedColumn() const;
    void addColumnNearSelected() const;
    void addColumnLeft() const;
    void addColumnRight() const;

private:
    Ui::TableTools *ui;
    //Non-owner, lifetime are the same
    QTableView *view_;
    TableModel *model;
};

#endif // TABLETOOLS_H
