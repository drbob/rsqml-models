#pragma once

#include "gxsgroupmodel.h"
#include <retroshare/rsgxschannels.h>


class ChannelGroupModel : public GxsGroupModel
{
    Q_OBJECT
public:
    enum ForumGroupRoles {
        DescRole = GxsGroupModel::GroupExtraRole + 1,
        ImageRole,
        AutoDownloadRole,
    };

    ChannelGroupModel(QObject *parent = 0);

public slots:
    //virtual void updateEntries();
    //virtual void createGroup();



    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    virtual void load(uint32_t token);
    virtual QHash<int, QByteArray> roleNames() const;

    std::vector<RsGxsChannelGroup> mEntries;
};


