
#include "postedgroupmodel.h"
#include <retroshare/rsposted.h>

#include <iostream>


PostedGroupModel::PostedGroupModel(QObject *parent)
    : GxsGroupModel(rsPosted->getTokenService(), parent)
{
}


void PostedGroupModel::load(uint32_t token)
{
	beginResetModel();

	mEntries.clear();

	// sort them into Subscription order.
        std::vector<RsPostedGroup> entries;
	rsPosted->getGroupData(mToken, entries);

        std::vector<RsPostedGroup>::iterator it;
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


int PostedGroupModel::rowCount(const QModelIndex & parent) const 
{
	return mEntries.size();
}


QVariant PostedGroupModel::data(const QModelIndex & index, int role) const 
{
	int idx = index.row();
	if (idx < 0 || idx >= mEntries.size())
		return QVariant();

	if (role < GxsGroupModel::GroupExtraRole)
	{
		const RsGroupMetaData &peerData = mEntries[idx].mMeta;
		return GxsGroupModel::metadata(peerData, role);
	}

	const RsPostedGroup &entry = mEntries[idx];

	if (role == DescRole)
		return QString::fromStdString(entry.mDescription);
	return QVariant();
}


QHash<int, QByteArray> PostedGroupModel::roleNames() const 
{
    QHash<int, QByteArray> roles = GxsGroupModel::roleNames();
    roles[DescRole] = "Desc";
    return roles;
}

