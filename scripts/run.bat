@echo off
rem --- arguments ---
set folder=Lycksele2
set ext=.jpg
set shcoef=sh_coefficients.txt
set arguments=%folder%\posx%ext% %folder%\negx%ext% %folder%\posy%ext% %folder%\negy%ext% %folder%\posz%ext% %folder%\negz%ext% %folder%\%shcoef%
rem --- sampling ---
.\sh_sampler.exe %arguments%
rem --- viewer ---
.\sh_lighting.exe %arguments%