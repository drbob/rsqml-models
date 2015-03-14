#pragma once

#include <QAbstractListModel>
#include <QStringList>
#include <retroshare/rstokenservice.h>

// Low Base Model for any GXS system.
// This includes a basic selection.

class GxsMsgModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum MsgRoles {
        GroupIdRole = Qt::UserRole + 1,
        MsgIdRole,
        ThreadIdRole,
        ParentIdRole,
        OrigMsgIdRole,
        AuthorIdRole,

        MsgNameRole,
        PublishTsRole,
        MsgFlagsRole,
        MsgStatusRole,
        ChildTsRole,

        // space for extra ones in Gxs specific subclasses.
	MsgExtraRole
    };

    GxsMsgModel(RsTokenService *service, QObject *parent = 0);

public slots:
    void refresh(uint32_t token);
    void check_load();
    void updateEntries(QString grpId);

protected:
    virtual void load(const uint32_t token) = 0;
    virtual QVariant metadata(const RsMsgMetaData &data, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

    RsTokenService *mService;
    uint32_t mToken;
};


