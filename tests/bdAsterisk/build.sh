set -x
DIRBUILD=`pwd`/"build-"`uname -m`"-"`uname -s`
mkdir $DIRBUILD
cd $DIRBUILD
cmake ..
make

