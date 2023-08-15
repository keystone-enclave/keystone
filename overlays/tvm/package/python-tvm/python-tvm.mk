################################################################################
#
# python-tvm
#
################################################################################

# todo: figure out how to share source tarball directly
HOST_PYTHON_TVM_VERSION = $(TVM_VERSION)
HOST_PYTHON_TVM_SITE = $(TVM_SITE)
HOST_PYTHON_TVM_SITE_METHOD = $(TVM_SITE_METHOD)

HOST_PYTHON_TVM_DEPENDENCIES += host-tvm host-python-decorator host-python-psutil \
                                    host-python-typing-extensions
HOST_PYTHON_TVM_ENV += TVM_LIBRARY_PATH=$(HOST_DIR)/usr/lib
HOST_PYTHON_TVM_SETUP_TYPE = setuptools
HOST_PYTHON_TVM_SUBDIR = python

$(eval $(host-python-package))
