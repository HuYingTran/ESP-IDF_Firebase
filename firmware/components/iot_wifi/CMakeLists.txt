idf_component_register(
    SRCS "iot_wifi_ap.c" "iot_wifi_sta.c"
    INCLUDE_DIRS "include"
    REQUIRES iot_var iot_realtime
) 
