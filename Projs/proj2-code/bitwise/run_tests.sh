#! /bin/bash
# Author: John Kolb <jhkolb@umn.edu>
# SPDX-License-Identifier: GPL-3.0-or-later

./ndlc puzzle_rules.json bits.c
ndlc_result=$?

if [ "$ndlc_result" -eq 0 ]; then
    if [ $# -gt 0 ]; then
        ./testius test_cases/tests.json -v -n "$1";
    else
        ./testius test_cases/tests.json;
    fi
else
    echo
    echo "Your code does not adhere to the bitwise puzzle restrictions.";
    echo "You must fix the violations identified above before you can run tests.";
fi
