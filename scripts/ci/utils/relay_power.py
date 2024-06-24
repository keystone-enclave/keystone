#!/bin/python3

from relay_ft245r import relay_ft245r
import sys
import time

# Check arguments
if len(sys.argv) != 4:
        print('Usage: relay_power.py [serial#] [port] [on/off]')
        sys.exit()

serial_number = sys.argv[1]
port = int(sys.argv[2])
state = sys.argv[3].strip().lower()

if state not in ['on', 'off', 'reboot']:
        print('Unrecognized command ', state)
        sys.exit()

rb = relay_ft245r.FT245R()
dev_list = rb.list_dev()

# list of FT245R devices are returned
if len(dev_list) == 0:
        print('No FT245R devices found')
        sys.exit()

found = False
for dev in dev_list:
        # This is the one we should use
        if dev.serial_number == serial_number:
                found = True
                break

if found:
        rb.connect(dev)
        if state == 'on':
                if not rb.getstatus(port):
                        rb.switchoff(port)

                rb.switchon(port)
        else:
                # Cursed: for some reason, we need to switch the port on which
                # is effectively a no-op before we can turn it off. We don't
                # want to do this spuriously though (fast switches may be bad
                # in case the port is already off) so we check this case.
                if rb.getstatus(port):
                        rb.switchon(port)

                rb.switchoff(port)

        if state == 'reboot':
                time.sleep(0.1)
                rb.switchon(port)

else:
        print('FT245R with specified serial not found: ', SERIAL_NUMBER)
        sys.exit()
