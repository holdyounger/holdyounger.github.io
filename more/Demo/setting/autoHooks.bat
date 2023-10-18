@echo off
set "curDir=%cd%"
echo -------------------------------
set "curDir=%curDir:setting=%"

set source=%cd%\commit-msg
set dest=%curDir%.git\hooks\commit-msg
echo %source%
echo %dest%
copy "%source%" "%dest%"

echo -------------------------------
