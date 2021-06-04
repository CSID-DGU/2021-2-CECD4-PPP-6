#include <QHeaderView>
#include <QTableWidgetItem>

#include "TableWidget.h"


TableWidget::TableWidget(QWidget *parent)
    : QTableWidget(10, 2, parent)
{
    _checkNumMap.clear();
    _checkLevelMap.clear();
}

TableWidget::~TableWidget() {
    _checkNumMap.clear();
    _checkLevelMap.clear();
}

void TableWidget::addHeadList(QList<QString> &headList) {
    _initTable(headList);
}

void TableWidget::addCheckList(QMap<CheckType, QString> &checkList) {
    if (checkList.isEmpty()) {
        return;
    }

    clearContents();

    int rows = rowCount();
    int count = checkList.count();
    for(int row = 0; row < count; row++) {
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

void TableWidget::_initTable(QList<QString> &headList) {
    if (headList.isEmpty()) {
        return;
    }

    int columns = headList.count();
    setColumnCount(columns);
    setHorizontalHeaderLabels(headList);

    setEditTriggers(NoEditTriggers);

    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    setColumnWidth(1, 120);

    verticalHeader()->setVisible(false);

    QFont font = this->horizontalHeader()->font();
    font.setBold(true);
    horizontalHeader()->setFont(font);
}

void TableWidget::_insertItem(int row, int column, QString value) {
    QTableWidgetItem *tableItem = item(row, column);
    if (nullptr == tableItem) {
        tableItem = new QTableWidgetItem();
    }

    tableItem->setText(value);
    tableItem->setTextAlignment(Qt::AlignLeft);
    setItem(row, column, tableItem);
}
