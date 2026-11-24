#!/bin/bash

# Default release (debug)
meson setup build

# Modern release
meson setup build_modern --buildtype=release -Dc_args="-march=x86-64-v3"

# Legacy release
meson setup build_legacy --buildtype=release -Dc_args="-march=x86-64"
