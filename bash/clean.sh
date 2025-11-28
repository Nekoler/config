apt-get clean
apt-get autoremove
dpkg -l |grep ^rc|awk '{print $2}' |xargs apt purge -y
rm -rf \
	~/.bash_history\
	~/.python_history\
	~/.cache\
	~/.npm/
