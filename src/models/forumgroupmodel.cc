
#include "forumgroupmodel.h"
#include <retroshare/rsgxsforums.h>

#include <iostream>


ForumGroupModel::ForumGroupModel(QObject *parent)
    : GxsGroupModel(rsGxsForums->getTokenService(), parent)
{
}


void ForumGroupModel::load(uint32_t token)
{
	beginResetModel();

	mEntries.clear();

	// sort them into Subscription order.
        std::vector<RsGxsForumGroup> entries;
	rsGxsForums->getGroupData(mToken, entries);

        std::vector<RsGxsForumGroup>::iterator it;
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


int ForumGroupModel::rowCount(const QModelIndex & parent) const 
{
	return mEntries.size();
}


QVariant ForumGroupModel::data(const QModelIndex & index, int role) const 
{
	int idx = index.row();
	if (idx < 0 || idx >= mEntries.size())
		return QVariant();

	if (role < GxsGroupModel::GroupExtraRole)
	{
		const RsGroupMetaData &peerData = mEntries[idx].mMeta;
		return GxsGroupModel::metadata(peerData, role);
	}

	const RsGxsForumGroup &entry = mEntries[idx];

	//std::cerr << "ForumGroupModel::data() row: " << index.row() << " b#: " << bucketno << " e#: " << entryno;
	//std::cerr << std::endl;

	if (role == DescRole)
		return QString::fromStdString(entry.mDescription);
	return QVariant();
}


QHash<int, QByteArray> ForumGroupModel::roleNames() const 
{
    QHash<int, QByteArray> roles = GxsGroupModel::roleNames();
    roles[DescRole] = "Desc";
    return roles;
}

