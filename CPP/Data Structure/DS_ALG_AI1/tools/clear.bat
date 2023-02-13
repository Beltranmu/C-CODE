cls
@echo off
IF EXIST ..\build\.vs rmdir /s /q ..\build\.vs
IF EXIST ..\build\PR0_MemoryNode rmdir /s /q ..\build\PR0_MemoryNode
IF EXIST ..\build\PR1_Vector rmdir /s /q ..\build\PR1_Vector
IF EXIST ..\build\PR2_Circular_Vector rmdir /s /q ..\build\PR2_Circular_Vector
IF EXIST ..\build\PR3_Movable_Head_Vector rmdir /s /q ..\build\PR3_Movable_Head_Vector
IF EXIST ..\build\PR4_List rmdir /s /q ..\build\PR4_List
IF EXIST ..\build\PR5_Circular_List rmdir /s /q ..\build\PR5_Circular_List
IF EXIST ..\build\PR6_DL_List rmdir /s /q ..\build\PR6_DL_List
IF EXIST ..\build\PR7_Circular_DL_List rmdir /s /q ..\build\PR7_Circular_DL_List
IF EXIST ..\build\PR8_Stack rmdir /s /q ..\build\PR8_Stack
IF EXIST ..\build\PR9_Queue rmdir /s /q ..\build\PR9_Queue
IF EXIST ..\build\PR10_Logger rmdir /s /q ..\build\PR10_Logger
IF EXIST ..\build\PR11_Comparative rmdir /s /q ..\build\PR11_Comparative
IF EXIST ..\build\PR12_Sorting rmdir /s /q ..\build\PR12_Sorting
IF EXIST ..\bin\debug rmdir /s /q ..\bin\debug
IF EXIST ..\bin\release rmdir /s /q ..\bin\release

IF EXIST ..\build\DS_ALG_AI1vs2017.sln del /F ..\build\DS_ALG_AI1vs2017.sln
IF EXIST ..\build\DS_ALG_AI1vs2019.sln del /F ..\build\DS_ALG_AI1vs2019.sln
