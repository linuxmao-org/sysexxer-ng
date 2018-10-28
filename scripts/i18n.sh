#!/bin/bash
set -e
set -x

po_update() {
    if test -f "$1"; then
        msgmerge -U "$1" "$1"t
    else
        msginit -i "$1"t -l "$locale" -o "$1"
    fi
}

for locale in "fr"; do
    mkdir -p po/"$locale"
    xgettext -k_ -k_P:1c,2 -L C++ --from-code=UTF-8 -c -s -o po/"$locale"/sysexxer-ng.pot \
             sources/*.{h,cc,cxx} \
             sources/device/*.{h,cc}
    po_update po/"$locale"/sysexxer-ng.po
    rm -f po/"$locale"/*.pot
done
