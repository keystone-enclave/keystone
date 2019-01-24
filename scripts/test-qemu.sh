#!/usr/bin/expect

set timeout 60
log_file -noappend  output.log
spawn ssh root@localhost -p 5555
expect "yes/no" { 
  send "yes\r"
  expect "*?assword" { send "busybear\r" }
  } "*?assword" { send "busybear\r" }

expect "# " { send "busybox --install -s /bin\r" }
expect "# " { send "insmod keystone-driver.ko\r" }
expect "# " { send "./test\r" }
expect "# " { send "poweroff\r" }
expect eof
