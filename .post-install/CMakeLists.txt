install(CODE "message(\"\n * Keystone SDK has been installed at ${out_dir}\")")
install(CODE "message(\" * Run `export KEYSTONE_SDK_DIR=${out_dir}`\")")
install(CODE "message(\" * Use `make uninstall` to uninstall\")")
