#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui/qpainter.h>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QProcess>
#include <QProcessEnvironment>
#include <QStandardItemModel>
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qstyleditemdelegate.h>
#include <QtWidgets/qtablewidget.h>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QTextEdit>

typedef QList<QString> hamed;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class ClickableLabel : public QLabel{
    Q_OBJECT

public:
    explicit ClickableLabel(int row, QWidget *parent = nullptr) : QLabel(parent), row(row) {}

signals:
    void clicked(int row);

protected:
    void mousePressEvent(QMouseEvent *event) override {
        emit clicked(row);
        QLabel::mousePressEvent(event);
    }
private:
    int row;

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum NotifType{Error,Warning,Info,Alarm};
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void connectToDatabase(QString databaseName);
    void showNotification(const QString &text, const QString &from, NotifType type = Info);
    void loadTableSchema(QSqlDatabase &db, const QString &tableName);
    void createNewRow();
    void initialNewTable();
    void delTableIcon(int row);
    void onCheckBoxStateChanged(bool state);
    void onIconLabelClicked(int row);

private slots:

    void on_btnLogin_clicked();

    void on_btnCreateUser_clicked();

    void on_btnExecuteQuery_clicked();

    void on_opTable_cellChanged(int row, int column);

    void on_tblList_cellChanged(int row, int column);

    void on_tblList_cellClicked(int row, int column);

    void on_btnAddtbl_pressed();

    void on_btnGenQuery_pressed();

    void on_btnDelRow_pressed();

    void on_edtQuery_textChanged();

    void on_actionUndo_triggered();

    void on_opTable_cellClicked(int row, int column);

    void on_opTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void handleEditIconClick(const QModelIndex &index);

    void handleDeleteIconClick(const QModelIndex &index);

    void on_dbTable_cellDoubleClicked(int row, int column);

    void on_btnBackDb_clicked();

    void on_btnCreateDB_pressed();

    void on_btnBackDb2_pressed();

    void on_cbCollation_currentTextChanged(const QString &arg1);

    void on_btnExportDb_pressed();

    void on_btnImportDb_pressed();

    void on_btnTables_pressed();

    void on_tblDbUsers_cellClicked(int row, int column);

    void on_btnBackDb2_2_pressed();

    void on_btnUsers_pressed();

    void on_btnRevokeAll_pressed();

    void on_btnSetPrivileges_pressed();

    void on_chFullAccess_checkStateChanged(const Qt::CheckState &arg1);

    void on_btnAccess_pressed();

    void on_tabWidget_currentChanged(int index);

    void on_tblUsersList_cellChanged(int row, int column);

    void on_btnCreateNewUser_pressed();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlQuery query;
    QString userName, password, hostName, selectedDatabase, dbName, tblName, preCell, edtQuery, preQuery;
    QList<QString> newTables, tblList, prvgList;
    QSqlQueryModel qmTables,qmColumns;
    QProcess terminal;
    QHash<int, QStringList> cmList, lastEdit;
    QList<int> preRowTbl;
    int preRow, preColumn;
    bool tblChanged; 
    QStringList collations;
    QList <QCheckBox*> checkboxes;
};


#endif // MAINWINDOW_H
