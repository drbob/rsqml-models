TEMPLATE = app
TARGET = rsqml

QT += quick
QT += declarative
QT += widgets
QT += qml

RCC_DIR = temp/qrc
UI_DIR  = temp/ui
MOC_DIR = temp/moc


# if you are linking against the libretroshare with gxs.
# this option links against the required sqlite library.
CONFIG += bitdht
CONFIG += gxs

#CONFIG += debug
debug {
        QMAKE_CFLAGS -= -O2
        QMAKE_CFLAGS += -O0
        QMAKE_CFLAGS += -g

        QMAKE_CXXFLAGS -= -O2
        QMAKE_CXXFLAGS += -O0
        QMAKE_CXXFLAGS += -g
}

################################# Linux ##########################################
linux-* {
	#CONFIG += version_detail_bash_script
	QMAKE_CXXFLAGS *= -D_FILE_OFFSET_BITS=64

	LIBS += ../../libretroshare/src/lib/libretroshare.a
	LIBS += ../../openpgpsdk/src/lib/libops.a -lbz2
	LIBS += -lssl -lupnp -lixml -lgnome-keyring
	LIBS *= -lcrypto -ldl -lz -lpthread
	LIBS *= -rdynamic

	gxs {
		SQLCIPHER_OK = $$system(pkg-config --exists sqlcipher && echo yes)
			isEmpty(SQLCIPHER_OK) {
# We need a explicit path here, to force using the home version of sqlite3 that really encrypts the database.

				exists(../../../lib/sqlcipher/.libs/libsqlcipher.a) {

					LIBS += ../../../lib/sqlcipher/.libs/libsqlcipher.a
						DEPENDPATH += ../../../lib/sqlcipher/src/
						INCLUDEPATH += ../../../lib/sqlcipher/src/
				} else {
					message(libsqlcipher.a not found. Compilation will not use SQLCIPHER. Database will be unencrypted.)
						DEFINES *= NO_SQLCIPHER
						LIBS *= -lsqlite3
				}

			} else {
				LIBS *= -lsqlcipher
			}
	}
}

linux-g++ {
	OBJECTS_DIR = temp/linux-g++/obj
}

linux-g++-64 {
	OBJECTS_DIR = temp/linux-g++-64/obj
}

#################### Cross compilation for windows under Linux ###################

win32-x-g++ {
	OBJECTS_DIR = temp/win32-x-g++/obj

	LIBS += ../../../../lib/win32-x-g++/libretroshare.a 
	LIBS += ../../../../lib/win32-x-g++/libssl.a 
	LIBS += ../../../../lib/win32-x-g++/libcrypto.a 
	LIBS += ../../../../lib/win32-x-g++/libminiupnpc.a 
	LIBS += ../../../../lib/win32-x-g++/libz.a 
	LIBS += -L${HOME}/.wine/drive_c/pthreads/lib -lpthreadGCE2
	LIBS += -lws2_32 -luuid -lole32 -liphlpapi -lcrypt32 -gdi32
	LIBS += -lole32 -lwinmm

	RC_FILE = gui/images/retroshare_win.rc

	DEFINES *= WIN32
}

#################################### Windows #####################################

win32 {
	CONFIG += console
	OBJECTS_DIR = temp/obj
	RCC_DIR = temp/qrc
	UI_DIR  = temp/ui
	MOC_DIR = temp/moc

	PRE_TARGETDEPS *= ../../libretroshare/src/lib/libretroshare.a
	PRE_TARGETDEPS *= ../../openpgpsdk/src/lib/libops.a

	LIBS_DIR = $$PWD/../../../libs

	LIBS += ../../libretroshare/src/lib/libretroshare.a
	LIBS += ../../openpgpsdk/src/lib/libops.a -lbz2
	LIBS += -L"$$LIBS_DIR/lib"
	LIBS += -lssl -lcrypto -lpthread -lminiupnpc -lz
# added after bitdht
#	LIBS += -lcrypto -lws2_32 -lgdi32
	LIBS += -luuid -lole32 -liphlpapi -lcrypt32
	LIBS += -lole32 -lwinmm

	PROTOCPATH=$$LIBS_DIR/bin/

	RC_FILE = resources/retroshare_win.rc

	DEFINES *= WINDOWS_SYS _USE_32BIT_TIME_T

	DEPENDPATH += $$LIBS_DIR/include
	INCLUDEPATH += $$LIBS_DIR/include

	gxs {
		LIBS += ../../supportlibs/pegmarkdown/lib/libpegmarkdown.a
		LIBS += -lsqlcipher
	}
}

