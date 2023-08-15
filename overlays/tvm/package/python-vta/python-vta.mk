################################################################################
#
# python-vta
#
################################################################################

# todo: figure out how to share source tarball directly
HOST_PYTHON_VTA_VERSION = $(TVM_VERSION)
HOST_PYTHON_VTA_SITE = $(TVM_SITE)
HOST_PYTHON_VTA_SITE_METHOD = $(TVM_SITE_METHOD)

HOST_PYTHON_VTA_DEPENDENCIES += host-tvm
HOST_PYTHON_VTA_ENV += TVM_LIBRARY_PATH=$(HOST_DIR)/usr/lib
HOST_PYTHON_VTA_SETUP_TYPE = setuptools
HOST_PYTHON_VTA_SUBDIR = vta/python

$(eval $(host-python-package))
