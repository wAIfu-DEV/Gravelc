@ECHO OFF

SET ROOT_PATH=%cd%
SET DIST_WIN="dist/x86_64-win32"

SET /p GVC_ARGS=Enter Gravelc arguments: 

CMD /k %ROOT_PATH%/%DIST_WIN%/gvc.exe %GVC_ARGS%

PAUSE