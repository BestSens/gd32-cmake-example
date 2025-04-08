/*!
    \file    netconf.c
    \brief   network connection configuration

    \version 2024-12-20, V3.3.1, firmware for GD32F4xx
*/

/*
    Copyright (c) 2024, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "ethernetif.h"
#include "stdint.h"
#include "main.h"
#include "netconf.h"
#include <stdio.h>
#include "lwip/priv/tcp_priv.h"
#include "lwip/timeouts.h"

#define DHCP_TRIES_MAX_TIMES        4

typedef enum {
    DHCP_ADDR_NONE = 0,
    DHCP_ADDR_BEGIN,
    DHCP_ADDR_GOT,
    DHCP_ADDR_FAIL
} dhcp_addr_status_enum;

#ifdef USE_DHCP
uint32_t finecurtime = 0;
uint32_t coarsecurtime = 0;
dhcp_addr_status_enum dhcp_addr_status = DHCP_ADDR_NONE;
#endif /* USE_DHCP */

struct netif g_mynetif;
uint32_t tcpcurtime = 0;
uint32_t arpcurtime = 0;
uint32_t acdcurtime = 0;
ip_addr_t ip_address = {0};

void lwip_dhcp_address_get(void);

/*!
    \brief      initializes the LwIP stack
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lwip_stack_init(void)
{
    ip_addr_t gd_ipaddr;
    ip_addr_t gd_netmask;
    ip_addr_t gd_gw;

    /* initialize the lwIP dynamic memory heap and memory pools */
    mem_init();
    memp_init();

#ifdef TIMEOUT_CHECK_USE_LWIP
    sys_timeouts_init();
#endif /* TIMEOUT_CHECK_USE_LWIP */

#ifdef USE_DHCP
    gd_ipaddr.addr = 0;
    gd_netmask.addr = 0;
    gd_gw.addr = 0;
#else
    IP4_ADDR(&gd_ipaddr, BOARD_IP_ADDR0, BOARD_IP_ADDR1, BOARD_IP_ADDR2, BOARD_IP_ADDR3);
    IP4_ADDR(&gd_netmask, BOARD_NETMASK_ADDR0, BOARD_NETMASK_ADDR1, BOARD_NETMASK_ADDR2, BOARD_NETMASK_ADDR3);
    IP4_ADDR(&gd_gw, BOARD_GW_ADDR0, BOARD_GW_ADDR1, BOARD_GW_ADDR2, BOARD_GW_ADDR3);

#endif /* USE_DHCP */

    /* add a new network interface */
    netif_add(&g_mynetif, &gd_ipaddr, &gd_netmask, &gd_gw, NULL, &ethernetif_init, &ethernet_input);

    /* set a default network interface */
    netif_set_default(&g_mynetif);

    /* set a callback when interface is up/down */
    netif_set_status_callback(&g_mynetif, lwip_netif_status_callback);

    /* set the flag of netif as NETIF_FLAG_LINK_UP */
    netif_set_link_up(&g_mynetif);

    /* bring an interface up and set the flag of netif as NETIF_FLAG_UP */
    netif_set_up(&g_mynetif);
}

/*!
    \brief      called when a farme is received from the interface
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lwip_frame_recv(void)
{
    /* get frame from the interface and pass it to the LwIP stack */
    ethernetif_input(&g_mynetif);
}

/*!
    \brief      call the time-related function periodicallytasks
    \param[in]  curtime: the value of current time
    \param[out] none
    \retval     none
*/
void lwip_timeouts_check(__IO uint32_t curtime)
{
#if LWIP_TCP
    /* called periodically to dispatch TCP timers every 250 ms */
    if(curtime - tcpcurtime >= TCP_TMR_INTERVAL) {
        tcpcurtime =  curtime;
        tcp_tmr();
    }

#endif /* LWIP_TCP */

    /* called periodically to dispatch ARP timers every 1s */
    if((curtime - arpcurtime) >= ARP_TMR_INTERVAL) {
        arpcurtime = curtime;
        etharp_tmr();
    }

#ifdef USE_DHCP
    /* called periodically to check whether an outstanding DHCP request is timed out every 500 ms */
    if(curtime - finecurtime >= DHCP_FINE_TIMER_MSECS) {
        finecurtime = curtime;
        dhcp_fine_tmr();
        if((DHCP_ADDR_GOT != dhcp_addr_status) && (DHCP_ADDR_FAIL != dhcp_addr_status)) {
            /* process DHCP state machine */
            lwip_dhcp_address_get();
        }
    }

    /* called periodically to check for lease renewal/rebind timeouts every 60s */
    if(curtime - coarsecurtime >= DHCP_COARSE_TIMER_MSECS) {
        coarsecurtime = curtime;
        dhcp_coarse_tmr();
    }

#ifdef LWIP_ACD
if((curtime - acdcurtime) >= ACD_TMR_INTERVAL) {
        acdcurtime = curtime;
        acd_tmr();
    }
#endif /* LWIP_ACD */
#endif /* USE_DHCP */

}

#ifdef USE_DHCP
/*!
    \brief      get IP address through DHCP function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lwip_dhcp_address_get(void)
{
    ip_addr_t gd_ipaddr;
    ip_addr_t gd_netmask;
    ip_addr_t gd_gw;
    struct dhcp *dhcp_client;

    switch(dhcp_addr_status) {
    case DHCP_ADDR_NONE:
        dhcp_start(&g_mynetif);

        dhcp_addr_status = DHCP_ADDR_BEGIN;
        break;

    case DHCP_ADDR_BEGIN:
        /* got the IP address */
        ip_address.addr = g_mynetif.ip_addr.addr;

        if(0 != ip_address.addr) {
            dhcp_addr_status = DHCP_ADDR_GOT;

            printf("\r\nDHCP -- eval board ip address: %d.%d.%d.%d \r\n", ip4_addr1_16(&ip_address), \
                   ip4_addr2_16(&ip_address), ip4_addr3_16(&ip_address), ip4_addr4_16(&ip_address));
        } else {
            /* DHCP timeout */
            dhcp_client = netif_dhcp_data(&g_mynetif);
            if(dhcp_client->tries > DHCP_TRIES_MAX_TIMES) {
                dhcp_addr_status = DHCP_ADDR_FAIL;
                /* stop DHCP */
                dhcp_stop(&g_mynetif);

                /* use static address as IP address */
                IP4_ADDR(&gd_ipaddr, BOARD_IP_ADDR0, BOARD_IP_ADDR1, BOARD_IP_ADDR2, BOARD_IP_ADDR3);
                IP4_ADDR(&gd_netmask, BOARD_NETMASK_ADDR0, BOARD_NETMASK_ADDR1, BOARD_NETMASK_ADDR2, BOARD_NETMASK_ADDR3);
                IP4_ADDR(&gd_gw, BOARD_GW_ADDR0, BOARD_GW_ADDR1, BOARD_GW_ADDR2, BOARD_GW_ADDR3);
                netif_set_addr(&g_mynetif, &gd_ipaddr, &gd_netmask, &gd_gw);
            }
        }
        break;

    default:
        break;
    }
}
#endif /* USE_DHCP */

unsigned long sys_now(void)
{
    extern volatile unsigned int g_localtime;
    return g_localtime;
}
