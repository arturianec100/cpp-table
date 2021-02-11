#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "parseresult.h"
#include "tablemodel.h"

#include <QtCore>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void showError(const QString &message);

public slots:
    void openFile();
    void saveToFile();

protected:
    ParseResult parse(const QString &source);

    void setupActions();

    QString selectFileToOpen();
    QString selectFileToSave();
    void openFile(const QString &fileName);
    void saveToFile(const QString &fileName, const StringTable &table);

private:
    Ui::MainWindow *ui;
    TableModel *model;
    //To save table to the same file in the same position
    size_t beginTablePos = 0;
    size_t endTablePos = 0;
    QString lastLoadedFileName;
};
#endif // MAINWINDOW_H
