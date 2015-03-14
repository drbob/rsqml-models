#pragma once

#include "gxsmsgmodel.h"
#include <retroshare/rsposted.h>


class PostedMsgModel : public GxsMsgModel
{
    Q_OBJECT
public:
    enum PostedMsgRoles {
        LinkRole = GxsMsgModel::MsgExtraRole + 1,
	NotesRole,

	HotScoreRole,
	TopScoreRole,
	NewScoreRole,

        HaveVotedRole,
        UpVotesRole,
        DownVotesRole,
        CommentsRole,
    };

    PostedMsgModel(QObject *parent = 0);

public slots:
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    virtual void load(uint32_t token);
    virtual QHash<int, QByteArray> roleNames() const;

    std::vector<RsPostedPost> mEntries;
};


