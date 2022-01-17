package=native_mac_alias
$(package)_version=1.1.0
$(package)_download_path=https://github.com/al45tair/mac_alias/archive/
$(package)_file_name=v$($(package)_version).tar.gz
$(package)_sha256_hash=87ad827e66790028361e43fc754f68ed041a9bdb214cca03c853f079b04fb120
$(package)_install_libdir=$(build_prefix)/lib/python3/dist-packages

define $(package)_build_cmds
    python3 setup.py build
endef

define $(package)_stage_cmds
    mkdir -p $($(package)_install_libdir) && \
    python3 setup.py install --root=$($(package)_staging_dir) --prefix=$(build_prefix) --install-lib=$($(package)_install_libdir)
endef
