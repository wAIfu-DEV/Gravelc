@ECHO OFF

SET ROOT_PATH=%cd%
SET DIST_WIN="dist/x86_64-win64-gvc"
SET CPP_VERS="c++2a"

ECHO Building %DIST_WIN% . . .

rmdir /S /Q %DIST_WIN%
mkdir %DIST_WIN%
g++ -g src/main.cpp -o %DIST_WIN%/gvc.exe -static -std=%CPP_VERS% -m64 -Ofast -Werror -Wall -Wextra -pedantic
robocopy %cd%/lib %DIST_WIN%/lib /e

ECHO Finished building.

ECHO Testing build . . .

%ROOT_PATH%/%DIST_WIN%/gvc.exe -v

PAUSE