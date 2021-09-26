#include <QHeaderView>
#include <QTableWidgetItem>

#include "TableWidget.h"

/////////////////////////////////////////////////////////////////////////////////////
// 构造函数
// 默认 : 10行 2列
/////////////////////////////////////////////////////////////////////////////////////
TableWidget::TableWidget(QWidget *parent)
    : QTableWidget(10, 2, parent)
{
    _checkNumMap.clear();
    _checkLevelMap.clear();
}

/////////////////////////////////////////////////////////////////////////////////////
// 析构函数
/////////////////////////////////////////////////////////////////////////////////////
TableWidget::~TableWidget() {
    _checkNumMap.clear();
    _checkLevelMap.clear();
}

/////////////////////////////////////////////////////////////////////////////////////
// 添加表头
/////////////////////////////////////////////////////////////////////////////////////
void TableWidget::addHeadList(QList<QString> &headList) {
    _initTable(headList);
}

/////////////////////////////////////////////////////////////////////////////////////
// 添加检测队列
/////////////////////////////////////////////////////////////////////////////////////
void TableWidget::addCheckList(QMap<CheckType, QString> &checkList) {
    if (checkList.isEmpty()) {
        return;
    }

    // 清除表格内容
    clearContents();

    // 插入数据
    int rows = rowCount();
    int count = checkList.count();
    for(int row = 0; row < count; row++) {
        // 插入数据数大于初始化行数，新插入一行
        if (row >= rows) {
            insertRow(row);
        }

        CheckType type = checkList.keys().at(row);
        QString checkInfo = checkList.value(type);
        QString checkLevel = "";
        _insertItem(row, 0, checkInfo);
        _insertItem(row, 1, checkLevel);

        _checkNumMap.insert(type, row);
    }
}

/////////////////////////////////////////////////////////////////////////////////////
// 更新表格数据
/////////////////////////////////////////////////////////////////////////////////////
void TableWidget::updateTable(CheckType type, CheckLevel level) {
    _checkLevelMap.insert(type, level);

    QColor color = QColor(Qt::gray);
    QString text = "";
    switch(level) {
        case CheckLevel::HIGH_LEVEL :
            color = QColor(21, 159, 92);
            text = "안전";
            break;
        case CheckLevel::MEDIUM_LEVEL:
            color = QColor(255, 205, 67);
            text = "일반";
            break;
        case CheckLevel::LOW_LEVEL :
            color = QColor(221, 79, 67);
            text = "취약";
            break;
        default:
            break;
    }

    int row = _checkNumMap.value(type);
    QTableWidgetItem *tableItem = item(row, 1);
    if (nullptr == tableItem) {
        return;
    }

    tableItem->setBackground(color);
    tableItem->setText(text);
    tableItem->setTextColor(QColor(Qt::white));
}

/////////////////////////////////////////////////////////////////////////////////////
// 初始化表格
/////////////////////////////////////////////////////////////////////////////////////
void TableWidget::_initTable(QList<QString> &headList) {
    if (headList.isEmpty()) {
        return;
    }

    // 添加表头
    int columns = headList.count();
    setColumnCount(columns);
    setHorizontalHeaderLabels(headList);

    // 设置不可编辑
    setEditTriggers(NoEditTriggers);

    // 设置表头宽度
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    setColumnWidth(1, 120);

    // 设置垂直表头隐藏
    verticalHeader()->setVisible(false);

    // 表头加粗
    QFont font = this->horizontalHeader()->font();
    font.setBold(true);
    horizontalHeader()->setFont(font);
}

/////////////////////////////////////////////////////////////////////////////////////
// 插入单元格数据
/////////////////////////////////////////////////////////////////////////////////////
void TableWidget::_insertItem(int row, int column, QString value) {
    QTableWidgetItem *tableItem = item(row, column);
    if (nullptr == tableItem) {
        tableItem = new QTableWidgetItem();
    }

    tableItem->setText(value);
    tableItem->setTextAlignment(Qt::AlignLeft);
    setItem(row, column, tableItem);
}
