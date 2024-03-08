apt-get clean
apt-get autoremove
dpkg -l |grep ^rc|awk '{print $2}' |xargs apt purge -y
rm -rf ~/.bash_history ~/.python_history ~/.cache ~/.viminfo ~/.config/BaiduPCS-Go/pcs_command_history.txt ~/.npm/
