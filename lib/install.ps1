Remove-Item -Path sim.exe -Force -ErrorAction Ignore
New-Item -Path build -ItemType directory -ErrorAction Ignore | Out-Null
Set-Location -Path build
cmake .. -G "MinGW Makefiles"
mingw32-make
Set-Location -Path ..
Move-Item -Path "./build/src/sim.exe" -Destination "./sim.exe"
