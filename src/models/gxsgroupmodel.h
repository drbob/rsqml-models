#pragma once

#include <QAbstractListModel>
#include <QStringList>
#include <retroshare/rstokenservice.h>

// Low Base Model for any GXS system.
// This includes a basic selection.

class GxsGroupModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum GroupRoles {
        GroupIdRole = Qt::UserRole + 1,
        GroupNameRole,
        GroupFlagsRole,
        SignFlagsRole,
        PublishTsRole,
        AuthorIdRole,
        CircleIdRole,
        CircleTypeRole,
        AuthenFlagsRole,
        ParentIdRole,
        SubscribeFlagsRole,
        SubscribeStatusRole,
        // space for extra ones in Gxs specific subclasses.
	GroupExtraRole
    };

    GxsGroupModel(RsTokenService *service, QObject *parent = 0);

public slots:
    void refresh(uint32_t token);
    void check_load();
    void updateEntries();

    //virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    //virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    virtual void load(const uint32_t token) = 0;
    virtual QVariant metadata(const RsGroupMetaData &data, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

    RsTokenService *mService;
    uint32_t mToken;
};


