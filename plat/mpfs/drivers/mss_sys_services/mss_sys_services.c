/*******************************************************************************
 * Copyright 2019 Microchip Corporation.
 *
 * SPDX-License-Identifier: MIT
 *
 * PSE microcontroller subsystem System Services bare metal driver
 * implementation.
 *
 * SVN $Revision$
 * SVN $Date$
 */
#include "mss_sys_services.h"
#include "mss_sys_services_regs.h"
// #include "mpfs_hal/mss_hal.h"
// #include "mss_assert.h"
#include <assert.h>
#define ASSERT sm_assert

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Null buffer constant definition
 */
#define NULL_BUFFER                      (( uint8_t* ) 0)

/*-------------------------------------------------------------------------*//**
  System service response offset
  ============================
  The following constants are used to specify the offset in the mailbox where
  the service response is written by the system controller after service
  execution.
*/
#define MSS_SYS_COMMON_RET_OFFSET                               0u
#define MSS_SYS_DIGITAL_SIG_RET_OFFSET                          48u
#define MSS_SYS_SECURE_NVM_READ_RET_OFFSET                      16u
#define MSS_SYS_PUF_EMULATION_RET_OFFSET                        20u
#define MSS_SYS_DIGEST_CHECK_RET_OFFSET                         4u
#define MSS_SYS_GENERATE_OTP_RET_OFFSET                         20u

/*******************************************************************************
 * Global variables declarations
 */
volatile uint8_t g_message_received = 0u;
uint8_t g_service_mode = 0u;

uint8_t* gp_int_service_response;
uint16_t g_int_service_response_size;
uint16_t g_int_service_response_offset;

volatile uint8_t g_message_interrupt_counter = 0u;

/*******************************************************************************
 * Callback handler function declaration
 */
mss_sys_service_handler_t mss_sys_interrupt_handler;

/*******************************************************************************
 * Local function declarations.
 */
static uint16_t execute_ss_polling_mode
(
    uint8_t cmd_opcode,
    uint8_t* cmd_data,
    uint16_t cmd_data_size,
    uint8_t* p_response,
    uint16_t response_size,
    uint16_t mb_offset,
    uint16_t response_offset
);

static uint16_t execute_ss_interrupt_mode
(
    uint8_t cmd_opcode,
    uint8_t* cmd_data,
    uint16_t cmd_data_size,
    uint8_t* p_response,
    uint16_t response_size,
    uint16_t mb_offset,
    uint16_t response_offset
);

static uint16_t request_system_service
(
    uint8_t cmd_opcode,
    uint8_t* cmd_data,
    uint16_t cmd_data_size,
    uint8_t* p_response,
    uint16_t response_size,
    uint16_t mb_offset,
    uint16_t response_offset
);

/*-----------------------------------------------------------------------------
                             Public Functions
 -----------------------------------------------------------------------------*/

/***************************************************************************//**
 * MSS_SYS_get_serial_number()
 * See "mss_sysservices.h" for details of how to use this function.
 */
void
MSS_SYS_select_service_mode
(
    uint8_t sys_service_mode,
    mss_sys_service_handler_t mss_sys_service_interrupt_handler
)
{
    g_service_mode = sys_service_mode;
    mss_sys_interrupt_handler = mss_sys_service_interrupt_handler;
}


/***************************************************************************//**
 * MSS_SYS_get_serial_number()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_get_serial_number
(
    uint8_t * p_serial_number,
    uint16_t mb_offset
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                (uint8_t)MSS_SYS_SERIAL_NUMBER_REQUEST_CMD,
                NULL_BUFFER,
                MSS_SYS_WITHOUT_CMD_DATA,
                p_serial_number,
                (uint16_t)MSS_SYS_SERIAL_NUMBER_RESP_LEN,
                mb_offset,
                MSS_SYS_COMMON_RET_OFFSET);
    }
    else
    {
        status = execute_ss_polling_mode(
                (uint8_t)MSS_SYS_SERIAL_NUMBER_REQUEST_CMD,
                NULL_BUFFER,
                MSS_SYS_WITHOUT_CMD_DATA,
                p_serial_number,
                (uint16_t)MSS_SYS_SERIAL_NUMBER_RESP_LEN,
                mb_offset,
                MSS_SYS_COMMON_RET_OFFSET);
    }

    return status;
}

/***************************************************************************//**
 * SYS_get_user_code()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_get_user_code
(
    uint8_t * p_user_code,
    uint16_t mb_offset
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                (uint8_t)MSS_SYS_USERCODE_REQUEST_CMD,
                NULL_BUFFER,
                MSS_SYS_WITHOUT_CMD_DATA,
                p_user_code,
                (uint16_t)MSS_SYS_USERCODE_RESP_LEN,
                mb_offset,
                MSS_SYS_COMMON_RET_OFFSET);
    }
    else
    {
        status = execute_ss_polling_mode(
                (uint8_t)MSS_SYS_USERCODE_REQUEST_CMD,
                NULL_BUFFER,
                MSS_SYS_WITHOUT_CMD_DATA,
                p_user_code,
                (uint16_t)MSS_SYS_USERCODE_RESP_LEN,
                mb_offset,
                MSS_SYS_COMMON_RET_OFFSET);
    }

    return status;
}

/***************************************************************************//**
 * SYS_get_design_info()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_get_design_info
(
    uint8_t * p_design_info,
    uint16_t mb_offset
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                (uint8_t)MSS_SYS_DESIGN_INFO_REQUEST_CMD,
                NULL_BUFFER,
                MSS_SYS_WITHOUT_CMD_DATA,
                p_design_info,
                (uint16_t)MSS_SYS_DESIGN_INFO_RESP_LEN,
                mb_offset,
                MSS_SYS_COMMON_RET_OFFSET);
    }
    else
    {
        status = execute_ss_polling_mode(
                 (uint8_t)MSS_SYS_DESIGN_INFO_REQUEST_CMD,
                 NULL_BUFFER,
                 MSS_SYS_WITHOUT_CMD_DATA,
                 p_design_info,
                 (uint16_t)MSS_SYS_DESIGN_INFO_RESP_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }

    return status;
}

/***************************************************************************//**
 * SYS_get_device_certificate()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_get_device_certificate
(
    uint8_t * p_device_certificate,
    uint16_t mb_offset
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                 (uint8_t)MSS_SYS_DEVICE_CERTIFICATE_REQUEST_CMD,
                 NULL_BUFFER,
                 MSS_SYS_WITHOUT_CMD_DATA,
                 p_device_certificate,
                 (uint16_t)MSS_SYS_DEVICE_CERTIFICATE_RESP_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }
    else
    {
        status = execute_ss_polling_mode(
                 (uint8_t)MSS_SYS_DEVICE_CERTIFICATE_REQUEST_CMD,
                 NULL_BUFFER,
                 MSS_SYS_WITHOUT_CMD_DATA,
                 p_device_certificate,
                 (uint16_t)MSS_SYS_DEVICE_CERTIFICATE_RESP_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }

    return status;
}

/***************************************************************************//**
 * SYS_read_digest()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_read_digest
(
   uint8_t * p_digest,
   uint16_t mb_offset
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                 (uint8_t)MSS_SYS_READ_DIGEST_REQUEST_CMD,
                 NULL_BUFFER,
                 MSS_SYS_WITHOUT_CMD_DATA,
                 p_digest,
                 (uint16_t)MSS_SYS_READ_DIGEST_RESP_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }
    else
    {
        status = execute_ss_polling_mode(
                 (uint8_t)MSS_SYS_READ_DIGEST_REQUEST_CMD,
                 NULL_BUFFER,
                 MSS_SYS_WITHOUT_CMD_DATA,
                 p_digest,
                 (uint16_t)MSS_SYS_READ_DIGEST_RESP_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }

    return status;
}

/***************************************************************************//**
 * SYS_query_security()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_query_security
(
    uint8_t * p_security_locks,
    uint16_t mb_offset
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;
    uint8_t idx=0;
    uint8_t buf[36] = {0};

    /*Actual QUERY_SECURITY_RESP_LEN is 9 but CoreSysService_PF IP needs number
    of words instead of number of bytes to be written to or read from MailBox*/
    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                 (uint8_t)MSS_SYS_QUERY_SECURITY_REQUEST_CMD,
                 NULL_BUFFER,
                 MSS_SYS_WITHOUT_CMD_DATA,
                 buf,
                 (uint16_t)(MSS_SYS_QUERY_SECURITY_RESP_LEN + 3u),
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }
    else
    {
        status = execute_ss_polling_mode(
                 (uint8_t)MSS_SYS_QUERY_SECURITY_REQUEST_CMD,
                 NULL_BUFFER,
                 MSS_SYS_WITHOUT_CMD_DATA,
                 buf,
                 (uint16_t)(MSS_SYS_QUERY_SECURITY_RESP_LEN + 3u),
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }

    for (idx = 0u; idx < MSS_SYS_QUERY_SECURITY_RESP_LEN; idx++)
    {
        *(p_security_locks + idx) = buf[idx];
    }

    return status;
}

