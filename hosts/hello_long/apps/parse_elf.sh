#!/bin/bash
# via https://stackoverflow.com/questions/29903049/get-names-and-addresses-of-exported-functions-from-in-linux
file=$1
out_base=$2
out_file=$out_base.h
printf "unsigned long ${out_base} = 0x" > $out_file
eu-readelf -s "$file" | while read num value size type bind vix index name dummy ; do
    case $name in
	eapp_entry) printf "$value;" >> $out_file; break ;;
    esac
done
