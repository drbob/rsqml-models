#pragma once

#include "gxsmsgmodel.h"
#include <retroshare/rsgxschannels.h>


class ChannelMsgModel : public GxsMsgModel
{
    Q_OBJECT
public:
    enum ChannelMsgRoles {
        MsgRole = GxsMsgModel::MsgExtraRole + 1,

	ThumbnailRole,
	NumberFilesRole,
	TotalFileSizeRole,

	// This are returned as a StringList... could be handled better.
	FileNamesRole,
	FileSizesRole,
	FileHashesRole,

        // These are not in ChannelPosts Yet - but they should be!
        HaveVotedRole,
        UpVotesRole,
        DownVotesRole,
        CommentsRole,
    };

    ChannelMsgModel(QObject *parent = 0);

public slots:
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    virtual void load(uint32_t token);
    virtual QHash<int, QByteArray> roleNames() const;

    std::vector<RsGxsChannelPost> mEntries;
};