/***************************************************************************//**
 * SYS_read_debug_info()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_read_debug_info
(
    uint8_t * p_debug_info,
    uint16_t mb_offset
)
{

    uint16_t status = MSS_SYS_PARAM_ERR;

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                 (uint8_t)MSS_SYS_READ_DEBUG_INFO_REQUEST_CMD,
                 NULL_BUFFER,
                 MSS_SYS_WITHOUT_CMD_DATA,
                 p_debug_info,
                 (uint16_t)MSS_SYS_READ_DEBUG_INFO_RESP_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }
    else
    {
        status = execute_ss_polling_mode(
                 (uint8_t)MSS_SYS_READ_DEBUG_INFO_REQUEST_CMD,
                 NULL_BUFFER,
                 MSS_SYS_WITHOUT_CMD_DATA,
                 p_debug_info,
                 (uint16_t)MSS_SYS_READ_DEBUG_INFO_RESP_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }

    return status;
}

/**************************************************************************//**
 * SYS_read_envm_param()
 * See "mss_sysservices.h" for details of how to use this function.
 */

uint16_t
MSS_SYS_read_envm_parameter
(
    uint8_t * p_envm_param,
    uint16_t mb_offset
)
{

    uint16_t status = MSS_SYS_PARAM_ERR;

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                 (uint8_t)MSS_SYS_READ_ENVM_PARAM_REQUEST_CMD,
                 NULL_BUFFER,
                 MSS_SYS_WITHOUT_CMD_DATA,
                 p_envm_param,
                 (uint16_t)MSS_SYS_READ_ENVM_PARAM_RESP_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }
    else
    {
        status = execute_ss_polling_mode(
                 (uint8_t)MSS_SYS_READ_ENVM_PARAM_REQUEST_CMD,
                 NULL_BUFFER,
                 MSS_SYS_WITHOUT_CMD_DATA,
                 p_envm_param,
                 (uint16_t)MSS_SYS_READ_ENVM_PARAM_RESP_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }

    return status;
}

/***************************************************************************//**
 * SYS_puf_emulation_service()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_puf_emulation_service
(
    uint8_t * p_challenge,
    uint8_t op_type,
    uint8_t* p_response,
    uint16_t mb_offset
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;
    uint8_t mb_format[20] = {0x00};
    uint8_t index = 0u;

    /* Frame the data required for mailbox */
    mb_format[index] = op_type;

    for (index = 4u; index < 20u; index++)
    {
        mb_format[index] = p_challenge[index - 4u];
    }

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                 (uint8_t)MSS_SYS_PUF_EMULATION_SERVICE_REQUEST_CMD,
                 mb_format,
                 (uint16_t)MSS_SYS_PUF_EMULATION_SERVICE_CMD_LEN,
                 p_response,
                 (uint16_t)MSS_SYS_PUF_EMULATION_SERVICE_RESP_LEN,
                 mb_offset,
                 (uint16_t)MSS_SYS_PUF_EMULATION_RET_OFFSET);
    }
    else
    {
        status = execute_ss_polling_mode(
                 (uint8_t)MSS_SYS_PUF_EMULATION_SERVICE_REQUEST_CMD,
                 mb_format,
                 (uint16_t)MSS_SYS_PUF_EMULATION_SERVICE_CMD_LEN,
                 p_response,
                 (uint16_t)MSS_SYS_PUF_EMULATION_SERVICE_RESP_LEN,
                 mb_offset,
                 (uint16_t)MSS_SYS_PUF_EMULATION_RET_OFFSET);
    }

    return status;
}

