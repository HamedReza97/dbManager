#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dualicondelegate.h"
#include <QSqlError>
#include <QMessageBox>
#include <QFileDialog>
#include <QProcess>
#include <QStandardItemModel>
#include <QTimer>
#include <QDir>
#include <QCheckBox>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QCompleter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QMARIADB");
    query = QSqlQuery(db);
    if (!db.open()) {
        qDebug() << "Failed to connect to database:" << db.lastError().text();
        return;
    }
    ui->stackedWidget->setCurrentIndex(0);

    frmNotification = new QFrame(this);
    frmNotification->setHidden(true);
    QVBoxLayout *notifLayout = new QVBoxLayout(frmNotification);
    frmNotification->setLayout(notifLayout);
    lblNotification = new QLabel("Test",this);
    lblNotifFrom = new QLabel("test", this);
    lblNotification->setFont(QFont("Vazir",12,400));
    lblNotifFrom->setFont(QFont("Vazir",9,400));
    notifLayout->addWidget(lblNotification);
    notifLayout->addWidget(lblNotifFrom);

    ui->tblList->horizontalHeader()->hide();
    ui->tblList->verticalHeader()->hide();
    ui->opTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->opTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    ui->dbTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->dbTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->dbTable->setColumnCount(4);
    ui->dbTable->setHorizontalHeaderLabels({"Database Name", "Size(MB)", "Tables", "Action"});
    ui->dbTable->verticalHeader()->hide();
    ui->dbTable->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Fixed);

    ui->tblDbUsers->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tblDbUsers->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tblDbUsers->setColumnCount(3);
    ui->tblDbUsers->setHorizontalHeaderLabels({"User Name", "Privileges", "Action"});
    ui->tblDbUsers->verticalHeader()->hide();
    ui->tblDbUsers->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Fixed);

    ui->tblUsersList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tblUsersList->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tblUsersList->setColumnCount(4);
    ui->tblUsersList->setHorizontalHeaderLabels({"User Name", "Host Name", "Set New Password", "Action"});
    ui->tblUsersList->verticalHeader()->hide();
    ui->tblUsersList->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Fixed);
    ui->tblUsersList->setColumnWidth(3,80);

    DualIconDelegate *delegate = new DualIconDelegate(ui->dbTable);
    ui->dbTable->setItemDelegate(delegate);
    connect(delegate, &DualIconDelegate::icon1Clicked, this, &MainWindow::handleEditIconClick);
    connect(delegate, &DualIconDelegate::icon2Clicked, this, &MainWindow::handleDeleteIconClick);

    QString qry = "show collation;";
    query.prepare(qry);
    query.exec();
    while(query.next()){
        if(query.value(1).toString() != ""){
        collations.append(query.value(0).toString());
    }
    }

    checkboxes.append(ui->chAlter);
    checkboxes.append(ui->chCreate);
    checkboxes.append(ui->chCreateR);
    checkboxes.append(ui->chCreateTT);
    checkboxes.append(ui->chCreateView);
    checkboxes.append(ui->chShowView);
    checkboxes.append(ui->chShowCR);
    checkboxes.append(ui->chDelete);
    checkboxes.append(ui->chDeleteH);
    checkboxes.append(ui->chDrop);
    checkboxes.append(ui->chEvent);
    checkboxes.append(ui->chIndex);
    checkboxes.append(ui->chInsert);
    checkboxes.append(ui->chLookTable);
    checkboxes.append(ui->chReferences);
    checkboxes.append(ui->chSelect);
    checkboxes.append(ui->chTriggers);
    checkboxes.append(ui->chUpdate);

    foreach (QCheckBox* checkBox, checkboxes) {
        connect(checkBox, &QCheckBox::stateChanged, this, &MainWindow::onCheckBoxStateChanged);
    }

}

MainWindow::~MainWindow()
{
    if(db.isOpen()) db.close();
    delete frmNotification;
    delete lblNotification;
    delete lblNotifFrom;
    delete ui;
}


void MainWindow::showNotification(const QString &text, const QString &from, NotifType type)
{
    lblNotification->setText(text);
    lblNotifFrom->setText(from);
    lblNotification->adjustSize();
    lblNotifFrom->adjustSize();
    QString frmStyleSheet;
    switch(type)
    {

    case Error:
        frmStyleSheet = "background-color: rgba(255,0,0,.7);"
                        "color: white;";
        break;
    case Warning:
        frmStyleSheet = "background-color: rgba(100,100,0,0.7);";
        break;
    case Info:
        frmStyleSheet = "background-color: rgba(200,200,200,0.7);";
        break;
    case Alarm:
        frmStyleSheet = "background-color: rgba(100,100,0,0.7);";
        break;
    }
    frmStyleSheet.append("border: 2px solid transparent;"
                         "padding: 2x 4px 2px 4px;"
                         "border-radius: 15px;"
                         "QLabel{"
                         "color: white;"
                         "}");
    frmNotification->setStyleSheet(frmStyleSheet);
    frmNotification->setGeometry(QRect(5,5,lblNotification->contentsRect().width()+30, lblNotification->contentsRect().height()*5.3));
    frmNotification->setHidden(false);
    QTimer::singleShot(3000,[&](){
        frmNotification->setHidden(true);
    });
}

// Database operations
void MainWindow::on_btnLogin_clicked()
{
    ui->dbTable->clearContents();
    userName = ui->edtUsername->text();
    password = ui->edtPassword->text();

    QString qry = "SELECT s.SCHEMA_NAME AS Database, IFNULL(ROUND(SUM(t.data_length + t.index_length)"
                  " / 1024 / 1024, 2), 0) AS \"Size (MB)\", COUNT(t.table_name) AS \"Number of Tables\" FROM"
                  " information_schema.SCHEMATA s LEFT JOIN  information_schema.tables t ON s.SCHEMA_NAME ="
                  " t.table_schema GROUP BY s.SCHEMA_NAME;";

    QIcon edtIcon(":/icons/Icons/edit.svg");
    QIcon delIcon(":/icons/Icons/trash.svg");
    query.prepare(qry);
    query.exec();

    for (int i = 0; query.next(); i++) {
        ui->dbTable->insertRow(i);
        ui->dbTable->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->dbTable->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->dbTable->setItem(i, 2, new QTableWidgetItem(query.value(2).toString()));

        QTableWidgetItem *item = new QTableWidgetItem();
        QVariantList icons;
        icons << QVariant(edtIcon) << QVariant(delIcon);
        item->setData(Qt::UserRole, icons);

        ui->dbTable->setItem(i, 3, item);
    }

    ui->stackedWidget->setCurrentIndex(1);
    ui->tabWidget->setCurrentIndex(0);


}

