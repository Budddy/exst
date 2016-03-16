cd ~
rm -r htd
git clone "https://github.com/mabseher/htd"
cd htd
mkdir build
cd build
cmake ../
make
rm /usr/lib/libhtd.so
rm /usr/lib/libhtd.so.0
rm /usr/lib/libhtd.so.0.0.0
cp lib/libhtd.so /usr/lib/
cp lib/libhtd.so.0 /usr/lib/
cp lib/libhtd.so.0.0.0 /usr/lib/
rm -r /usr/include/htd/
cp -r ../include/htd/ /usr/include/