Configuring and building Eyrie
==============================

The Eyrie runtime can be configured and built either with standard
``make`` and setting the ``OPTIONS_FLAGS`` environment variable, or by
using the ``build.sh`` wrapper script.

OPTIONS_FLAGS
-------------

Eyrie supports specifiying plugins via the ``OPTIONS_FLAGS``
environment variable. See the Eyrie Makefile and ``build.sh`` wrapper
for an up-to-date list of plugins.


build.sh wrapper
----------------

``build.sh`` supports automatically setting and rebuilding the Eyrie
runtime based on commandline options.

Example::

  ./build.sh freemem env_setup

Will cleanly rebuild Eyrie with free memory management and libc-style
environment initialization.
