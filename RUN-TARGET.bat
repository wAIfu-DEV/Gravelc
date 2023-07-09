@ECHO OFF

SET ROOT_PATH=%cd%
SET DIST_WIN="dist/x86_64-win32-gvc"

CMD /k %ROOT_PATH%/%DIST_WIN%/gvc.exe -c target/text.gv -o out.exe --verbose

PAUSE