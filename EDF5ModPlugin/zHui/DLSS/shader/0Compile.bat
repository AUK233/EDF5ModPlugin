@echo off
set FXC="C:\Program Files (x86)\Windows Kits\10\bin\10.0.26100.0\x64\fxc.exe"

%FXC% /T cs_5_0 /nologo /E CS_main /Fh "Z:\TEMP\1FullAO_MainColorCS.hpp" /Vn "FullAOMainColorShader" 0MainColorCS.hlsl /D _HasShadow=1
%FXC% /T cs_5_0 /nologo /E CS_main /Fh "Z:\TEMP\1FullAO_MainColorCS_UG.hpp" /Vn "FullAOMainColorShaderUG" 0MainColorCS.hlsl
%FXC% /T cs_5_0 /nologo /E CS_main /Fh "Z:\TEMP\1SetPostProcess_CS.hpp" /Vn "D3DPostProcess_ComputeShader" 0PostProcessCS.hlsl
%FXC% /T cs_5_0 /nologo /E CS_main /Fh "Z:\TEMP\1SetPostProcess_MV.hpp" /Vn "D3DPostProcess_MotionVector" 0CalculateMotionVector.hlsl

pause