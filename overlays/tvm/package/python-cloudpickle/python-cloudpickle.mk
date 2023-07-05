################################################################################
#
# python-cloudpickle
#
################################################################################

PYTHON_CLOUDPICKLE_VERSION = 2.2.1
PYTHON_CLOUDPICKLE_SOURCE = cloudpickle-$(PYTHON_CLOUDPICKLE_VERSION).tar.gz
PYTHON_CLOUDPICKLE_SITE = https://files.pythonhosted.org/packages/5f/51/913ecca3970a2227cf4d5e8937df52cc28f465ac442216110b8e3323262d
PYTHON_CLOUDPICKLE_SETUP_TYPE = setuptools
PYTHON_CLOUDPICKLE_LICENSE = BSD-3-Clause
PYTHON_CLOUDPICKLE_LICENSE_FILES = LICENSE

$(eval $(python-package))
