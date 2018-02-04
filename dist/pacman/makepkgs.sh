#! /bin/bash
BASEDIR=$(dirname "$(readlink -f makepkgs.sh)")
cd "${BASEDIR}"

PKG_VERSION=1.1.0
PKG_RELEASE=1

echo Building package qync ...
cd "${BASEDIR}"/qync
. "${BASEDIR}"/makepkg.sh --pkg-version $PKG_VERSION --pkg-release $PKG_RELEASE --pkg-depends "rsync qt5-base hicolor-icon-theme" --pkg-makedepends "cmake qt5-tools"

if [ 0 -eq $? ]; then
    echo "OK"
else
    echo "failed"
	exit 1
fi

cp *.pkg.tar.xz "${BASEDIR}"/
