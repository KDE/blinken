
# Define here your path to the root containing libkok
PATHTOLIBKOKROOT = /home/tsdgeos/devel/libkok/

TEMPLATE = app
TARGET =
DEPENDPATH += .
INCLUDEPATH += . ../src
QT += svg phonon declarative

INCLUDEPATH += $${PATHTOLIBKOKROOT}/usr/include $${PATHTOLIBKOKROOT}/usr/include/KDE
LIBS += -L $${PATHTOLIBKOKROOT}/usr/lib -lkok

KCONFIG_COMPILER_INPUT = ../src/blinken.kcfg

kconfig_compiler.output  = settings.o
kconfig_compiler.commands = kconfig_compiler ${QMAKE_FILE_NAME} ../src/settings.kcfgc -d ${OBJECTS_DIR} && $${QMAKE_CXX} $(CXXFLAGS) -O0 $(INCPATH) -c settings.cpp -o ${QMAKE_FILE_OUT}
kconfig_compiler.input = KCONFIG_COMPILER_INPUT
QMAKE_EXTRA_COMPILERS += kconfig_compiler

# Input
HEADERS += blinkenproxywidget.h \
           ../src/blinken.h \
           ../src/blinkengame.h \
           ../src/button.h \
           ../src/counter.h \
           ../src/highscoredialog.h \
           ../src/number.h \
           ../src/soundsplayer.h

SOURCES += blinkenproxywidget.cpp \
           ../src/blinken.cpp \
           ../src/blinkengame.cpp \
           ../src/button.cpp \
           ../src/counter.cpp \
           ../src/highscoredialog.cpp \
           ../src/main.cpp \
           ../src/number.cpp \
           ../src/soundsplayer.cpp \
           kfontutils.cpp

DEFINES += HARMATTAN_BUILD

target.path = /opt/blinken/bin
INSTALLS += target

svg.files = ../images/blinken.svg
svg.path = /opt/blinken/images
INSTALLS += svg

sounds.files = ../sounds/1.wav ../sounds/2.wav ../sounds/3.wav ../sounds/4.wav ../sounds/lose.wav
sounds.path = /opt/blinken/sounds
INSTALLS += sounds

qml.files = qml/main.qml
qml.path = /opt/blinken/qml
INSTALLS += qml

font.files = ../fonts/steve.ttf
font.path = /opt/blinken/fonts
INSTALLS += font

desktopfile.files = blinken_harmattan.desktop
desktopfile.path = /usr/share/applications/
INSTALLS += desktopfile

desktopicon.files = blinken80.png
desktopicon.path = /usr/share/icons/hicolor/80x80/apps
INSTALLS += desktopicon

copying.files = ../COPYING
copying.path = /opt/blinken/
INSTALLS += copying

evil_hack_to_fool_lupdate {
    SOURCES += qml/main.qml
}

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog
