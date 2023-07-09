@ECHO OFF

SET ROOT_PATH=%cd%
SET DIST_WIN="dist/x86_64-win32"
SET DIST_LINUX="dist/x86_64-elf64"
SET CPP_VERS="c++2a"

ECHO Building %DIST_WIN% . . .

WSL rm -rf %DIST_WIN%/gvc.exe
g++ -g src/main.cpp -o %DIST_WIN%/gvc.exe -static -std=%CPP_VERS% -m64 -Ofast -Werror -Wall -Wextra -pedantic

ECHO Building %DIST_LINUX% . . .
WSL rm -rf %DIST_LINUX%/gvc
WSL g++ -g src/main.cpp -o %DIST_LINUX%/gvc -static -std=%CPP_VERS% -m64 -Ofast -Werror -Wall -Wextra -pedantic

ECHO Finished building.

ECHO Testing builds . . .

%ROOT_PATH%/%DIST_WIN%/gvc.exe -v
WSL %DIST_LINUX%/gvc -v

PAUSE