#!/usr/bin/env python3

# Copyright (C) 2016 Kaspar Schleiser <kaspar@schleiser.de>
# Copyright (C) 2017 HAW Hamburg
#
# This file is subject to the terms and conditions of the GNU Lesser
# General Public License v2.1. See the file LICENSE in the top level
# directory for more details.

import os
import sys


def testfunc(child):
    child.expect(r"MAIN: reply from T-\d+")
    child.expect(r"MAIN: \d+! = \d+")
    child.expect_exact("[SUCCESS]")


if __name__ == "__main__":
    sys.path.append(os.path.join(os.environ['RIOTBASE'], 'dist/tools/testrunner'))
    from testrunner import run
    sys.exit(run(testfunc))
