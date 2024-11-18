# Systeme_expert

## Installation requise :

- Cmake
- bison
- flex


** Sur ubuntu : sudo apt update && sudo apt install cmake bison flex **


## Build :

* mkdir build
* cd build
* cmake ..
* make

## Utilisation :

### Forward :

./Expert_System -f "chemin/nomDuFichierInstance"

### backward avec un but :

./Expert_System -g "GOAL(X)" -f "chemin/nomDuFichierInstance"

### Trace :

./Expert_System -t -g "GOAL(X)" -f "chemin/nomDuFichierInstance"

