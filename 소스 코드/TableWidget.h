#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

//
// 表格窗体
//

#include <QMap>
#include <QTableWidget>
#include "ConfDefined.h"

class TableWidget : public QTableWidget {
    Q_OBJECT
    public:
        TableWidget(QWidget *parent = nullptr);
        ~TableWidget();

        // 添加表头
        void addHeadList(QList<QString> &headList);

        // 添加检测项队列
        void addCheckList(QMap<CheckType, QString> &checkList);

        // 更新表格数据
        void updateTable(CheckType type, CheckLevel level);

        // 获取当前检测项状态
        QMap<CheckType, CheckLevel> getCheckState() {
            return _checkLevelMap;
        }

    private:
        // 初始化表格
        void _initTable(QList<QString> &headList);

        // 插入表格项
        void _insertItem(int row, int column, QString value);

    private:
        QMap<CheckType, int> _checkNumMap;                      // 检测类型行数映射
        QMap<CheckType, CheckLevel> _checkLevelMap;             // 检测类型等级映射
};

#endif // TABLEWIDGET_H