##################################### MacOS ######################################

macx {
    # ENABLE THIS OPTION FOR Univeral Binary BUILD.
    # CONFIG += ppc x86 

	LIBS += -Wl,-search_paths_first
	LIBS += ../../libretroshare/src/lib/libretroshare.a
	LIBS += ../../openpgpsdk/src/lib/libops.a -lbz2
        LIBS += -lssl -lcrypto -lz 
	LIBS += ../../../miniupnpc-1.0/libminiupnpc.a
	LIBS += -framework CoreFoundation
	LIBS += -framework Security

	gxs {
		# We need a explicit path here, to force using the home version of sqlite3 that really encrypts the database.
	    # LIBS += ../../../lib/sqlcipher/.libs/libsqlcipher.a
	    LIBS += ../../../lib/libsqlcipher.a
	}

	sshserver {
		LIBS += -L../../../lib
		#LIBS += -L../../../lib/libssh-0.6.0
	}

    QMAKE_CXXFLAGS *= -Dfseeko64=fseeko -Dftello64=ftello -Dstat64=stat -Dstatvfs64=statvfs -Dfopen64=fopen

}

##################################### FreeBSD ######################################

freebsd-* {
	INCLUDEPATH *= /usr/local/include/gpgme
	LIBS *= ../../libretroshare/src/lib/libretroshare.a
	LIBS *= -lssl
	LIBS *= -lgpgme
	LIBS *= -lupnp
	LIBS *= -lgnome-keyring
	PRE_TARGETDEPS *= ../../libretroshare/src/lib/libretroshare.a
}

##################################### OpenBSD  ######################################

openbsd-* {
	INCLUDEPATH *= /usr/local/include
	QMAKE_CXXFLAGS *= -Dfseeko64=fseeko -Dftello64=ftello -Dstat64=stat -Dstatvfs64=statvfs -Dfopen64=fopen
	LIBS *= ../../libretroshare/src/lib/libretroshare.a
	LIBS *= ../../openpgpsdk/src/lib/libops.a -lbz2
	LIBS *= -lssl -lcrypto
	LIBS *= -lgpgme
	LIBS *= -lupnp
	LIBS *= -lgnome-keyring
	PRE_TARGETDEPS *= ../../libretroshare/src/lib/libretroshare.a
	PRE_TARGETDEPS *= ../../openpgpsdk/src/lib/libops.a
	LIBS *= -rdynamic
}


############################## Common stuff ######################################

# bitdht config
bitdht {
	LIBS += ../../libbitdht/src/lib/libbitdht.a
}

DEPENDPATH += . ../../libretroshare/src
INCLUDEPATH += . ../../libretroshare/src

# rs-nogui
HEADERS +=  notifytxt.h 
SOURCES +=  notifytxt.cc \
            retroshare.cc 

# rsqml
HEADERS +=  rsqml_main.h
SOURCES +=  rsqml_main.cc \

# models
HEADERS += models/gxsgroupmodel.h \
		models/gxsmsgmodel.h \
		models/gxsidmodel.h \
		models/forumgroupmodel.h \
		models/forummsgmodel.h \
		models/channelgroupmodel.h \
		models/channelmsgmodel.h \
		models/postedgroupmodel.h \
		models/postedmsgmodel.h

SOURCES += models/gxsgroupmodel.cc \
		models/gxsmsgmodel.cc \
		models/gxsidmodel.cc \
		models/forumgroupmodel.cc \
		models/forummsgmodel.cc \
		models/channelgroupmodel.cc \
		models/channelmsgmodel.cc \
		models/postedgroupmodel.cc \
		models/postedmsgmodel.cc

RESOURCES += rsqml.qrc

win32 {
# must be added after ssh
	LIBS += -lcrypto -lws2_32 -lgdi32
}
