#!/usr/bin/env bash

exit 300

#--------------------------------------------------------------------------------
# routekeysdest.sh, documentation:
#--------------------------------------------------------------------------------
# reading ~/.routekeysrc for keycode-sshdestination pairs (keycode:xx:sshdest)
# return result of script should be:
#     - 0-ok, and output string to stdout is sshdestination
#     - 1-exitAll request
#     - 2-noKeyFound
#--------------------------------------------------------------------------------