/***************************************************************************//**
 * SYS_digital_signature_service()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_digital_signature_service
(
    uint8_t* p_hash,
    uint8_t format,
    uint8_t* p_response,
    uint16_t mb_offset
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;

    if (format == MSS_SYS_DIGITAL_SIGNATURE_RAW_FORMAT_REQUEST_CMD)
    {
        if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
        {
            status = execute_ss_interrupt_mode
                     ((uint8_t)MSS_SYS_DIGITAL_SIGNATURE_RAW_FORMAT_REQUEST_CMD,
                     p_hash,
                     (uint16_t)MSS_SYS_DIGITAL_SIGNATURE_HASH_DATA_LEN,
                     p_response,
                     (uint16_t)MSS_SYS_DIGITAL_SIGNATURE_RAW_FORMAT_RESP_SIZE,
                     mb_offset,
                     (uint16_t)MSS_SYS_DIGITAL_SIG_RET_OFFSET);
        }
        else
        {
            status = execute_ss_polling_mode
                     ((uint8_t)MSS_SYS_DIGITAL_SIGNATURE_RAW_FORMAT_REQUEST_CMD,
                     p_hash,
                     (uint16_t)MSS_SYS_DIGITAL_SIGNATURE_HASH_DATA_LEN,
                     p_response,
                     (uint16_t)MSS_SYS_DIGITAL_SIGNATURE_RAW_FORMAT_RESP_SIZE,
                     mb_offset,
                     (uint16_t)MSS_SYS_DIGITAL_SIG_RET_OFFSET);
        }
    }

    else
    {
        if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
        {
            status = execute_ss_interrupt_mode
                     ((uint8_t)MSS_SYS_DIGITAL_SIGNATURE_DER_FORMAT_REQUEST_CMD,
                     p_hash,
                     (uint16_t)MSS_SYS_DIGITAL_SIGNATURE_HASH_DATA_LEN,
                     p_response,
                     (uint16_t)MSS_SYS_DIGITAL_SIGNATURE_DER_FORMAT_RESP_SIZE,
                     mb_offset,
                     (uint16_t)MSS_SYS_DIGITAL_SIG_RET_OFFSET);
        }
        else
        {
            status = execute_ss_polling_mode
                     ((uint8_t)MSS_SYS_DIGITAL_SIGNATURE_DER_FORMAT_REQUEST_CMD,
                     p_hash,
                     (uint16_t)MSS_SYS_DIGITAL_SIGNATURE_HASH_DATA_LEN,
                     p_response,
                     (uint16_t)MSS_SYS_DIGITAL_SIGNATURE_DER_FORMAT_RESP_SIZE,
                     mb_offset,
                     (uint16_t)MSS_SYS_DIGITAL_SIG_RET_OFFSET);
        }
    }

    return status;
}

/***************************************************************************//**
 * SYS_secure_nvm_write()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_secure_nvm_write
(
    uint8_t format,
    uint8_t snvm_module,
    uint8_t* p_data,
    uint8_t* p_user_key,
    uint16_t mb_offset
)
{
    uint8_t frame[256] = {0x00};
    uint8_t* p_frame = &frame[0];
    uint16_t index = 0;
    uint16_t status = MSS_SYS_PARAM_ERR;

    ASSERT(!(NULL_BUFFER == p_data));
    ASSERT(!(NULL_BUFFER == p_user_key));
    ASSERT(!(snvm_module >= 221u));

    *p_frame = snvm_module; /*SNVMADDR - SNVM module*/
    p_frame += 4; /* Next 3 bytes RESERVED - For alignment */

    /* Copy user key and send the command/data to mailbox. */
    if ((format == MSS_SYS_SNVM_AUTHEN_TEXT_REQUEST_CMD) ||
       (format == MSS_SYS_SNVM_AUTHEN_CIPHERTEXT_REQUEST_CMD))
    {
        /* Copy user data */
        for (index = 0u; index < (MSS_SYS_AUTHENTICATED_TEXT_DATA_LEN
                - MSS_SYS_USER_SECRET_KEY_LEN); index++)
        {
            *p_frame = p_data[index];
            p_frame++;
        }

        /* Copy user key */
        for (index = 0u; index < MSS_SYS_USER_SECRET_KEY_LEN; index++)
        {
            *p_frame = p_user_key[index];
            p_frame++;
        }

        if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
        {
            status = execute_ss_interrupt_mode(
                     format,
                     &frame[0],
                     (uint16_t)MSS_SYS_AUTHENTICATED_TEXT_DATA_LEN,
                     NULL_BUFFER,
                     MSS_SYS_NO_RESPONSE_LEN,
                     mb_offset,
                     MSS_SYS_COMMON_RET_OFFSET);
        }
        else
        {
            status = execute_ss_polling_mode(
                     format,
                     &frame[0],
                     (uint16_t)MSS_SYS_AUTHENTICATED_TEXT_DATA_LEN,
                     NULL_BUFFER,
                     MSS_SYS_NO_RESPONSE_LEN,
                     mb_offset,
                     MSS_SYS_COMMON_RET_OFFSET);
        }
    }
    else
    {
        /* Copy user data */
        for (index = 0u; index < (MSS_SYS_NON_AUTHENTICATED_TEXT_DATA_LEN - 4u);
                index++)
        {
            *(p_frame+index) = p_data[index];
        }

        if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
        {
            status = execute_ss_interrupt_mode(
                     format,
                     &frame[0],
                     (uint16_t)MSS_SYS_NON_AUTHENTICATED_TEXT_DATA_LEN,
                     NULL_BUFFER,
                     MSS_SYS_NO_RESPONSE_LEN,
                     mb_offset,
                     MSS_SYS_COMMON_RET_OFFSET);
        }
        else
        {
            status = execute_ss_polling_mode(
                    format,
                    &frame[0],
                    (uint16_t)MSS_SYS_NON_AUTHENTICATED_TEXT_DATA_LEN,
                    NULL_BUFFER,
                    MSS_SYS_NO_RESPONSE_LEN,
                    mb_offset,
                    MSS_SYS_COMMON_RET_OFFSET);
        }
    }

    return status;
}

/***************************************************************************//**
 * SYS_secure_nvm_read()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_secure_nvm_read
(
    uint8_t snvm_module,
    uint8_t* p_user_key,
    uint8_t* p_admin,
    uint8_t* p_data,
    uint16_t data_len,
    uint16_t mb_offset
)
{
    /* Frame the message. */
    uint8_t frame[16] = {0x00u};
    uint8_t* p_frame = &frame[0];
    uint16_t index = 0u;
    uint16_t status = MSS_SYS_PARAM_ERR;
    uint8_t response[256] = {0x00};

    ASSERT(!(NULL_BUFFER == p_data));
    ASSERT(!(NULL_BUFFER == p_admin));
    ASSERT(!(snvm_module > 221u));

    ASSERT((data_len == 236u) || (data_len == 252u));

    *p_frame = snvm_module; /*SNVMADDR - SNVM module*/
    p_frame += 4u; /* RESERVED - For alignment */

    /* Copy user key */
    if (236u == data_len)
    {
        for (index = 0u; index < 12u; index++)
        {
            ASSERT(p_user_key !=  NULL_BUFFER);
            *p_frame = p_user_key[index];
            p_frame++;
        }
    }
    else
    {
        p_frame += 12u;
    }

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                 (uint8_t)MSS_SYS_SNVM_READ_REQUEST_CMD,
                 &frame[0],
                 (uint16_t)MSS_SYS_SECURE_NVM_READ_DATA_LEN,
                 response,
                 (data_len + 4u),
                 mb_offset,
                 (uint16_t)MSS_SYS_SECURE_NVM_READ_RET_OFFSET);
        }
    else
    {
        status = execute_ss_polling_mode(
                 (uint8_t)MSS_SYS_SNVM_READ_REQUEST_CMD,
                 &frame[0],
                 (uint16_t)MSS_SYS_SECURE_NVM_READ_DATA_LEN,
                 response,
                 (data_len + 4u),
                 mb_offset,
                 (uint16_t)MSS_SYS_SECURE_NVM_READ_RET_OFFSET);
    }

    if (MSS_SYS_SUCCESS == status)
    {
        for (index = 0u; index < 4u; index++)
        {
            *(p_admin+index) = (uint32_t)response[index];
        }

       /* Copy data into user buffer. */
        for (index = 4u; index < (data_len + 4u); index++)
        {
            *(p_data + (index - 4u)) = response[index];
        }
    }
    else
    {
        ;
    }

    return status;
}

