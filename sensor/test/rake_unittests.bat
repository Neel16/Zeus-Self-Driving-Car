echo off
set working_dir=%cd%

:::::::: Check if the our working directory contains spaces
set working_dir_no_spaces=%working_dir%
set working_dir_no_spaces=%working_dir_no_spaces: =%

:::::::: If we've got a space, then quit here.
if not "%working_dir_no_spaces%"=="%working_dir%" (
    echo ------------------------------------------------------------
    echo Your directory path contains a space.
    echo Try putting this package without a space in the path
    echo ------------------------------------------------------------
    echo Recommended location is your C drive itself
    echo Common mistake is when people put this at their desktop
    echo and unfortunately their name has a space :(
    echo ------------------------------------------------------------
    echo Pressing any key will exit this process.
    echo ------------------------------------------------------------
    pause
    exit
)

:::::::: Ruby for unit-tests
set PATH=%working_dir%\install_ruby25;%PATH%
set PATH=%working_dir%\install_ruby25\bin;%PATH%

:::::::: MINGW
set PATH=%working_dir%\install_mingw\mingw32\bin;%PATH%
set PATH=%working_dir%\install_mingw\mingw32\i686-w64-mingw32\bin;%PATH%;

:::::::: Open the command prompt
start rake