
#include "forummsgmodel.h"
#include <retroshare/rsgxsforums.h>

#include <iostream>


ForumMsgModel::ForumMsgModel(QObject *parent)
    : GxsMsgModel(rsGxsForums->getTokenService(), parent)
{
}


void ForumMsgModel::load(uint32_t token)
{
	beginResetModel();

	mEntries.clear();
	rsGxsForums->getMsgData(mToken, mEntries);

	endResetModel();
}


int ForumMsgModel::rowCount(const QModelIndex & parent) const 
{
	return mEntries.size();
}


QVariant ForumMsgModel::data(const QModelIndex & index, int role) const 
{
	int idx = index.row();
	if (idx < 0 || idx >= mEntries.size())
		return QVariant();

	if (role < GxsMsgModel::MsgExtraRole)
	{
		const RsMsgMetaData &peerData = mEntries[idx].mMeta;
		return GxsMsgModel::metadata(peerData, role);
	}

	const RsGxsForumMsg &entry = mEntries[idx];

	//std::cerr << "ForumMsgModel::data() row: " << index.row() << " b#: " << bucketno << " e#: " << entryno;
	//std::cerr << std::endl;

	if (role == MsgRole)
		return QString::fromStdString(entry.mMsg);
	return QVariant();
}


QHash<int, QByteArray> ForumMsgModel::roleNames() const 
{
    QHash<int, QByteArray> roles = GxsMsgModel::roleNames();
    roles[MsgRole] = "Msg";
    return roles;
}

