#pragma once

#include "gxsmsgmodel.h"
#include <retroshare/rsgxsforums.h>


class ForumMsgModel : public GxsMsgModel
{
    Q_OBJECT
public:
    enum ForumMsgRoles {
        MsgRole = GxsMsgModel::MsgExtraRole + 1,
    };

    ForumMsgModel(QObject *parent = 0);

public slots:
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    virtual void load(uint32_t token);
    virtual QHash<int, QByteArray> roleNames() const;

    std::vector<RsGxsForumMsg> mEntries;
};


