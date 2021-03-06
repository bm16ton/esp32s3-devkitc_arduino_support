/* 
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * This file is part of the TinyUSB stack.
 */

#ifndef _TUSB_USBH_H_
#define _TUSB_USBH_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "common/tusb_common.h"
#include "hcd.h"

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF
//--------------------------------------------------------------------+

// forward declaration
struct tuh_control_xfer_s;
typedef struct tuh_control_xfer_s tuh_control_xfer_t;

typedef bool (*tuh_control_xfer_cb_t)(uint8_t daddr, tuh_control_xfer_t const * xfer, xfer_result_t result);

struct tuh_control_xfer_s
{
  tusb_control_request_t request TU_ATTR_ALIGNED(4);
  uint8_t* buffer;
  tuh_control_xfer_cb_t complete_cb;
  uintptr_t user_arg;
};

//--------------------------------------------------------------------+
// APPLICATION CALLBACK
//--------------------------------------------------------------------+

//TU_ATTR_WEAK uint8_t tuh_attach_cb (tusb_desc_device_t const *desc_device);

// Invoked when device is mounted (configured)
TU_ATTR_WEAK void tuh_mount_cb (uint8_t daddr);

/// Invoked when device is unmounted (bus reset/unplugged)
TU_ATTR_WEAK void tuh_umount_cb(uint8_t daddr);

//--------------------------------------------------------------------+
// APPLICATION API
//--------------------------------------------------------------------+

// Init host stack
bool tuh_init(uint8_t rhport);

// Check if host stack is already initialized
bool tuh_inited(void);

// Task function should be called in main/rtos loop
void tuh_task(void);

// Interrupt handler, name alias to HCD
extern void hcd_int_handler(uint8_t rhport);
#define tuh_int_handler   hcd_int_handler

bool tuh_vid_pid_get(uint8_t daddr, uint16_t* vid, uint16_t* pid);

tusb_speed_t tuh_speed_get(uint8_t daddr);

// Check if device is connected and configured
bool tuh_mounted(uint8_t daddr);

// Check if device is suspended
TU_ATTR_ALWAYS_INLINE
static inline bool tuh_suspended(uint8_t daddr)
{
  // TODO implement suspend & resume on host
  (void) daddr;
  return false;
}

// Check if device is ready to communicate with
TU_ATTR_ALWAYS_INLINE
static inline bool tuh_ready(uint8_t daddr)
{
  return tuh_mounted(daddr) && !tuh_suspended(daddr);
}

// Carry out a control transfer
// true on success, false if there is on-going control transfer or incorrect parameters
// Blocking if complete callback is NULL, in this case 'user_arg' must contain xfer_result_t variable
bool tuh_control_xfer (uint8_t daddr, tuh_control_xfer_t const* xfer);

// Sync (blocking) version of tuh_control_xfer()
// return transfer result
uint8_t tuh_control_xfer_sync(uint8_t daddr, tuh_control_xfer_t const* xfer, uint32_t timeout_ms);

// Set Configuration (control transfer)
// config_num = 0 will un-configure device. Note: config_num = config_descriptor_index + 1
// true on success, false if there is on-going control transfer or incorrect parameters
// Blocking if complete callback is NULL, in this case 'user_arg' must contain xfer_result_t variable
bool tuh_configuration_set(uint8_t daddr, uint8_t config_num,
                           tuh_control_xfer_cb_t complete_cb, uintptr_t user_arg);

//--------------------------------------------------------------------+
// Descriptors Asynchronous (non-blocking)
//--------------------------------------------------------------------+

// Get an descriptor (control transfer)
// true on success, false if there is on-going control transfer or incorrect parameters
bool tuh_descriptor_get(uint8_t daddr, uint8_t type, uint8_t index, void* buffer, uint16_t len,
                        tuh_control_xfer_cb_t complete_cb, uintptr_t user_arg);

// Get device descriptor (control transfer)
// true on success, false if there is on-going control transfer or incorrect parameters
bool tuh_descriptor_get_device(uint8_t daddr, void* buffer, uint16_t len,
                               tuh_control_xfer_cb_t complete_cb, uintptr_t user_arg);

