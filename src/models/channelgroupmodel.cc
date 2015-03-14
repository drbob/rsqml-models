
#include "channelgroupmodel.h"
#include <retroshare/rsgxschannels.h>

#include <iostream>

#include <QPixmap>


ChannelGroupModel::ChannelGroupModel(QObject *parent)
    : GxsGroupModel(rsGxsChannels->getTokenService(), parent)
{
}


void ChannelGroupModel::load(uint32_t token)
{
	beginResetModel();

	mEntries.clear();

	// sort them into Subscription order.
        std::vector<RsGxsChannelGroup> entries;
	rsGxsChannels->getGroupData(mToken, entries);

        std::vector<RsGxsChannelGroup>::iterator it;
        for(it = entries.begin(); it != entries.end();)
        {
		if (IS_GROUP_ADMIN(it->mMeta.mSubscribeFlags)) {
			mEntries.push_back(*it);
			it = entries.erase(it);
		} else {
			it++;
		}
	}

        for(it = entries.begin(); it != entries.end();)
        {
		if (IS_GROUP_SUBSCRIBED(it->mMeta.mSubscribeFlags)) {
			mEntries.push_back(*it);
			it = entries.erase(it);
		} else {
			it++;
		}
	}

	/* add in the last */
	mEntries.insert(mEntries.end(), entries.begin(), entries.end());

	endResetModel();
}


int ChannelGroupModel::rowCount(const QModelIndex & parent) const 
{
	return mEntries.size();
}


QVariant ChannelGroupModel::data(const QModelIndex & index, int role) const 
{
	int idx = index.row();
	if (idx < 0 || idx >= mEntries.size())
		return QVariant();

	if (role < GxsGroupModel::GroupExtraRole)
	{
		const RsGroupMetaData &peerData = mEntries[idx].mMeta;
		return GxsGroupModel::metadata(peerData, role);
	}

	const RsGxsChannelGroup &entry = mEntries[idx];

	//std::cerr << "ChannelGroupModel::data() row: " << index.row() << " b#: " << bucketno << " e#: " << entryno;
	//std::cerr << std::endl;

	if (role == DescRole)
	{
		return QString::fromStdString(entry.mDescription);
	}
	else if (role == ImageRole)
	{
		QPixmap pix ;
    		if (entry.mImage.mSize == 0 || !pix.loadFromData(entry.mImage.mData, entry.mImage.mSize, "PNG"))
		{
			QVariant();
		}
		return pix;
	}
	else if (role == AutoDownloadRole)
	{
		return QVariant(entry.mAutoDownload);
	}
	return QVariant();
}


QHash<int, QByteArray> ChannelGroupModel::roleNames() const 
{
    QHash<int, QByteArray> roles = GxsGroupModel::roleNames();
    roles[DescRole] = "Desc";
    roles[ImageRole] = "Image";
    roles[AutoDownloadRole] = "AutoDownload";

    return roles;
}

