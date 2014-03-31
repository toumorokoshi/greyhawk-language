=====================
Dependency Management
=====================

Every package in Greyhawk needs to have a description of it's
dependencies. This is stored in a dependencies.yaml file format::

  requires:
    - http://github.com/toumorokoshi/greyhawk-fuzzy.git#84dafc
    - http://yusuketsutsumi.com/downloads/greyhawk-json.zip

The dependencies are pulled in and built during a 'greyhawk build'
