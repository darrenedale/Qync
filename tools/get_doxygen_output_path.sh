#! /usr/bin/env bash
PROJECTNAME="$1"

cd "$(dirname "$0")/.."

if [ "" == "${PROJECTNAME}" ]; then
    PROJECTNAME="$(basename "$(pwd)")"
	echo >&2 "WRN No project name specified - using ${PROJECTNAME}"
fi

DOXYFILE="${PROJECTNAME}.Doxyfile"

if ! [ -e "${DOXYFILE}" ]; then
    echo >&2 "ERR \"${DOXYFILE}\" not found - trying Doxyfile"
	DOXYFILE="Doxyfile"

    if ! [ -e "${DOXYFILE}" ]; then
	    echo >&2 "ERR \"${DOXYFILE}\" not found - exiting"
		exit 1
	fi
fi

cat "${DOXYFILE}" | grep OUTPUT_DIRECTORY | sed -E "s/.*OUTPUT_DIRECTORY.*= *(.*) */\\1/"
