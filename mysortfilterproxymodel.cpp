#include "mysortfilterproxymodel.h"

MySortFilterProxyModel::MySortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

bool MySortFilterProxyModel::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, 2, sourceParent);
    QModelIndex index1 = sourceModel()->index(sourceRow, 3, sourceParent);
    QModelIndex index2 = sourceModel()->index(sourceRow, 4, sourceParent);
    QModelIndex index3 = sourceModel()->index(sourceRow, 5, sourceParent);

    //return ((if() ? true : sourceModel()->data(index0).toString().contains());
}
