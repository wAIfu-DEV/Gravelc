@ECHO OFF

SET ROOT_PATH=%cd%
SET DIST_WIN="dist/x86_64-win64-gvc"
SET DIST_LINUX="dist/x86_64-linux-gvc"
SET CPP_VERS="c++2a"

ECHO Building %DIST_WIN% . . .

WSL rm -rf %DIST_WIN%/gvc.exe
WSL mkdir %DIST_WIN%
g++ -g src/main.cpp -o %DIST_WIN%/gvc.exe -static -std=%CPP_VERS% -m64 -Ofast -Werror -Wall -Wextra -pedantic
WSL cp -r lib %DIST_WIN%


ECHO Building %DIST_LINUX% . . .
WSL rm -rf %DIST_LINUX%/gvc
WSL mkdir %DIST_LINUX%
WSL g++ -g src/main.cpp -o %DIST_LINUX%/gvc -static -std=%CPP_VERS% -m64 -Ofast -Werror -Wall -Wextra -pedantic
WSL cp -r lib %DIST_LINUX%

ECHO Finished building.

ECHO Testing builds . . .

%ROOT_PATH%/%DIST_WIN%/gvc.exe -v
WSL %DIST_LINUX%/gvc -v

PAUSE