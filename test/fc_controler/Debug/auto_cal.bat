@echo off

set START=46
set END=62

for /l %%i in (%START%, 1, %END%) do (
    echo Running fc_controler.exe %%i
    start cmd /c fc_controler.exe %%i
)

pause
