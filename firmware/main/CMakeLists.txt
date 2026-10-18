idf_component_register(SRCS "main.c" "network.c" "board.c"
    INCLUDE_DIRS "."
    REQUIRES esp_wifi esp_event esp_netif esp_http_server esp_http_client mbedtls
    app_update nvs_flash esp_lcd esp_driver_gpio
    esp_driver_i2c esp_timer esp_psram esp_hw_support esp_app_format esp_system)
