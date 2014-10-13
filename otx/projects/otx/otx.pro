#-------------------------------------------------
#
# OTX QT Project File
#
#-------------------------------------------------

# note:  make sure you read: http://www.qtcentre.org/wiki/index.php?title=Undocumented_qmake
# so many functions that are not documented.

#-------------------------------------------------
# Global

TEMPLATE    = lib
CONFIG     += precompile_header
CONFIG += staticlib
win32:CONFIG += console

TARGET      = OTX
VERSION     = 0.0.1

QT         += core gui sql network widgets
DEFINES    += "OT_ZMQ_MODE=1"

unix:{
LIBS       += -L"/usr/local/lib" -lzmq -lxmlrpc_client++ -lxmlrpc -lxmlrpc++ -lotapi -lot -ldl
}
else: {
LIBS       +=  -lzmq -lxmlrpc_client++ -lxmlrpc -lxmlrpc++ -lotapi -lot -ldl
}

#-------------------------------------------------
# Common Settings

include(../common.pri)

#-------------------------------------------------
# Source

PRECOMPILED_HEADER = $${SOLUTION_DIR}../src/core/stable.hpp

include($${SOLUTION_DIR}../src/core/core.pri)
include($${SOLUTION_DIR}../src/gui/gui.pri)
include($${SOLUTION_DIR}../src/bitcoin/bitcoin.pri)
include($${SOLUTION_DIR}../src/namecoin/namecoin.pri)



#-------------------------------------------------
# Package Config

# MAC AND LINUX:
unix:{
    #PKGCONFIG += opentxs
}


#-------------------------------------------------
# Include

win32:{
    equals(TEMPLATE,vcapp):{
        INCLUDEPATH += $(SystemDrive)/OpenSSL-Win$(PlatformArchitecture)/include
        }
    else:{
        !contains(QMAKE_HOST.arch, x86_64):{
            INCLUDEPATH += C:/OpenSSL-Win32/include
        }
        else:{
            INCLUDEPATH += C:/OpenSSL-Win64/include
        }
    }
}

mac:{
    !contains(MAC_OS_VERSION, 10.9): {
        INCLUDEPATH += $$QMAKE_MAC_SDK/System/Library/Frameworks/CoreFoundation.framework/Versions/A/Headers

        INCLUDEPATH += $${SOLUTION_DIR}..src/ycmd/boost

    }
}

#-------------------------------------------------
# Linked Libs

# MAC AND LINUX:
unix: {

##  LIBS += -L$${OUT_PWD}/../curl
##  LIBS += -lcurl

    LIBS += -L$${OUT_PWD}/../bitcoin-api
    LIBS += -lbitcoin-api

    LIBS += -L$${OUT_PWD}/../jsoncpp
    LIBS += -ljsoncpp

    LIBS += -L$${OUT_PWD}/../nmcrpc
    LIBS += -lnmcrpc


    mac:{
        !contains(MAC_OS_VERSION, 10.9):{
            # if not on Mavericks
            LIBS += -lboost_system-mt
            LIBS += -lboost_thread-mt
            LIBS += -lboost_chrono-mt
            LIBS += -lboost_atomic-mt

        }
    }
    # LINUX:
    else: {
        lessThan(GCC_VERSION, 4.7):{
            # only need if no C++11 available
            LIBS += -lboost_system-mt
            LIBS += -lboost_thread-mt
            LIBS += -lboost_chrono-mt
            LIBS += -lboost_atomic-mt
        }
    }


}

win32: {
    QMAKE_LIBDIR += $${DESTDIR}

    equals(TEMPLATE,vcapp):{
        QMAKE_LIBDIR += $(SystemDrive)/OpenSSL-Win$(PlatformArchitecture)/lib/VC
        QMAKE_LIBDIR += $${SOLUTION_DIR}../../Open-Transactions/lib/$(PlatformName)/$(Configuration)/
    }
    else:{
        !contains(QMAKE_HOST.arch, x86_64):{
            QMAKE_LIBDIR += C:/OpenSSL-Win32/lib/VC
            CONFIG(debug, debug|release):{
                QMAKE_LIBDIR += $${SOLUTION_DIR}../../Open-Transactions/lib/Win32/Debug/
            }
            else:{
                QMAKE_LIBDIR += $${SOLUTION_DIR}../../Open-Transactions/lib/Win32/Release/
            }
        }
        else:{
            QMAKE_LIBDIR += C:/OpenSSL-Win64/lib/VC
            CONFIG(debug, debug|release):{
                QMAKE_LIBDIR += $${SOLUTION_DIR}../../Open-Transactions/lib/x64/Debug/
            }
            else:{
                QMAKE_LIBDIR += $${SOLUTION_DIR}../../Open-Transactions/lib/x64/Release/
            }
        }
    }

    LIBS += bitcoin-api.lib
    LIBS += jsoncpp.lib
    LIBS += curl.lib
    LIBS += nmcrpc.lib

    LIBS += otlib.lib
    LIBS += otapi.lib

    LIBS += Advapi32.lib
}



# MAC AND LINUX:
# need to put -ldl last.
unix:{

    LIBS += -lcurl


}
