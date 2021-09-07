shopt -s extglob
cd /sdcard/
rm -rf !(Android|DCIM|Download|Pictures|MT2|Music)
rm -rf .* &> /dev/null
shopt -u extglob
cd ~-
