CONFIG+= release shared rtti no_plugin_manifest qpa
host_build {
    QT_ARCH = i386
    QT_TARGET_ARCH = i386
} else {
    QT_ARCH = i386
}
QT_CONFIG += minimal-config small-config medium-config large-config full-config debug_and_release build_all debug release shared zlib icu png freetype build_all accessibility opengl openssl audio-backend wmf-backend native-gestures qpa concurrent
#versioning 
QT_VERSION = 5.3.2
QT_MAJOR_VERSION = 5
QT_MINOR_VERSION = 3
QT_PATCH_VERSION = 2
