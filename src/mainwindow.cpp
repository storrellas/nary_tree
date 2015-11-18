#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Conenct signals and slots for logging
    connect(this, SIGNAL(consoleWriteSignal(QString)),
                        ui->consoleTextEdit, SLOT(appendPlainText(QString)));

    ///////////////////////////
    // Cheating section
    ///////////////////////////

    // Put here the default values

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::consoleWrite(const QString &line){
    QCoreApplication::processEvents();
    emit consoleWriteSignal(line);
}

void MainWindow::initialize(){
    loggerMacroDebug("Log from Initialize")

}

void MainWindow::on_generateTablesBtn_clicked()
{
    QSqlQuery query;

    loggerMacroDebug("Generating tables")

    // Generate employee table
    QString sqlStr  = QString("DROP table IF EXISTS employee;");
    query.exec(sqlStr);
    sqlStr  = QString("DROP table  IF EXISTS employee_stats;");
    query.exec(sqlStr);

    // Generate employee table
    sqlStr  = QString("CREATE TABLE employee ( ") +
                      QString("    id         INTEGER        PRIMARY KEY AUTOINCREMENT,") +
                      QString("    supervisor INT            REFERENCES employee ( id ),") +
                      QString("    name       VARCHAR( 20 ) ") +
                      QString(");");
    query.exec(sqlStr);

    query.exec("INSERT INTO [employee] ([id], [supervisor], [name]) VALUES (1, null, 'Sergi');");
    query.exec("INSERT INTO [employee] ([id], [supervisor], [name]) VALUES (2, 1, 'Albert');");
    query.exec("INSERT INTO [employee] ([id], [supervisor], [name]) VALUES (3, 2, 'Joan');");
    query.exec("INSERT INTO [employee] ([id], [supervisor], [name]) VALUES (4, 1, 'Carles');");
    query.exec("INSERT INTO [employee] ([id], [supervisor], [name]) VALUES (5, 3, 'Josep');");
    query.exec("INSERT INTO [employee] ([id], [supervisor], [name]) VALUES (6, 4, 'Maria');");
    query.exec("INSERT INTO [employee] ([id], [supervisor], [name]) VALUES (7, 4, 'Anna');");


    // Generate employee_stats table
    sqlStr  = QString("CREATE TABLE employee_stats ( ") +
              QString("    id          INTEGER PRIMARY KEY AUTOINCREMENT,") +
              QString("    employee_id INTEGER REFERENCES employee ( id ),") +
              QString("    n_sup       INTEGER,") +
              QString("    n_t_sup     INTEGER,") +
              QString("    depth       INTEGER,") +
              QString("    level       INTEGER ") +
              QString(");");
    query.exec(sqlStr);
    loggerMacroDebug("DONE!")
    return;
}

void MainWindow::on_performAnalysisBtn_clicked()
{

    // Load database results
    // -----------------------------

    // Load Hierarchy from DB
    this->loadHierarchy();


    // Calculate parameters
    loggerMacroDebug("-- Calculate parameters --")
    for(int i = 0; i < treeNode_list.size(); i++){
        this->calculateParameters(treeNode_list[i]);
    }


    // Commit results to DB
    loggerMacroDebug("-- Commiting results to DB --")
    foreach(TreeNode* treeNode, treeNode_list){
        this->commitResults(treeNode);
    }

}

///////////////////////////
// Algorithm
///////////////////////////

void MainWindow::loadHierarchy(){


    QList<Employee> employee_list;

    TreeNode* rootNode = new TreeNode();
    loggerMacroDebug("-- Looking for rootNode -- ")
    QSqlQuery query;
    query.exec("Select * from employee;");
    while (query.next()) {

        Employee employee;
        employee.id = query.value(0).toInt();
        employee.supervisor = query.value(1).toInt();
        employee.name = query.value(2).toString();
        loggerMacroDebug("id: " + QString::number(employee.id) + " supervisor:" + QString::number(employee.supervisor) + ":name: " + employee.name)
        employee_list.append(employee);

        if(employee.supervisor == 0 ){
            rootNode->set(employee);
            treeNode_list.append( rootNode );

        }
    }


    // Load Children for the employee_list
    loggerMacroDebug("-- Loading Tree -- ")
    loadChildren(*rootNode, employee_list);

}


void MainWindow::loadChildren(TreeNode& treeNode, QList<Employee> employee_list){
    //loggerMacroDebug("-- Entering for " + treeNode.get().name + " -- ")


    foreach (Employee employee, employee_list) {

        //loggerMacroDebug("Evaluating " + employee.name)
        if( employee.supervisor == ((Employee) treeNode.get()).id){
            //QString parentStr = "Parent:" + treeNode.get().toString();
            //QString childStr = "Child:" + employee.toString();
            //loggerMacroDebug(parentStr + "<>" + childStr)

            TreeNode* childNode = new TreeNode(employee, &treeNode);
            loadChildren(*childNode, employee_list);
            treeNode.addChild(*childNode);

            treeNode_list.append( childNode );

        }

    }

    //loggerMacroDebug("Children for node " + treeNode.node.toString() + " " + QString::number( treeNode.successors.count()))
}


int MainWindow::getLevel(TreeNode* treeNode){
    if( treeNode->parent == NULL ){
        // Do nothing
    }else{
        level++;
        this->getLevel(treeNode->parent);
    }
    return level;
}

int MainWindow::getNSUP(TreeNode* treeNode){
    return treeNode->successors.size();
}

int MainWindow::getNTSUP(TreeNode* treeNode){
    foreach(TreeNode childNode, treeNode->successors){
        ntsup++;
        getNTSUP(&childNode);
    }
}

int MainWindow::getDepth(TreeNode* treeNode){
    QList<int> childrenDepthList;
    foreach(TreeNode childNode, treeNode->successors){
        childrenDepthList.append( getDepth(&childNode) );
    }

    // If we are not in a leaf
    int max = 0;
    if( childrenDepthList.count() != 0){
        max = *std::max_element(childrenDepthList.begin(), childrenDepthList.end());
    }

    return (max+1);
}

void MainWindow::calculateParameters(TreeNode *treeNode){

    int local_level, local_nsup, local_ntsup, local_depth;

    // Get number of supervisors
    level = 0;
    this->getLevel(treeNode);
    local_level = level;


    // Get total number of immediate subordinates
    local_nsup = this->getNSUP(treeNode);

    // Get total number of subordinates
    ntsup = 0;
    this->getNTSUP(treeNode);
    local_ntsup = ntsup;


    // Get depth
    local_depth = this->getDepth(treeNode);

    // Store result in object
    treeNode->node.depth = local_depth;
    treeNode->node.nsup  = local_nsup;
    treeNode->node.ntsup = local_ntsup;
    treeNode->node.level = local_level;

    loggerMacroDebug( treeNode->get().toString() ) ;

}

void MainWindow::commitResults(TreeNode* treeNode){

    QSqlQuery query;
    QString sqlStr  = QString("INSERT INTO [employee_stats] ([employee_id], [n_sup], [n_t_sup], [depth], [level])") +
                      QString("VALUES(") +
                      QString::number(treeNode->node.id) + "," +
                      QString::number(treeNode->node.nsup) + "," +
                      QString::number(treeNode->node.ntsup) + "," +
                      QString::number(treeNode->node.depth) + "," +
                      QString::number(treeNode->node.level) +
                      QString(");");


    query.exec(sqlStr);

}






