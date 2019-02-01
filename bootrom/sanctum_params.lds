. = 0x801ff000; /* the last page before the payload */

/* ## manufacturer_keys : */

/* 32 Bytes : manufacturer public key */
PROVIDE( sanctum_m_public_key = . );
. += 0x20;

/* 32 Bytes : device public key */
PROVIDE( sanctum_dev_public_key = . );
. += 0x20;

/* 64 Bytes : device secret key */
PROVIDE( sanctum_dev_secret_key = . );
. += 0x40;

/* ## security_monitor_keys : */

/* 64 Bytes : security monitor hash */
PROVIDE( sanctum_sm_hash = . );
. += 0x40;

/* 32 Bytes : security monitor public key */
PROVIDE( sanctum_sm_public_key = . );
. += 0x20;

/* 64 Bytes : security monitor secret key */
PROVIDE( sanctum_sm_secret_key = . );
. += 0x40;

/* 64 Bytes : security monitor's signature by device */
PROVIDE( sanctum_sm_signature = . );
. += 0x40;
