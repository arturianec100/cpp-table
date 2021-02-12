#include "tabletools.h"
#include "ui_tabletools.h"

TableTools::TableTools(TableModel *tableModel, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableTools),
    model(tableModel)
{
    ui->setupUi(this);

    connect(ui->rowBtn1, SIGNAL(clicked(bool)),
            this, SLOT(removeSelectedRow()));
    connect(ui->rowBtn2, SIGNAL(clicked(bool)),
            this, SLOT(addRowNearSelected()));
    connect(ui->rowBtn3, SIGNAL(clicked(bool)),
            this, SLOT(addRowTop()));
    connect(ui->rowBtn4, SIGNAL(clicked(bool)),
            this, SLOT(addRowBottom()));

    connect(ui->clmBtn1, SIGNAL(clicked(bool)),
            this, SLOT(removeSelectedColumn()));
    connect(ui->clmBtn2, SIGNAL(clicked(bool)),
            this, SLOT(addColumnNearSelected()));
    connect(ui->clmBtn3, SIGNAL(clicked(bool)),
            this, SLOT(addColumnLeft()));
    connect(ui->clmBtn4, SIGNAL(clicked(bool)),
            this, SLOT(addColumnRight()));
}

TableTools::~TableTools()
{
    delete ui;
}

void TableTools::showError(const QString &message)
{
    QMainWindow *mainWindow = nullptr;
    for (auto *window : QGuiApplication::allWindows()) {
        if (mainWindow = qobject_cast<QMainWindow*>(window)) {
            QMetaObject::invokeMethod(mainWindow, "showError",
                                      Q_ARG(const QString&, message));
            break;
        }
    }
}

QTableView *TableTools::view() const
{
    return view_;
}

void TableTools::setView(QTableView *view)
{
    view_ = view;
}

void TableTools::removeSelectedRow() const
{
    QItemSelectionModel *selection = view_->selectionModel();
    int row = selection->currentIndex().row();
    if (row < 0 || row > model->rowCount()) {
        return;
    }
    model->removeRows(row, 1);
}

void TableTools::addRowNearSelected() const
{
    QItemSelectionModel *selection = view_->selectionModel();
    int row = selection->currentIndex().row();
    if (row < 0 || row > model->rowCount()) {
        return;
    }
    model->insertRows(row + 1, 1);
}

void TableTools::addRowTop() const
{
    model->insertRows(0, 1);
}

void TableTools::addRowBottom() const
{
    model->insertRows(model->rowCount(), 1);
}

void TableTools::removeSelectedColumn() const
{
    QItemSelectionModel *selection = view_->selectionModel();
    int column = selection->currentIndex().column();
    if (column < 0 || column > model->columnCount()) {
        return;
    }
    model->removeColumns(column, 1);
}

void TableTools::addColumnNearSelected() const
{
    QItemSelectionModel *selection = view_->selectionModel();
    int column = selection->currentIndex().column();
    if (column < 0 || column > model->columnCount()) {
        return;
    }
    model->insertColumns(column + 1, 1);
}

void TableTools::addColumnLeft() const
{
    model->insertColumns(0, 1);
}

void TableTools::addColumnRight() const
{
    model->insertColumns(model->columnCount(), 1);
}
