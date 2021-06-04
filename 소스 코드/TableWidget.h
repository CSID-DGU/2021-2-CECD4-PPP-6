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

        void addHeadList(QList<QString> &headList);

        void addCheckList(QMap<CheckType, QString> &checkList);

        void updateTable(CheckType type, CheckLevel level);

        QMap<CheckType, CheckLevel> getCheckState() {
            return _checkLevelMap;
        }

    private:
        void _initTable(QList<QString> &headList);

        void _insertItem(int row, int column, QString value);

    private:
        QMap<CheckType, int> _checkNumMap;
        QMap<CheckType, CheckLevel> _checkLevelMap;
};

#endif // TABLEWIDGET_H
