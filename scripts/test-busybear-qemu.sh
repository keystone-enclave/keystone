#!/usr/bin/expect

set timeout 60
spawn ssh -o "UserKnownHostsFile /dev/null" root@localhost -p $::env(HOST_PORT)
expect "yes/no" {
  send "yes\r"
  expect "*?assword" { send "busybear\r" }
  } "*?assword" { send "busybear\r" }

log_file -noappend  output.log

expect "# " { send "busybox --install -s /bin\r" }
expect "# " { send "insmod keystone-driver.ko\r" }
expect "# " { send "./test\r" }
expect "# " { send "poweroff\r" }
expect eof