/***************************************************************************//**
 * SYS_nonce_service()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_nonce_service
(
    uint8_t * p_nonce,
    uint16_t mb_offset
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                 (uint8_t)MSS_SYS_NONCE_SERVICE_REQUEST_CMD,
                 NULL_BUFFER,
                 MSS_SYS_WITHOUT_CMD_DATA,
                 p_nonce,
                 (uint16_t)MSS_SYS_NONCE_SERVICE_RESP_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }
    else
    {
        status = execute_ss_polling_mode(
                (uint8_t)MSS_SYS_NONCE_SERVICE_REQUEST_CMD,
                NULL_BUFFER,
                MSS_SYS_WITHOUT_CMD_DATA,
                p_nonce,
                (uint16_t)MSS_SYS_NONCE_SERVICE_RESP_LEN,
                mb_offset,
                MSS_SYS_COMMON_RET_OFFSET);
    }

    return status;
}

/***************************************************************************//**
 * MSS_SYS_execute_uic_script()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_execute_uic_script
(
    uint8_t src_periph_type,
    uint32_t periph_address,
    uint16_t mb_offset
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;
    uint8_t input_data[8];
    uint32_t l_periph_addr = periph_address;

    if (src_periph_type == MSS_SYS_UIC_SOURCE_PERIPH_SNVM)
    {
        l_periph_addr &= 0x000000FFu;   /*only first 8 bits are valid*/
    }
    else if ((src_periph_type == MSS_SYS_UIC_SOURCE_PERIPH_NONAUTHEN_SPIFLASH )||
            (src_periph_type == MSS_SYS_UIC_SOURCE_PERIPH_AUTHEN_SPIFLASH ))
    {
        l_periph_addr &= 0xFFFFFFFFu;   /*only first 24 or 32 bits are valid*/
    }
    else if (src_periph_type == MSS_SYS_UIC_SOURCE_PERIPH_UPROM)
    {
        l_periph_addr &= 0x000000FFu;   /*only first 8 bits are valid*/
        l_periph_addr = (l_periph_addr << 14u);
    }
    else
    {
        return status;
    }

    *(uint32_t*)input_data = l_periph_addr;
    input_data[4] = src_periph_type;

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                 (uint8_t)MSS_SYS_UIC_EXECUTE_SCRIPT_CMD,
                 &input_data[0],
                 (uint16_t)MSS_SYS_EXECUTE_UIC_SCRIPT_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }
    else
    {
        status = execute_ss_polling_mode(
                 (uint8_t)MSS_SYS_UIC_EXECUTE_SCRIPT_CMD,
                 &input_data[0],
                 (uint16_t)MSS_SYS_EXECUTE_UIC_SCRIPT_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }

    return status;
}

/***************************************************************************//**
 * MSS_SYS_authenticate_uic_bitstream()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_authenticate_uic_bitstream
(
    uint32_t spi_flash_address,
    uint16_t mb_offset
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;
    uint32_t l_spi_flash_address = spi_flash_address;

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                 (uint8_t)MSS_SYS_UIC_BITSTREAM_AUTHENTICATE_CMD,
                 (uint8_t* )&l_spi_flash_address,
                 (uint16_t)MSS_SYS_UIC_BITSTREAM_AUTHENTICATE_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }
    else
    {
        status = execute_ss_polling_mode(
                 (uint8_t)MSS_SYS_UIC_BITSTREAM_AUTHENTICATE_CMD,
                 (uint8_t* )&l_spi_flash_address,
                 (uint16_t)MSS_SYS_UIC_BITSTREAM_AUTHENTICATE_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }

    return status;
}

/***************************************************************************//**
 * MSS_SYS_authenticate_bitstream()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_authenticate_bitstream
(
    uint32_t spi_flash_address,
    uint16_t mb_offset
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;
    uint32_t l_spi_flash_address = spi_flash_address;

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                 (uint8_t)MSS_SYS_BITSTREAM_AUTHENTICATE_CMD,
                 (uint8_t* )&l_spi_flash_address,
                 (uint16_t)MSS_SYS_BITSTREAM_AUTHENTICATE_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }
    else
    {
        status = execute_ss_polling_mode(
                 (uint8_t)MSS_SYS_BITSTREAM_AUTHENTICATE_CMD,
                 (uint8_t* )&l_spi_flash_address,
                 (uint16_t)MSS_SYS_BITSTREAM_AUTHENTICATE_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }

    return status;
}

/***************************************************************************//**
 * MSS_SYS_authenticate_iap_image()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_authenticate_iap_image
(
    uint32_t spi_idx
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;

    ASSERT(!(spi_idx == 1u));

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                (uint8_t)MSS_SYS_BITSTREAM_AUTHENTICATE_CMD,
                NULL_BUFFER,
                MSS_SYS_WITHOUT_CMD_DATA,
                NULL_BUFFER,
                MSS_SYS_NO_RESPONSE_LEN,
                (uint16_t)spi_idx,
                MSS_SYS_COMMON_RET_OFFSET);
    }
    else
    {
        status = execute_ss_polling_mode(
                (uint8_t)MSS_SYS_BITSTREAM_AUTHENTICATE_CMD,
                NULL_BUFFER,
                MSS_SYS_WITHOUT_CMD_DATA,
                NULL_BUFFER,
                MSS_SYS_NO_RESPONSE_LEN,
                (uint16_t)spi_idx,
                MSS_SYS_COMMON_RET_OFFSET);
    }

    return status;
}

/***************************************************************************//**
 * MSS_SYS_digest_check()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_digest_check
(
    uint32_t options,
    uint8_t* digesterr,
    uint16_t mb_offset
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;
    uint32_t l_options = options;

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                (uint8_t)MSS_SYS_DIGEST_CHECK_CMD,
                (uint8_t* )&l_options,
                (uint16_t)MSS_SYS_DIGEST_CHECK_DATA_LEN,
                digesterr,
                (uint16_t)MSS_SYS_DIGEST_CHECK_SERVICE_RESP_LEN,
                mb_offset,
                MSS_SYS_COMMON_RET_OFFSET);
    }
    else
    {
        status = execute_ss_polling_mode(
                 (uint8_t)MSS_SYS_DIGEST_CHECK_CMD,
                 (uint8_t* )&l_options,
                 (uint16_t)MSS_SYS_DIGEST_CHECK_DATA_LEN,
                 digesterr,
                 (uint16_t)MSS_SYS_DIGEST_CHECK_SERVICE_RESP_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }

    return status;
}

/***************************************************************************//**
 * MSS_SYS_execute_iap()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_execute_iap
(
    uint8_t iap_cmd,
    uint32_t spiaddr
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;
    uint32_t l_spiaddr = spiaddr;

    if ((MSS_SYS_IAP_PROGRAM_BY_SPIIDX_CMD == iap_cmd)
    || (MSS_SYS_IAP_VERIFY_BY_SPIIDX_CMD == iap_cmd))
    {
        ASSERT(!(1u == spiaddr));
    }

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                 (uint8_t)iap_cmd,
                 (uint8_t*)&l_spiaddr,
                 MSS_SYS_IAP_SERVICE_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 (uint16_t)spiaddr,
                 MSS_SYS_COMMON_RET_OFFSET);
    }
    else
    {
        status = execute_ss_polling_mode(
                 (uint8_t)iap_cmd,
                 (uint8_t*)&l_spiaddr,
                 MSS_SYS_IAP_SERVICE_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 (uint16_t)spiaddr,
                 MSS_SYS_COMMON_RET_OFFSET);
    }

    return status;
}

/***************************************************************************//**
 * MSS_SYS_spi_copy()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_spi_copy
(
    uint64_t mss_dest_addr,
    uint32_t mss_spi_flash,
    uint32_t n_bytes,
    uint8_t options,
    uint16_t mb_offset
)
{
     uint16_t status = MSS_SYS_PARAM_ERR;
     uint8_t mb_format[17];

     *(uint64_t *)mb_format         = mss_dest_addr;
     *(uint32_t *)(mb_format + 8u)  = mss_spi_flash;
     *(uint32_t *)(mb_format + 12u) = n_bytes;
     mb_format[16] = options;

     if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
     {
         status = execute_ss_interrupt_mode(
                 (uint8_t)MSS_SYS_SPI_COPY_CMD,
                 mb_format,
                 (uint16_t)MSS_SYS_SPI_COPY_MAILBOX_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
     }
     else
     {
         status = execute_ss_polling_mode(
                 (uint8_t)MSS_SYS_SPI_COPY_CMD,
                 mb_format,
                 (uint16_t)MSS_SYS_SPI_COPY_MAILBOX_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
     }

     return status;
}

/***************************************************************************//**
 * MSS_SYS_debug_read_probe()
 * See "mss_sysservices.h" for details of how to use this function.
 */
 uint16_t
 MSS_SYS_debug_read_probe
