 /*******************************************************************************
 * Copyright 2019 Microchip Corporation.
 *
 * SPDX-License-Identifier: MIT
 *
 * Register bit offsets and masks definitions for PolarFire SoC MSS system
 * services
 *
 * SVN $Revision$
 * SVN $Date$
 */
#ifndef MSS_SYS_SERVICES_REGS_H_
#define MSS_SYS_SERVICES_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************SCBCTRL  SERVICES_CR register*************************/
#define SCBCTRL_SERVICESCR_REQ                      (0u)
#define SCBCTRL_SERVICESCR_REQ_MASK                 (1u << SCBCTRL_SERVICESCR_REQ)

#define SCBCTRL_SERVICESCR_BUSY                     (1u)
#define SCBCTRL_SERVICESCR_BUSY_MASK                (1u << SCBCTRL_SERVICESCR_BUSY)

#define SCBCTRL_SERVICESCR_ABORT                    (2u)
#define SCBCTRL_SERVICESCR_ABORT_MASK               (1u << SCBCTRL_SERVICESCR_ABORT)

#define SCBCTRL_SERVICESCR_NOTIFY                   (3u)
#define SCBCTRL_SERVICESCR_NOTIFY_MASK              (1u << SCBCTRL_SERVICESCR_NOTIFY)

#define SCBCTRL_SERVICESCR_COMMAND                  (16u)
#define SCBCTRL_SERVICESCR_COMMAND_MASK             (0xFFFFu << SCBCTRL_SERVICESCR_COMMAND)


/***************SCBCTRL  SERVICES_SR registers*************************/
#define SCBCTRL_SERVICESSR_REQ                      (0u)
#define SCBCTRL_SERVICESSR_REQ_MASK                 (1u << SCBCTRL_SERVICESSR_REQ)

#define SCBCTRL_SERVICESSR_BUSY                     (1u)
#define SCBCTRL_SERVICESSR_BUSY_MASK                (1u << SCBCTRL_SERVICESSR_BUSY)

#define SCBCTRL_SERVICESSR_ABORT                    (2u)
#define SCBCTRL_SERVICESSR_ABORT_MASK               (1u << SCBCTRL_SERVICESSR_ABORT)

#define SCBCTRL_SERVICESSR_NOTIFY                   (3u)
#define SCBCTRL_SERVICESSR_NOTIFY_MASK              (1u << SCBCTRL_SERVICESSR_NOTIFY)

#define SCBCTRL_SERVICESSR_STATUS                   (16u)
#define SCBCTRL_SERVICESSR_STATUS_MASK              (0xFFFFu << SCBCTRL_SERVICESSR_STATUS)

#ifdef __cplusplus
}
#endif

#endif /* MSS_SYS_SERVICES_REGS_H_ */
