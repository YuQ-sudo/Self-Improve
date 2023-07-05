## 查询僵尸进程
ps -A -ostat,pid,ppid | grep -e '[zZ]'
## 终止某用户所有的进程
ps -ef | grep yckj2733 | awk '{ print $2 }'
pgrep -u yckj2733 | xargs kill -9