(
    uint8_t ipseg_addr,
    uint8_t iprow_addr,
    uint8_t *prdata,
    uint16_t mb_offset,
    uint8_t resp_offset
)
{
     uint16_t status = MSS_SYS_PARAM_ERR;
     uint8_t mb_format[2];
     uint16_t service_data = 0u;
     uint8_t l_resp_offset = resp_offset;

     service_data = iprow_addr;
     service_data = service_data << 6u;

     service_data = service_data + ipseg_addr;

     *(uint16_t*)mb_format = service_data;

     l_resp_offset = (4u + (4u * l_resp_offset));

     if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
     {
         status = execute_ss_interrupt_mode(
                  (uint8_t)MSS_SYS_PROBE_READ_DEBUG_CMD,
                  mb_format,
                  (uint16_t)MSS_SYS_PROBE_READ_SERVICE_DATA_LEN,
                  prdata,
                  (uint16_t)MSS_SYS_PROBE_READ_SERVICE_RESP_LEN,
                  mb_offset,
                  (uint16_t)l_resp_offset);
     }
     else
     {
         status = execute_ss_polling_mode(
                  (uint8_t)MSS_SYS_PROBE_READ_DEBUG_CMD,
                  mb_format,
                  (uint16_t)MSS_SYS_PROBE_READ_SERVICE_DATA_LEN,
                  prdata,
                  (uint16_t)MSS_SYS_PROBE_READ_SERVICE_RESP_LEN,
                  mb_offset,
                  (uint16_t)l_resp_offset);
     }

     return status;
 }

/***************************************************************************//**
 * MSS_SYS_debug_write_probe()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_debug_write_probe
(
    uint8_t prb_addr,
    uint8_t ipseg_addr,
    uint8_t iprow_addr,
    uint32_t pwmask,
    uint32_t pwdata,
    uint16_t mb_offset
)
{
     uint16_t status = MSS_SYS_PARAM_ERR;
     uint8_t mb_format[12] = {0};

     /* Local variable to store the combination of iprow_addr, ipseg_addr and
      * prb_addr*/
     uint32_t service_data = 0u;

     service_data = iprow_addr;
     service_data = service_data << 12u;

     uint16_t temp = ipseg_addr;
     temp = temp << 6u;
     temp += prb_addr;

     service_data = service_data + temp;

     *(uint32_t *)mb_format       = service_data;
     *(uint32_t *)(mb_format + 4u) = pwmask;
     *(uint32_t *)(mb_format + 8u) = pwdata;

     if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
     {
         status = execute_ss_interrupt_mode(
                  (uint8_t)MSS_SYS_PROBE_WRITE_DEBUG_CMD,
                  mb_format,
                  (uint16_t)MSS_SYS_PROBE_WRITE_SERVICE_DATA_LEN,
                  NULL_BUFFER,
                  MSS_SYS_NO_RESPONSE_LEN,
                  mb_offset,
                  MSS_SYS_COMMON_RET_OFFSET);
     }
     else
     {
         status = execute_ss_polling_mode(
                  (uint8_t)MSS_SYS_PROBE_WRITE_DEBUG_CMD,
                  mb_format,
                  (uint16_t)MSS_SYS_PROBE_WRITE_SERVICE_DATA_LEN,
                  NULL_BUFFER,
                  MSS_SYS_NO_RESPONSE_LEN,
                  mb_offset,
                  MSS_SYS_COMMON_RET_OFFSET);
     }

    return status;
}