void MainWindow::on_dbTable_cellDoubleClicked(int row, int column)
{
    if(column == 0){
    dbName = ui->dbTable->item(row,column)->text();
    connectToDatabase(dbName);
    ui->stackedWidget->setCurrentIndex(2);
    ui->lbldbName->setText("Database: "+ dbName);
    }
}

void MainWindow::handleEditIconClick(const QModelIndex &index) {

    QIcon edtIcon(":/icons/Icons/edit.svg");
    int row = index.row();
    ui->tblDbUsers->clearContents();
    dbName = ui->dbTable->item(row,0)->text();
    ui->lblDbName->setText(dbName);
    QFont font;
    font.setUnderline(true);

    QString qry = QStringLiteral(R"(SELECT
    GRANTEE AS User,
    GROUP_CONCAT(PRIVILEGE_TYPE ORDER BY PRIVILEGE_TYPE ASC SEPARATOR ', ') AS Privileges
    FROM
    information_schema.SCHEMA_PRIVILEGES
    WHERE
    TABLE_SCHEMA = '%1'
    GROUP BY
    GRANTEE;)")
                      .arg(dbName);
    query.prepare(qry);
    if(query.exec()){
        for(int i = 0; query.next(); i++){

            QString str = query.value(1).toString();

            QStringList requiredPrivileges = {
                "ALTER", "CREATE", "CREATE ROUTINE", "CREATE TEMPORARY TABLES",
                "CREATE VIEW", "DELETE", "DELETE HISTORY", "DROP", "EVENT",
                "INDEX", "INSERT", "LOCK TABLES", "REFERENCES", "SELECT",
                "SHOW CREATE ROUTINE", "SHOW VIEW", "TRIGGER", "UPDATE"
            };

            QStringList privilegesList = str.split(",", Qt::SkipEmptyParts);

            for (QString& privilege : privilegesList) {
                privilege = privilege.trimmed();
            }

            for (const QString& privilege : requiredPrivileges) {
                if (!privilegesList.contains(privilege)) {
                    str = query.value(1).toString();
                }
                else
                {
                    str = "Full Access";
                }
            }
            ui->tblDbUsers->insertRow(i);
            ui->tblDbUsers->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
            ui->tblDbUsers->setItem(i, 1, new QTableWidgetItem(str));
            QTableWidgetItem *action = new QTableWidgetItem("Edit User Previleges");
            action->setFont(font);
            ui->tblDbUsers->setItem(i,2, action);
            ui->tblDbUsers->resizeRowToContents(i);
            ui->tblDbUsers->item(i,0)->setTextAlignment(Qt::AlignCenter);
            ui->tblDbUsers->item(i,1)->setTextAlignment(Qt::AlignCenter);
            ui->tblDbUsers->item(i,2)->setTextAlignment(Qt::AlignCenter);
        }



    }
    ui->cbCollation->blockSignals(true);
    ui->cbCollation->addItems(collations);
    ui->cbCollation->blockSignals(false);

    qry = QStringLiteral("USE %1;").arg(dbName);
    QSqlQuery query;
    if (!query.exec(qry)) {
        ui->statusbar->showMessage("Failed to switch database: " + query.lastError().text());
        return;
    }

    qry = QStringLiteral("SHOW VARIABLES LIKE 'character_set_database';");
    if (!query.exec(qry)) {
        ui->statusbar->showMessage("Failed to get character set: " + query.lastError().text());
        return;
    }
    if (query.next()) {
        ui->lblCharset->setText(query.value(1).toString());
    }

    qry = QStringLiteral("SHOW VARIABLES LIKE 'collation_database';");
    if (!query.exec(qry)) {
        ui->statusbar->showMessage("Failed to get collation: " + query.lastError().text());
        return;
    }
    if (query.next()) {
        ui->cbCollation->blockSignals(true);
        ui->cbCollation->setCurrentText(query.value(1).toString());
        ui->cbCollation->blockSignals(false);
    }

    qry = QStringLiteral("SHOW TABLES;");
    if (!query.exec(qry)) {
        ui->statusbar->showMessage("Failed to show tables: " + query.lastError().text());
        return;
    }

    qry = "SELECT s.SCHEMA_NAME AS Database, IFNULL(ROUND(SUM(t.data_length + t.index_length)"
          " / 1024 / 1024, 2), 0) AS \"Size (MB)\", COUNT(t.table_name) AS \"Number of Tables\" FROM"
          " information_schema.SCHEMATA s LEFT JOIN  information_schema.tables t ON s.SCHEMA_NAME ="
          " t.table_schema GROUP BY s.SCHEMA_NAME;";
    if (!query.exec(qry)) {
        ui->statusbar->showMessage("Failed to get collation: " + query.lastError().text());
        return;
    }
    while(query.next())
    {
        if(query.value(0).toString() == dbName)
        {
            ui->lblSize->setText(query.value(1).toString());
            ui->lblTables->setText(query.value(2).toString());
        }
    }

    QList<QString> users;

    qry = QStringLiteral("SELECT User, Host FROM mysql.user");

    if (!query.exec(qry)) {
        qWarning() << "Failed to execute query:" << query.lastError().text();
        return;
    }

    while(query.next()) {
        QString user = query.value(0).toString();
        QString host = query.value(1).toString();
        QString str = QStringLiteral("%1'@'%2").arg(user).arg(host);
        users.append(str);
    }

    QCompleter *completer = new QCompleter(users, ui->cbUsersList);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->cbUsersList->setCompleter(completer);

    ui->cbUsersList->addItems(users);
    ui->cbUsersList->setDuplicatesEnabled(false);
    ui->lblSize->setText(ui->dbTable->item(row,2)->text());
    ui->lblSize->setText(ui->dbTable->item(row,1)->text().append("MB"));

    ui->stackedWidget->setCurrentIndex(3);

}

