#!/usr/bin/expect

set timeout 60
spawn ssh -o "UserKnownHostsFile /dev/null" root@localhost -p 5555
expect "yes/no" {
  send "yes\r"
  expect "*?assword" { send "sifive\r" }
  } "*?assword" { send "sifive\r" }

log_file -noappend  output.log

expect "# " { send "insmod keystone-driver.ko\r" }
expect "# " { send "./test\r" }
expect "# " { send "poweroff\r" }
expect eof
