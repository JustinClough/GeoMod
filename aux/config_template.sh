# Configures build system using cmake

# Modify for your system
GM_DIR=<Absolute_Path_To_Project_Root>


source $GM_DIR/aux/setup.sh

if [ -d "$GM_DIR/build" ]; then
  rm -r $GM_DIR/build/
fi 

if [ -d "$GM_DIR/install" ]; then
  rm -r $GM_DIR/install/
fi 

mkdir $GM_DIR/build
cd $GM_DIR/build/
cmake \
  -DSCOREC_PREFIX=${TRILINOS_INSTALL_DIR} \
  -DCMAKE_INSTALL_PREFIX=$GM_DIR/install \
..

make
make install

cd $GM_DIR

echo $'\nGeoMod built and installed.\n'
