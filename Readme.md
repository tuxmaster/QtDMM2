# Build 
cmake -B bauen </br>
cmake --build bauen <--parallel $(nproc --all)>
## Options
Defaults are **bold**</br>
-DDIGITAL=ON/**OFF** "Use the digital scale instand the analog one." </br>
-DDISPLAYTEST=ON/**OFF** "Show all scales as an kind of display test."
