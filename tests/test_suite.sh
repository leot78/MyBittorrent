#!/bin/bash
RED='\033[0;31;7m'
GREEN='\033[0;32;7m'
BL='\033[0m'
YEL='\033[1;37m'
CYAN='\033[0;36;1m'

test_print () {
  count=$(($count + 1))
  echo -ne "TEST ${YEL}$1${BL}: "
  echo -ne "${CYAN}pretty-print $BL"

  if [ $2 -ne 0 ]; then
    echo -e "${RED}FAILED${BL} JSON not correct"
  else
    OK=$(($OK + 1))
    echo -e "${GREEN}OK${BL}"
  fi
}

test_leak () {
  count=$(($count + 1))
  echo -ne "TEST ${YEL}$1${BL}: "
  echo -ne "${CYAN}leak $BL"

  if [ $2 -ne 0 ]; then
    echo -e "${RED}FAILED${BL} JSON not correct"
  else
    OK=$(($OK + 1))
    echo -e "${GREEN}OK${BL}"
  fi
}

tests () {
  IFS=' '
  #echo "testing: '$1' args"
 

  res=$(./my-bittorrent --pretty-print-torrent-file $1 | python -m json.tool)
  test_print $1 $?
  leak=$(valgrind -q ./my-bittorrent --pretty-print-torrent-file $1)
  test_leak $1 $?
}

echo ""
lib=$1

args=$(echo tests/torrents/*.torrent)
tests_failed=""
OK=0
count=0
for arg in $args; do
  tests $arg
done

echo
echo "-------SUMMARY----------"
if [ $(wc -c <<< "$tests_failed") -gt 2 ]; then
  echo -e '\033[1;31m'"$OK/$count$BL tests are OK."
  echo -e '\033[1;31m'"Failed${BL} :$tests_failed"
else
  echo -e '\033[1;32m'"$OK/$count$BL tests are OK."
fi
echo "------------------------"
