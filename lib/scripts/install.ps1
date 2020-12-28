Remove-Item -Path ./bin/sim.exe -Force -ErrorAction Ignore

New-Item -Path build -ItemType directory -ErrorAction Ignore | Out-Null
Set-Location -Path build
cmake .. -G "MinGW Makefiles"
mingw32-make -j5

Set-Location -Path ..
New-Item -Path bin -ItemType directory -ErrorAction Ignore | Out-Null
Move-Item -Path "./build/src/sim.exe" -Destination "./bin/sim.exe"
