#!/bin/bash

# This file is part of Roboverb
# Copyright (C) 2015  Kushview, LLC.  All rights reserved.

here=`pwd`
version="1.0.5"
# code sign id
cid="9511F5241B78B0D38961CD756873066A5F0ED225"
# installer sign id
installer_identity="5F3E07CD73FE49A2977993251492068772DDD9FC"

set -e

rm -rf build
cd roboverb/Builds/MacOSX
xcodebuild clean build -project Roboverb.xcodeproj -configuration 'Release'

cd "${here}/installers/packages"
rm -rf build/
/usr/local/bin/packagesbuild -v Roboverb.pkgproj

cd build
mkdir Roboverb-OSX-${version}
productsign --sign ${installer_identity} \
    Roboverb-OSX-${version}_unsigned.pkg \
    Roboverb-OSX-${version}/Roboverb-OSX-${version}.pkg
zip -rT Roboverb-OSX-${version}.zip Roboverb-OSX-${version}
rm  -rf Roboverb-OSX-${version}
rm  -f *.pkg *.mpkg

cd "$here"
