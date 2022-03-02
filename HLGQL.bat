@echo off

echo ----------------------------------------------------
echo 清理这些垃圾东西
echo Press any key to delete all files with ending:
echo  *.obj *.aps *.idb *.ncp *.sdf *.obj *.pch *.tmp *.sbr *.log *.tlog unsuccessfulbuild *.lastbuildstate
echo Visual c++/.Net junk 
echo ----------------------------------------------------
pause

del /F /Q /S *.obj *.aps *.idb *.ncp *.sdf *.obj *.pch *.sbr *.tmp *.pdb *.bsc *.ilk *.res *.ncb *.opt *.suo *.manifest *.dep *.log *.tlog unsuccessfulbuild *.lastbuildstate



pause


