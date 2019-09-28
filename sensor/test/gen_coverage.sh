#!/bin/sh

# To generate coverage for artifacts at "ut_build" directory
# ./gen_coverage.sh ut_build

# To generate coverage and open browser
# ./gen_coverage.sh ut_build coverage

LCOV=lcov
GENHTML=genhtml
BROWSER="/Applications/Google Chrome.app/Contents/MacOS/Google Chrome"

SRC_DIR="${1}"
HTML_RESULTS="${1}/html"

# Override html directoy to current
#HTML_RESULTS="./html"

mkdir -p ${HTML_RESULTS}

# Do not remove un-wanted files like so, instead, use ${LCOV}" -r 
#rm "${SRC_DIR}"/unity.gcno

# generate our initial info
echo "--- Process files ---"
"${LCOV}" -d "${SRC_DIR}" -c -o "${SRC_DIR}/coverage.info"
 
# remove some paths
echo "--- Remove files ---"
"${LCOV}" -r "${SRC_DIR}/coverage.info" "*/usr/include/math.h" "*/test/*" "*/third_party/*" "*/ut_build/*" -o "${SRC_DIR}/coverage-filtered.info"

# generate our HTML
echo "--- Generate HTML ---"
"${GENHTML}" -o "${HTML_RESULTS}" "${SRC_DIR}/coverage-filtered.info"
 
# reset our counts
echo "--- Reset counts ---"
"${LCOV}" -d "${SRC_DIR}" -z
 
# open in browser and bring to front
# We are only checking if a 2nd argument is present
if [ "$#" -eq 2 ] ; then
echo "--- Attempting to open Browser"
"${BROWSER}" "${HTML_RESULTS}/index.html"
open -a "${BROWSER}"
fi

