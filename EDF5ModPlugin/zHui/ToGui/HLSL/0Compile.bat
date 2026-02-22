@echo off
set FXC="C:\Program Files (x86)\Windows Kits\10\bin\10.0.26100.0\x64\fxc.exe"

%FXC% /T vs_5_0 /nologo /E VS_main /Fo "Z:\TEMP\digitFixedPos_vs.cso" digit.hlsl
%FXC% /T ps_5_0 /nologo /E PS_main /Fo "Z:\TEMP\digitFixedPos_ps.cso" digit.hlsl

pause