void MainWindow::on_cbCollation_currentTextChanged(const QString &arg1)
{
    QString coll;
    if (arg1 == "binary"){
        coll = arg1;
    }
    else
    {
    coll = arg1.section('_', 0, 0);
    }
    QString qry = QStringLiteral("ALTER DATABASE %1 CHARACTER SET %2 COLLATE %3;").arg(dbName).arg(coll).arg(arg1);
    query.prepare(qry);
    if (!query.exec(qry)) {
        ui->statusbar->showMessage("Failed to get collation: " + query.lastError().text());
        return;
    }
    ui->statusbar->showMessage("Collation and charset changed successfully");
}


void MainWindow::handleDeleteIconClick(const QModelIndex &index) {
    int row = index.row();
    QString qry;
    qry = QStringLiteral("DROP DATABASE IF EXISTS %1").arg(ui->dbTable->item(row,0)->text());
    query.prepare(qry);
    if(QMessageBox::question(this,"Delete Selected Database","The selected database will be removed permanently and can't be restored. Are you sure?",
            QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
    {
    if (!query.exec()) {
        ui->statusbar->showMessage(query.lastError().text());
        return;
    }
    on_btnBackDb_clicked();
    ui->statusbar->showMessage("Database Removed Successfully",2000);
    }
}

void MainWindow::on_btnBackDb2_pressed()
{
    on_btnLogin_clicked();
}


void MainWindow::on_btnCreateDB_pressed()
{
    QString qry, user, pass, databaseName;
    user = ui->edtDBUser->text();
    pass = ui->edtDBUserPass->text();
    databaseName = ui->edtDbName->text();

    qry = QStringLiteral("CREATE DATABASE IF NOT EXISTS %1").arg(databaseName);
    query.prepare(qry);
    if (!query.exec()) {
        ui->statusbar->showMessage(query.lastError().text());
        return;
    }

    qry = QStringLiteral("CREATE USER IF NOT EXISTS '%1'@'localhost' IDENTIFIED BY '%2'").arg(user, pass);
    query.prepare(qry);
    if (!query.exec()) {
        ui->statusbar->showMessage(query.lastError().text());
        return;
    }

    qry = QStringLiteral("GRANT ALL PRIVILEGES ON %1.* TO '%2'@'localhost'").arg(databaseName, user);
    query.prepare(qry);
    if (!query.exec()) {
        ui->statusbar->showMessage(query.lastError().text());
        return;
    }

    qry = QStringLiteral("FLUSH PRIVILEGES");
    query.prepare(qry);
    if (!query.exec()) {
        ui->statusbar->showMessage(query.lastError().text());
        return;
    }

    ui->statusbar->showMessage("Database successfully created", 2000);
    on_btnLogin_clicked();

}

void MainWindow::on_btnBackDb_clicked()
{
    on_btnLogin_clicked();
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::delTableIcon(int row){
    if (QMessageBox::question(this, "Delete Table",
                              QStringLiteral("Are you sure you want to remove the %1 table?")
                                  .arg(ui->edtAddtbl->text()),
                              QMessageBox::Yes,
                              QMessageBox::No) == QMessageBox::Yes) {
        ui->tblList->blockSignals(true);
        edtQuery.append(QStringLiteral("DROP TABLE IF EXISTS %1;").arg(ui->tblList->item(row,0)->text()));
        QSqlQuery query(db);
        query.prepare(edtQuery);
        if(query.exec()){
            ui->statusbar->showMessage("Table successfully removed",2000);
            edtQuery.clear();
            ui->opTable->clear();
            connectToDatabase(dbName);
            preRowTbl.clear();
            tblChanged = false;
        }
        else{
            ui->statusbar->showMessage(query.lastError().text(),2000);
            connectToDatabase(dbName);
            ui->tblList->item(row,0)->setBackground(QBrush(QColor(Qt::transparent)));
            preRowTbl.clear();
            ui->opTable->clear();
            tblChanged = false;
        }
        ui->tblList->blockSignals(false);
    }
}


void MainWindow::on_btnCreateUser_clicked()
{
    userName  = ui->edtUsername->text();
    password = ui->edtPassword->text();
    hostName = ui->edtHostname->text();
    db.open();
    query.prepare("CREATE USER :user@:host IDENTIFIED BY :password;");
    query.bindValue(":user", userName);
    query.bindValue(":host", hostName);
    query.bindValue(":password", password);
    if(query.exec()){
        ui->statusbar->showMessage("User Created. Please login...",2000);
    }
    else{
        ui->statusbar->showMessage(query.lastError().text(),2000);
    }
}

// Tables operations
void MainWindow::connectToDatabase(QString databaseName)
{
    // if(db.isOpen())
    // {
    //     if(db.databaseName() == databaseName) return;
    // }
    db.setDatabaseName(databaseName);
    db.open();
    QSqlQuery query(db);
    query.prepare("show tables;");
    if (!query.exec()) {
        if(db.databaseName() == databaseName) return;
        QMessageBox::critical(nullptr, QObject::tr("Query Error"),
                              QObject::tr("Failed to retrieve tables: %1").arg(query.lastError().text()));
        return;
    }
    ui->tblList->clear();
    ui->tblList->setRowCount(0);
    ui->tblList->setColumnCount(2);
    ui->tblList->blockSignals(true);
    ui->tblList->setHorizontalHeaderLabels({"Table Name","Action"});
    ui->tblList->horizontalHeader()->show();
    ui->tblList->setColumnWidth(1, 60);
    QHeaderView *header = ui->tblList->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    header->setSectionResizeMode(1, QHeaderView::Fixed);
    tblList.clear();

    for(int row =0; query.next(); row++)
    {
        QPushButton *button = new QPushButton(QString(""));
        button->setStyleSheet("QPushButton{background-color: transparent}");
        button->setIcon(QIcon(QIcon(":/icons/Icons/trash.svg")));
        button->setIconSize(QSize(20, 20));
        tblList.append(query.value(QStringLiteral("Tables_in_%1").arg(databaseName)).toString());
        ui->tblList->insertRow(row);
        ui->tblList->setItem(row, 0, new QTableWidgetItem(query.value(QStringLiteral("Tables_in_%1").arg(databaseName)).toString()));
        ui->tblList->setIndexWidget(QModelIndex(ui->tblList->model()->index(row,1)), button);
        QObject::connect(button, &QPushButton::clicked, this, [row, this]() {
            delTableIcon(row);
        });
    }
    ui->tblList->blockSignals(false);
}

void MainWindow::loadTableSchema(QSqlDatabase &db, const QString &tableName)
{
    ui->opTable->blockSignals(true);
    if(newTables.isEmpty() || !newTables.contains(tblName)){
    QSqlQuery query(db);
    query.prepare("DESCRIBE " + tableName);
    if (!query.exec()) {
        QMessageBox::critical(nullptr, QObject::tr("Query Error"),
                              QObject::tr("Failed to retrieve table schema: %1").arg(query.lastError().text()));
        return;
    }
    cmList.clear();
    ui->opTable->clear();
    ui->opTable->setRowCount(0);
    ui->opTable->setColumnCount(6);
    ui->opTable->setHorizontalHeaderLabels({"Field", "Type", "Null", "Extra", "Key", "Default"});
    ui->opTable->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);
    ui->opTable->setColumnWidth(2, 34);
    for(int row =0; query.next(); row++)
    {
        ui->opTable->insertRow(row);
        ui->opTable->setItem(row, 0, new QTableWidgetItem(query.value("Field").toString()));
        ui->opTable->setItem(row, 1, new QTableWidgetItem(query.value("Type").toString()));

        if(query.value("Null").toString() == "NO"){
            QTableWidgetItem *check = new QTableWidgetItem();
            check->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            check->setCheckState(Qt::Unchecked);
            check->setText("");
            ui->opTable->setItem(row, 2, check);
        }
        else
        {
            QTableWidgetItem *check = new QTableWidgetItem();
            check->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            check->setCheckState(Qt::Checked);
            ui->opTable->setItem(row, 2, check);
        }

        ui->opTable->setItem(row, 3, new QTableWidgetItem(query.value("Extra").toString()));
        ui->opTable->setItem(row, 4, new QTableWidgetItem(query.value("Key").toString()));
        ui->opTable->setItem(row, 5, new QTableWidgetItem(query.value("Default").toString()));
        ui->opTable->item(row,1)->setTextAlignment(Qt::AlignCenter);
        ui->opTable->item(row,3)->setTextAlignment(Qt::AlignCenter);
        ui->opTable->item(row,4)->setTextAlignment(Qt::AlignCenter);
        ui->opTable->item(row,5)->setTextAlignment(Qt::AlignCenter);
        ui->opTable->update();

        cmList[row].append(query.value("Field").toString());
        cmList[row].append(query.value("Type").toString());
        cmList[row].append(query.value("Null").toString());
        cmList[row].append(query.value("Extra").toString());
        cmList[row].append(query.value("Key").toString());
        cmList[row].append(query.value("Default").toString());
    }
    QIcon addRowIcon(":/icons/Icons/add.svg");
    QTableWidgetItem *headerItem = new QTableWidgetItem;
    headerItem->setIcon(addRowIcon);
    ui->opTable->insertRow(ui->opTable->rowCount());
    ui->opTable->setVerticalHeaderItem(ui->opTable->rowCount()-1, headerItem);

    connect(ui->opTable->verticalHeader(), &QHeaderView::sectionClicked, this, [this](int section) {
        if (ui->opTable->rowCount() > 0 && section == ui->opTable->rowCount()-1) {
            createNewRow();
        }
    });
    }
    else if(!newTables.isEmpty() && newTables.contains(tblName))
    {
        cmList.clear();
        initialNewTable();
    }
    ui->opTable->blockSignals(false);

}

void MainWindow::createNewRow()
{
    QIcon addRowIcon(":/icons/Icons/add.svg");
    QTableWidgetItem *headerItem = new QTableWidgetItem;
    headerItem->setIcon(addRowIcon);
    ui->opTable->blockSignals(true);
    ui->opTable->takeVerticalHeaderItem(ui->opTable->rowCount()-1);
    ui->opTable->insertRow(ui->opTable->rowCount());
    ui->opTable->setItem(ui->opTable->rowCount()-2, 0, new QTableWidgetItem(""));
    ui->opTable->setItem(ui->opTable->rowCount()-2, 1, new QTableWidgetItem(""));
    ui->opTable->setItem(ui->opTable->rowCount()-2, 2, new QTableWidgetItem(""));
    ui->opTable->setItem(ui->opTable->rowCount()-2, 3, new QTableWidgetItem(""));
    ui->opTable->setItem(ui->opTable->rowCount()-2, 4, new QTableWidgetItem(""));
    ui->opTable->setItem(ui->opTable->rowCount()-2, 5, new QTableWidgetItem(""));
    ui->opTable->item(ui->opTable->rowCount()-2,1)->setTextAlignment(Qt::AlignCenter);
    ui->opTable->item(ui->opTable->rowCount()-2,3)->setTextAlignment(Qt::AlignCenter);
    ui->opTable->item(ui->opTable->rowCount()-2,4)->setTextAlignment(Qt::AlignCenter);
    ui->opTable->item(ui->opTable->rowCount()-2,5)->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem *check = new QTableWidgetItem();
    check->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    check->setCheckState(Qt::Unchecked);
    check->setText("");
    ui->opTable->setItem(ui->opTable->rowCount()-2, 2, check);
    ui->opTable->setVerticalHeaderItem(ui->opTable->rowCount()-1, headerItem);
    ui->opTable->blockSignals(false);
}

void MainWindow::on_tblList_cellChanged(int row, int column)
{
    if(ui->tblList->item(row,column)->text() != ""){
        edtQuery.append(QStringLiteral("ALTER TABLE %1 %2;").arg(tblName, ui->tblList->item(row,column)->text()));
        ui->edtQuery->setPlainText(edtQuery);
        preRowTbl.append(row);
        tblChanged = true;
        ui->opTable->setDisabled(true);
    }
}

void MainWindow::on_tblList_cellClicked(int row, int column)
{
    if(tblChanged){
        ui->opTable->blockSignals(true);
        tblName = tblList[row];
        db.setDatabaseName(dbName);
        loadTableSchema(db, tblName);
    }
    else
    {
        ui->opTable->blockSignals(true);
        tblName = ui->tblList->item(row,column)->text();
        db.setDatabaseName(dbName);
        loadTableSchema(db, tblName);
    }
    ui->lblTableName->setText("Table: "+tblName);
}

void MainWindow::on_btnAddtbl_pressed()
{
    if(!ui->edtAddtbl->text().isEmpty()){
        ui->tblList->blockSignals(true);
        ui->tblList->insertRow(ui->tblList->rowCount());
        int row = ui->tblList->rowCount()-1;
        ui->tblList->setItem(row,0, new QTableWidgetItem(QString(ui->edtAddtbl->text())));
        ui->tblList->item(row,0)->setBackground(QBrush(QColor(255,165,0,150)));

        QPushButton *button = new QPushButton(QString(""));
        button->setStyleSheet("QPushButton{background-color: transparent}");
        button->setIcon(QIcon(QIcon(":/icons/Icons/trash.svg")));
        button->setIconSize(QSize(20, 20));
        ui->tblList->setIndexWidget(QModelIndex(ui->tblList->model()->index(row,1)), button);

        QObject::connect(button, &QPushButton::clicked, this, [row, this]() {
            delTableIcon(row);
        });

        newTables.append(ui->edtAddtbl->text());
        tblName = ui->edtAddtbl->text();
        ui->edtAddtbl->clear();
        cmList.clear();
        initialNewTable();
        ui->tblList->blockSignals(false);
    }
}

void MainWindow::initialNewTable()
{
    ui->opTable->blockSignals(true);
    ui->opTable->clear();
    ui->opTable->setRowCount(1);
    ui->opTable->setColumnCount(6);
    ui->opTable->setHorizontalHeaderLabels({"Field", "Type", "Null", "Extra", "Key", "Default"});
    ui->opTable->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);
    ui->opTable->setColumnWidth(2, 34);

    QIcon addRowIcon(":/icons/Icons/add.svg");
    QTableWidgetItem *headerItem = new QTableWidgetItem;
    headerItem->setIcon(addRowIcon);
    ui->opTable->setVerticalHeaderItem(ui->opTable->rowCount()-1, headerItem);

    connect(ui->opTable->verticalHeader(), &QHeaderView::sectionClicked, this, [this](int section) {
        if (ui->opTable->rowCount() > 0 && section == ui->opTable->rowCount()-1) {
            createNewRow();
        }
    });

    ui->opTable->blockSignals(false);
}

void MainWindow::on_opTable_cellChanged(int row, int column)
{
    if(ui->opTable->item(row,0)->text().isEmpty() || ui->opTable->item(row,1)->text().isEmpty()){
        ui->opTable->item(row,column)->setText(preCell);
    }
    if(preCell != ui->opTable->item(row,column)->text()){
    int i = lastEdit.count();
    lastEdit[i].append(db.databaseName());
    lastEdit[i].append(tblName);
    lastEdit[i].append(QString::number(row));
    lastEdit[i].append(QString::number(column));
    lastEdit[i].append(preCell);
    lastEdit[i].append(ui->opTable->item(row,column)->text());
    }

}

void MainWindow::on_opTable_cellClicked(int row, int column)
{
    if(ui->opTable->item(row,column) != nullptr)
    {
        preCell = ui->opTable->item(row,column)->text();
    }
}

void MainWindow::on_opTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(previousRow);
    Q_UNUSED(previousColumn);
    if(ui->opTable->item(currentRow,currentColumn) != nullptr)
    {
        preCell = ui->opTable->item(currentRow,currentColumn)->text();
    }
}

