@echo off
rem --- arguments ---
set folder=Yokohama
set ext=.jpg
set shcoef=sh_coefficients.txt
set obj=Sphere.obj
set arguments=%folder%\posx%ext% %folder%\negx%ext% %folder%\posy%ext% %folder%\negy%ext% %folder%\posz%ext% %folder%\negz%ext% %folder%\%shcoef%
rem --- sampling ---
.\sh_sampler.exe %arguments%
rem --- viewer ---
.\sh_lighting.exe %arguments% %obj%