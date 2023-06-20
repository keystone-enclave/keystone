
##############
## Packages ##
##############

# Bootloaders
include $(sort $(wildcard $(BR2_EXTERNAL_KEYSTONE_PATH)/boot/*/*.mk))

# Packages
include $(sort $(wildcard $(BR2_EXTERNAL_KEYSTONE_PATH)/package/*/*.mk))