void MainWindow::on_btnGenQuery_pressed()
{
    QString oldField, field, type, isNull, key, def, extra;
    QString nullCheck;
    if(!cmList.isEmpty()){
        for (int i = 0; i < cmList.count(); i++){
            nullCheck = ui->opTable->item(i,2)->checkState()?"YES":"NO";
            if(ui->opTable->item(i,0)->text() != cmList[i][0] || ui->opTable->item(i,1)->text() != cmList[i][1] ||
                 nullCheck != cmList[i][2] || ui->opTable->item(i,3)->text() != cmList[i][3] ||
                ui->opTable->item(i,4)->text() != cmList[i][4] || ui->opTable->item(i,4)->text() != cmList[i][4] ||
                ui->opTable->item(i,5)->text() != cmList[i][5])
            {
                oldField = cmList[i][0];
                field = ui->opTable->item(i,0)->text();
                type = ui->opTable->item(i,1)->text();
                isNull = ui->opTable->item(i, 2)->checkState()?"":"not null";
                extra = ui->opTable->item(i,3)->text();
                key = ui->opTable->item(i,4)->text().contains("PRI",Qt::CaseInsensitive) ?"PRIMARY KEY":"";
                def = ui->opTable->item(i,5)->text();
                if(def == ""){
                    edtQuery.append(QStringLiteral("ALTER TABLE %1 CHANGE %2 %3 %4 %5 %6 %7;").arg(
                            tblName, oldField, field, type, isNull, extra, key).simplified());
                }
                else
                {
                    edtQuery.append(QStringLiteral("ALTER TABLE %1 CHANGE %2 %3 %4 %5 %6 %7 DEFAULT %8;").arg(
                            tblName, oldField, field, type, isNull, extra, key, def).simplified());
                }
            }
        }
        for(int i = cmList.count(); i < ui->opTable->rowCount()-1; i++){
            field = ui->opTable->item(i,0)->text();
            type = ui->opTable->item(i,1)->text();
            isNull = ui->opTable->item(i, 2)->checkState()?"":"not null";
            extra = ui->opTable->item(i,3)->text();
            key = ui->opTable->item(i,4)->text().contains("PRI",Qt::CaseInsensitive) ?"PRIMARY KEY":"";
            def = ui->opTable->item(i,5)->text();
            if(def == ""){
                edtQuery.append(QStringLiteral("ALTER TABLE %1 ADD COLUMN IF NOT EXISTS %2 %3 %4 %5 %6;").arg(
                    tblName, field, type, isNull, extra, key).simplified());
            }
            else
            {
                edtQuery.append(QStringLiteral("ALTER TABLE %1 ADD COLUMN IF NOT EXISTS %2 %3 %4 %5 %6 DEFAULT %7;").arg(
                    tblName, field, type, isNull, extra, key, def).simplified());
            }
        }
        ui->edtQuery->document()->setPlainText(edtQuery);
        edtQuery.clear();
    }
    else if(!newTables.isEmpty() && cmList.isEmpty())
    {
        edtQuery = QStringLiteral("CREATE TABLE IF NOT EXISTS %1 (").arg(tblName);
        for (int i = 0; i < ui->opTable->rowCount()-1; i++){
            nullCheck = ui->opTable->item(i,2)->checkState()?"YES":"NO";
            field = ui->opTable->item(i,0)->text();
            type = ui->opTable->item(i,1)->text();
            isNull = ui->opTable->item(i, 2)->checkState()?"":"not null";
            extra = ui->opTable->item(i,3)->text();
            key = ui->opTable->item(i,4)->text().contains("PRI",Qt::CaseInsensitive) ?"PRIMARY KEY":"";
            def = ui->opTable->item(i,5)->text();
            if(def == ""){
                edtQuery.append(QStringLiteral("%2 %3 %4 %5 %6,").arg(
                                            field, type, isNull, extra, key).simplified());
            }
            else
            {
                edtQuery.append(QStringLiteral("%2 %3 %4 %5 %6 DEFAULT %7,").arg(
                                            field, type, isNull, extra, key, def).simplified());
            }
        }
        edtQuery.append(");").replace(",);",");");
        ui->edtQuery->document()->setPlainText(edtQuery.simplified());
        newTables.data()->remove(tblName);
        edtQuery.clear();
    }
}