/***************************************************************************//**
 * MSS_SYS_debug_live_probe()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_debug_live_probe
(
    uint8_t x_addr,
    uint8_t y_addr,
    uint8_t ipseg_addr,
    uint8_t iprow_addr,
    uint8_t clear,
    uint8_t ioen,
    uint16_t mb_offset,
    uint8_t service_cmd
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;
    uint8_t mb_format[6] = {0};
    uint32_t service_data = 0u;

    uint16_t channel_addr = 0u;
    uint16_t probe_addr = 0u;

    channel_addr = y_addr;
    channel_addr = (channel_addr << 5u) + x_addr;

    probe_addr = iprow_addr;
    probe_addr = (probe_addr << 6u) + ipseg_addr;

    service_data = probe_addr;
    service_data = (service_data << 11u) + channel_addr;

    *(uint32_t*)mb_format = service_data;
    mb_format[4] = clear;
    mb_format[5] = ioen;

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                 service_cmd,
                 mb_format,
                 (uint16_t)MSS_SYS_LIVE_PROBE_DEBUG_SERVICE_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }
    else
    {
        status = execute_ss_polling_mode(
                 service_cmd,
                 mb_format,
                 (uint16_t)MSS_SYS_LIVE_PROBE_DEBUG_SERVICE_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }

    return status;
}

/***************************************************************************//**
 * MSS_SYS_debug_select_mem()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_debug_select_mem
(
    uint8_t ipblk_addr,
    uint8_t ipseg_addr,
    uint8_t iprow_addr,
    uint8_t memtype,
    uint8_t memlock_mode,
    uint16_t timeout,
    uint16_t mb_offset
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;
    uint8_t mb_format[6] = {0};
    uint16_t service_data = 0u;

    service_data = iprow_addr;

    uint16_t temp = ipseg_addr;
    temp = ((temp << 3u) + ipblk_addr);
    service_data = ((temp << 9u) + temp);

    *(uint16_t *)mb_format = service_data;
    mb_format[2] = memtype;
    mb_format[3] = memlock_mode;
    *(uint16_t*)(mb_format + 4u) = timeout;

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                 (uint8_t)MSS_SYS_MEM_SELECT_DEBUG_CMD,
                 mb_format,
                 (uint16_t)MSS_SYS_MEM_SELECT_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }
    else
    {
        status = execute_ss_polling_mode(
                 (uint8_t)MSS_SYS_MEM_SELECT_DEBUG_CMD,
                 mb_format,
                 (uint16_t)MSS_SYS_MEM_SELECT_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }

    return status;
}


/***************************************************************************//**
 * MSS_SYS_debug_read_mem()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_debug_read_mem
(
    uint16_t mem_addr,
    uint16_t n_words,
    uint64_t mss_addr,
    uint16_t mb_offset
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;
    uint8_t mb_format[16] = {0};

    *(uint16_t*)(mb_format)     = mem_addr;
    *(uint16_t*)(mb_format + 2u) = n_words;
    *(uint64_t*)(mb_format + 8u) = mss_addr;

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                 (uint8_t)MSS_SYS_MEM_READ_DEBUG_CMD,
                 mb_format,
                 (uint16_t)MSS_SYS_MEM_READ_WRITE_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }
    else
    {
        status = execute_ss_polling_mode(
                 (uint8_t)MSS_SYS_MEM_READ_DEBUG_CMD,
                 mb_format,
                 (uint16_t)MSS_SYS_MEM_READ_WRITE_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }

    return status;
 }


/***************************************************************************//**
 * MSS_SYS_debug_write_mem()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_debug_write_mem
(
    uint16_t mem_addr,
    uint16_t n_words,
    uint64_t mss_addr,
    uint16_t mb_offset
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;
    uint8_t mb_format[16] = {0};

    *(uint16_t*)(mb_format)     = mem_addr;
    *(uint16_t*)(mb_format + 2u) = n_words;
    *(uint64_t*)(mb_format + 8u) = mss_addr;

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                (uint8_t)MSS_SYS_MEM_WRITE_DEBUG_CMD,
                mb_format,
                (uint16_t)MSS_SYS_MEM_READ_WRITE_DATA_LEN,
                NULL_BUFFER,
                MSS_SYS_NO_RESPONSE_LEN,
                mb_offset,
                MSS_SYS_COMMON_RET_OFFSET);
    }
    else
    {
        status = execute_ss_polling_mode(
                 (uint8_t)MSS_SYS_MEM_WRITE_DEBUG_CMD,
                 mb_format,
                 (uint16_t)MSS_SYS_MEM_READ_WRITE_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }

    return status;
}

/***************************************************************************//**
 * MSS_SYS_debug_read_apb()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_debug_read_apb
(
    uint32_t apb_addr,
    uint8_t  apb_wsize,
    uint16_t max_bytes,
    uint64_t mss_addr,
    uint16_t mb_offset
)
{
     uint16_t status = MSS_SYS_PARAM_ERR;
    uint8_t mb_format[24] = {0};

    *(uint32_t *)mb_format = apb_addr;
    mb_format[5] = apb_wsize;
    *(uint16_t *)(mb_format + 8u)  = max_bytes;
    *(uint64_t *)(mb_format + 16u) = mss_addr;

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                 (uint8_t)MSS_SYS_APB_READ_DEBUG_CMD,
                 mb_format,
                 (uint16_t)MSS_SYS_APB_SERVICE_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }
    else
    {
        status = execute_ss_polling_mode(
                 (uint8_t)MSS_SYS_APB_READ_DEBUG_CMD,
                 mb_format,
                 (uint16_t)MSS_SYS_APB_SERVICE_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }

    return status;
}

/***************************************************************************//**
 * MSS_SYS_debug_write_apb()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_debug_write_apb
(
    uint32_t apb_addr,
    uint8_t  apb_wsize,
    uint16_t max_bytes,
    uint64_t mss_addr,
    uint16_t mb_offset
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;
    uint8_t mb_format[24] = {0};

    *(uint32_t *)mb_format = apb_addr;
    mb_format[5] = apb_wsize;
    *(uint16_t *)(mb_format + 8u)  = max_bytes;
    *(uint64_t *)(mb_format + 16u) = mss_addr;

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                 (uint8_t)MSS_SYS_APB_WRITE_DEBUG_CMD,
                 mb_format,
                 (uint16_t)MSS_SYS_APB_SERVICE_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }
    else
    {
        status = execute_ss_polling_mode(
                 (uint8_t)MSS_SYS_APB_WRITE_DEBUG_CMD,
                 mb_format,
                 (uint16_t)MSS_SYS_APB_SERVICE_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }

    return status;
}

/***************************************************************************//**
 * MSS_SYS_debug_fabric_snapshot()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_debug_fabric_snapshot
(
    uint32_t port_addr,
    uint8_t apb_fast_write,
    uint16_t mb_offset
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;
    uint8_t mb_format[5]={0};

    *(uint32_t *)mb_format = port_addr;
    mb_format[4] = apb_fast_write;

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                 (uint8_t)MSS_SYS_DEBUG_SNAPSHOT_CMD,
                 mb_format,
                 (uint16_t)MSS_SYS_DEBUG_SNAPSHOT_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }
    else
    {
        status = execute_ss_polling_mode(
                 (uint8_t)MSS_SYS_DEBUG_SNAPSHOT_CMD,
                 mb_format,
                 (uint16_t)MSS_SYS_DEBUG_SNAPSHOT_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 MSS_SYS_COMMON_RET_OFFSET);
    }

    return status;
}

/***************************************************************************//**
 * MSS_SYS_otp_generate()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_otp_generate
(
    uint8_t keymode,
    uint8_t* n_user,
    uint8_t* n_fpga,
    uint16_t mb_offset,
    uint16_t resp_offset
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;
    uint8_t mb_format[20] = {0};
    uint8_t index = 0u;

    mb_format[index] = keymode;

    for (index = 0u; index < 16u; index++ )
    {
        mb_format[index + 4u] = *(n_user + index);
    }

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                 (uint8_t)MSS_SYS_GENERATE_OTP_CMD,
                 mb_format,
                 (uint16_t)MSS_SYS_GENERATE_OTP_DATA_LEN,
                 n_fpga,
                 (uint16_t)MSS_SYS_GENERATE_OTP_RESP_LEN,
                 mb_offset,
                 resp_offset);
    }
    else
    {
        status = execute_ss_polling_mode(
                 (uint8_t)MSS_SYS_GENERATE_OTP_CMD,
                 mb_format,
                 (uint16_t)MSS_SYS_GENERATE_OTP_DATA_LEN,
                 n_fpga,
                 (uint16_t)MSS_SYS_GENERATE_OTP_RESP_LEN,
                 mb_offset,
                 resp_offset);
    }

    return status;
}

/***************************************************************************//**
 * MSS_SYS_otp_match()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t MSS_SYS_otp_match
(
    uint8_t * user_id,
    uint8_t * validator,
    uint8_t * otp,
    uint16_t mb_offset,
    uint16_t resp_offset
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;
    uint8_t mb_format[80] = {0};
    uint8_t index = 0u;

    for (index = 0u; index < 80u; index++)
    {
        if (index < 16u)
        {
            mb_format[index] = user_id[index];
        }
        if ((index > 15u) && (index < 48u))
        {
            mb_format[index] = validator[index - 16u];
        }
        if (index > 47u)
        {
            mb_format[index] = otp[index - 48u];
        }
    }

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                 (uint8_t)MSS_SYS_MATCH_OTP_CMD,
                 mb_format,
                 (uint16_t)MSS_SYS_MATCH_OTP_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 resp_offset);
    }
    else
    {
        status = execute_ss_polling_mode(
                 (uint8_t)MSS_SYS_MATCH_OTP_CMD,
                 mb_format,
                 (uint16_t)MSS_SYS_MATCH_OTP_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 resp_offset);
    }

    return status;
}

/***************************************************************************//**
 * MSS_SYS_unlock_debug_passcode()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_unlock_debug_passcode
(
    uint8_t* cmd_data,
    uint16_t mb_offset,
    uint16_t resp_offset
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;
    //uint8_t mb_format[32] = {0};
    //uint8_t index = 0u;

    //for (index = 0u; index < 32u; index++)
    //{
    //    mb_format[index] = cmd_data[index];
    //}

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                 (uint8_t)MSS_SYS_UNLOCK_DEBUG_PASSCODE,
                 cmd_data,
                 (uint16_t)MSS_SYS_UNLOCK_DEBUG_PASSCODE_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 resp_offset);
    }
    else
    {
        status = execute_ss_polling_mode(
                 (uint8_t)MSS_SYS_UNLOCK_DEBUG_PASSCODE,
                 cmd_data,
                 (uint16_t)MSS_SYS_UNLOCK_DEBUG_PASSCODE_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 resp_offset);
    }

    return status;
}

/***************************************************************************//**
 * MSS_SYS_one_way_passcode()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_one_way_passcode
(
    uint8_t* msg_id,
    uint8_t* validator,
    uint8_t keymode,
    uint8_t* dsn,
    uint8_t* hash,
    uint8_t* plaintext_passcode,
    uint8_t* hwm,
    uint16_t mb_offset,
    uint16_t resp_offset
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;
    uint8_t mb_format[480] = {0};
    uint16_t index = 0;
    for (index = 0u; index < 480u; index++)
    {
        if ( index < 16u)
        {
            mb_format[index] = msg_id[index];
        }
        if ((index > 15u) && (index < 48u))
        {
            mb_format[index] = validator[index - 16];
        }
        if ( index == 51u)
        {
            mb_format[index] = keymode;
        }
        if ((index > 67u) && (index < 84u))
        {
            mb_format[index] = dsn[index - 68];
        }
        if ((index > 351u) && (index < 384u))
        {
            mb_format[index] = hash[index - 352];
        }
        if ((index > 383u) && (index < 416u))
        {
            mb_format[index] = plaintext_passcode[index - 384];
        }
        if ((index > 415u) && (index < 432u))
        {
            mb_format[index] = hwm[index];
        }
    }

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                 (uint8_t)MSS_SYS_ONE_WAY_PASSCODE_CMD,
                 mb_format,
                 (uint16_t)MSS_SYS_ONE_WAY_PASSCODE_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 resp_offset);
    }
    else
    {
        status = execute_ss_polling_mode(
                 (uint8_t)MSS_SYS_ONE_WAY_PASSCODE_CMD,
                 mb_format,
                 (uint16_t)MSS_SYS_ONE_WAY_PASSCODE_DATA_LEN,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 resp_offset);
    }

    return status;
}

/***************************************************************************//**
 * MSS_SYS_debug_terminate()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t
MSS_SYS_debug_terminate
(
    uint16_t mb_offset,
    uint16_t resp_offset
)
{
    uint16_t status = MSS_SYS_PARAM_ERR;

    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        status = execute_ss_interrupt_mode(
                 (uint8_t)MSS_SYS_TERMINATE_DEBUG_CMD,
                 NULL_BUFFER,
                 MSS_SYS_WITHOUT_CMD_DATA,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 resp_offset);
    }
    else
    {
        status = execute_ss_polling_mode(
                 (uint8_t)MSS_SYS_TERMINATE_DEBUG_CMD,
                 NULL_BUFFER,
                 MSS_SYS_WITHOUT_CMD_DATA,
                 NULL_BUFFER,
                 MSS_SYS_NO_RESPONSE_LEN,
                 mb_offset,
                 resp_offset);
    }

    return status;
}

/***************************************************************************//**
 * MSS_SYS_read_response()
 * See "mss_sysservices.h" for details of how to use this function.
 */
