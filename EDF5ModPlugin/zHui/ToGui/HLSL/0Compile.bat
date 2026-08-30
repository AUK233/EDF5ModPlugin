@echo off
set FXC="C:\Program Files (x86)\Windows Kits\10\bin\10.0.26100.0\x64\fxc.exe"

@REM %FXC% /T vs_5_0 /nologo /E VS_main /Fo "Z:\TEMP\digitFixedPos_vs.cso" digit.hlsl
@REM %FXC% /T ps_5_0 /nologo /E PS_main /Fo "Z:\TEMP\digitFixedPos_ps.cso" digit.hlsl

@REM %FXC% /T vs_5_0 /nologo /E VS_main /Fo "Z:\TEMP\digitDynamicPos_vs.cso" digit.hlsl /D _DynamicPos=1
@REM %FXC% /T ps_5_0 /nologo /E PS_main /Fo "Z:\TEMP\digitDynamicPos_ps.cso" digit.hlsl /D _DynamicPos=1

%FXC% /T vs_5_0 /nologo /E VS_main /Fh "Z:\TEMP\0digitFixedPos_vs.hpp" /Vn "DR_digitFixedPos_vs" digit.hlsl
%FXC% /T ps_5_0 /nologo /E PS_main /Fh "Z:\TEMP\0digitFixedPos_ps.hpp" /Vn "DR_digitFixedPos_ps" digit.hlsl

%FXC% /T vs_5_0 /nologo /E VS_main /Fh "Z:\TEMP\0digitDynamicPos_vs.hpp" /Vn "DR_digitDynamicPos_vs" digit.hlsl /D _DynamicPos=1
%FXC% /T ps_5_0 /nologo /E PS_main /Fh "Z:\TEMP\0digitDynamicPos_ps.hpp" /Vn "DR_digitDynamicPos_ps" digit.hlsl /D _DynamicPos=1

pause