// Get configuration descriptor (control transfer)
// true on success, false if there is on-going control transfer or incorrect parameters
bool tuh_descriptor_get_configuration(uint8_t daddr, uint8_t index, void* buffer, uint16_t len,
                                      tuh_control_xfer_cb_t complete_cb, uintptr_t user_arg);

// Get HID report descriptor (control transfer)
// true on success, false if there is on-going control transfer or incorrect parameters
bool tuh_descriptor_get_hid_report(uint8_t daddr, uint8_t itf_num, uint8_t desc_type, uint8_t index, void* buffer, uint16_t len,
                                   tuh_control_xfer_cb_t complete_cb, uintptr_t user_arg);

// Get string descriptor (control transfer)
// true on success, false if there is on-going control transfer or incorrect parameters
// Blocking if complete callback is NULL, in this case 'user_arg' must contain xfer_result_t variable
bool tuh_descriptor_get_string(uint8_t daddr, uint8_t index, uint16_t language_id, void* buffer, uint16_t len,
                               tuh_control_xfer_cb_t complete_cb, uintptr_t user_arg);

// Get manufacturer string descriptor (control transfer)
// true on success, false if there is on-going control transfer or incorrect parameters
bool tuh_descriptor_get_manufacturer_string(uint8_t daddr, uint16_t language_id, void* buffer, uint16_t len,
                                            tuh_control_xfer_cb_t complete_cb, uintptr_t user_arg);

// Get product string descriptor (control transfer)
// true on success, false if there is on-going control transfer or incorrect parameters
bool tuh_descriptor_get_product_string(uint8_t daddr, uint16_t language_id, void* buffer, uint16_t len,
                                       tuh_control_xfer_cb_t complete_cb, uintptr_t user_arg);

// Get serial string descriptor (control transfer)
// true on success, false if there is on-going control transfer or incorrect parameters
bool tuh_descriptor_get_serial_string(uint8_t daddr, uint16_t language_id, void* buffer, uint16_t len,
                                      tuh_control_xfer_cb_t complete_cb, uintptr_t user_arg);

//--------------------------------------------------------------------+
// Descriptors Synchronous (blocking)
//--------------------------------------------------------------------+

// Sync (blocking) version of tuh_descriptor_get()
// return transfer result
uint8_t tuh_descriptor_get_sync(uint8_t daddr, uint8_t type, uint8_t index, void* buffer, uint16_t len, uint8_t timeout_ms);

// Sync (blocking) version of tuh_descriptor_get_device()
// return transfer result
uint8_t tuh_descriptor_get_device_sync(uint8_t daddr, void* buffer, uint16_t len, uint8_t timeout_ms);

// Sync (blocking) version of tuh_descriptor_get_configuration()
// return transfer result
uint8_t tuh_descriptor_get_configuration_sync(uint8_t daddr, uint8_t index, void* buffer, uint16_t len, uint8_t timeout_ms);

// Sync (blocking) version of tuh_descriptor_get_hid_report()
// return transfer result
uint8_t tuh_descriptor_get_hid_report_sync(uint8_t daddr, uint8_t itf_num, uint8_t desc_type, uint8_t index, void* buffer, uint16_t len, uint8_t timeout_ms);

// Sync (blocking) version of tuh_descriptor_get_string()
// return transfer result
uint8_t tuh_descriptor_get_string_sync(uint8_t daddr, uint8_t index, uint16_t language_id, void* buffer, uint16_t len, uint8_t timeout_ms);

// Sync (blocking) version of tuh_descriptor_get_manufacturer_string()
// return transfer result
uint8_t tuh_descriptor_get_manufacturer_string_sync(uint8_t daddr, uint16_t language_id, void* buffer, uint16_t len, uint8_t timeout_ms);

// Sync (blocking) version of tuh_descriptor_get_product_string()
// return transfer result
uint8_t tuh_descriptor_get_product_string_sync(uint8_t daddr, uint16_t language_id, void* buffer, uint16_t len, uint8_t timeout_ms);

// Sync (blocking) version of tuh_descriptor_get_serial_string()
// return transfer result
uint8_t tuh_descriptor_get_serial_string_sync(uint8_t daddr, uint16_t language_id, void* buffer, uint16_t len, uint8_t timeout_ms);

#ifdef __cplusplus
 }
#endif

#endif
