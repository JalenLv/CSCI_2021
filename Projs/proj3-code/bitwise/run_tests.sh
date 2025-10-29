#! /bin/bash
# Author: John Kolb <jhkolb@umn.edu>
# SPDX-License-Identifier: GPL-3.0-or-later

arch=$(uname -m)
if [ "$arch" = "x86_64" ]; then
    test_cases_file="test_cases/tests.json"
else
    test_cases_file="test_cases/non_x86_tests.json"
fi

python3 cc_check.py puzzle_list.json bits.s
cc_check_result=$?

if [ "$cc_check_result" -eq 0 ]; then
    if [ $# -gt 0 ]; then
        ./testius $test_cases_file -v -n "$1";
    else
        ./testius $test_cases_file;
    fi
else
    echo
    echo "Your code does not adhere to the x86-64 calling convention.";
    echo "You must fix the violations identified above before you can run tests.";
fi
