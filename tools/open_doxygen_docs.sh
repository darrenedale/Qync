#! /usr/bin/env bash
THISTOOL=$(basename "$0")
PROJECTNAME="$1"
cd "$(dirname "$0")/.."

if [ "" == "${PROJECTNAME}" ]; then
    PROJECTNAME="$(basename "$(pwd)")"
	echo >&2 "${THISTOOL}: WRN No project name specified - using \"${PROJECTNAME}\""
fi

DOCPATH=$(./tools/get_doxygen_output_path.sh "${PROJECTNAME}")

if [ "" == "${DOCPATH}" ]; then
    echo >&2 "${THISTOOL}: ERR The path to the documentation for ${PROJECTNAME} is not known."
	exit 1
fi

if [ "/" == "${DOCPATH:0:1}" ]; then
    # it's an absolute path
	xdg-open "file://${DOCPATH}/html/index.html"
else
    # it's a relative path
	xdg-open "file://$(pwd)/${DOCPATH}/html/index.html"
fi