uint16_t MSS_SYS_read_response
(
    void
)
{
    uint16_t response_limit = 0u;
    uint32_t idx;
    uint16_t status = MSS_SYS_PARAM_ERR;

    if (g_message_interrupt_counter > 0u)
    {
        g_message_interrupt_counter = 0u;

        if (g_int_service_response_size > 0u)
        {
            response_limit = g_int_service_response_size +
                             g_int_service_response_offset;

            for (idx = g_int_service_response_offset; idx < response_limit; idx++)
            {
                gp_int_service_response[idx - g_int_service_response_offset] =
                        *((uint8_t *)MSS_SCBMAILBOX + idx);
            }
        }

        /* Read the status returned by System Controller*/
        status = ((MSS_SCBCTRL->SERVICES_SR & SCBCTRL_SERVICESSR_STATUS_MASK) >>
                SCBCTRL_SERVICESSR_STATUS);
    }

    return status;
}

/***************************************************************************//**
 Internal functions.
*/

/*
 * This function requests the system service to the system controller. It will
 * first write the Mailbox input data to the mailbox from the cmd_data if
 * required for that service.
 *
 */
static uint16_t request_system_service
(
    uint8_t cmd_opcode,
    uint8_t* cmd_data,
    uint16_t cmd_data_size,
    uint8_t* p_response,
    uint16_t response_size,
    uint16_t mb_offset,
    uint16_t response_offset

)
{
    uint32_t idx;
    uint16_t ss_command = 0u;
    uint32_t* word_buf ;
    uint8_t* byte_buf ;
    uint8_t byte_off;
    uint8_t byte_index;
    uint32_t * mailbox_reg;
    uint32_t mailbox_val = 0u;

    if (MSS_SCBCTRL->SERVICES_SR & SCBCTRL_SERVICESSR_BUSY_MASK)
    {
        /*System controller is busy with executing service*/
        return MSS_SYS_BUSY;
    }

    /*Code for MSS_SYS_PARAM_ERR is not implemented with this version of driver.*/

    *MSS_SCBMESSAGE_INT = 0x0u; /*clear message_int reg*/

    if (g_service_mode == MSS_SYS_SERVICE_INTERRUPT_MODE)
    {
        gp_int_service_response = (uint8_t*)p_response;
        g_int_service_response_offset = response_offset;
        g_int_service_response_size = response_size;
    }

    if (cmd_data_size > 0u)
    {
        word_buf = (uint32_t*)cmd_data;

        /* Write the user data into mail box. */
        for (idx = 0u; idx < (cmd_data_size / 4u); idx++)
        {
            *(MSS_SCBMAILBOX + idx) = word_buf[idx];
        }

        if ((cmd_data_size % 4u) > 0u)
        {
             byte_off = (((cmd_data_size / 4u) * 4u));
             byte_buf = (uint8_t*)(cmd_data + byte_off);

             mailbox_reg = (MSS_SCBMAILBOX + idx);
             mailbox_val = *mailbox_reg;

             for (byte_index = 0u; byte_index < (cmd_data_size % 4u);
                  byte_index++)
             {
                 mailbox_val &= ~(0xffu << (byte_index * 8u));
                 mailbox_val |= (byte_buf[byte_index] << (byte_index * 8u));
             }
             *mailbox_reg = mailbox_val;
        }
    }

    /*Form the SS command: bit 0to6 is the opcode, bit 7to15 is the Mailbox
         offset For some services this field has another meaning.
        (e.g. for IAP bit-stream auth. it means spi_idx)*/
    ss_command = ((mb_offset << 7u) |  (cmd_opcode & 0x7Fu));

    /*Interrupt based implementation of services */
    if (MSS_SYS_SERVICE_INTERRUPT_MODE == g_service_mode)
    {
        MSS_SCBCTRL->SERVICES_CR = (((ss_command << SCBCTRL_SERVICESCR_COMMAND)
                & SCBCTRL_SERVICESCR_COMMAND_MASK) |
                SCBCTRL_SERVICESCR_REQ_MASK |
                SCBCTRL_SERVICESSR_NOTIFY_MASK);
    }
    else
    {
        MSS_SCBCTRL->SERVICES_CR = (((ss_command << SCBCTRL_SERVICESCR_COMMAND)
                & SCBCTRL_SERVICESCR_COMMAND_MASK) |
                SCBCTRL_SERVICESCR_REQ_MASK);

    }

    /*Service requested successfully*/
    return MSS_SYS_SUCCESS;
}

