copy models\* win_build
copy res\*.ttf win_build
cd win_build
cmd.exe /c start /wait TEngine.exe

pause
exit