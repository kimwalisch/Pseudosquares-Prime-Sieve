#!/bin/sh

if [ $# -ne 1 ]
then
    echo "Usage example:"
    echo "$ ./scripts/update_version.sh 1.2"
    echo ""
    echo "Updates the pseudosquares_prime_sieve version to 1.2 in all files"

    exit 1
fi

# Run from pseudosquares_prime_sieve root directory
test -e ../src && cd ..

new_version=$1
old_version=$(grep '^[[:space:]]*VERSION ' CMakeLists.txt | head -n 1 | awk '{print $2}')

if [ -z "$old_version" ]
then
    echo "Error: failed to detect current version in CMakeLists.txt"
    exit 1
fi

if ! grep -q "pseudosquares_prime_sieve $old_version" src/main.cpp
then
    echo "Error: version in src/main.cpp does not match CMakeLists.txt"
    exit 1
fi

new_year=$(date +'%Y')

main_year_count=$(grep -Ec 'Copyright \(C\) [0-9]{4} Kim Walisch' src/main.cpp)

if [ "$main_year_count" -ne 2 ]
then
    echo "Error: expected two copyright-year occurrences in src/main.cpp"
    exit 1
fi

license_line=$(sed -n '3p' LICENSE)

if ! echo "$license_line" | grep -Eq 'Copyright \(c\) [0-9]{4} - [0-9]{4}, Kim Walisch\.'
then
    echo "Error: unexpected copyright line in LICENSE"
    exit 1
fi

echo "New version: $new_version"
echo "Old version: $old_version"
echo ""
echo "Copyright year: $new_year"
echo ""

# 1. Update VERSION in CMakeLists.txt
echo "Update version in CMakeLists.txt"
sed "s/VERSION $old_version/VERSION $new_version/" CMakeLists.txt > CMakeLists.txt.tmp
mv -f CMakeLists.txt.tmp CMakeLists.txt

# 2. Update version and copyright years in src/main.cpp
echo "Update version and years in src/main.cpp"
sed -e "s/pseudosquares_prime_sieve $old_version/pseudosquares_prime_sieve $new_version/" \
    -e "s/Copyright (C) [0-9][0-9][0-9][0-9] Kim Walisch/Copyright (C) $new_year Kim Walisch/g" \
    src/main.cpp > src/main.cpp.tmp
mv -f src/main.cpp.tmp src/main.cpp

# 3. Update copyright year in LICENSE
echo "Update year in LICENSE"
sed "3 s/ - [0-9][0-9][0-9][0-9],/ - $new_year,/" LICENSE > LICENSE.tmp
mv -f LICENSE.tmp LICENSE

echo "Version has been updated!"
