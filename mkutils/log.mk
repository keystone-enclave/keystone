
####################################
## Logging utilites for Makefiles ##
####################################

# Externally define log levels
export LOG_DEBUG                := 0
export LOG_INFO                 := 1
export LOG_WARNING              := 2
export LOG_ERROR                := 3
export LOG_FATAL                := 4

export KEYSTONE_LOG_LEVEL       ?= $(LOG_INFO)

# Define some commonly used values in log strings

log_bold        := $(shell tput bold)
log_sgr0        := $(shell tput sgr0)
log_black       := $(shell tput setaf 0)
log_red         := $(shell tput setaf 1)
log_green       := $(shell tput setaf 2)
log_yellow      := $(shell tput setaf 3)
log_blue        := $(shell tput setaf 4)
log_magenta     := $(shell tput setaf 5)
log_cyan        := $(shell tput setaf 6)
log_white       := $(shell tput setaf 7)

str_debug       := (DBG)
str_info        := (INF)
str_warning     := (WRN)
str_error       := (ERR)
str_fatal       := (FAT)

col_debug       := $(log_cyan)
col_info        := $(log_magenta)
col_warning     := $(log_yellow)
col_error       := $(log_red)
col_fatal       := $(log_white)

#define some useful macros
to_upper = $(shell echo $(1) | tr '[:lower:]' '[:upper:]')
to_lower = $(shell echo $(1) | tr '[:upper:]' '[:lower:]')

# Define the logging macros

define __log_generic
@- if [[ $$KEYSTONE_LOG_LEVEL -le $(2) ]]; then echo "$(log_bold)$(3)>>> $(4) $(1)$(log_sgr0)"; fi
endef

define __mlog_generic
ifeq ($$(shell [[ $$$$KEYSTONE_LOG_LEVEL -le $(2) ]] && echo y),y)
$$(info $$(log_bold)$(3)>>> $(4) $(1) $$(log_sgr0))
endif
endef

# This one can be used within recipes, and is used like $(call log,level,message)
define log
$(call __log_generic,$(2),              \
        $(LOG_$(call to_upper,$(1))),   \
        $(col_$(call to_lower,$(1))),   \
        $(str_$(call to_lower,$(1))))
endef

# This one can be used outside of recipes, and is used like $(eval $(call mlog,level,message))
define mlog
$(call __mlog_generic,$(2),             \
        $(LOG_$(call to_upper,$(1))),   \
        $$(col_$(call to_lower,$(1))),   \
        $(str_$(call to_lower,$(1))))
endef
