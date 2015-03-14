
#include "postedmsgmodel.h"
#include <retroshare/rsposted.h>

#include <iostream>


PostedMsgModel::PostedMsgModel(QObject *parent)
    : GxsMsgModel(rsPosted->getTokenService(), parent)
{
}


void PostedMsgModel::load(uint32_t token)
{
	beginResetModel();

	mEntries.clear();
	rsPosted->getPostData(mToken, mEntries);

	endResetModel();
}


int PostedMsgModel::rowCount(const QModelIndex & parent) const 
{
	return mEntries.size();
}


QVariant PostedMsgModel::data(const QModelIndex & index, int role) const 
{
	int idx = index.row();
	if (idx < 0 || idx >= mEntries.size())
		return QVariant();

	if (role < GxsMsgModel::MsgExtraRole)
	{
		const RsMsgMetaData &peerData = mEntries[idx].mMeta;
		return GxsMsgModel::metadata(peerData, role);
	}

	const RsPostedPost &entry = mEntries[idx];

	if (role == LinkRole)
	{
		return QString::fromStdString(entry.mLink);
	}
	else if (role == NotesRole)
	{
		return QString::fromUtf8(entry.mNotes.c_str());
	}
	else if (role == HotScoreRole)
	{
		return QVariant(entry.mHotScore);
	}
	else if (role == TopScoreRole)
	{
		return QVariant(entry.mTopScore);
	}
	else if (role == NewScoreRole)
	{
		return QVariant(entry.mNewScore);
	}
	else if (role == HaveVotedRole)
	{
		bool v = entry.mHaveVoted;
		return QVariant(v);
	}
	else if (role == UpVotesRole)
	{
		uint32_t v = entry.mUpVotes;
		return QVariant(v);
	}
	else if (role ==DownVotesRole)
	{
		uint32_t v = entry.mDownVotes;
		return QVariant(v);
	}
	else if (role ==CommentsRole)
	{
		uint32_t v = entry.mComments;
		return QVariant(v);
	}

	return QVariant();
}


QHash<int, QByteArray> PostedMsgModel::roleNames() const 
{
	QHash<int, QByteArray> roles = GxsMsgModel::roleNames();
	roles[LinkRole] = "Link";
	roles[NotesRole] = "Notes";

	roles[HotScoreRole] = "HotScore";
	roles[TopScoreRole] = "TopScore";
	roles[NewScoreRole] = "NewScore";

	roles[HaveVotedRole] = "HaveVoted";
	roles[UpVotesRole] = "UpVotes";
	roles[DownVotesRole] = "DownVotes";
	roles[CommentsRole] = "Comments";

    return roles;
}

