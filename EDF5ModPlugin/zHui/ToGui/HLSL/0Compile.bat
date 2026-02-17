@echo off
set FXC="C:\Program Files (x86)\Windows Kits\10\bin\10.0.26100.0\x64\fxc.exe"

%FXC% /T vs_5_0 /E VS_main /Fo "Z:\TEMP\vs_digit.cso" shader.hlsl
%FXC% /T ps_5_0 /E PS_main /Fo "Z:\TEMP\ps_digit.cso" shader.hlsl

pause