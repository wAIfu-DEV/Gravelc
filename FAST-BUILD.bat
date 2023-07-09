@ECHO OFF

SET ROOT_PATH=%cd%
SET DIST_WIN="dist/x86_64-win32"
SET CPP_VERS="c++2a"

ECHO Building %DIST_WIN% . . .

WSL rm -rf %DIST_WIN%/gvc.exe
g++ -g src/main.cpp -o %DIST_WIN%/gvc.exe -static -std=%CPP_VERS% -m64 -O0 -Werror -Wall -Wextra -pedantic

ECHO Finished building.

ECHO Testing build . . .

%ROOT_PATH%/%DIST_WIN%/gvc.exe -v

PAUSE