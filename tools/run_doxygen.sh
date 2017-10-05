#! /usr/bin/env bash
THISTOOL=$(basename "$0")
DOXYGEN=$(which doxygen)
PROJECTNAME="$1"

if [ "" == "${DOXYGEN}" ]; then
    echo >&2 "${THISTOOL}: ERR Doxygen does not appear to be installed. Try \"sudo apt install doxygen\"."
	exit 1
fi

cd "$(dirname "$0")/.."

if [ "" == "${PROJECTNAME}" ]; then
	PROJECTNAME="$(basename "$(pwd)")"
	echo "${THISTOOL}: WRN No project name specified - using \"${PROJECTNAME}\""
fi

if ! [ -e "${PROJECTNAME}.Doxyfile" ]; then
    echo "${THISTOOL}: Creating default Doxygen configuration file \"${PROJECTNAME}.Doxyfile\""
	$DOXYGEN -s -g "${PROJECTNAME}.Doxyfile"
	sed -i -E "s/^ *OUTPUT_DIRECTORY *=.*\$/OUTPUT_DIRECTORY = ./docs/${PROJECTNAME}/system"
fi

OUTPATH="$(./tools/get_doxygen_output_path.sh)"

echo "${THISTOOL}: Building documentation in \"${OUTPATH}\""
mkdir -p "${OUTPATH}"
$DOXYGEN "${PROJECTNAME}.Doxyfile"
