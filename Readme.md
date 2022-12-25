# Build 
cmake -B bauen </br>
cmake --build bauen <--parallel $(nproc --all)>
## Options
-DDIGITAL "Use the digital scale instand the analog one." </br>
-DDISPLAYTEST "Show all scales as an kind of display test."
