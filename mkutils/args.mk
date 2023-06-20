

null  :=
space := $(null) #

define SEPERATE_LIST
$(subst $(space),$(1),$(strip $(2)))
endef
