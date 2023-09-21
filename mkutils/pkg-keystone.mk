
#############
## Summary ##
#############

# Since we don't assume that our users are necessarily experts at buildroot,
# we try to detect painful situations which may arise and (at least) notify
# about them. Specifically, we want to detect a situation where someone is
# working on an in-tree build (i.e. the Keystone overlay lives next to the
# Keystone sources) and some Keystone source file has changed. In this
# situation, Buildroot would not automatically rebuild the package which
# could lead to confusion when testing the changes. Therefore, we do some
# slightly weird things with Buildroot.
#
# Specifically, we set the version number of the package equal to a hash of
# all of the Keystone subproject's files, directories, and their attributes.
# This allows us to easily detect when some source file changes. Then, since
# the Buildroot package version changes when a source file changes, Buildroot
# will automatically build a new version of this package (since, from its
# perspective, a package of the "correct" version is not yet built). The last
# situation we need to handle is for stale build directories from old package
# versions. For these, we notify the user that the package should be dircleaned
# and add a new dirclean step to take care of the extra directories.

############
## Macros ##
############

# Include the other helpers that we have defined. Note that these includes
# append to MAKEFILE_LIST, which is parsed further below in order to extract
# the correct pkgdir/pkgname which Buildroot expects. Therefore, if you add
# or remove one of these includes, MAKE SURE YOU FIX THE PKGNAME LOGIC BELOW
# AS WELL TO ADJUST. Otherwise, things will start to break in tough-to-debug
# ways.

include $(KEYSTONE)/mkutils/log.mk
include $(KEYSTONE)/mkutils/args.mk

# This macro hashes the contents of a specific directory (truncated to 16 chars)
define keystone_hash_contents
$(shell (find $(1) -type f -print0 | sort -z | xargs -0 sha256sum ; \
            find $(1) \( -type f -o -type d \) -print0 | sort -z | \
                xargs -0 stat -c '%n %a') | sha256sum | awk -F' ' '{ print $$1 }' | head -c16)
endef

# This macro returns any stale directories which may exist for a package
define keystone_stale_dirs
$(shell find $(BUILDROOT_BUILDDIR)/build -maxdepth 1 -name '$(1)*' | grep -v -e '$(1)-$(2)')
endef

# This macro implements the additional dircleaning logic. First argument is the
# name of the package, second argument is the package's version.
define keystone_dirclean

ifneq ($$(call keystone_stale_dirs,$(1),$(2)),)
$(1)-dirclean: $(1)-stale-dirclean
$(1)-stale-dirclean:
	$$(call log,info,Cleaning stale build directories for $(1))
	rm -Rf $$(call keystone_stale_dirs,$(1),$(2))

$$(eval $$(call mlog,warning,Stale build directory detected for $(1)!))
$$(eval $$(call mlog,warning,    You should build $(1)-dirclean to remove them.))
$$(eval $$(call mlog,warning,    Not doing so may leave your build in an inconsistent state.))
endif
endef

################################################################################
# inner-keystone-package -- defines how Keystone packages should be versioned
# as described above. Also implements additional dircleaning targets.
#
#    $(1): lowercase name of the package, prefixed with host for host packages
#    $(2): uppercase name of the package, prefixed with host for host packages
#    $(3): uppercase name of the package, no prefix
################################################################################

define inner-keystone-package

$(2)_VERSION := $$(call keystone_hash_contents,$($(3)))
$(2)_SITE = $($(3))
$(2)_SITE_METHOD = local

$(call keystone_dirclean,$(1),$$($(2)_VERSION))

endef

# Fixup pkgdir and pkgname macros, since these have gotten clobbered by various
# includes. Note specifically that we look a couple positions back in the MAKEFILE_LIST
# to determine who called us. The exact number of positions is determined by the
# number of includes between this file and the package attempting to initialize itself.
# Therefore, if this number changes, the logic below needs to be changed as well
# (most likely by changing the arithmetic on $(words $(MAKEFILE_LIST))

pkgdir = $(dir $(word $(shell echo $$(( $(words $(MAKEFILE_LIST)) - 3 )) ), $(MAKEFILE_LIST)))
pkgname = $(lastword $(subst /, ,$(pkgdir)))

# Define the macros that keystone packages are actually expected to call
keystone-package = $(call inner-keystone-package,$(pkgname),$(call UPPERCASE,$(pkgname)),$(call UPPERCASE,$(pkgname)))
host-keystone-package = $(call inner-keystone-package,host-$(pkgname),$(call UPPERCASE,host-$(pkgname)),$(call UPPERCASE,$(pkgname)))
