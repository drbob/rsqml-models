
#include "channelmsgmodel.h"
#include <retroshare/rsgxschannels.h>

#include <iostream>

#include <QPixmap>


ChannelMsgModel::ChannelMsgModel(QObject *parent)
    : GxsMsgModel(rsGxsChannels->getTokenService(), parent)
{
}


void ChannelMsgModel::load(uint32_t token)
{
	beginResetModel();

	mEntries.clear();
	rsGxsChannels->getPostData(mToken, mEntries);

	endResetModel();
}


int ChannelMsgModel::rowCount(const QModelIndex & parent) const 
{
	return mEntries.size();
}


QVariant ChannelMsgModel::data(const QModelIndex & index, int role) const 
{
	int idx = index.row();
	if (idx < 0 || idx >= mEntries.size())
		return QVariant();

	if (role < GxsMsgModel::MsgExtraRole)
	{
		const RsMsgMetaData &peerData = mEntries[idx].mMeta;
		return GxsMsgModel::metadata(peerData, role);
	}

	const RsGxsChannelPost &entry = mEntries[idx];

	if (role == MsgRole)
	{
		return QString::fromStdString(entry.mMsg);
	}
	else if (role == ThumbnailRole)
	{
		QPixmap pix ;
    		if (entry.mThumbnail.mSize == 0 || !pix.loadFromData(entry.mThumbnail.mData, entry.mThumbnail.mSize, "PNG"))
		{
			QVariant();
		}
		return pix;
	}
	else if (role == NumberFilesRole)
	{
		return QVariant(entry.mCount);
	}
	else if (role == TotalFileSizeRole)
	{
		return QVariant((int) entry.mSize);
	}
	else if (role == FileNamesRole)
	{
		QStringList names;
		std::list<RsGxsFile>::const_iterator it;
		for(it = entry.mFiles.begin(); it != entry.mFiles.end(); it++)
		{
			names.push_back(QString::fromUtf8(it->mName.c_str()));
		}
		return names;
	}
	else if (role == FileSizesRole)
	{
		QStringList sizes;
		std::list<RsGxsFile>::const_iterator it;
		for(it = entry.mFiles.begin(); it != entry.mFiles.end(); it++)
		{
			sizes.push_back(QString::number(it->mSize));
		}
		return sizes;
	}
	else if (role == FileHashesRole)
	{
		QStringList hashes;
		std::list<RsGxsFile>::const_iterator it;
		for(it = entry.mFiles.begin(); it != entry.mFiles.end(); it++)
		{
			hashes.push_back(QString::fromStdString(it->mHash.toStdString()));
		}
		return hashes;
	}

	else if (role == HaveVotedRole)
	{
		bool v = false;
		return QVariant(v);
	}
	else if (role == UpVotesRole)
	{
		int v = 0;
		return QVariant(v);
	}
	else if (role ==DownVotesRole)
	{
		int v = 0;
		return QVariant(v);
	}
	else if (role ==CommentsRole)
	{
		int v = 0;
		return QVariant(v);
	}

	return QVariant();
}


QHash<int, QByteArray> ChannelMsgModel::roleNames() const 
{
	QHash<int, QByteArray> roles = GxsMsgModel::roleNames();
	roles[MsgRole] = "Msg";
	roles[ThumbnailRole] = "Thumbnail";
	roles[NumberFilesRole] = "NumberFiles";
	roles[TotalFileSizeRole] = "TotalFileSize";

	roles[FileNamesRole] = "FileNames";
	roles[FileSizesRole] = "FileSizes";
	roles[FileHashesRole] = "FileHashes";

	roles[HaveVotedRole] = "HaveVoted";
	roles[UpVotesRole] = "UpVotes";
	roles[DownVotesRole] = "DownVotes";
	roles[CommentsRole] = "Comments";

    return roles;
}

