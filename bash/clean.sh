apt-get clean
apt-get autoremove
dpkg -l |grep ^rc|awk '{print $2}' |xargs apt purge -y
rm ~/.bash_history ~/.python_history -r ~/.cache ~/.viminfo ~/.config/BaiduPCS-Go/pcs_command_history.txt
