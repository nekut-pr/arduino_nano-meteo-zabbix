/etc/crontab
*/3 *	* * * root	sh /home/admin/meteo/stat.sh

/home/admin/meteo/stat.sh
#!/bin/bash
curl 'http://ip' > /home/admin/meteo/stat.txt

/etc/zabbix/zabbix_agentd.d/meteo.conf
UserParameter=hum,cat /home/admin/meteo/stat.txt | awk {'print $4'}
UserParameter=temp,cat /home/admin/meteo/stat.txt | awk {'print $6'}
