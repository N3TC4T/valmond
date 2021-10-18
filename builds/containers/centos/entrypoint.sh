#!/bin/bash
source scl_source enable devtoolset-8
source scl_source enable rh-ruby23
exec "$@"
