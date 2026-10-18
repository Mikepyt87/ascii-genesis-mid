// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <stdbool.h>
typedef enum {
    LINK_OFFLINE, LINK_CONNECTING, LINK_CONNECTED, LINK_AUTH_FAILED,
    LINK_AUTH_TIMEOUT, LINK_NOT_FOUND, LINK_SECURITY_FAILED, LINK_FAILED, LINK_TIMEOUT
} wifi_link_state_t;
static inline const char *wifi_link_message(wifi_link_state_t state) {
    switch(state){
        case LINK_CONNECTING:return "Connecting to home Wi-Fi...";
        case LINK_CONNECTED:return "Connected to home Wi-Fi";
        case LINK_AUTH_FAILED:return "Wi-Fi login failed - check password";
        case LINK_AUTH_TIMEOUT:return "Router login timed out - check security";
        case LINK_NOT_FOUND:return "Network not found - check name and 2.4 GHz";
        case LINK_SECURITY_FAILED:return "Router security unsupported - try WPA2";
        case LINK_TIMEOUT:return "Connection timed out - reopen Wi-Fi setup";
        case LINK_FAILED:return "Connection lost - retrying shortly";
        default:return "Offline ocean ready";
    }
}
static inline bool wifi_link_retry_due(wifi_link_state_t state,long long now,long long retry_at) {
    return state!=LINK_CONNECTED&&state!=LINK_CONNECTING&&now>=retry_at;
}
static inline bool wifi_link_timed_out(wifi_link_state_t state,long long now,long long deadline) {
    return state==LINK_CONNECTING&&now>=deadline;
}