void MainWindow::on_btnDelRow_pressed()
{
    QSet<int> selectedRowIndices;
    QList<QTableWidgetItem*> selectedItems = ui->opTable->selectedItems();
    foreach (QTableWidgetItem* item, selectedItems) {
        selectedRowIndices.insert(item->row());
    }
    foreach (int rowIndex, selectedRowIndices) {
        edtQuery.append(QStringLiteral("ALTER TABLE %1 DROP COLUMN IF EXISTS %2;").arg(
                       tblName, ui->opTable->item(rowIndex,0)->text()));
    }
    query.prepare(edtQuery);
    if(QMessageBox::question(this,"Delete Column","Are you sure want to delete selected row(s)?",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
    {
    if(query.exec()){
        ui->statusbar->showMessage("Selected row(s) successfully removed",2000);
        edtQuery.clear();
        loadTableSchema(db,tblName);
    }
    else{
        ui->statusbar->showMessage(query.lastError().text());
    }
    }
}

void MainWindow::on_btnExecuteQuery_clicked()
{
    if(ui->edtQuery->toPlainText().isEmpty()){
        ui->statusbar->showMessage("Query can't be empty!",2000);
    }
    else{
        db.open();
        query.prepare(ui->edtQuery->toPlainText());
        if(query.exec()){
            ui->statusbar->showMessage("Query successfully executed",2000);
            ui->edtQuery->clear();
            preQuery = edtQuery;
            edtQuery.clear();
            connectToDatabase(dbName);
            loadTableSchema(db,tblName);
            ui->tblList->blockSignals(true);
            for(int i = 0; i < ui->tblList->rowCount();i++){
                ui->tblList->item(i,0)->setBackground(QBrush(QColor(Qt::transparent)));
            }
            preRowTbl.clear();
            tblChanged = false;
            ui->tblList->blockSignals(false);
            ui->opTable->setDisabled(false);
        }
        else{
            ui->statusbar->showMessage(query.lastError().text());
        }
    }
}

void MainWindow::on_edtQuery_textChanged()
{

}

void MainWindow::on_actionUndo_triggered()
{
    connectToDatabase(db.databaseName());
    // loadTableSchema(db,tblName);
    if(tblChanged){
        ui->tblList->blockSignals(true);
        ui->opTable->setDisabled(false);
        ui->edtQuery->clear();
        tblChanged = false;
        ui->tblList->blockSignals(false);
    }
    if(!lastEdit.isEmpty()){
        ui->opTable->setItem(lastEdit[lastEdit.count()-1][2].toInt()
                             ,lastEdit[lastEdit.count()-1][3].toInt(),
                             new QTableWidgetItem(QString(lastEdit[lastEdit.count()-1][4])));
        if(lastEdit[lastEdit.count()-1][3].toInt() != 0){
        ui->opTable->item(lastEdit[lastEdit.count()-1][2].toInt()
                             ,lastEdit[lastEdit.count()-1][3].toInt()
                            )->setTextAlignment(Qt::AlignCenter);
        }
        lastEdit.remove(lastEdit.count()-1);
    }
}

void MainWindow::on_btnExportDb_pressed()
{
    QProcess *process = new QProcess(this);

    QString defaultFileName = dbName + ".sql";
    QFileDialog dialog(this, "Export Database", "/", "*.sql");
    dialog.setOption(QFileDialog::DontUseNativeDialog, true);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.selectFile(defaultFileName);

    if (dialog.exec() == QDialog::Accepted) {
        QString saveFile = dialog.selectedFiles().first();
        QString script = QString(
                             "osascript -e 'do shell script \"/usr/local/bin/mysqldump %1 > %2\"'").arg(dbName, saveFile);

        process->start("bash", QStringList() << "-c" << script);

        if (!process->waitForStarted()) {
            qWarning() << "Failed to start process:" << process->errorString();
            process->deleteLater();
            return;
        }

        if (!process->waitForFinished()) {
            qWarning() << "Failed to finish process:" << process->errorString();
            process->deleteLater();
            return;
        }

        if (process->exitCode() != 0) {
            qWarning() << "Process finished with errors. Output:" << process->readAllStandardError();
        } else {
            ui->statusbar->showMessage("Database Exported Successfully", 3000);
        }

        process->deleteLater();
    }


}

void MainWindow::on_btnImportDb_pressed()
{
    QProcess *process = new QProcess(this);

    QFileDialog dialog(this, "Import Database", "/", "*.sql");
    dialog.setOption(QFileDialog::DontUseNativeDialog, true);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    if (dialog.exec() == QDialog::Accepted) {
        if (QMessageBox::question(this, "Replace Database",
                                  "By continuing, the current database will be replaced with the imported one. Are you sure?",
                                  QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {

            QString openFile = dialog.selectedFiles().first();

            QSqlQuery query;
            QString qry = QStringLiteral("DROP DATABASE %1;").arg(dbName);
            if (!query.exec(qry)) {
                ui->statusbar->showMessage(query.lastError().text());
                return;
            }

            qry = QStringLiteral("CREATE DATABASE %1;").arg(dbName);
            if (!query.exec(qry)) {
                ui->statusbar->showMessage(query.lastError().text());
                return;
            }

            qry = QStringLiteral("USE %1;").arg(dbName);
            if (!query.exec(qry)) {
                ui->statusbar->showMessage(query.lastError().text());
                return;
            }

            QString script = QString(
                                 "osascript -e 'do shell script \"/usr/local/bin/mysql %1 < %2\" with administrator privileges'")
                                 .arg(dbName)
                                 .arg(openFile);

            process->start("bash", QStringList() << "-c" << script);

            if (!process->waitForStarted()) {
                qWarning() << "Failed to start process:" << process->errorString();
                process->deleteLater();
                return;
            }

            if (!process->waitForFinished()) {
                qWarning() << "Failed to finish process:" << process->errorString();
                process->deleteLater();
                return;
            }

            if (process->exitCode() != 0) {
                qWarning() << "Process finished with errors. Output:" << process->readAllStandardError();
            } else {
                ui->statusbar->showMessage("Database Imported Successfully", 3000);
            }

            process->deleteLater();
            handleEditIconClick(ui->dbTable->currentIndex());
        }
    }

}

void MainWindow::on_btnTables_pressed()
{

    on_dbTable_cellDoubleClicked(ui->dbTable->currentIndex().row(),0);
}


void MainWindow::on_tblDbUsers_cellClicked(int row, int column)
{
    if(column == 2)
    {
        ui->stackedWidget->setCurrentIndex(4);
        ui->lblDbName_2->setText(dbName);
        ui->lblUserName->setText(ui->tblDbUsers->item(row,0)->text());
        if(ui->tblDbUsers->item(row,1)->text() == "Full Access"){
            ui->chFullAccess->setCheckState(Qt::Checked);
        }
        else
        {
            ui->chFullAccess->setCheckState(Qt::Unchecked);
            QString str = ui->tblDbUsers->item(row,1)->text();
            str.replace(", ", ",");
            QList<QString> prev = str.split(",");
            foreach (QString str, prev) {
                foreach(QCheckBox *checkbox, checkboxes) {
                    if(checkbox->text() == str){
                        checkbox->setCheckState(Qt::Checked);
                    }
                }
            }
        }
    }
}


void MainWindow::on_btnBackDb2_2_pressed()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->tabWidget->setCurrentIndex(0);
}


void MainWindow::on_btnUsers_pressed()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->tabWidget->setCurrentIndex(1);
}


void MainWindow::on_btnRevokeAll_pressed()
{
    QString qry = QStringLiteral("REVOKE ALL PRIVILEGES ON %1.* FROM %2;").arg(dbName).arg(ui->lblUserName->text());
    query.prepare(qry);
    if(!query.exec()){
        ui->statusbar->showMessage("Failed." + query.lastError().text());
    }

    qry = "FLUSH PRIVILEGES;";
    query.prepare(qry);
    if(!query.exec()){
        ui->statusbar->showMessage("Failed." + query.lastError().text());
    }
    else{
        ui->statusbar->showMessage(QStringLiteral("All previleges of %1 revoked from %2").arg(dbName).arg(ui->lblUserName->text()));
        ui->chFullAccess->setCheckState(Qt::Unchecked);
    }
}


void MainWindow::on_btnSetPrivileges_pressed()
{
    QString qry;
    if(ui->chFullAccess->checkState() == Qt::Checked){
        qry = QStringLiteral("GRANT ALL PRIVILEGES ON %1.* TO %2;").arg(dbName).arg(ui->lblUserName->text());
        query.prepare(qry);
        if(!query.exec()){
            ui->statusbar->showMessage("Failed." + query.lastError().text());
        }

        qry = "FLUSH PRIVILEGES;";
        query.prepare(qry);
        if(!query.exec()){
            ui->statusbar->showMessage("Failed." + query.lastError().text());
        }
        else{
            ui->statusbar->showMessage(QStringLiteral("All previleges of %1 granted to %2").arg(dbName).arg(ui->lblUserName->text()));
        }
    }
    else{
        qry = QStringLiteral("REVOKE ALL PRIVILEGES ON %1.* FROM %2;").arg(dbName).arg(ui->lblUserName->text());
        query.prepare(qry);
        if(!query.exec()){
            ui->statusbar->showMessage("Failed." + query.lastError().text());
        }

        QString list = prvgList.join(",");
        if(!prvgList.isEmpty()){

        qry = QStringLiteral("GRANT %1 ON %2.* TO %3;").arg(list).arg(dbName).arg(ui->lblUserName->text());
        query.prepare(qry);
        if(!query.exec()){
            ui->statusbar->showMessage("Failed." + query.lastError().text());
        }

        qry = "FLUSH PRIVILEGES;";
        query.prepare(qry);
        if(!query.exec()){
            ui->statusbar->showMessage("Failed." + query.lastError().text());
        }
        else{
            ui->statusbar->showMessage(QStringLiteral("Selected previleges of %1 granted to %2").arg(dbName).arg(ui->lblUserName->text()));
        }

        }
        else{
            ui->statusbar->showMessage(QStringLiteral("All previleges of %1 revoked from %1").arg(dbName).arg(ui->lblUserName->text()));
        }
    }

}

void MainWindow::onCheckBoxStateChanged(bool state)
{
    Q_UNUSED(state);
    QCheckBox* senderCheckBox = qobject_cast<QCheckBox*>(sender());
    if (senderCheckBox) {
        if(senderCheckBox->checkState() == Qt::Checked){
            prvgList.append(senderCheckBox->text());
        }
        else
        {
            prvgList.removeAt(prvgList.indexOf(senderCheckBox->text()));
        }
        int checked = 0;
        foreach(QCheckBox* checkbox, checkboxes){
            if(checkbox->isChecked()){
                checked ++;
            }
        }
        if(checked == checkboxes.count()){
            ui->chFullAccess->setCheckState(Qt::Checked);
        }
        else if(checked == 0){
            ui->chFullAccess->setCheckState(Qt::Unchecked);
            prvgList.clear();
        }
        else{
            ui->chFullAccess->blockSignals(true);
            ui->chFullAccess->setCheckState(Qt::Unchecked);
            ui->chFullAccess->blockSignals(false);
        }
    }
}


void MainWindow::on_chFullAccess_checkStateChanged(const Qt::CheckState &arg1)
{
    foreach(QCheckBox* checkbox, checkboxes) {
        checkbox->setCheckState(arg1);
    }
}


void MainWindow::on_btnAccess_pressed()
{
    QString qry;
    QString user = QStringLiteral("'%1'").arg(ui->cbUsersList->currentText());
    qry = QStringLiteral("GRANT ALL PRIVILEGES ON %1.* TO %2;").arg(dbName).arg(user);
    query.prepare(qry);
    if(!query.exec()){
        ui->statusbar->showMessage("Failed." + query.lastError().text());
    }

    qry = "FLUSH PRIVILEGES;";
    query.prepare(qry);
    if(!query.exec()){
        ui->statusbar->showMessage("Failed." + query.lastError().text());
    }
    else{
        handleEditIconClick(ui->dbTable->currentIndex());
        ui->statusbar->showMessage(QStringLiteral("All previleges of %1 granted to %2").arg(dbName).arg(ui->cbUsersList->currentText()));
    }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    QString qry;
    qry = QStringLiteral("SELECT User, Host, authentication_string AS password from mysql.user;");
    if (!query.exec(qry)) {
        qWarning() << "Failed to execute query:" << query.lastError().text();
        return;
    }
    ui->tblUsersList->clearContents();
    QIcon icon = QIcon(":/icons/Icons/trash.svg");
    ui->tblUsersList->blockSignals(true);
    QStringList usrList = {"root","PUBLIC","mariadb.sys"};

    for(int i = 0; query.next(); i++) {
        ui->tblUsersList->insertRow(i);
        ui->tblUsersList->setItem(i,0, new QTableWidgetItem(query.value(0).toString()));
        ui->tblUsersList->setItem(i,1, new QTableWidgetItem(query.value(1).toString()));
        ui->tblUsersList->setItem(i,2, new QTableWidgetItem(query.value(2).toString()));
        if(ui->tblUsersList->item(i, 2)->text() != ""){
            ui->tblUsersList->item(i, 2)->setText("********");
        }
        else
        {
           ui->tblUsersList->item(i, 2)->setText("");
        }
        if(usrList.contains(ui->tblUsersList->item(i,0)->text(),Qt::CaseSensitive)){

        }
        else
        {
        ClickableLabel *iconLabel = new ClickableLabel(i, this);
        iconLabel->setPixmap(icon.pixmap(20, 20));
        iconLabel->setAlignment(Qt::AlignCenter);
        connect(iconLabel, &ClickableLabel::clicked, this, &MainWindow::onIconLabelClicked);
        ui->tblUsersList->setCellWidget(i, 3, iconLabel);
        }
        ui->tblUsersList->item(i,0)->setTextAlignment(Qt::AlignCenter);
        ui->tblUsersList->item(i,1)->setTextAlignment(Qt::AlignCenter);
        ui->tblUsersList->item(i,2)->setTextAlignment(Qt::AlignCenter);
    }
    ui->tblUsersList->blockSignals(false);
}

void MainWindow::onIconLabelClicked(int row){
    QString qry;
    QString user = ui->tblUsersList->item(row,0)->text();
    QString host = ui->tblUsersList->item(row,1)->text();
    qry = QStringLiteral("DROP USER '%1'@'%2';").arg(user).arg(host);
    query.prepare(qry);
    if(QMessageBox::question(this,"Delete User","Are you sure want to delete this user?",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes){
    if(!query.exec())
    {
        ui->statusbar->showMessage("Failed: " + query.lastError().text());
        return;
    }
    on_tabWidget_currentChanged(1);
    ui->statusbar->showMessage("User Removed Successfully");
    }
}



void MainWindow::on_tblUsersList_cellChanged(int row, int column)
{
    if(column == 2){
        QString qry;
        QString user = ui->tblUsersList->item(row,0)->text();
        QString host = ui->tblUsersList->item(row,1)->text();
        QString pass = ui->tblUsersList->item(row, 2)->text();
        QStringList usrList = {"root","PUBLIC","mariadb.sys"};
        if(usrList.contains(user,Qt::CaseSensitive)){
           ui->statusbar->showMessage(QStringLiteral("Can't change password for %1").arg(user));
        }
        else
        {
        qry = QStringLiteral("ALTER USER '%1'@'%2' IDENTIFIED BY '%3';").arg(user).arg(host).arg(pass);
        query.prepare(qry);
        if(!query.exec())
        {
            ui->statusbar->showMessage("Failed: " + query.lastError().text());
        }
        ui->statusbar->showMessage(QStringLiteral("Password successfully changed for %1").arg(user));
        ui->tblUsersList->item(row,column)->setText("********");
    }
    }
}


void MainWindow::on_btnCreateNewUser_pressed()
{
    QString qry;
    QString user = ui->edtUserName->text();
    QString host = ui->edtUserHost->text();
    QString pass = ui->edtUserPassword->text();
    if(ui->edtUserHost->text() == ""){
        host = "localhost";
    }
    qry = QStringLiteral("CREATE USER '%1'@'%2';").arg(user).arg(host);
    if(ui->edtUserPassword->text() != ""){
    qry = QStringLiteral("CREATE USER '%1'@'%2' IDENTIFIED BY '%3';").arg(user).arg(host).arg(pass);
    }
    query.prepare(qry);
    if(!query.exec()){
        ui->statusbar->showMessage("Failed: " + query.lastError().text());
        return;
    }
    ui->statusbar->showMessage("User successfully created");
    on_tabWidget_currentChanged(1);
}