/*
 * This function executes the SS command in interrupt mode. If Mailbox input data
 * is required by the service, the call to request_system_service() function will
 * first load it from cmd_data into the Mailbox. The response of the service is
 * not read by this function as it depends on message interrupt. Application
 * will have to read the response of service by calling MSS_SYS_read_response(),
 * only after interrupt occurs.
 */
static uint16_t execute_ss_interrupt_mode
(
    uint8_t cmd_opcode,
    uint8_t* cmd_data,
    uint16_t cmd_data_size,
    uint8_t* p_response,
    uint16_t response_size,
    uint16_t mb_offset,
    uint16_t response_offset
)
{

    uint16_t status;
    status = request_system_service(cmd_opcode, cmd_data, cmd_data_size,
                                    p_response, response_size, mb_offset,
                                    response_offset);

    return status;
 }

/*
 * This function executes the SS command in polling mode. If Mailbox input data
 * is required by the it will first load it from cmd_data into the Mailbox.
 * After requesting the service it will poll the request and busy bit. If the
 * service requires the response data to be read from mailbox, it will read the
 * mailbox contents and store it in p_response buffer.
 */
static uint16_t execute_ss_polling_mode
(
    uint8_t cmd_opcode,
    uint8_t* cmd_data,
    uint16_t cmd_data_size,
    uint8_t* p_response,
    uint16_t response_size,
    uint16_t mb_offset,
    uint16_t response_offset
)
{
    uint32_t idx;
    uint16_t status = 0u;
    uint16_t response_limit = 0u;
    uint8_t* response_buf;

    status = request_system_service(cmd_opcode, cmd_data, cmd_data_size,
                                   p_response,response_size, mb_offset,
                                   response_offset);

    if (status == MSS_SYS_SUCCESS)
    {
        /**REQ bit will remain set till the system controller starts
         * processing command. Since DRI is slow interface, we are waiting
         * here to make sure System controller has started processing
         * command*/
        while (SCBCTRL_SERVICESCR_REQ_MASK == (MSS_SCBCTRL->SERVICES_CR &
                SCBCTRL_SERVICESCR_REQ_MASK))
        {
            ;
        }

        /*Once system controller starts processing command The busy bit will
         *  go 1. Make sure that service is complete i.e. BUSY bit is gone 0*/
        while (SCBCTRL_SERVICESSR_BUSY_MASK == (MSS_SCBCTRL->SERVICES_SR &
                SCBCTRL_SERVICESSR_BUSY_MASK))
        {
            ;
        }

        if (response_size > 0u)
        {
            response_limit = response_size + response_offset;
            response_buf = (uint8_t*)p_response;

            for (idx = response_offset; idx < response_limit; idx++)
            {
                response_buf[idx - response_offset] =
                        *((uint8_t *)MSS_SCBMAILBOX + idx);
            }
        }

        /*Read the status returned by System Controller*/
        status = ((MSS_SCBCTRL->SERVICES_SR & SCBCTRL_SERVICESSR_STATUS_MASK) >>
                SCBCTRL_SERVICESSR_STATUS);
    }
    else
    {
        status = MSS_SYS_BUSY;
    }

    return status;
}

/***************************************************************************//**
 * Interrupt service routine triggered by message interrupt.
 * This routine will call handler function which will read the service response
 * in interrupt mode of operation.
 */
uint8_t
g5c_message_plic_IRQHandler
(
    void
)
{
    g_message_interrupt_counter++;

    volatile uint32_t reg = *MSS_SCBMESSAGE; /*read message reg.*/
    reg = *MSS_SCBMESSAGE_INT;
    *MSS_SCBMESSAGE_INT = 0x0u; /*clear message_int reg*/
    reg = *MSS_SCBMESSAGE_INT;
    (void)reg; // reference to avoid compiler warning

    mss_sys_interrupt_handler();

    return 0;
}

#ifdef __cplusplus
}
#endif
