echo '确认清理(y/n)'
read key
if [ $key != 'y' ]
then
	exit
fi
shopt -s extglob
cd /sdcard/
rm -rf !(Android|DCIM|Download|Pictures|MT2)
cd ~-
shopt -u extglob
