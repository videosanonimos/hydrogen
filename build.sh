#! /bin/bash

# Hydrogen
# Copyright(c) 2002-2008 by Alex >Comix< Cominu [comix@users.sourceforge.net]
# Copyright(c) 2008-2023 The hydrogen development team [hydrogen-devel@lists.sourceforge.net]
#
# http://www.hydrogen-music.org
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

QTDIR=${QTDIR:-/usr/lib/qt}
VERBOSE=${VERBOSE:-0}
CMAKE_OPTIONS="
    -DCMAKE_COLOR_MAKEFILE=1 \
    -DWANT_DEBUG=1 \
    -DWANT_JACK=1 \
    -DWANT_ALSA=1 \
    -DWANT_LIBARCHIVE=1 \
    -DWANT_RUBBERBAND=1 \
    -DWANT_OSS=1 \
    -DWANT_PORTAUDIO=1 \
    -DWANT_PORTMIDI=1 \
    -DWANT_LASH=0 \
    -DWANT_LRDF=1 \
    -DWANT_COREAUDIO=1 \
    -DWANT_COREMIDI=1
"
MAKE_OPTS="-j 3"
H2FLAGS="-V0xf"
BUILD_DIR=./build


PLATFORM_STR=`uname`

[ -f cmake_opts ] && source cmake_opts

function cmake_init() {
    echo -e " * cmake init\n"
    if [ ! -d $BUILD_DIR ]; then
        mkdir $BUILD_DIR || exit 1
    fi
    cd $BUILD_DIR || exit 1
    if [ ! -e CMakeCache.txt ]; then
        cmake ${CMAKE_OPTIONS} .. || exit 1
    fi
    cd .. || exit 1
}

function cmake_clean() {
    echo -e " * clean cmake files\n" && rm $BUILD_DIR/CMakeCache.txt 2>/dev/null
}

function cmake_rm() {
    echo -e " * rm cmake files\n" && rm -fr $BUILD_DIR 2>/dev/null
}

function cmake_make() {
    cmake_init
    echo -e " * cmake make\n" && cd $BUILD_DIR || exit 1
    if [ $VERBOSE -eq 1 ]; then
        VERBOSE=1 make translations $MAKE_OPTS || exit 1
        VERBOSE=1 make $MAKE_OPTS || exit 1
    else
        make translations $MAKE_OPTS || exit 1
        make $MAKE_OPTS || exit 1
    fi

    if [[ "$PLATFORM_STR" == 'Linux' ]]; then
        cp src/gui/hydrogen ..
    elif [[ "$PLATFORM_STR" == *BSD ]]; then
        cp src/gui/hydrogen ..
    elif [[ "$PLATFORM_STR" == 'Darwin' ]]; then
        cp -rf src/gui/hydrogen.app ..
    fi

    cd ..
}

function cmake_graph() {
    cmake_init
    echo -e " * cmake graphviz\n" && cd $BUILD_DIR && cmake --graphviz=cmake.dot .. && dot -Tpng -o cmake_dep.png cmake.dot && cd .. || exit 1
}

function cmake_doc() {
    cmake_init
    echo -e " * cmake doc\n" && cd $BUILD_DIR && make doc && cd .. || exit 1
}

function cmake_help() {
    cmake_init
    echo -e " * cmake help\n" && cd $BUILD_DIR && cmake .. -L && cd .. || exit 1
}

function cmake_exec() {
    cmake_init
    echo -e " * execute hydrogen\n" && ./hydrogen $H2FLAGS || exit 1
}

function cmake_tests() {
    cmake_init
    echo -e " * execute tests\n" && $BUILD_DIR/src/tests/tests || exit 1
}

function cmake_pkg() {
    cmake_init
    echo -e " * execute hydrogen\n" && cd $BUILD_DIR && make package_source && cd .. || exit 1
}

function zoop() {
    cmake_make
    LD_PRELOAD=$(find $BUILD_DIR -name 'libhydrogen-core*' | head -n 1) ./hydrogen $H2FLAGS
}

if [ $# -eq 0 ]; then
    echo "usage $0 [cmds list]"
    echo "cmds may be"
    echo "   r[m]     => all built, temp and cache files"
    echo "   c[lean]  => remove cache files"
    echo "   m[ake]   => launch the build process"
    echo "   mm       => launch the build process using ccache"
    echo "   mt       => launch the build process with clang tidy checks enabled"
    echo "   d[oc]    => build html documentation"
    echo "   g[raph]  => draw a dependencies graph"
    echo "   h[elp]   => show the build options"
    echo "   x[exec]  => execute hydrogen"
    echo "   t[ests]  => execute tests"
    echo "   p[kg]    => build source package"
    echo "   z        => build using ccache and run from tree"
    echo "ex: $0 r m pkg x"
    exit 1
fi

for arg in $@; do
    case $arg in
        c|clean)
            cmd="cmake_clean";;
        r|rm)
            cmd="cmake_rm";;
        m|make)
            cmd="cmake_make";;
        mm)
            CMAKE_OPTIONS="$CMAKE_OPTIONS -DCMAKE_CXX_COMPILER_LAUNCHER=ccache"
            cmd="cmake_make";;
        mt)
            CMAKE_OPTIONS="$CMAKE_OPTIONS -DWANT_CLANG_TIDY=1"
            cmd="cmake_make";;
        g|graph)
            cmd="cmake_graph";;
        d|doc)
            cmd="cmake_doc";;
        h|help)
            cmd="cmake_help";;
        x|exec)
            cmd="cmake_exec";;
        t|test)
            cmd="cmake_tests";;
        p|pkg)
            cmd="cmake_pkg";;
        z)
            CMAKE_OPTIONS="$CMAKE_OPTIONS -DCMAKE_CXX_COMPILER_LAUNCHER=ccache"
            cmd="zoop";;
        *)
            echo "unknown command ${arg}" && exit 1
    esac
    $cmd
done

# vim: set softtabstop=4 expandtab:
