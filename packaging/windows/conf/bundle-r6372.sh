#!/bin/bash
export BUNDLE_YARP_VERSION=2.3.22
export BUNDLE_YARP_REVISION=6372
export BUNDLE_ACE_VERSION=6.1.1
export BUNDLE_CMAKE_VERSION=2.8.9
export BUNDLE_NSIS_VERSION=2.46
export BUNDLE_GSL_VERSION=1.14
export BUNDLE_GTKMM_VERSION=2.22.0-2
export BUNDLE_TWEAK=0
# override GTKMM package with Lorenzo's ZIP file
export BUNDLE_GTKMM_32_ZIP=http://www.icub.org/download/software/windows/common/gtkmm-2.22-0-2.zip
export BUNDLE_GTKMM_64_ZIP=http://www.icub.org/download/software/windows/common/gtkmm-2.22-win64.zip
# workaround for firewall problemssource
"#export BUNDLE_NSIS_ZIP="http://www.icub.org/download/software/windows//nsis/nsis-2.46.zip"
#export BUNDLE_NSIS_PATCHES_ZIP="http://www.icub.org/download/software/windows//nsis/nsis-2.46-strlen_8192.zip"