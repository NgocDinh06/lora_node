// #include <stdio.h>
// #include <stdint.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/gpio.h"
// #include "driver/spi_master.h"
// #include "esp_log.h"

// #define LORA_CS   5
// #define LORA_RST  14
// #define LORA_DIO0 26

// static const char *TAG = "LORA_TX";
// static spi_device_handle_t spi_handle;
// static uint8_t sequence_number = 0;

// esp_err_t lora_write_reg(uint8_t reg, uint8_t val) {
//     uint8_t data[] = {reg | 0x80, val};
//     spi_transaction_t t = {
//         .length = 16,
//         .tx_buffer = data,
//     };
//     return spi_device_transmit(spi_handle, &t);
// }

// esp_err_t lora_read_reg(uint8_t reg, uint8_t *val) {
//     uint8_t tx[] = {reg & 0x7F, 0x00};
//     uint8_t rx[2];
//     spi_transaction_t t = {
//         .length = 16,
//         .tx_buffer = tx,
//         .rx_buffer = rx,
//     };
//     esp_err_t err = spi_device_transmit(spi_handle, &t);
//     *val = rx[1];
//     return err;
// }

// void lora_init(void) {
//     gpio_set_direction(LORA_RST, GPIO_MODE_OUTPUT);
//     gpio_set_level(LORA_RST, 0);
//     vTaskDelay(pdMS_TO_TICKS(10));
//     gpio_set_level(LORA_RST, 1);
//     vTaskDelay(pdMS_TO_TICKS(100));

//     lora_write_reg(0x01, 0x00);  // Sleep
//     lora_write_reg(0x01, 0x80);  // LoRa mode

//     uint32_t frf = (uint32_t)(470000000 / 61.03515625);
//     lora_write_reg(0x06, (frf >> 16) & 0xFF);
//     lora_write_reg(0x07, (frf >> 8) & 0xFF);
//     lora_write_reg(0x08, frf & 0xFF);

//     lora_write_reg(0x09, 0x9F);  // ~+17 dBm
//     lora_write_reg(0x1D, 0x72);  // BW=7.8 kHz, CR=4/8
//     lora_write_reg(0x1E, 0xC4);  // SF=10, CRC on
//     lora_write_reg(0x20, 0x00);  // Preamble MSB
//     lora_write_reg(0x21, 0x06);  // Preamble LSB (6 symbols)

//     lora_write_reg(0x0E, 0x80);  // Low Data Rate Optimize
//     lora_write_reg(0x0D, 0x00);  // Reset FIFO

//     lora_write_reg(0x12, 0xFF);  // Clear IRQ flags
//     lora_write_reg(0x01, 0x01);  // Standby
// }

// void lora_send_packet(uint8_t dest_addr, uint8_t *data, uint8_t len) {
//     lora_write_reg(0x0D, 0x80);  // Đặt con trỏ FIFO

//     // Gửi preamble đã được cấu hình
//     lora_write_reg(0x00, dest_addr);  // Chỉ gửi dest_addr, preamble tự động thêm
//     lora_write_reg(0x00, sequence_number);  // Gửi STT

//     // Gửi dữ liệu
//     for (int i = 0; i < len; i++) {
//         lora_write_reg(0x00, data[i]);
//     }

//     lora_write_reg(0x22, len + 2);  // 1 byte dest + 1 byte STT + data
//     lora_write_reg(0x01, 0x83);     // TX mode

//     uint8_t irq = 0;
//     do {
//         vTaskDelay(pdMS_TO_TICKS(10));
//         lora_read_reg(0x12, &irq);
//     } while (!(irq & 0x08));

//     lora_write_reg(0x12, 0xFF);
//     lora_write_reg(0x01, 0x01);

//     sequence_number++;
//     if (sequence_number > 255) sequence_number = 0;
// }

// void app_main(void) {
//     gpio_set_direction(LORA_CS, GPIO_MODE_OUTPUT);

//     spi_bus_config_t bus_cfg = {
//         .miso_io_num = 19,
//         .mosi_io_num = 23,
//         .sclk_io_num = 18,
//         .quadwp_io_num = -1,
//         .quadhd_io_num = -1,
//     };
//     spi_device_interface_config_t dev_cfg = {
//         .clock_speed_hz = 1000000,
//         .mode = 0,
//         .spics_io_num = LORA_CS,
//         .queue_size = 1,
//     };

//     spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH_AUTO);
//     spi_bus_add_device(SPI2_HOST, &dev_cfg, &spi_handle);

//     lora_init();

//     while (1) {
//         uint8_t dest_addr = 0x01;  // Địa chỉ đích (của RX)
//         uint8_t data[] = {'H','E','L','L','O'};
//         ESP_LOGI(TAG, "Sending to Node 0x%02X, STT: %d, Data: %s", dest_addr, sequence_number, (char*)data);
//         lora_send_packet(dest_addr, data, sizeof(data));
//         vTaskDelay(pdMS_TO_TICKS(2000));
//     }
// }


/// test byte cao

// #include <stdio.h>
// #include <stdint.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/gpio.h"
// #include "driver/spi_master.h"
// #include "esp_log.h"

// #define LORA_CS   5
// #define LORA_RST  14
// #define LORA_DIO0 26

// static const char *TAG = "LORA_TX";
// static spi_device_handle_t spi_handle;
// static uint8_t sequence_number = 0;

// esp_err_t lora_write_reg(uint8_t reg, uint8_t val) {
//     uint8_t data[] = {reg | 0x80, val};
//     spi_transaction_t t = {
//         .length = 16,
//         .tx_buffer = data,
//     };
//     return spi_device_transmit(spi_handle, &t);
// }

// esp_err_t lora_read_reg(uint8_t reg, uint8_t *val) {
//     uint8_t tx[] = {reg & 0x7F, 0x00};
//     uint8_t rx[2];
//     spi_transaction_t t = {
//         .length = 16,
//         .tx_buffer = tx,
//         .rx_buffer = rx,
//     };
//     esp_err_t err = spi_device_transmit(spi_handle, &t);
//     *val = rx[1];
//     return err;
// }

// void lora_init(void) {
//     gpio_set_direction(LORA_RST, GPIO_MODE_OUTPUT);
//     gpio_set_level(LORA_RST, 0);
//     vTaskDelay(pdMS_TO_TICKS(10));
//     gpio_set_level(LORA_RST, 1);
//     vTaskDelay(pdMS_TO_TICKS(100));

//     lora_write_reg(0x01, 0x00);  // Sleep
//     lora_write_reg(0x01, 0x80);  // LoRa mode

//     uint32_t frf = (uint32_t)(470000000 / 61.03515625);
//     lora_write_reg(0x06, (frf >> 16) & 0xFF);
//     lora_write_reg(0x07, (frf >> 8) & 0xFF);
//     lora_write_reg(0x08, frf & 0xFF);
//     lora_write_reg(0x09, 0xFF);  // +20 dBm (PA_BOOST, MaxPower=7, OutputPower=15)
//     lora_write_reg(0x1D, 0x80);  // BW=125 kHz, CR=4/5
//     lora_write_reg(0x1E, 0xA4);  // SF=10, CRC on1
//     lora_write_reg(0x20, 0x00);  // Preamble MSB
//     lora_write_reg(0x21, 0x06);  // Preamble LSB (6 symbols)

//     lora_write_reg(0x0E, 0x80);  // Low Data Rate Optimize
//     lora_write_reg(0x0D, 0x00);  // Reset FIFO

//     lora_write_reg(0x12, 0xFF);  // Clear IRQ flags
//     lora_write_reg(0x01, 0x01);  // Standby
// }

// void lora_send_packet(uint8_t dest_addr, uint8_t *data, uint8_t len) {
//     if (len + 2 > 255) {  // Kiểm tra giới hạn tải trọng LoRa
//         ESP_LOGE(TAG, "Dữ liệu quá dài: %d byte", len + 2);
//         return;
//     }

//     lora_write_reg(0x0D, 0x80);  // Đặt con trỏ FIFO

//     lora_write_reg(0x00, dest_addr);  // Địa chỉ đích
//     lora_write_reg(0x00, sequence_number);  // Số thứ tự

//     for (int i = 0; i < len; i++) {
//         lora_write_reg(0x00, data[i]);
//     }

//     lora_write_reg(0x22, len + 2);  // 1 byte đích + 1 byte STT + dữ liệu
//     lora_write_reg(0x01, 0x83);     // Chế độ TX

//     uint8_t irq = 0;
//     do {
//         vTaskDelay(pdMS_TO_TICKS(10));
//         lora_read_reg(0x12, &irq);
//     } while (!(irq & 0x08));  // Chờ TX Done

//     lora_write_reg(0x12, 0xFF);  // Xóa cờ IRQ
//     lora_write_reg(0x01, 0x01);  // Chuyển về Standby

//     sequence_number++;
//     if (sequence_number > 255) sequence_number = 0;
// }

// void app_main(void) {
//     gpio_set_direction(LORA_CS, GPIO_MODE_OUTPUT);

//     spi_bus_config_t bus_cfg = {
//         .miso_io_num = 19,
//         .mosi_io_num = 23,
//         .sclk_io_num = 18,
//         .quadwp_io_num = -1,
//         .quadhd_io_num = -1,
//     };
//     spi_device_interface_config_t dev_cfg = {
//         .clock_speed_hz = 1000000,
//         .mode = 0,
//         .spics_io_num = LORA_CS,
//         .queue_size = 1,
//     };

//     spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH_AUTO);
//     spi_bus_add_device(SPI2_HOST, &dev_cfg, &spi_handle);

//     lora_init();

//     while (1) {
//         uint8_t dest_addr = 0x01;  // Địa chỉ đích (của RX)
//         const char *poem = "Bèo dạt về đâu, hàng nối hàng; Hiu hiu gió thổi, chiều tà tà. Trôi mãi.";
//         uint8_t data[102];
//         memcpy(data, poem, 102);  // Sao chép 102 byte của câu thơ
//         ESP_LOGI(TAG, "Gửi đến Node 0x%02X, STT: %d, Dữ liệu: %s", dest_addr, sequence_number, (char*)data);
//         lora_send_packet(dest_addr, data, 102);
//         vTaskDelay(pdMS_TO_TICKS(2000));
//     }
// }




// test truyen nhan 2 lora


// #include <stdio.h>
// #include <stdint.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/gpio.h"
// #include "driver/spi_master.h"
// #include "esp_log.h"

// #define LORA_CS   5
// #define LORA_RST  14
// #define LORA_DIO0 26
// #define LOCAL_ADDR 0x01  // Địa chỉ của TX
// #define ACK_TIMEOUT pdMS_TO_TICKS(5000)  // Thời gian chờ ACK: 5s

// static const char *TAG = "LORA_TX";
// static spi_device_handle_t spi_handle;
// static uint8_t sequence_number = 0;

// esp_err_t lora_write_reg(uint8_t reg, uint8_t val) {
//     uint8_t data[] = {reg | 0x80, val};
//     spi_transaction_t t = {
//         .length = 16,
//         .tx_buffer = data,
//     };
//     return spi_device_transmit(spi_handle, &t);
// }

// esp_err_t lora_read_reg(uint8_t reg, uint8_t *val) {
//     uint8_t tx[] = {reg & 0x7F, 0x00};
//     uint8_t rx[2];
//     spi_transaction_t t = {
//         .length = 16,
//         .tx_buffer = tx,
//         .rx_buffer = rx,
//     };
//     esp_err_t err = spi_device_transmit(spi_handle, &t);
//     *val = rx[1];
//     return err;
// }

// void lora_init(void) {
//     gpio_set_direction(LORA_RST, GPIO_MODE_OUTPUT);
//     gpio_set_level(LORA_RST, 0);
//     vTaskDelay(pdMS_TO_TICKS(10));
//     gpio_set_level(LORA_RST, 1);
//     vTaskDelay(pdMS_TO_TICKS(100));

//     lora_write_reg(0x01, 0x00);  // Sleep
//     lora_write_reg(0x01, 0x80);  // LoRa mode

//     uint32_t frf = (uint32_t)(470000000 / 61.03515625);
//     lora_write_reg(0x06, (frf >> 16) & 0xFF);
//     lora_write_reg(0x07, (frf >> 8) & 0xFF);
//     lora_write_reg(0x08, frf & 0xFF);
//     lora_write_reg(0x09, 0xFF);  // +20 dBm (PA_BOOST, MaxPower=7, OutputPower=15)
//     lora_write_reg(0x1D, 0x80);  // BW=125 kHz, CR=4/5
//     lora_write_reg(0x1E, 0xA4);  // SF=10, CRC on
//     lora_write_reg(0x20, 0x00);  // Preamble MSB
//     lora_write_reg(0x21, 0x06);  // Preamble LSB (6 symbols)

//     lora_write_reg(0x0E, 0x80);  // Low Data Rate Optimize
//     lora_write_reg(0x0D, 0x00);  // Reset FIFO

//     lora_write_reg(0x12, 0xFF);  // Clear IRQ flags
//     lora_write_reg(0x01, 0x01);  // Standby
// }

// void lora_send_packet(uint8_t dest_addr, uint8_t *data, uint8_t len) {
//     if (len + 2 > 255) {  // Kiểm tra giới hạn tải trọng LoRa
//         ESP_LOGE(TAG, "Dữ liệu quá dài: %d byte", len + 2);
//         return;
//     }

//     lora_write_reg(0x0D, 0x80);  // Đặt con trỏ FIFO

//     lora_write_reg(0x00, dest_addr);  // Địa chỉ đích
//     lora_write_reg(0x00, sequence_number);  // Số thứ tự

//     for (int i = 0; i < len; i++) {
//         lora_write_reg(0x00, data[i]);
//     }

//     lora_write_reg(0x22, len + 2);  // 1 byte đích + 1 byte STT + dữ liệu
//     lora_write_reg(0x01, 0x83);     // Chế độ TX

//     uint8_t irq = 0;
//     do {
//         vTaskDelay(pdMS_TO_TICKS(10));
//         lora_read_reg(0x12, &irq);
//     } while (!(irq & 0x08));  // Chờ TX Done

//     lora_write_reg(0x12, 0xFF);  // Xóa cờ IRQ
//     lora_write_reg(0x01, 0x01);  // Chuyển về Standby

//     sequence_number++;
//     if (sequence_number > 255) sequence_number = 0;
// }

// bool lora_receive_ack(uint8_t expected_seq_num) {
//     lora_write_reg(0x0D, 0x00);  // Đặt con trỏ FIFO về đầu
//     lora_write_reg(0x01, 0x85);  // Chế độ RX

//     uint8_t irq = 0;
//     TickType_t start_time = xTaskGetTickCount();
//     do {
//         vTaskDelay(pdMS_TO_TICKS(10));
//         lora_read_reg(0x12, &irq);
//         if (xTaskGetTickCount() - start_time > ACK_TIMEOUT) {
//             ESP_LOGW(TAG, "Hết thời gian chờ ACK cho STT: %d", expected_seq_num);
//             lora_write_reg(0x12, 0xFF);  // Xóa cờ IRQ
//             lora_write_reg(0x01, 0x01);  // Chuyển về Standby
//             return false;
//         }
//     } while (!(irq & 0x40));  // Chờ RX Done (bit 6)

//     uint8_t len;
//     lora_read_reg(0x13, &len);  // Đọc độ dài từ Reg 0x13 (RxNbBytes)
//     if (len < 2 || len > 128) {  // Ít nhất dest + STT, tối đa 128 byte
//         ESP_LOGW(TAG, "Độ dài ACK không hợp lệ: %d", len);
//         lora_write_reg(0x12, 0xFF);  // Xóa cờ IRQ
//         lora_write_reg(0x01, 0x01);  // Chuyển về Standby
//         return false;
//     }

//     uint8_t data[128];
//     lora_write_reg(0x0D, 0x00);  // Đặt lại con trỏ FIFO
//     for (int i = 0; i < len; i++) {
//         lora_read_reg(0x00, &data[i]);
//     }

//     uint8_t dest_addr = data[0];  // Địa chỉ đích (phải là LOCAL_ADDR)
//     uint8_t seq_num = data[1];    // Số thứ tự
//     if (dest_addr != LOCAL_ADDR || seq_num != expected_seq_num) {
//         ESP_LOGW(TAG, "ACK không hợp lệ: dest=0x%02X (kỳ vọng 0x%02X), STT=%d (kỳ vọng %d)",
//                  dest_addr, LOCAL_ADDR, seq_num, expected_seq_num);
//         lora_write_reg(0x12, 0xFF);  // Xóa cờ IRQ
//         lora_write_reg(0x01, 0x01);  // Chuyển về Standby
//         return false;
//     }

//     if (len - 2 >= 3) {  // Kiểm tra nếu có dữ liệu "ACK"
//         char received_data[len - 2 + 1];
//         memcpy(received_data, &data[2], len - 2);
//         received_data[len - 2] = '\0';
//         ESP_LOGI(TAG, "Nhận ACK từ Node 0x%02X, STT: %d, Dữ liệu: %s", dest_addr, seq_num, received_data);
//     } else {
//         ESP_LOGW(TAG, "ACK không có dữ liệu tải trọng");
//     }

//     lora_write_reg(0x12, 0xFF);  // Xóa cờ IRQ
//     lora_write_reg(0x01, 0x01);  // Chuyển về Standby
//     return true;
// }

// void app_main(void) {
//     gpio_set_direction(LORA_CS, GPIO_MODE_OUTPUT);

//     spi_bus_config_t bus_cfg = {
//         .miso_io_num = 19,
//         .mosi_io_num = 23,
//         .sclk_io_num = 18,
//         .quadwp_io_num = -1,
//         .quadhd_io_num = -1,
//     };
//     spi_device_interface_config_t dev_cfg = {
//         .clock_speed_hz = 1000000,
//         .mode = 0,
//         .spics_io_num = LORA_CS,
//         .queue_size = 1,
//     };

//     spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH_AUTO);
//     spi_bus_add_device(SPI2_HOST, &dev_cfg, &spi_handle);

//     lora_init();

//     while (1) {
//         uint8_t dest_addr = 0x02;  // Địa chỉ đích (của RX)
//         const char *poem = "Bèo dạt về đâu, hàng nối hàng; Hiu hiu gió thổi, chiều tà tà. Trôi mãi.";
//         uint8_t data[102];
//         memcpy(data, poem, 102);  // Sao chép 102 byte của câu thơ
//         uint8_t current_seq = sequence_number;
//         ESP_LOGI(TAG, "Gửi đến Node 0x%02X, STT: %d, Dữ liệu: %s", dest_addr, current_seq, (char*)data);
//         lora_send_packet(dest_addr, data, 102);

//         // Chờ ACK
//         if (lora_receive_ack(current_seq)) {
//             ESP_LOGI(TAG, "Nhận được ACK cho gói tin STT: %d", current_seq);
//         } else {
//             ESP_LOGE(TAG, "Không nhận được ACK cho gói tin STT: %d", current_seq);
//         }

//         vTaskDelay(pdMS_TO_TICKS(2000));
//     }
// }



/// code test toan ven du lieu



// #include <stdio.h>
// #include <stdint.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/gpio.h"
// #include "driver/spi_master.h"
// #include "esp_log.h"

// #define LORA_CS   5
// #define LORA_RST  14
// #define LORA_DIO0 26
// #define LOCAL_ADDR 0x01  // Địa chỉ của TX
// #define ACK_TIMEOUT pdMS_TO_TICKS(5000)  // Giảm xuống 5s

// static const char *TAG = "LORA_TX";
// static spi_device_handle_t spi_handle;
// static uint8_t sequence_number = 0;

// esp_err_t lora_write_reg(uint8_t reg, uint8_t val) {
//     uint8_t data[] = {reg | 0x80, val};
//     spi_transaction_t t = {
//         .length = 16,
//         .tx_buffer = data,
//     };
//     return spi_device_transmit(spi_handle, &t);
// }

// esp_err_t lora_read_reg(uint8_t reg, uint8_t *val) {
//     uint8_t tx[] = {reg & 0x7F, 0x00};
//     uint8_t rx[2];
//     spi_transaction_t t = {
//         .length = 16,
//         .tx_buffer = tx,
//         .rx_buffer = rx,
//     };
//     esp_err_t err = spi_device_transmit(spi_handle, &t);
//     *val = rx[1];
//     return err;
// }

// void lora_init(void) {
//     gpio_set_direction(LORA_RST, GPIO_MODE_OUTPUT);
//     gpio_set_level(LORA_RST, 0);
//     vTaskDelay(pdMS_TO_TICKS(10));
//     gpio_set_level(LORA_RST, 1);
//     vTaskDelay(pdMS_TO_TICKS(100));

//     lora_write_reg(0x01, 0x00);  // Sleep
//     lora_write_reg(0x01, 0x80);  // LoRa mode

//     uint32_t frf = (uint32_t)(470000000 / 61.03515625);
//     lora_write_reg(0x06, (frf >> 16) & 0xFF);
//     lora_write_reg(0x07, (frf >> 8) & 0xFF);
//     lora_write_reg(0x08, frf & 0xFF);
//     lora_write_reg(0x09, 0xFF);  // +20 dBm (PA_BOOST, MaxPower=7, OutputPower=15)
//     lora_write_reg(0x1D, 0x80);  // BW=125 kHz, CR=4/5
//     lora_write_reg(0x1E, 0xA4);  // SF=10, CRC on
//     lora_write_reg(0x20, 0x00);  // Preamble MSB
//     lora_write_reg(0x21, 0x06);  // Preamble LSB (6 symbols)

//     lora_write_reg(0x0E, 0x80);  // Low Data Rate Optimize
//     lora_write_reg(0x0D, 0x00);  // Reset FIFO

//     lora_write_reg(0x12, 0xFF);  // Clear IRQ flags
//     lora_write_reg(0x01, 0x01);  // Standby
// }

// void lora_send_packet(uint8_t dest_addr, uint8_t *data, uint8_t len) {
//     if (len + 3 > 255) {  // Kiểm tra giới hạn tải trọng LoRa (bao gồm checksum)
//         ESP_LOGE(TAG, "Dữ liệu quá dài: %d byte", len + 3);
//         return;
//     }

//     // Tính checksum (tổng các byte dữ liệu)
//     uint8_t checksum = 0;
//     for (int i = 0; i < len; i++) {
//         checksum += data[i];
//         ESP_LOGI(TAG, "Byte %d: %d", i, data[i]);  // Debug từng byte
//     }
//     ESP_LOGI(TAG, "Checksum tính: %d", checksum);  // Debug checksum

//     lora_write_reg(0x0D, 0x80);  // Đặt con trỏ FIFO

//     lora_write_reg(0x00, dest_addr);    // Địa chỉ đích
//     lora_write_reg(0x00, sequence_number); // Số thứ tự
//     for (int i = 0; i < len; i++) {
//         lora_write_reg(0x00, data[i]);
//     }
//     lora_write_reg(0x00, checksum);     // Gửi checksum

//     lora_write_reg(0x22, len + 3);  // 1 byte đích + 1 byte STT + dữ liệu + 1 byte checksum
//     lora_write_reg(0x01, 0x83);     // Chế độ TX

//     uint8_t irq = 0;
//     do {
//         vTaskDelay(pdMS_TO_TICKS(10));
//         lora_read_reg(0x12, &irq);
//     } while (!(irq & 0x08));  // Chờ TX Done

//     lora_write_reg(0x12, 0xFF);  // Xóa cờ IRQ
//     lora_write_reg(0x01, 0x01);  // Chuyển về Standby

//     sequence_number++;
//     if (sequence_number > 255) sequence_number = 0;
// }

// bool lora_receive_ack(uint8_t expected_seq_num) {
//     lora_write_reg(0x0D, 0x00);  // Đặt con trỏ FIFO về đầu
//     lora_write_reg(0x01, 0x85);  // Chế độ RX

//     uint8_t irq = 0;
//     TickType_t start_time = xTaskGetTickCount();
//     do {
//         vTaskDelay(pdMS_TO_TICKS(10));
//         lora_read_reg(0x12, &irq);
//         if (xTaskGetTickCount() - start_time > ACK_TIMEOUT) {
//             ESP_LOGW(TAG, "Hết thời gian chờ ACK cho STT: %d", expected_seq_num);
//             lora_write_reg(0x12, 0xFF);  // Xóa cờ IRQ
//             lora_write_reg(0x01, 0x01);  // Chuyển về Standby
//             return false;
//         }
//     } while (!(irq & 0x40));  // Chờ RX Done (bit 6)

//     uint8_t len;
//     lora_read_reg(0x13, &len);  // Đọc độ dài từ Reg 0x13 (RxNbBytes)
//     if (len < 2 || len > 128) {
//         ESP_LOGW(TAG, "Độ dài ACK không hợp lệ: %d", len);
//         lora_write_reg(0x12, 0xFF);  // Xóa cờ IRQ
//         lora_write_reg(0x01, 0x01);  // Chuyển về Standby
//         return false;
//     }

//     uint8_t data[128];
//     lora_write_reg(0x0D, 0x00);  // Đặt lại con trỏ FIFO
//     for (int i = 0; i < len; i++) {
//         lora_read_reg(0x00, &data[i]);
//     }

//     uint8_t dest_addr = data[0];
//     uint8_t seq_num = data[1];
//     if (dest_addr != LOCAL_ADDR || seq_num != expected_seq_num) {
//         ESP_LOGW(TAG, "ACK không hợp lệ: dest=0x%02X (kỳ vọng 0x%02X), STT=%d (kỳ vọng %d)",
//                  dest_addr, LOCAL_ADDR, seq_num, expected_seq_num);
//         lora_write_reg(0x12, 0xFF);  // Xóa cờ IRQ
//         lora_write_reg(0x01, 0x01);  // Chuyển về Standby
//         return false;
//     }

//     if (len - 2 >= 3) {
//         char received_data[len - 2 + 1];
//         memcpy(received_data, &data[2], len - 2);
//         received_data[len - 2] = '\0';
//         ESP_LOGI(TAG, "Nhận ACK từ Node 0x%02X, STT: %d, Dữ liệu: %s", dest_addr, seq_num, received_data);
//     } else {
//         ESP_LOGW(TAG, "ACK không có dữ liệu tải trọng");
//     }

//     lora_write_reg(0x12, 0xFF);  // Xóa cờ IRQ
//     lora_write_reg(0x01, 0x01);  // Chuyển về Standby
//     return true;
// }

// void app_main(void) {
//     gpio_set_direction(LORA_CS, GPIO_MODE_OUTPUT);

//     spi_bus_config_t bus_cfg = {
//         .miso_io_num = 19,
//         .mosi_io_num = 23,
//         .sclk_io_num = 18,
//         .quadwp_io_num = -1,
//         .quadhd_io_num = -1,
//     };
//     spi_device_interface_config_t dev_cfg = {
//         .clock_speed_hz = 1000000,
//         .mode = 0,
//         .spics_io_num = LORA_CS,
//         .queue_size = 1,
//     };

//     spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH_AUTO);
//     spi_bus_add_device(SPI2_HOST, &dev_cfg, &spi_handle);

//     lora_init();

//     while (1) {
//         uint8_t dest_addr = 0x02;  // Địa chỉ đích (của RX)
//         const char *poem = "Bèo dạt về đâu, hàng nối hàng; Hiu hiu gió thổi, chiều tà tà. Trôi mãi.";
//         uint8_t data[102];
//         size_t len = strlen(poem);
//         memcpy(data, poem, len);
//         uint8_t current_seq = sequence_number;
//         ESP_LOGI(TAG, "Gửi đến Node 0x%02X, STT: %d, Dữ liệu: %.*s", dest_addr, current_seq, len, (char*)data);
//         lora_send_packet(dest_addr, data, len);

//         if (lora_receive_ack(current_seq)) {
//             ESP_LOGI(TAG, "Nhận được ACK cho gói tin STT: %d", current_seq);
//         } else {
//             ESP_LOGE(TAG, "Không nhận được ACK cho gói tin STT: %d", current_seq);
//         }

//         vTaskDelay(pdMS_TO_TICKS(2000));
//     }
// }



// /// code test RSSI

// #include <stdio.h>
// #include <stdint.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/gpio.h"
// #include "driver/spi_master.h"
// #include "esp_log.h"
// #include "RadioLib.h"  // Thêm thư viện RadioLib

// #define LORA_CS   5
// #define LORA_RST  14
// #define LORA_DIO0 26
// #define LOCAL_ADDR 0x01  // Địa chỉ của TX
// #define ACK_TIMEOUT pdMS_TO_TICKS(5000)

// static const char *TAG = "LORA_TX";
// static spi_device_handle_t spi_handle;
// static uint8_t sequence_number = 0;

// Module* mod;
// SX1278* radio;

// esp_err_t lora_write_reg(uint8_t reg, uint8_t val) {
//     uint8_t data[] = {reg | 0x80, val};
//     spi_transaction_t t = {
//         .length = 16,
//         .tx_buffer = data,
//     };
//     return spi_device_transmit(spi_handle, &t);
// }

// esp_err_t lora_read_reg(uint8_t reg, uint8_t *val) {
//     uint8_t tx[] = {reg & 0x7F, 0x00};
//     uint8_t rx[2];
//     spi_transaction_t t = {
//         .length = 16,
//         .tx_buffer = tx,
//         .rx_buffer = rx,
//     };
//     esp_err_t err = spi_device_transmit(spi_handle, &t);
//     *val = rx[1];
//     return err;
// }

// void lora_init(void) {
//     gpio_set_direction(LORA_RST, GPIO_MODE_OUTPUT);
//     gpio_set_level(LORA_RST, 0);
//     vTaskDelay(pdMS_TO_TICKS(10));
//     gpio_set_level(LORA_RST, 1);
//     vTaskDelay(pdMS_TO_TICKS(100));

//     // Khởi tạo RadioLib với SX1278
//     mod = new Module(LORA_CS, LORA_DIO0, 18, 23);  // CS, DIO0, SCK, MOSI
//     radio = new SX1278(*mod);
//     int state = radio->begin(470.0, 125.0, 10, 7, 20, 8);  // Freq, BW, SF, CR, Pwr, PRlen
//     if (state != RADIOLIB_ERR_NONE) {
//         ESP_LOGE(TAG, "Khởi tạo LoRa thất bại, mã lỗi: %d", state);
//         while (true) vTaskDelay(1);
//     }
//     ESP_LOGI(TAG, "LoRa khởi tạo thành công!");

//     // Cấu hình thủ công (tương thích SX1278)
//     lora_write_reg(0x01, 0x00);  // Sleep
//     lora_write_reg(0x01, 0x80);  // LoRa mode
//     uint32_t frf = (uint32_t)(470000000 / 61.03515625);
//     lora_write_reg(0x06, (frf >> 16) & 0xFF);
//     lora_write_reg(0x07, (frf >> 8) & 0xFF);
//     lora_write_reg(0x08, frf & 0xFF);
//     lora_write_reg(0x09, 0xFF);  // +20 dBm (PA_BOOST, MaxPower=7, OutputPower=15)
//     lora_write_reg(0x1D, 0x80);  // BW=125 kHz, CR=4/5
//     lora_write_reg(0x1E, 0xA4);  // SF=10, CRC on
//     lora_write_reg(0x20, 0x00);  // Preamble MSB
//     lora_write_reg(0x21, 0x06);  // Preamble LSB (6 symbols)
//     lora_write_reg(0x0E, 0x80);  // Low Data Rate Optimize
//     lora_write_reg(0x0D, 0x00);  // Reset FIFO
//     lora_write_reg(0x12, 0xFF);  // Clear IRQ flags
//     lora_write_reg(0x01, 0x01);  // Standby
// }

// void lora_send_packet(uint8_t dest_addr, uint8_t *data, uint8_t len) {
//     if (len + 3 > 255) {
//         ESP_LOGE(TAG, "Dữ liệu quá dài: %d byte", len + 3);
//         return;
//     }

//     uint8_t checksum = 0;
//     for (int i = 0; i < len; i++) {
//         checksum += data[i];
//         ESP_LOGI(TAG, "Byte %d: %d", i, data[i]);
//     }
//     ESP_LOGI(TAG, "Checksum tính: %d", checksum);

//     lora_write_reg(0x0D, 0x80);
//     lora_write_reg(0x00, dest_addr);
//     lora_write_reg(0x00, sequence_number);
//     for (int i = 0; i < len; i++) {
//         lora_write_reg(0x00, data[i]);
//     }
//     lora_write_reg(0x00, checksum);

//     lora_write_reg(0x22, len + 3);
//     lora_write_reg(0x01, 0x83);

//     uint8_t irq = 0;
//     do {
//         vTaskDelay(pdMS_TO_TICKS(10));
//         lora_read_reg(0x12, &irq);
//     } while (!(irq & 0x08));

//     lora_write_reg(0x12, 0xFF);
//     lora_write_reg(0x01, 0x01);

//     sequence_number++;
//     if (sequence_number > 255) sequence_number = 0;
// }

// bool lora_receive_ack(uint8_t expected_seq_num) {
//     lora_write_reg(0x0D, 0x00);
//     lora_write_reg(0x01, 0x85);

//     uint8_t irq = 0;
//     TickType_t start_time = xTaskGetTickCount();
//     do {
//         vTaskDelay(pdMS_TO_TICKS(10));
//         lora_read_reg(0x12, &irq);
//         if (xTaskGetTickCount() - start_time > ACK_TIMEOUT) {
//             ESP_LOGW(TAG, "Hết thời gian chờ ACK cho STT: %d", expected_seq_num);
//             lora_write_reg(0x12, 0xFF);
//             lora_write_reg(0x01, 0x01);
//             return false;
//         }
//     } while (!(irq & 0x40));

//     uint8_t len;
//     lora_read_reg(0x13, &len);
//     if (len < 2 || len > 128) {
//         ESP_LOGW(TAG, "Độ dài ACK không hợp lệ: %d", len);
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     uint8_t data[128];
//     lora_write_reg(0x0D, 0x00);
//     for (int i = 0; i < len; i++) {
//         lora_read_reg(0x00, &data[i]);
//     }

//     uint8_t dest_addr = data[0];
//     uint8_t seq_num = data[1];
//     if (dest_addr != LOCAL_ADDR || seq_num != expected_seq_num) {
//         ESP_LOGW(TAG, "ACK không hợp lệ: dest=0x%02X (kỳ vọng 0x%02X), STT=%d (kỳ vọng %d)",
//                  dest_addr, LOCAL_ADDR, seq_num, expected_seq_num);
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     if (len - 2 >= 3) {
//         char received_data[len - 2 + 1];
//         memcpy(received_data, &data[2], len - 2);
//         received_data[len - 2] = '\0';
//         int rssi = radio->getRSSI();  // Đọc RSSI từ ACK
//         ESP_LOGI(TAG, "Nhận ACK từ Node 0x%02X, STT: %d, Dữ liệu: %s, RSSI: %d dBm",
//                  dest_addr, seq_num, received_data, rssi);
//     } else {
//         ESP_LOGW(TAG, "ACK không có dữ liệu tải trọng");
//     }

//     lora_write_reg(0x12, 0xFF);
//     lora_write_reg(0x01, 0x01);
//     return true;
// }

// void app_main(void) {
//     gpio_set_direction(LORA_CS, GPIO_MODE_OUTPUT);

//     spi_bus_config_t bus_cfg = {
//         .miso_io_num = 19,
//         .mosi_io_num = 23,
//         .sclk_io_num = 18,
//         .quadwp_io_num = -1,
//         .quadhd_io_num = -1,
//     };
//     spi_device_interface_config_t dev_cfg = {
//         .clock_speed_hz = 1000000,
//         .mode = 0,
//         .spics_io_num = LORA_CS,
//         .queue_size = 1,
//     };

//     spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH_AUTO);
//     spi_bus_add_device(SPI2_HOST, &dev_cfg, &spi_handle);

//     lora_init();

//     while (1) {
//         uint8_t dest_addr = 0x02;
//         const char *poem = "Bèo dạt về đâu, hàng nối hàng; Hiu hiu gió thổi, chiều tà tà. Trôi mãi.";
//         uint8_t data[102];
//         size_t len = strlen(poem);
//         memcpy(data, poem, len);
//         uint8_t current_seq = sequence_number;
//         ESP_LOGI(TAG, "Gửi đến Node 0x%02X, STT: %d, Dữ liệu: %.*s", dest_addr, current_seq, len, (char*)data);
//         lora_send_packet(dest_addr, data, len);

//         if (lora_receive_ack(current_seq)) {
//             ESP_LOGI(TAG, "Nhận được ACK cho gói tin STT: %d", current_seq);
//         } else {
//             ESP_LOGE(TAG, "Không nhận được ACK cho gói tin STT: %d", current_seq);
//         }

//         vTaskDelay(pdMS_TO_TICKS(2000));
//     }
// }





/// code TX cos giao tiep 2 chieu, chua co led timeout


// #include <stdio.h>
// #include <stdint.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/gpio.h"
// #include "driver/spi_master.h"
// #include "esp_log.h"

// #define LORA_CS   5
// #define LORA_RST  14
// #define LORA_DIO0 26
// #define LED_PIN   2
// #define MY_ADDRESS 0x01
// #define DEST_ADDRESS 0x02

// static const char *TAG = "LORA_BIDIRECTIONAL";
// static spi_device_handle_t spi_handle;
// static uint8_t sequence_number = 0;

// esp_err_t lora_write_reg(uint8_t reg, uint8_t val) {
//     uint8_t data[] = {reg | 0x80, val};
//     spi_transaction_t t = {
//         .length = 16,
//         .tx_buffer = data,
//     };
//     return spi_device_transmit(spi_handle, &t);
// }

// esp_err_t lora_read_reg(uint8_t reg, uint8_t *val) {
//     uint8_t tx[] = {reg & 0x7F, 0x00};
//     uint8_t rx[2];
//     spi_transaction_t t = {
//         .length = 16,
//         .tx_buffer = tx,
//         .rx_buffer = rx,
//     };
//     esp_err_t err = spi_device_transmit(spi_handle, &t);
//     *val = rx[1];
//     return err;
// }

// void lora_init(void) {
//     gpio_set_direction(LORA_RST, GPIO_MODE_OUTPUT);
//     gpio_set_level(LORA_RST, 0);
//     vTaskDelay(pdMS_TO_TICKS(10));
//     gpio_set_level(LORA_RST, 1);
//     vTaskDelay(pdMS_TO_TICKS(100));

//     lora_write_reg(0x01, 0x00);  // Ngủ
//     lora_write_reg(0x01, 0x80);  // Chế độ LoRa

//     uint32_t frf = (uint32_t)(470000000 / 61.03515625);
//     lora_write_reg(0x06, (frf >> 16) & 0xFF);
//     lora_write_reg(0x07, (frf >> 8) & 0xFF);
//     lora_write_reg(0x08, frf & 0xFF);

//     lora_write_reg(0x09, 0xFF);  // Công suất +20 dBm
//     lora_write_reg(0x1D, 0x80);  // BW=125 kHz, CR=4/5
//     lora_write_reg(0x1E, 0xA4);  // SF=10, CRC bật
//     lora_write_reg(0x20, 0x00);  // Preamble MSB
//     lora_write_reg(0x21, 0x06);  // Preamble LSB (6 ký hiệu)

//     lora_write_reg(0x0E, 0x80);  // Tối ưu hóa tốc độ dữ liệu thấp
//     lora_write_reg(0x0D, 0x00);  // Reset FIFO
//     lora_write_reg(0x12, 0xFF);  // Xóa cờ IRQ
//     lora_write_reg(0x01, 0x01);  // Chuyển sang Standby
// }

// uint8_t calculate_checksum(uint8_t *data, uint8_t len) {
//     uint8_t checksum = 0;
//     for (int i = 0; i < len; i++) {
//         checksum ^= data[i];
//     }
//     return checksum;
// }

// void toggle_led(int times, int delay_ms) {
//     for (int i = 0; i < times; i++) {
//         gpio_set_level(LED_PIN, 1);
//         vTaskDelay(pdMS_TO_TICKS(delay_ms));
//         gpio_set_level(LED_PIN, 0);
//         vTaskDelay(pdMS_TO_TICKS(delay_ms));
//     }
// }

// void lora_send_packet(uint8_t dest_addr, uint8_t *data, uint8_t len) {
//     if (len + 4 > 255) {  // +4 vì địa chỉ đích, nguồn, số thứ tự, checksum
//         ESP_LOGE(TAG, "Dữ liệu quá dài: %d byte", len + 4);
//         return;
//     }

//     uint8_t checksum = calculate_checksum(data, len);

//     lora_write_reg(0x0D, 0x80);  // Đặt con trỏ FIFO
//     lora_write_reg(0x00, dest_addr);  // Địa chỉ đích
//     lora_write_reg(0x00, MY_ADDRESS); // Địa chỉ nguồn
//     lora_write_reg(0x00, sequence_number);  // Số thứ tự
//     for (int i = 0; i < len; i++) {
//         lora_write_reg(0x00, data[i]);
//     }
//     lora_write_reg(0x00, checksum);  // Thêm checksum

//     lora_write_reg(0x22, len + 4);  // Độ dài gói tin
//     lora_write_reg(0x01, 0x83);     // Chế độ TX

//     uint8_t irq = 0;
//     do {
//         vTaskDelay(pdMS_TO_TICKS(10));
//         lora_read_reg(0x12, &irq);
//     } while (!(irq & 0x08));  // Chờ TX Done

//     lora_write_reg(0x12, 0xFF);  // Xóa cờ IRQ
//     lora_write_reg(0x01, 0x01);  // Chuyển về Standby

//     ESP_LOGI(TAG, "Gửi đến Node 0x%02X, STT: %d, Dữ liệu: %s, Checksum: 0x%02X", dest_addr, sequence_number, (char*)data, checksum);
//     sequence_number++;
// }

// bool lora_receive_packet(uint8_t *data, uint8_t *len, uint8_t *dest_addr, uint8_t *src_addr, uint8_t *seq_num) {
//     lora_write_reg(0x0D, 0x00);  // Đặt con trỏ FIFO về đầu
//     lora_write_reg(0x01, 0x85);  // Chế độ RX

//     uint8_t irq = 0;
//     int timeout = 500;  // Timeout 5 giây
//     do {
//         vTaskDelay(pdMS_TO_TICKS(10));
//         lora_read_reg(0x12, &irq);
//         timeout--;
//     } while (!(irq & 0x40) && timeout > 0);

//     if (!(irq & 0x40)) {
//         ESP_LOGW(TAG, "Timeout khi chờ nhận dữ liệu");
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     if (irq & 0x20) {
//         ESP_LOGW(TAG, "Lỗi CRC trong gói tin");
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     lora_read_reg(0x13, len);
//     if (*len < 4 || *len > 128) {  // Cần ít nhất 4 byte (địa chỉ đích, nguồn, số thứ tự, checksum)
//         ESP_LOGW(TAG, "Độ dài không hợp lệ: %d", *len);
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     uint8_t rssi, snr;
//     lora_read_reg(0x1A, &rssi);
//     lora_read_reg(0x19, &snr);
//     ESP_LOGI(TAG, "RSSI: -%d dBm, SNR: %d dB", (rssi >> 1), (int8_t)snr / 4);

//     lora_write_reg(0x0D, 0x00);
//     for (int i = 0; i < *len; i++) {
//         lora_read_reg(0x00, &data[i]);
//     }

//     *dest_addr = data[0];
//     *src_addr = data[1];
//     *seq_num = data[2];
//     uint8_t received_checksum = data[*len - 1];
//     uint8_t calculated_checksum = calculate_checksum(&data[3], *len - 4);

//     if (*src_addr == MY_ADDRESS) {
//         ESP_LOGW(TAG, "Bỏ qua gói tin từ chính thiết bị này");
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     if (*dest_addr != MY_ADDRESS) {
//         ESP_LOGW(TAG, "Gói tin không dành cho thiết bị này (dest: 0x%02X)", *dest_addr);
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     if (received_checksum != calculated_checksum) {
//         ESP_LOGW(TAG, "Checksum sai: Nhận 0x%02X, Tính toán 0x%02X", received_checksum, calculated_checksum);
//         toggle_led(3, 200);
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         vTaskDelay(pdMS_TO_TICKS(3000));
//         return false;
//     }

//     toggle_led(1, 500);
//     lora_write_reg(0x12, 0xFF);
//     lora_write_reg(0x01, 0x01);
//     return true;
// }

// void app_main(void) {
//     gpio_set_direction(LORA_CS, GPIO_MODE_OUTPUT);
//     gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
//     gpio_set_level(LED_PIN, 0);

//     spi_bus_config_t bus_cfg = {
//         .miso_io_num = 19,
//         .mosi_io_num = 23,
//         .sclk_io_num = 18,
//         .quadwp_io_num = -1,
//         .quadhd_io_num = -1,
//     };
//     spi_device_interface_config_t dev_cfg = {
//         .clock_speed_hz = 1000000,
//         .mode = 0,
//         .spics_io_num = LORA_CS,
//         .queue_size = 1,
//     };

//     esp_err_t ret = spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH_AUTO);
//     if (ret != ESP_OK) {
//         ESP_LOGE(TAG, "Khởi tạo SPI bus thất bại: %s", esp_err_to_name(ret));
//         return;
//     }
//     ret = spi_bus_add_device(SPI2_HOST, &dev_cfg, &spi_handle);
//     if (ret != ESP_OK) {
//         ESP_LOGE(TAG, "Thêm thiết bị SPI thất bại: %s", esp_err_to_name(ret));
//         return;
//     }

//     lora_init();

//     while (1) {
//         if (MY_ADDRESS == 0x01) {
//             uint8_t dest_addr = DEST_ADDRESS;
//             const char *message = "Xin chào từ TX: Bèo dạt mây trôi, chốn xa xôi. Em ơi, anh vẫn đợi...";
//             uint8_t data[128];
//             memset(data, 0, sizeof(data));
//             uint8_t len = strlen(message);
//             memcpy(data, message, len);
//             lora_send_packet(dest_addr, data, len);

//             uint8_t received_data[128];
//             uint8_t received_len, received_dest, received_src, received_seq;
//             if (lora_receive_packet(received_data, &received_len, &received_dest, &received_src, &received_seq)) {
//                 char payload[received_len - 4 + 1];
//                 memcpy(payload, &received_data[3], received_len - 4);
//                 payload[received_len - 4] = '\0';
//                 ESP_LOGI(TAG, "Nhận phản hồi từ Node 0x%02X, STT: %d, Dữ liệu: %s", 
//                          received_src, received_seq, payload);
//             }
//             vTaskDelay(pdMS_TO_TICKS(3000));
//         }
//     }
// }


/// code led timeout


// #include <stdio.h>
// #include <stdint.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/gpio.h"
// #include "driver/spi_master.h"
// #include "esp_log.h"

// #define LORA_CS   5
// #define LORA_RST  14
// #define LORA_DIO0 26
// #define LED_SUCCESS_PIN 2  // GPIO2 cho nhận thành công hoặc checksum sai
// #define LED_TIMEOUT_PIN 4  // GPIO4 cho timeout
// #define MY_ADDRESS 0x01
// #define DEST_ADDRESS 0x02

// static const char *TAG = "LORA_BIDIRECTIONAL";
// static spi_device_handle_t spi_handle;
// static uint8_t sequence_number = 0;

// esp_err_t lora_write_reg(uint8_t reg, uint8_t val) {
//     uint8_t data[] = {reg | 0x80, val};
//     spi_transaction_t t = {
//         .length = 16,
//         .tx_buffer = data,
//     };
//     return spi_device_transmit(spi_handle, &t);
// }

// esp_err_t lora_read_reg(uint8_t reg, uint8_t *val) {
//     uint8_t tx[] = {reg & 0x7F, 0x00};
//     uint8_t rx[2];
//     spi_transaction_t t = {
//         .length = 16,
//         .tx_buffer = tx,
//         .rx_buffer = rx,
//     };
//     esp_err_t err = spi_device_transmit(spi_handle, &t);
//     *val = rx[1];
//     return err;
// }

// void lora_init(void) {
//     gpio_set_direction(LORA_RST, GPIO_MODE_OUTPUT);
//     gpio_set_level(LORA_RST, 0);
//     vTaskDelay(pdMS_TO_TICKS(10));
//     gpio_set_level(LORA_RST, 1);
//     vTaskDelay(pdMS_TO_TICKS(100));

//     lora_write_reg(0x01, 0x00);  // Ngủ
//     lora_write_reg(0x01, 0x80);  // Chế độ LoRa

//     uint32_t frf = (uint32_t)(470000000 / 61.03515625);
//     lora_write_reg(0x06, (frf >> 16) & 0xFF);
//     lora_write_reg(0x07, (frf >> 8) & 0xFF);
//     lora_write_reg(0x08, frf & 0xFF);

//     lora_write_reg(0x09, 0xFF);  // Công suất +20 dBm
//     lora_write_reg(0x1D, 0x80);  // BW=125 kHz, CR=4/5
//     lora_write_reg(0x1E, 0xA4);  // SF=10, CRC bật
//     lora_write_reg(0x20, 0x00);  // Preamble MSB
//     lora_write_reg(0x21, 0x06);  // Preamble LSB (6 ký hiệu)

//     lora_write_reg(0x0E, 0x80);  // Tối ưu hóa tốc độ dữ liệu thấp
//     lora_write_reg(0x0D, 0x00);  // Reset FIFO
//     lora_write_reg(0x12, 0xFF);  // Xóa cờ IRQ
//     lora_write_reg(0x01, 0x01);  // Chuyển sang Standby
// }

// uint8_t calculate_checksum(uint8_t *data, uint8_t len) {
//     uint8_t checksum = 0;
//     for (int i = 0; i < len; i++) {
//         checksum ^= data[i];
//     }
//     return checksum;
// }

// void toggle_led(gpio_num_t pin, int times, int delay_ms) {
//     for (int i = 0; i < times; i++) {
//         gpio_set_level(pin, 1);
//         vTaskDelay(pdMS_TO_TICKS(delay_ms));
//         gpio_set_level(pin, 0);
//         vTaskDelay(pdMS_TO_TICKS(delay_ms));
//     }
// }

// void lora_send_packet(uint8_t dest_addr, uint8_t *data, uint8_t len) {
//     if (len + 4 > 255) {
//         ESP_LOGE(TAG, "Dữ liệu quá dài: %d byte", len + 4);
//         return;
//     }

//     uint8_t checksum = calculate_checksum(data, len);

//     lora_write_reg(0x0D, 0x80);
//     lora_write_reg(0x00, dest_addr);
//     lora_write_reg(0x00, MY_ADDRESS);
//     lora_write_reg(0x00, sequence_number);
//     for (int i = 0; i < len; i++) {
//         lora_write_reg(0x00, data[i]);
//     }
//     lora_write_reg(0x00, checksum);

//     lora_write_reg(0x22, len + 4);
//     lora_write_reg(0x01, 0x83);

//     uint8_t irq = 0;
//     do {
//         vTaskDelay(pdMS_TO_TICKS(10));
//         lora_read_reg(0x12, &irq);
//     } while (!(irq & 0x08));

//     lora_write_reg(0x12, 0xFF);
//     lora_write_reg(0x01, 0x01);

//     toggle_led(LED_SUCCESS_PIN, 1, 500);  // Chớp LED GPIO2 khi gửi
//     ESP_LOGI(TAG, "Gửi đến Node 0x%02X, STT: %d, Dữ liệu: %s, Checksum: 0x%02X", dest_addr, sequence_number, (char*)data, checksum);
//     sequence_number++;
// }

// bool lora_receive_packet(uint8_t *data, uint8_t *len, uint8_t *dest_addr, uint8_t *src_addr, uint8_t *seq_num) {
//     lora_write_reg(0x0D, 0x00);
//     lora_write_reg(0x01, 0x85);

//     uint8_t irq = 0;
//     int timeout = 500;  // 5 giây
//     do {
//         vTaskDelay(pdMS_TO_TICKS(10));
//         lora_read_reg(0x12, &irq);
//         timeout--;
//     } while (!(irq & 0x40) && timeout > 0);

//     if (!(irq & 0x40)) {
//         ESP_LOGW(TAG, "Timeout khi chờ nhận dữ liệu");
//         toggle_led(LED_TIMEOUT_PIN, 3, 200);  // Chớp LED GPIO4 3 lần khi timeout
//         ESP_LOGI(TAG, "LED GPIO4 chớp 3 lần do timeout");
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     if (irq & 0x20) {
//         ESP_LOGW(TAG, "Lỗi CRC trong gói tin");
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     lora_read_reg(0x13, len);
//     if (*len < 4 || *len > 128) {
//         ESP_LOGW(TAG, "Độ dài không hợp lệ: %d", *len);
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     uint8_t rssi, snr;
//     lora_read_reg(0x1A, &rssi);
//     lora_read_reg(0x19, &snr);
//     ESP_LOGI(TAG, "RSSI: -%d dBm, SNR: %d dB", (rssi >> 1), (int8_t)snr / 4);

//     lora_write_reg(0x0D, 0x00);
//     for (int i = 0; i < *len; i++) {
//         lora_read_reg(0x00, &data[i]);
//     }

//     *dest_addr = data[0];
//     *src_addr = data[1];
//     *seq_num = data[2];
//     uint8_t received_checksum = data[*len - 1];
//     uint8_t calculated_checksum = calculate_checksum(&data[3], *len - 4);

//     if (*src_addr == MY_ADDRESS) {
//         ESP_LOGW(TAG, "Bỏ qua gói tin từ chính thiết bị này");
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     if (*dest_addr != MY_ADDRESS) {
//         ESP_LOGW(TAG, "Gói tin không dành cho thiết bị này (dest: 0x%02X)", *dest_addr);
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     if (received_checksum != calculated_checksum) {
//         ESP_LOGW(TAG, "Checksum sai: Nhận 0x%02X, Tính toán 0x%02X", received_checksum, calculated_checksum);
//         toggle_led(LED_SUCCESS_PIN, 3, 200);  // Chớp LED GPIO2 3 lần khi checksum sai
//         ESP_LOGI(TAG, "LED GPIO2 chớp 3 lần do checksum sai");
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         vTaskDelay(pdMS_TO_TICKS(3000));
//         return false;
//     }

//     toggle_led(LED_SUCCESS_PIN, 1, 500);  // Chớp LED GPIO2 khi nhận thành công
//     ESP_LOGI(TAG, "LED GPIO2 chớp 1 lần do nhận thành công");
//     lora_write_reg(0x12, 0xFF);
//     lora_write_reg(0x01, 0x01);
//     return true;
// }

// void app_main(void) {
//     gpio_set_direction(LORA_CS, GPIO_MODE_OUTPUT);
//     gpio_set_direction(LED_SUCCESS_PIN, GPIO_MODE_OUTPUT);
//     gpio_set_direction(LED_TIMEOUT_PIN, GPIO_MODE_OUTPUT);
//     gpio_set_level(LED_SUCCESS_PIN, 0);
//     gpio_set_level(LED_TIMEOUT_PIN, 0);

//     toggle_led(LED_SUCCESS_PIN, 5, 500);  // Kiểm tra LED GPIO2 lúc khởi động
//     toggle_led(LED_TIMEOUT_PIN, 5, 500);  // Kiểm tra LED GPIO4 lúc khởi động

//     spi_bus_config_t bus_cfg = {
//         .miso_io_num = 19,
//         .mosi_io_num = 23,
//         .sclk_io_num = 18,
//         .quadwp_io_num = -1,
//         .quadhd_io_num = -1,
//     };
//     spi_device_interface_config_t dev_cfg = {
//         .clock_speed_hz = 1000000,
//         .mode = 0,
//         .spics_io_num = LORA_CS,
//         .queue_size = 1,
//     };

//     esp_err_t ret = spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH_AUTO);
//     if (ret != ESP_OK) {
//         ESP_LOGE(TAG, "Khởi tạo SPI bus thất bại: %s", esp_err_to_name(ret));
//         return;
//     }
//     ret = spi_bus_add_device(SPI2_HOST, &dev_cfg, &spi_handle);
//     if (ret != ESP_OK) {
//         ESP_LOGE(TAG, "Thêm thiết bị SPI thất bại: %s", esp_err_to_name(ret));
//         return;
//     }

//     lora_init();

//     while (1) {
//         if (MY_ADDRESS == 0x01) {
//             uint8_t dest_addr = DEST_ADDRESS;
//             const char *message = "Xin chào từ TX: Bèo dạt mây trôi, chốn xa xôi. Em ơi, anh vẫn đợi...";
//             uint8_t data[128];
//             memset(data, 0, sizeof(data));
//             uint8_t len = strlen(message);
//             memcpy(data, message, len);
//             lora_send_packet(dest_addr, data, len);

//             uint8_t received_data[128];
//             uint8_t received_len, received_dest, received_src, received_seq;
//             if (lora_receive_packet(received_data, &received_len, &received_dest, &received_src, &received_seq)) {
//                 char payload[received_len - 4 + 1];
//                 memcpy(payload, &received_data[3], received_len - 4);
//                 payload[received_len - 4] = '\0';
//                 ESP_LOGI(TAG, "Nhận phản hồi từ Node 0x%02X, STT: %d, Dữ liệu: %s", 
//                          received_src, received_seq, payload);
//             }
//             vTaskDelay(pdMS_TO_TICKS(3000));
//         }
//     }
// }



// code checksum loi

// #include <stdio.h>
// #include <stdint.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/gpio.h"
// #include "driver/spi_master.h"
// #include "esp_log.h"

// #define LORA_CS   5
// #define LORA_RST  14
// #define LORA_DIO0 26
// #define LED_SUCCESS_PIN 2  // GPIO2 cho nhận thành công, checksum sai, gửi
// #define LED_TIMEOUT_PIN 4  // GPIO4 cho timeout
// #define MY_ADDRESS 0x01
// #define DEST_ADDRESS 0x02
// #define FORCE_CHECKSUM_ERROR 1  // Bật (1) để gây lỗi checksum, tắt (0) để bình thường

// static const char *TAG = "LORA_BIDIRECTIONAL";
// static spi_device_handle_t spi_handle;
// static uint8_t sequence_number = 0;

// esp_err_t lora_write_reg(uint8_t reg, uint8_t val) {
//     uint8_t data[] = {reg | 0x80, val};
//     spi_transaction_t t = {
//         .length = 16,
//         .tx_buffer = data,
//     };
//     return spi_device_transmit(spi_handle, &t);
// }

// esp_err_t lora_read_reg(uint8_t reg, uint8_t *val) {
//     uint8_t tx[] = {reg & 0x7F, 0x00};
//     uint8_t rx[2];
//     spi_transaction_t t = {
//         .length = 16,
//         .tx_buffer = tx,
//         .rx_buffer = rx,
//     };
//     esp_err_t err = spi_device_transmit(spi_handle, &t);
//     *val = rx[1];
//     return err;
// }

// void lora_init(void) {
//     gpio_set_direction(LORA_RST, GPIO_MODE_OUTPUT);
//     gpio_set_level(LORA_RST, 0);
//     vTaskDelay(pdMS_TO_TICKS(10));
//     gpio_set_level(LORA_RST, 1);
//     vTaskDelay(pdMS_TO_TICKS(100));

//     lora_write_reg(0x01, 0x00);  // Ngủ
//     lora_write_reg(0x01, 0x80);  // Chế độ LoRa

//     uint32_t frf = (uint32_t)(470000000 / 61.03515625);
//     lora_write_reg(0x06, (frf >> 16) & 0xFF);
//     lora_write_reg(0x07, (frf >> 8) & 0xFF);
//     lora_write_reg(0x08, frf & 0xFF);

//     lora_write_reg(0x09, 0xFF);  // Công suất +20 dBm
//     lora_write_reg(0x1D, 0x80);  // BW=125 kHz, CR=4/5
//     lora_write_reg(0x1E, 0xA4);  // SF=10, CRC bật
//     lora_write_reg(0x20, 0x00);  // Preamble MSB
//     lora_write_reg(0x21, 0x06);  // Preamble LSB (6 ký hiệu)

//     lora_write_reg(0x0E, 0x80);  // Tối ưu hóa tốc độ dữ liệu thấp
//     lora_write_reg(0x0D, 0x00);  // Reset FIFO
//     lora_write_reg(0x12, 0xFF);  // Xóa cờ IRQ
//     lora_write_reg(0x01, 0x01);  // Chuyển sang Standby
// }

// uint8_t calculate_checksum(uint8_t *data, uint8_t len) {
//     uint8_t checksum = 0;
//     for (int i = 0; i < len; i++) {
//         checksum ^= data[i];
//     }
//     return checksum;
// }

// void toggle_led(gpio_num_t pin, int times, int delay_ms) {
//     for (int i = 0; i < times; i++) {
//         gpio_set_level(pin, 1);
//         vTaskDelay(pdMS_TO_TICKS(delay_ms));
//         gpio_set_level(pin, 0);
//         vTaskDelay(pdMS_TO_TICKS(delay_ms));
//     }
// }

// void lora_send_packet(uint8_t dest_addr, uint8_t *data, uint8_t len) {
//     if (len + 4 > 255) {
//         ESP_LOGE(TAG, "Dữ liệu quá dài: %d byte", len + 4);
//         return;
//     }

//     uint8_t checksum = calculate_checksum(data, len);
//     #if FORCE_CHECKSUM_ERROR
//         checksum = checksum + 1;  // Gây lỗi checksum
//         ESP_LOGI(TAG, "Gây lỗi checksum: Gửi 0x%02X (thay vì 0x%02X)", checksum, calculate_checksum(data, len));
//     #endif

//     lora_write_reg(0x0D, 0x80);
//     lora_write_reg(0x00, dest_addr);
//     lora_write_reg(0x00, MY_ADDRESS);
//     lora_write_reg(0x00, sequence_number);
//     for (int i = 0; i < len; i++) {
//         lora_write_reg(0x00, data[i]);
//     }
//     lora_write_reg(0x00, checksum);

//     lora_write_reg(0x22, len + 4);
//     lora_write_reg(0x01, 0x83);

//     uint8_t irq = 0;
//     do {
//         vTaskDelay(pdMS_TO_TICKS(10));
//         lora_read_reg(0x12, &irq);
//     } while (!(irq & 0x08));

//     lora_write_reg(0x12, 0xFF);
//     lora_write_reg(0x01, 0x01);

//     toggle_led(LED_SUCCESS_PIN, 1, 500);  // Chớp LED GPIO2 khi gửi
//     ESP_LOGI(TAG, "Gửi đến Node 0x%02X, STT: %d, Dữ liệu: %s, Checksum: 0x%02X", dest_addr, sequence_number, (char*)data, checksum);
//     sequence_number++;
// }

// bool lora_receive_packet(uint8_t *data, uint8_t *len, uint8_t *dest_addr, uint8_t *src_addr, uint8_t *seq_num) {
//     lora_write_reg(0x0D, 0x00);
//     lora_write_reg(0x01, 0x85);

//     uint8_t irq = 0;
//     int timeout = 500;  // 5 giây
//     do {
//         vTaskDelay(pdMS_TO_TICKS(10));
//         lora_read_reg(0x12, &irq);
//         timeout--;
//     } while (!(irq & 0x40) && timeout > 0);

//     if (!(irq & 0x40)) {
//         ESP_LOGW(TAG, "Timeout khi chờ nhận dữ liệu");
//         toggle_led(LED_TIMEOUT_PIN, 3, 200);  // Chớp LED GPIO4 3 lần khi timeout
//         ESP_LOGI(TAG, "LED GPIO4 chớp 3 lần do timeout");
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     if (irq & 0x20) {
//         ESP_LOGW(TAG, "Lỗi CRC trong gói tin");
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     lora_read_reg(0x13, len);
//     if (*len < 4 || *len > 128) {
//         ESP_LOGW(TAG, "Độ dài không hợp lệ: %d", *len);
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     uint8_t rssi, snr;
//     lora_read_reg(0x1A, &rssi);
//     lora_read_reg(0x19, &snr);
//     ESP_LOGI(TAG, "RSSI: -%d dBm, SNR: %d dB", (rssi >> 1), (int8_t)snr / 4);

//     lora_write_reg(0x0D, 0x00);
//     for (int i = 0; i < *len; i++) {
//         lora_read_reg(0x00, &data[i]);
//     }

//     *dest_addr = data[0];
//     *src_addr = data[1];
//     *seq_num = data[2];
//     uint8_t received_checksum = data[*len - 1];
//     uint8_t calculated_checksum = calculate_checksum(&data[3], *len - 4);

//     if (*src_addr == MY_ADDRESS) {
//         ESP_LOGW(TAG, "Bỏ qua gói tin từ chính thiết bị này");
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     if (*dest_addr != MY_ADDRESS) {
//         ESP_LOGW(TAG, "Gói tin không dành cho thiết bị này (dest: 0x%02X)", *dest_addr);
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     if (received_checksum != calculated_checksum) {
//         ESP_LOGW(TAG, "Checksum sai: Nhận 0x%02X, Tính toán 0x%02X", received_checksum, calculated_checksum);
//         toggle_led(LED_SUCCESS_PIN, 3, 200);  // Chớp LED GPIO2 3 lần khi checksum sai
//         ESP_LOGI(TAG, "LED GPIO2 chớp 3 lần do checksum sai");
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         vTaskDelay(pdMS_TO_TICKS(3000));
//         return false;
//     }

//     toggle_led(LED_SUCCESS_PIN, 1, 500);  // Chớp LED GPIO2 khi nhận thành công
//     ESP_LOGI(TAG, "LED GPIO2 chớp 1 lần do nhận thành công");
//     lora_write_reg(0x12, 0xFF);
//     lora_write_reg(0x01, 0x01);
//     return true;
// }

// void app_main(void) {
//     gpio_set_direction(LORA_CS, GPIO_MODE_OUTPUT);
//     gpio_set_direction(LED_SUCCESS_PIN, GPIO_MODE_OUTPUT);
//     gpio_set_direction(LED_TIMEOUT_PIN, GPIO_MODE_OUTPUT);
//     gpio_set_level(LED_SUCCESS_PIN, 0);
//     gpio_set_level(LED_TIMEOUT_PIN, 0);

//     toggle_led(LED_SUCCESS_PIN, 5, 500);  // Kiểm tra LED GPIO2 lúc khởi động
//     toggle_led(LED_TIMEOUT_PIN, 5, 500);  // Kiểm tra LED GPIO4 lúc khởi động

//     spi_bus_config_t bus_cfg = {
//         .miso_io_num = 19,
//         .mosi_io_num = 23,
//         .sclk_io_num = 18,
//         .quadwp_io_num = -1,
//         .quadhd_io_num = -1,
//     };
//     spi_device_interface_config_t dev_cfg = {
//         .clock_speed_hz = 1000000,
//         .mode = 0,
//         .spics_io_num = LORA_CS,
//         .queue_size = 1,
//     };

//     esp_err_t ret = spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH_AUTO);
//     if (ret != ESP_OK) {
//         ESP_LOGE(TAG, "Khởi tạo SPI bus thất bại: %s", esp_err_to_name(ret));
//         return;
//     }
//     ret = spi_bus_add_device(SPI2_HOST, &dev_cfg, &spi_handle);
//     if (ret != ESP_OK) {
//         ESP_LOGE(TAG, "Thêm thiết bị SPI thất bại: %s", esp_err_to_name(ret));
//         return;
//     }

//     lora_init();

//     while (1) {
//         if (MY_ADDRESS == 0x01) {
//             uint8_t dest_addr = DEST_ADDRESS;
//             const char *message = "Xin chào từ TX: Bèo dạt mây trôi, chốn xa xôi. Em ơi, anh vẫn đợi...";
//             uint8_t data[128];
//             memset(data, 0, sizeof(data));
//             uint8_t len = strlen(message);
//             memcpy(data, message, len);
//             lora_send_packet(dest_addr, data, len);

//             uint8_t received_data[128];
//             uint8_t received_len, received_dest, received_src, received_seq;
//             if (lora_receive_packet(received_data, &received_len, &received_dest, &received_src, &received_seq)) {
//                 char payload[received_len - 4 + 1];
//                 memcpy(payload, &received_data[3], received_len - 4);
//                 payload[received_len - 4] = '\0';
//                 ESP_LOGI(TAG, "Nhận phản hồi từ Node 0x%02X, STT: %d, Dữ liệu: %s", 
//                          received_src, received_seq, payload);
//             }
//             vTaskDelay(pdMS_TO_TICKS(3000));
//         }
//     }
// }




/// code 3 node, node 1#include <stdio.h>


// #include <stdio.h>
// #include <stdint.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/gpio.h"
// #include "driver/spi_master.h"
// #include "esp_log.h"

// #define LORA_CS   5
// #define LORA_RST  14
// #define LORA_DIO0 26
// #define LED_SUCCESS_PIN 2
// #define LED_TIMEOUT_PIN 4
// #define MY_ADDRESS 0x01
// #define DEST_ADDRESS 0x02

// static const char *TAG = "LORA_NODE1";
// static spi_device_handle_t spi_handle;
// static uint8_t sequence_number = 0;

// esp_err_t lora_write_reg(uint8_t reg, uint8_t val) {
//     uint8_t data[] = {reg | 0x80, val};
//     spi_transaction_t t = {
//         .length = 16,
//         .tx_buffer = data,
//     };
//     return spi_device_transmit(spi_handle, &t);
// }

// esp_err_t lora_read_reg(uint8_t reg, uint8_t *val) {
//     uint8_t tx[] = {reg & 0x7F, 0x00};
//     uint8_t rx[2];
//     spi_transaction_t t = {
//         .length = 16,
//         .tx_buffer = tx,
//         .rx_buffer = rx,
//     };
//     esp_err_t err = spi_device_transmit(spi_handle, &t);
//     *val = rx[1];
//     return err;
// }

// bool lora_init(void) {
//     gpio_set_direction(LORA_RST, GPIO_MODE_OUTPUT);
//     gpio_set_level(LORA_RST, 0);
//     vTaskDelay(pdMS_TO_TICKS(10));
//     gpio_set_level(LORA_RST, 1);
//     vTaskDelay(pdMS_TO_TICKS(100));

//     uint8_t version;
//     lora_read_reg(0x42, &version);
//     if (version != 0x12) {
//         ESP_LOGE(TAG, "Không phát hiện module LoRa SX1278, version: 0x%02X", version);
//         return false;
//     }

//     lora_write_reg(0x01, 0x00);
//     lora_write_reg(0x01, 0x80);
//     uint32_t frf = (uint32_t)(470000000 / 61.03515625);
//     lora_write_reg(0x06, (frf >> 16) & 0xFF);
//     lora_write_reg(0x07, (frf >> 8) & 0xFF);
//     lora_write_reg(0x08, frf & 0xFF);
//     lora_write_reg(0x09, 0xFF);
//     lora_write_reg(0x1D, 0x80);
//     lora_write_reg(0x1E, 0xA4);
//     lora_write_reg(0x20, 0x00);
//     lora_write_reg(0x21, 0x0C);  // Preamble = 12
//     lora_write_reg(0x0E, 0x80);
//     lora_write_reg(0x0D, 0x00);
//     lora_write_reg(0x12, 0xFF);
//     lora_write_reg(0x01, 0x01);

//     uint8_t mode;
//     lora_read_reg(0x01, &mode);
//     if ((mode & 0x80) != 0x80) {
//         ESP_LOGE(TAG, "Không chuyển sang chế độ LoRa, mode: 0x%02X", mode);
//         return false;
//     }
//     ESP_LOGI(TAG, "Khởi tạo LoRa thành công: 470 MHz, SF=10, BW=125 kHz, CR=4/5");
//     return true;
// }

// uint8_t calculate_checksum(uint8_t *data, uint8_t len) {
//     uint8_t checksum = 0;
//     for (int i = 0; i < len; i++) {
//         checksum ^= data[i];
//     }
//     return checksum;
// }

// void toggle_led(gpio_num_t pin, int times, int delay_ms) {
//     for (int i = 0; i < times; i++) {
//         gpio_set_level(pin, 1);
//         vTaskDelay(pdMS_TO_TICKS(delay_ms));
//         gpio_set_level(pin, 0);
//         vTaskDelay(pdMS_TO_TICKS(delay_ms));
//     }
// }

// void lora_send_packet(uint8_t dest_addr, uint8_t *data, uint8_t len) {
//     if (len + 4 > 255) {
//         ESP_LOGE(TAG, "Dữ liệu quá dài: %d byte", len + 4);
//         return;
//     }

//     uint8_t checksum = calculate_checksum(data, len);

//     lora_write_reg(0x0D, 0x80);
//     lora_write_reg(0x00, dest_addr);
//     lora_write_reg(0x00, MY_ADDRESS);
//     lora_write_reg(0x00, sequence_number);
//     for (int i = 0; i < len; i++) {
//         lora_write_reg(0x00, data[i]);
//     }
//     lora_write_reg(0x00, checksum);

//     lora_write_reg(0x22, len + 4);
//     lora_write_reg(0x01, 0x83);

//     uint8_t irq = 0;
//     int timeout = 800;  // 15 giây
//     do {
//         vTaskDelay(pdMS_TO_TICKS(10));
//         lora_read_reg(0x12, &irq);
//         timeout--;
//     } while (!(irq & 0x08) && timeout > 0);

//     if (!(irq & 0x08)) {
//         ESP_LOGW(TAG, "Timeout khi gửi gói tin");
//         toggle_led(LED_TIMEOUT_PIN, 3, 200);
//         return;
//     }

//     lora_write_reg(0x12, 0xFF);
//     lora_write_reg(0x01, 0x01);

//     toggle_led(LED_SUCCESS_PIN, 1, 500);
//     ESP_LOGI(TAG, "Gửi đến Node 0x%02X, STT: %d, Dữ liệu: %s, Checksum: 0x%02X", 
//              dest_addr, sequence_number, (char*)data, checksum);
//     sequence_number++;
// }

// bool lora_receive_packet(uint8_t *data, uint8_t *len, uint8_t *dest_addr, uint8_t *src_addr, uint8_t *seq_num) {
//     lora_write_reg(0x0D, 0x00);
//     lora_write_reg(0x01, 0x85);

//     uint8_t irq = 0;
//     int timeout = 800;  // 15 giây
//     do {
//         vTaskDelay(pdMS_TO_TICKS(10));
//         lora_read_reg(0x12, &irq);
//         timeout--;
//     } while (!(irq & 0x40) && timeout > 0);

//     if (!(irq & 0x40)) {
//         ESP_LOGW(TAG, "Timeout khi chờ nhận dữ liệu");
//         toggle_led(LED_TIMEOUT_PIN, 3, 200);
//         return false;
//     }

//     if (irq & 0x20) {
//         ESP_LOGW(TAG, "Lỗi CRC trong gói tin");
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     lora_read_reg(0x13, len);
//     if (*len < 4 || *len > 128) {
//         ESP_LOGW(TAG, "Độ dài không hợp lệ: %d", *len);
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     uint8_t rssi, snr;
//     lora_read_reg(0x1A, &rssi);
//     lora_read_reg(0x19, &snr);
//     ESP_LOGI(TAG, "RSSI: -%d dBm, SNR: %d dB", (rssi >> 1), (int8_t)snr / 4);

//     lora_write_reg(0x0D, 0x00);
//     for (int i = 0; i < *len; i++) {
//         lora_read_reg(0x00, &data[i]);
//     }

//     *dest_addr = data[0];
//     *src_addr = data[1];
//     *seq_num = data[2];
//     uint8_t received_checksum = data[*len - 1];
//     uint8_t calculated_checksum = calculate_checksum(&data[3], *len - 4);

//     if (*src_addr == MY_ADDRESS) {
//         ESP_LOGW(TAG, "Bỏ qua gói tin từ chính thiết bị này");
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     if (*dest_addr != MY_ADDRESS) {
//         ESP_LOGW(TAG, "Gói tin không dành cho thiết bị này (dest: 0x%02X)", *dest_addr);
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     if (received_checksum != calculated_checksum) {
//         ESP_LOGW(TAG, "Checksum sai: Nhận 0x%02X, Tính toán 0x%02X", 
//                  received_checksum, calculated_checksum);
//         toggle_led(LED_SUCCESS_PIN, 3, 200);
//         return false;
//     }

//     toggle_led(LED_SUCCESS_PIN, 1, 500);
//     ESP_LOGI(TAG, "LED GPIO2 chớp 1 lần do nhận thành công");
//     lora_write_reg(0x12, 0xFF);
//     lora_write_reg(0x01, 0x01);
//     return true;
// }

// void app_main(void) {
//     gpio_set_direction(LORA_CS, GPIO_MODE_OUTPUT);
//     gpio_set_direction(LED_SUCCESS_PIN, GPIO_MODE_OUTPUT);
//     gpio_set_direction(LED_TIMEOUT_PIN, GPIO_MODE_OUTPUT);
//     gpio_set_level(LED_SUCCESS_PIN, 0);
//     gpio_set_level(LED_TIMEOUT_PIN, 0);

//     toggle_led(LED_SUCCESS_PIN, 5, 500);
//     toggle_led(LED_TIMEOUT_PIN, 5, 500);

//     spi_bus_config_t bus_cfg = {
//         .miso_io_num = 19,
//         .mosi_io_num = 23,
//         .sclk_io_num = 18,
//         .quadwp_io_num = -1,
//         .quadhd_io_num = -1,
//     };
//     spi_device_interface_config_t dev_cfg = {
//         .clock_speed_hz = 1000000,
//         .mode = 0,
//         .spics_io_num = LORA_CS,
//         .queue_size = 1,
//     };

//     esp_err_t ret = spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH_AUTO);
//     if (ret != ESP_OK) {
//         ESP_LOGE(TAG, "Khởi tạo SPI bus thất bại: %s", esp_err_to_name(ret));
//         toggle_led(LED_TIMEOUT_PIN, 3, 200);
//         while (1) vTaskDelay(pdMS_TO_TICKS(1000));
//     }
//     ret = spi_bus_add_device(SPI2_HOST, &dev_cfg, &spi_handle);
//     if (ret != ESP_OK) {
//         ESP_LOGE(TAG, "Thêm thiết bị SPI thất bại: %s", esp_err_to_name(ret));
//         toggle_led(LED_TIMEOUT_PIN, 3, 200);
//         while (1) vTaskDelay(pdMS_TO_TICKS(1000));
//     }

//     if (!lora_init()) {
//         ESP_LOGE(TAG, "Khởi tạo LoRa thất bại");
//         toggle_led(LED_TIMEOUT_PIN, 3, 200);
//         while (1) vTaskDelay(pdMS_TO_TICKS(1000));
//     }

//     const char *message = "Bèo dạt mây trôi, chốn xa xôi. Em ơi, anh vẫn đợi...";
//     uint8_t data[128];
//     uint8_t len = strlen(message);

//     while (1) {
//         memset(data, 0, sizeof(data));
//         memcpy(data, message, len);
//         uint8_t dest_addr = DEST_ADDRESS;

//         int max_retries = 3;
//         bool success = false;
//         for (int retry = 0; retry < max_retries; retry++) {
//             lora_send_packet(dest_addr, data, len);
//             uint8_t received_data[128];
//             uint8_t received_len, received_dest, received_src, received_seq;
//             if (lora_receive_packet(received_data, &received_len, &received_dest, &received_src, &received_seq)) {
//                 if (received_src == 0x02) {
//                     char payload[received_len - 4 + 1];
//                     memcpy(payload, &received_data[3], received_len - 4);
//                     payload[received_len - 4] = '\0';
//                     ESP_LOGI(TAG, "Nhận từ Node 0x%02X, STT: %d, Dữ liệu: %s, Dữ liệu bảo toàn: %s", 
//                              received_src, received_seq, payload, 
//                              strcmp(payload, message) == 0 ? "Có" : "Không");
//                     success = true;
//                     break;
//                 } else {
//                     ESP_LOGW(TAG, "Nhận từ Node không mong đợi: 0x%02X", received_src);
//                 }
//             }
//             //ESP_LOGW(TAG, "Retry %d/%d khi chờ phản hồi từ Node 0x02", retry + 1, max_retries);
//             vTaskDelay(pdMS_TO_TICKS(500));
//         }
//         if (!success) {
//             ESP_LOGW(TAG, "Hết lượt thử, không nhận được phản hồi từ Node 0x02");
//         }
//         vTaskDelay(pdMS_TO_TICKS(3000));  // Chờ 3s trước khi gửi gói tin mới
//     }
// }




/// code 3 node, 100 byte du lieu




// #include <stdio.h>
// #include <stdint.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/gpio.h"
// #include "driver/spi_master.h"
// #include "esp_log.h"

// #define LORA_CS   5
// #define LORA_RST  14
// #define LORA_DIO0 26
// #define LED_SUCCESS_PIN 2
// #define LED_TIMEOUT_PIN 4
// #define MY_ADDRESS 0x01
// #define DEST_ADDRESS 0x02

// static const char *TAG = "LORA_NODE1";
// static spi_device_handle_t spi_handle;
// static uint8_t sequence_number = 0;

// esp_err_t lora_write_reg(uint8_t reg, uint8_t val) {
//     uint8_t data[] = {reg | 0x80, val};
//     spi_transaction_t t = {
//         .length = 16,
//         .tx_buffer = data,
//     };
//     return spi_device_transmit(spi_handle, &t);
// }

// esp_err_t lora_read_reg(uint8_t reg, uint8_t *val) {
//     uint8_t tx[] = {reg & 0x7F, 0x00};
//     uint8_t rx[2];
//     spi_transaction_t t = {
//         .length = 16,
//         .tx_buffer = tx,
//         .rx_buffer = rx,
//     };
//     esp_err_t err = spi_device_transmit(spi_handle, &t);
//     *val = rx[1];
//     return err;
// }

// bool lora_init(void) {
//     gpio_set_direction(LORA_RST, GPIO_MODE_OUTPUT);
//     gpio_set_level(LORA_RST, 0);
//     vTaskDelay(pdMS_TO_TICKS(10));
//     gpio_set_level(LORA_RST, 1);
//     vTaskDelay(pdMS_TO_TICKS(100));

//     uint8_t version;
//     lora_read_reg(0x42, &version);
//     if (version != 0x12) {
//         ESP_LOGE(TAG, "Không phát hiện module LoRa SX1278, version: 0x%02X", version);
//         return false;
//     }

//     lora_write_reg(0x01, 0x00);
//     lora_write_reg(0x01, 0x80);
//     uint32_t frf = (uint32_t)(470000000 / 61.03515625);
//     lora_write_reg(0x06, (frf >> 16) & 0xFF);
//     lora_write_reg(0x07, (frf >> 8) & 0xFF);
//     lora_write_reg(0x08, frf & 0xFF);
//     lora_write_reg(0x09, 0xFF);  // Công suất tối đa
//     lora_write_reg(0x1D, 0x8C);  // BW=500 kHz
//     lora_write_reg(0x1E, 0x74);  // SF=7, CRC bật
//     lora_write_reg(0x20, 0x00);
//     lora_write_reg(0x21, 0x08);  // Preamble = 8
//     lora_write_reg(0x0E, 0x80);
//     lora_write_reg(0x0D, 0x00);
//     lora_write_reg(0x12, 0xFF);
//     lora_write_reg(0x01, 0x01);

//     uint8_t mode;
//     lora_read_reg(0x01, &mode);
//     if ((mode & 0x80) != 0x80) {
//         ESP_LOGE(TAG, "Không chuyển sang chế độ LoRa, mode: 0x%02X", mode);
//         return false;
//     }
//     ESP_LOGI(TAG, "Khởi tạo LoRa thành công: 470 MHz, SF=7, BW=500 kHz, CR=4/5");
//     return true;
// }

// uint8_t calculate_checksum(uint8_t *data, uint8_t len) {
//     uint8_t checksum = 0;
//     for (int i = 0; i < len; i++) {
//         checksum ^= data[i];
//     }
//     return checksum;
// }

// void toggle_led(gpio_num_t pin, int times, int delay_ms) {
//     for (int i = 0; i < times; i++) {
//         gpio_set_level(pin, 1);
//         vTaskDelay(pdMS_TO_TICKS(delay_ms));
//         gpio_set_level(pin, 0);
//         vTaskDelay(pdMS_TO_TICKS(delay_ms));
//     }
// }

// void lora_send_packet(uint8_t dest_addr, uint8_t *data, uint8_t len) {
//     if (len + 4 > 255) {
//         ESP_LOGE(TAG, "Dữ liệu quá dài: %d byte", len + 4);
//         return;
//     }

//     uint8_t checksum = calculate_checksum(data, len);

//     lora_write_reg(0x0D, 0x80);
//     lora_write_reg(0x00, dest_addr);
//     lora_write_reg(0x00, MY_ADDRESS);
//     lora_write_reg(0x00, sequence_number);
//     for (int i = 0; i < len; i++) {
//         lora_write_reg(0x00, data[i]);
//     }
//     lora_write_reg(0x00, checksum);

//     lora_write_reg(0x22, len + 4);
//     lora_write_reg(0x01, 0x83);

//     uint8_t irq = 0;
//     int timeout = 400;  // 2 giây
//     do {
//         vTaskDelay(pdMS_TO_TICKS(10));
//         lora_read_reg(0x12, &irq);
//         timeout--;
//     } while (!(irq & 0x08) && timeout > 0);

//     if (!(irq & 0x08)) {
//         ESP_LOGW(TAG, "Timeout khi gửi gói tin");
//         toggle_led(LED_TIMEOUT_PIN, 3, 200);
//         return;
//     }

//     lora_write_reg(0x12, 0xFF);
//     lora_write_reg(0x01, 0x01);

//     toggle_led(LED_SUCCESS_PIN, 1, 500);
//     ESP_LOGI(TAG, "Gửi đến Node 0x%02X, STT: %d, Dữ liệu: %s, Checksum: 0x%02X", 
//              dest_addr, sequence_number, (char*)data, checksum);
//     sequence_number++;
// }

// bool lora_receive_packet(uint8_t *data, uint8_t *len, uint8_t *dest_addr, uint8_t *src_addr, uint8_t *seq_num) {
//     lora_write_reg(0x0D, 0x00);
//     lora_write_reg(0x01, 0x85);

//     uint8_t irq = 0;
//     int timeout = 400;  // 2 giây
//     do {
//         vTaskDelay(pdMS_TO_TICKS(10));
//         lora_read_reg(0x12, &irq);
//         timeout--;
//     } while (!(irq & 0x40) && timeout > 0);

//     if (!(irq & 0x40)) {
//         ESP_LOGW(TAG, "Timeout khi chờ nhận dữ liệu");
//         toggle_led(LED_TIMEOUT_PIN, 3, 200);
//         return false;
//     }

//     if (irq & 0x20) {
//         ESP_LOGW(TAG, "Lỗi CRC trong gói tin");
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     lora_read_reg(0x13, len);
//     if (*len < 4 || *len > 255) {
//         ESP_LOGW(TAG, "Độ dài không hợp lệ: %d", *len);
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     uint8_t rssi, snr;
//     lora_read_reg(0x1A, &rssi);
//     lora_read_reg(0x19, &snr);
//     ESP_LOGI(TAG, "RSSI: -%d dBm, SNR: %d dB", (rssi >> 1), (int8_t)snr / 4);

//     lora_write_reg(0x0D, 0x00);
//     for (int i = 0; i < *len; i++) {
//         lora_read_reg(0x00, &data[i]);
//     }

//     *dest_addr = data[0];
//     *src_addr = data[1];
//     *seq_num = data[2];
//     uint8_t received_checksum = data[*len - 1];
//     uint8_t calculated_checksum = calculate_checksum(&data[3], *len - 4);

//     if (*src_addr == MY_ADDRESS) {
//         ESP_LOGW(TAG, "Bỏ qua gói tin từ chính thiết bị này");
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     if (*dest_addr != MY_ADDRESS) {
//         ESP_LOGW(TAG, "Gói tin không dành cho thiết bị này (dest: 0x%02X)", *dest_addr);
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     if (received_checksum != calculated_checksum) {
//         ESP_LOGW(TAG, "Checksum sai: Nhận 0x%02X, Tính toán 0x%02X", 
//                  received_checksum, calculated_checksum);
//         toggle_led(LED_SUCCESS_PIN, 3, 200);
//         return false;
//     }

//     toggle_led(LED_SUCCESS_PIN, 1, 500);
//     ESP_LOGI(TAG, "LED GPIO2 chớp 1 lần do nhận thành công");
//     lora_write_reg(0x12, 0xFF);
//     lora_write_reg(0x01, 0x01);
//     return true;
// }

// void app_main(void) {
//     gpio_set_direction(LORA_CS, GPIO_MODE_OUTPUT);
//     gpio_set_direction(LED_SUCCESS_PIN, GPIO_MODE_OUTPUT);
//     gpio_set_direction(LED_TIMEOUT_PIN, GPIO_MODE_OUTPUT);
//     gpio_set_level(LED_SUCCESS_PIN, 0);
//     gpio_set_level(LED_TIMEOUT_PIN, 0);

//     toggle_led(LED_SUCCESS_PIN, 5, 500);
//     toggle_led(LED_TIMEOUT_PIN, 5, 500);

//     spi_bus_config_t bus_cfg = {
//         .miso_io_num = 19,
//         .mosi_io_num = 23,
//         .sclk_io_num = 18,
//         .quadwp_io_num = -1,
//         .quadhd_io_num = -1,
//     };
//     spi_device_interface_config_t dev_cfg = {
//         .clock_speed_hz = 1000000,
//         .mode = 0,
//         .spics_io_num = LORA_CS,
//         .queue_size = 1,
//     };

//     esp_err_t ret = spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH_AUTO);
//     if (ret != ESP_OK) {
//         ESP_LOGE(TAG, "Khởi tạo SPI bus thất bại: %s", esp_err_to_name(ret));
//         toggle_led(LED_TIMEOUT_PIN, 3, 200);
//         while (1) vTaskDelay(pdMS_TO_TICKS(1000));
//     }
//     ret = spi_bus_add_device(SPI2_HOST, &dev_cfg, &spi_handle);
//     if (ret != ESP_OK) {
//         ESP_LOGE(TAG, "Thêm thiết bị SPI thất bại: %s", esp_err_to_name(ret));
//         toggle_led(LED_TIMEOUT_PIN, 3, 200);
//         while (1) vTaskDelay(pdMS_TO_TICKS(1000));
//     }

//     if (!lora_init()) {
//         ESP_LOGE(TAG, "Khởi tạo LoRa thất bại");
//         toggle_led(LED_TIMEOUT_PIN, 3, 200);
//         while (1) vTaskDelay(pdMS_TO_TICKS(1000));
//     }

//     const char *message = "Bèo dạt mây trôi, chốn xa xôi. Em ơi, anh vẫn đợi. Dẫu biết yêu là đau, là khổ, nhưng lòng anh vẫn mãi khắc ghi em trong tim.";
//     uint8_t data[255];
//     uint8_t len = strlen(message);

//     while (1) {
//         memset(data, 0, sizeof(data));
//         memcpy(data, message, len);
//         uint8_t dest_addr = DEST_ADDRESS;

//         int max_retries = 3;
//         bool success = false;
//         for (int retry = 0; retry < max_retries; retry++) {
//             lora_send_packet(dest_addr, data, len);
//             uint8_t received_data[255];
//             uint8_t received_len, received_dest, received_src, received_seq;
//             if (lora_receive_packet(received_data, &received_len, &received_dest, &received_src, &received_seq)) {
//                 if (received_src == 0x02) {
//                     char payload[received_len - 4 + 1];
//                     memcpy(payload, &received_data[3], received_len - 4);
//                     payload[received_len - 4] = '\0';
//                     ESP_LOGI(TAG, "Nhận từ Node 0x%02X, STT: %d, Dữ liệu: %s, Dữ liệu bảo toàn: %s", 
//                              received_src, received_seq, payload, 
//                              strcmp(payload, message) == 0 ? "Có" : "Không");
//                     success = true;
//                     break;
//                 } else {
//                     ESP_LOGW(TAG, "Nhận từ Node không mong đợi: 0x%02X", received_src);
//                 }
//             }
//             ESP_LOGW(TAG, "Retry %d/%d khi chờ phản hồi từ Node 0x02", retry + 1, max_retries);
//         }
//         if (!success) {
//             ESP_LOGW(TAG, "Hết lượt thử, không nhận được phản hồi từ Node 0x02");
//         }
//         vTaskDelay(pdMS_TO_TICKS(100));
//     }
// }



/// code 3 node setconfig, node 1(master)


// #include <stdio.h>
// #include <stdint.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/gpio.h"
// #include "driver/spi_master.h"
// #include "esp_log.h"

// #define LORA_CS   5
// #define LORA_RST  14
// #define LORA_DIO0 26
// #define LED_SUCCESS_PIN 2
// #define LED_TIMEOUT_PIN 4
// #define MASTER_ADDRESS 0x01
// #define DEST_ADDRESS_NODE2 0x02
// #define DEST_ADDRESS_NODE3 0x03

// #define PKT_TYPE_CONTROL    0x01
// #define PKT_TYPE_GETCONFIG  0x02
// #define PKT_TYPE_SETCONFIG  0x03
// #define PKT_TYPE_PING       0x04
// #define PKT_TYPE_DISCONNECT 0x05

// #define CMD_LED_ON  0x01
// #define CMD_LED_OFF 0x02

// static const char *TAG = "LORA_NODE1";
// static spi_device_handle_t spi_handle;
// static uint8_t sequence_number = 0;

// // Hàm lora_write_reg, lora_read_reg, lora_init, calculate_checksum, toggle_led giống mã trước
// // ... (Sao chép từ mã trước)

// void lora_send_packet(uint8_t dest_addr, uint8_t type, uint8_t *data, uint8_t len) {
//     if (len + 5 > 255) {
//         ESP_LOGE(TAG, "Dữ liệu quá dài: %d byte", len + 5);
//         return;
//     }

//     uint8_t mode;
//     lora_read_reg(0x01, &mode);
//     if ((mode & 0x80) != 0x80) {
//         ESP_LOGE(TAG, "Không ở chế độ LoRa, mode: 0x%02X. Khởi tạo lại...", mode);
//         lora_init();
//     }

//     uint8_t checksum = calculate_checksum(data, len);

//     lora_write_reg(0x0D, 0x80);
//     lora_write_reg(0x00, dest_addr);
//     lora_write_reg(0x00, MASTER_ADDRESS);
//     lora_write_reg(0x00, type);
//     lora_write_reg(0x00, sequence_number);
//     for (int i = 0; i < len; i++) {
//         lora_write_reg(0x00, data[i]);
//     }
//     lora_write_reg(0x00, checksum);

//     lora_write_reg(0x22, len + 5);
//     lora_write_reg(0x01, 0x83);

//     uint8_t irq = 0;
//     int timeout = 250;  // 2,5 giây
//     do {
//         vTaskDelay(pdMS_TO_TICKS(10));
//         lora_read_reg(0x12, &irq);
//         timeout--;
//     } while (!(irq & 0x08) && timeout > 0);

//     if (!(irq & 0x08)) {
//         ESP_LOGW(TAG, "Timeout khi gửi gói tin đến Node 0x%02X", dest_addr);
//         toggle_led(LED_TIMEOUT_PIN, 3, 200);
//         return;
//     }

//     lora_write_reg(0x12, 0xFF);
//     lora_write_reg(0x01, 0x01);

//     toggle_led(LED_SUCCESS_PIN, 1, 500);
//     ESP_LOGI(TAG, "Gửi đến Node 0x%02X, Loại: 0x%02X, STT: %d, Checksum: 0x%02X", 
//              dest_addr, type, sequence_number, checksum);
//     sequence_number++;
// }

// bool lora_receive_packet(uint8_t *data, uint8_t *len, uint8_t *dest_addr, uint8_t *src_addr, uint8_t *type, uint8_t *seq_num) {
//     lora_write_reg(0x0D, 0x00);
//     lora_write_reg(0x01, 0x85);

//     uint8_t irq = 0;
//     int timeout = 250;  // 2,5 giây
//     do {
//         vTaskDelay(pdMS_TO_TICKS(10));
//         lora_read_reg(0x12, &irq);
//         timeout--;
//     } while (!(irq & 0x40) && timeout > 0);

//     if (!(irq & 0x40)) {
//         ESP_LOGW(TAG, "Timeout khi chờ nhận dữ liệu");
//         toggle_led(LED_TIMEOUT_PIN, 3, 200);
//         return false;
//     }

//     if (irq & 0x20) {
//         ESP_LOGW(TAG, "Lỗi CRC trong gói tin");
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     lora_read_reg(0x13, len);
//     if (*len < 5 || *len > 255) {
//         ESP_LOGW(TAG, "Độ dài không hợp lệ: %d", *len);
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     uint8_t rssi, snr;
//     lora_read_reg(0x1A, &rssi);
//     lora_read_reg(0x19, &snr);
//     ESP_LOGI(TAG, "RSSI: -%d dBm, SNR: %d dB", (rssi >> 1), (int8_t)snr / 4);

//     lora_write_reg(0x0D, 0x00);
//     for (int i = 0; i < *len; i++) {
//         lora_read_reg(0x00, &data[i]);
//     }

//     *dest_addr = data[0];
//     *src_addr = data[1];
//     *type = data[2];
//     *seq_num = data[3];
//     uint8_t received_checksum = data[*len - 1];
//     uint8_t calculated_checksum = calculate_checksum(&data[4], *len - 5);

//     if (*src_addr == MASTER_ADDRESS) {
//         ESP_LOGW(TAG, "Bỏ qua gói tin từ chính thiết bị này");
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     if (*dest_addr != MASTER_ADDRESS) {
//         ESP_LOGW(TAG, "Gói tin không dành cho thiết bị này (dest: 0x%02X)", *dest_addr);
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     if (received_checksum != calculated_checksum) {
//         ESP_LOGW(TAG, "Checksum sai: Nhận 0x%02X, Tính toán 0x%02X", 
//                  received_checksum, calculated_checksum);
//         toggle_led(LED_SUCCESS_PIN, 3, 200);
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }

//     toggle_led(LED_SUCCESS_PIN, 1, 500);
//     ESP_LOGI(TAG, "LED GPIO2 chớp 1 lần do nhận thành công, Loại: 0x%02X", *type);
//     lora_write_reg(0x12, 0xFF);
//     lora_write_reg(0x01, 0x01);
//     return true;
// }

// // Hàm gửi lệnh điều khiển LED
// void send_control_led(uint8_t dest_addr, uint8_t command) {
//     uint8_t data[] = {command}; // CMD_LED_ON (0x01) hoặc CMD_LED_OFF (0x02)
//     lora_send_packet(dest_addr, PKT_TYPE_CONTROL, data, 1);
// }

// // Hàm gửi GetConfig
// void send_get_config(uint8_t dest_addr) {
//     uint8_t data[] = {0x00}; // Gói tin rỗng
//     lora_send_packet(dest_addr, PKT_TYPE_GETCONFIG, data, 1);
// }

// // Hàm gửi SetConfig
// void send_set_config(uint8_t dest_addr, uint32_t freq, uint8_t sf, uint32_t bw, uint8_t power) {
//     uint8_t data[] = {freq >> 24, freq >> 16, freq >> 8, freq, sf, bw >> 8, bw, power};
//     lora_send_packet(dest_addr, PKT_TYPE_SETCONFIG, data, 8);
// }

// void app_main(void) {
//     gpio_set_direction(LORA_CS, GPIO_MODE_OUTPUT);
//     gpio_set_direction(LED_SUCCESS_PIN, GPIO_MODE_OUTPUT);
//     gpio_set_direction(LED_TIMEOUT_PIN, GPIO_MODE_OUTPUT);
//     gpio_set_level(LED_SUCCESS_PIN, 0);
//     gpio_set_level(LED_TIMEOUT_PIN, 0);

//     toggle_led(LED_SUCCESS_PIN, 5, 500);
//     toggle_led(LED_TIMEOUT_PIN, 5, 500);

//     spi_bus_config_t bus_cfg = {
//         .miso_io_num = 19,
//         .mosi_io_num = 23,
//         .sclk_io_num = 18,
//         .quadwp_io_num = -1,
//         .quadhd_io_num = -1,
//     };
//     spi_device_interface_config_t dev_cfg = {
//         .clock_speed_hz = 1000000,
//         .mode = 0,
//         .spics_io_num = LORA_CS,
//         .queue_size = 1,
//     };

//     esp_err_t ret = spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH_AUTO);
//     if (ret != ESP_OK) {
//         ESP_LOGE(TAG, "Khởi tạo SPI bus thất bại: %s", esp_err_to_name(ret));
//         toggle_led(LED_TIMEOUT_PIN, 3, 200);
//         while (1) vTaskDelay(pdMS_TO_TICKS(1000));
//     }
//     ret = spi_bus_add_device(SPI2_HOST, &dev_cfg, &spi_handle);
//     if (ret != ESP_OK) {
//         ESP_LOGE(TAG, "Thêm thiết bị SPI thất bại: %s", esp_err_to_name(ret));
//         toggle_led(LED_TIMEOUT_PIN, 3, 200);
//         while (1) vTaskDelay(pdMS_TO_TICKS(1000));
//     }

//     if (!lora_init()) {
//         ESP_LOGE(TAG, "Khởi tạo LoRa thất bại");
//         toggle_led(LED_TIMEOUT_PIN, 3, 200);
//         while (1) vTaskDelay(pdMS_TO_TICKS(1000));
//     }

//     while (1) {
//         // Gửi lệnh bật LED GPIO4 đến Node 3
//         send_control_led(DEST_ADDRESS_NODE3, CMD_LED_ON);
//         uint8_t data[255], len, dest_addr, src_addr, type, seq_num;
//         int max_retries = 3;
//         bool success = false;
//         for (int retry = 0; retry < max_retries; retry++) {
//             if (lora_receive_packet(data, &len, &dest_addr, &src_addr, &type, &seq_num)) {
//                 if (src_addr == DEST_ADDRESS_NODE2 && type == PKT_TYPE_CONTROL) {
//                     ESP_LOGI(TAG, "Nhận xác nhận từ Node 0x02, Loại: 0x%02X, STT: %d", type, seq_num);
//                     success = true;
//                     break;
//                 }
//             }
//             ESP_LOGW(TAG, "Retry %d/%d khi chờ phản hồi từ Node 0x02", retry + 1, max_retries);
//         }

//         // Gửi GetConfig đến Node 3
//         send_get_config(DEST_ADDRESS_NODE3);
//         if (lora_receive_packet(data, &len, &dest_addr, &src_addr, &type, &seq_num)) {
//             if (src_addr == DEST_ADDRESS_NODE2 && type == PKT_TYPE_GETCONFIG) {
//                 uint32_t freq = (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7];
//                 uint8_t sf = data[8];
//                 uint32_t bw = (data[9] << 8) | data[10];
//                 uint8_t power = data[11];
//                 uint8_t led_status = data[12];
//                 ESP_LOGI(TAG, "Cấu hình Node 0x%02X: Freq=%lu Hz, SF=%d, BW=%lu Hz, Power=%d dBm, LED=%s", 
//                          src_addr, freq, sf, bw, power, led_status ? "Bật" : "Tắt");
//             }
//         }

//         // Gửi SetConfig đến Node 3 (ví dụ: đổi tần số thành 868 MHz)
//         send_set_config(DEST_ADDRESS_NODE3, 868000000, 8, 250000, 17);
//         if (lora_receive_packet(data, &len, &dest_addr, &src_addr, &type, &seq_num)) {
//             if (src_addr == DEST_ADDRESS_NODE2 && type == PKT_TYPE_SETCONFIG) {
//                 ESP_LOGI(TAG, "Node 0x%02X xác nhận cập nhật cấu hình", src_addr);
//             }
//         }

//         vTaskDelay(pdMS_TO_TICKS(1000)); // Delay 1 giây để tránh gửi quá nhanh
//     }
// }




/// code dia chi MAC

// node_tx.c

// #include <stdio.h>
// #include <stdint.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/semphr.h"
// #include "driver/gpio.h"
// #include "driver/spi_master.h"
// #include "esp_log.h"
// #include "esp_system.h"
// #include "esp_mac.h"

// #define LORA_CS   5
// #define LORA_RST  14
// #define LORA_DIO0 26
// #define LED_PIN   2
// #define TYPE_REGISTER 0x01
// #define TYPE_ACK_REG  0xA1
// #define TYPE_CONTROL  0x02
// #define TYPE_REPORT   0x03

// static const char *TAG = "NODE_TX";
// static spi_device_handle_t spi_handle;
// static SemaphoreHandle_t dio0_semaphore;
// static bool device_assigned = false;

// static uint8_t calculate_checksum(uint8_t *data, uint8_t len) {
//     uint8_t checksum = 0;
//     for (int i = 0; i < len; i++) checksum ^= data[i];
//     return checksum;
// }

// static esp_err_t lora_write_reg(uint8_t reg, uint8_t val) {
//     uint8_t data[] = {reg | 0x80, val};
//     spi_transaction_t t = {.length = 16, .tx_buffer = data};
//     return spi_device_polling_transmit(spi_handle, &t);
// }

// static esp_err_t lora_read_reg(uint8_t reg, uint8_t *val) {
//     uint8_t tx[] = {reg & 0x7F, 0x00};
//     uint8_t rx[2];
//     spi_transaction_t t = {.length = 16, .tx_buffer = tx, .rx_buffer = rx};
//     esp_err_t ret = spi_device_polling_transmit(spi_handle, &t);
//     *val = rx[1];
//     return ret;
// }

// static void dio0_isr_handler(void *arg) {
//     BaseType_t higher_priority_task_woken = pdFALSE;
//     xSemaphoreGiveFromISR(dio0_semaphore, &higher_priority_task_woken);
//     portYIELD_FROM_ISR(higher_priority_task_woken);
// }

// static void dio0_init(void) {
//     gpio_config_t dio0_conf = {
//         .pin_bit_mask = 1ULL << LORA_DIO0,
//         .mode = GPIO_MODE_INPUT,
//         .pull_up_en = GPIO_PULLUP_ENABLE,
//         .pull_down_en = GPIO_PULLDOWN_DISABLE,
//         .intr_type = GPIO_INTR_POSEDGE,
//     };
//     ESP_ERROR_CHECK(gpio_config(&dio0_conf));
//     ESP_ERROR_CHECK(gpio_install_isr_service(0));
//     ESP_ERROR_CHECK(gpio_isr_handler_add(LORA_DIO0, dio0_isr_handler, NULL));
// }

// static void lora_init(void) {
//     gpio_config_t io = {
//         .pin_bit_mask = 1ULL << LORA_RST,
//         .mode = GPIO_MODE_OUTPUT,
//         .pull_up_en = GPIO_PULLUP_DISABLE,
//         .pull_down_en = GPIO_PULLDOWN_DISABLE,
//         .intr_type = GPIO_INTR_DISABLE,
//     };
//     gpio_config(&io);
//     gpio_set_level(LORA_RST, 0);
//     vTaskDelay(pdMS_TO_TICKS(10));
//     gpio_set_level(LORA_RST, 1);
//     vTaskDelay(pdMS_TO_TICKS(100));
//     lora_write_reg(0x01, 0x00);
//     lora_write_reg(0x01, 0x80);
//     uint32_t frf = (uint32_t)(470000000 / 61.03515625);
//     lora_write_reg(0x06, (frf >> 16) & 0xFF);
//     lora_write_reg(0x07, (frf >> 8) & 0xFF);
//     lora_write_reg(0x08, frf & 0xFF);
//     lora_write_reg(0x09, 0xFF);
//     lora_write_reg(0x1D, 0x70 | 0x02); // BW 125kHz, CR 4/5
//     lora_write_reg(0x1E, 0xA0 | 0x04); // SF10
//     lora_write_reg(0x26, 0x08 | 0x04);
//     lora_write_reg(0x0E, 0x80);
//     lora_write_reg(0x0F, 0x00);
//     lora_write_reg(0x40, 0x00);
//     lora_write_reg(0x01, 0x01);
//     lora_write_reg(0x12, 0xFF);
//     ESP_LOGI(TAG, "LoRa init done");
// }

// static void lora_send_packet(uint8_t *data, uint8_t len) {
//     lora_write_reg(0x0D, 0x80);
//     uint8_t checksum = calculate_checksum(data, len);
//     for (int i = 0; i < len; i++) lora_write_reg(0x00, data[i]);
//     lora_write_reg(0x00, checksum); // Gửi checksum
//     lora_write_reg(0x22, len + 1); // Bao gồm checksum trong độ dài
//     lora_write_reg(0x01, 0x83);
//     uint8_t irq = 0;
//     TickType_t start = xTaskGetTickCount();
//     do {
//         vTaskDelay(pdMS_TO_TICKS(10));
//         lora_read_reg(0x12, &irq);
//         if ((xTaskGetTickCount() - start) > pdMS_TO_TICKS(1000)) break;
//     } while (!(irq & 0x08));
//     lora_write_reg(0x12, 0xFF);
//     lora_write_reg(0x01, 0x01);
// }

// static bool lora_receive_packet(uint8_t *data, uint8_t *len, uint8_t *type, bool *is_assigned, uint32_t timeout_ms) {
//     lora_write_reg(0x01, 0x85);
//     lora_write_reg(0x12, 0xFF);
//     if (xSemaphoreTake(dio0_semaphore, pdMS_TO_TICKS(timeout_ms)) == pdTRUE) {
//         uint8_t irq;
//         lora_read_reg(0x12, &irq);
//         if (irq & 0x20) {
//             ESP_LOGW(TAG, "CRC error in packet");
//             lora_write_reg(0x12, 0xFF);
//             lora_write_reg(0x01, 0x01);
//             return false;
//         }
//         uint8_t bytes, curAddr;
//         lora_read_reg(0x13, &bytes);
//         lora_read_reg(0x10, &curAddr);
//         lora_write_reg(0x0D, curAddr);
//         if (bytes > *len) bytes = *len;
//         for (int i = 0; i < bytes; i++) lora_read_reg(0x00, &data[i]);
//         uint8_t received_checksum = data[bytes - 1];
//         uint8_t calculated_checksum = calculate_checksum(data, bytes - 1);
//         if (received_checksum != calculated_checksum) {
//             ESP_LOGW(TAG, "Checksum error: received 0x%02X, calculated 0x%02X", received_checksum, calculated_checksum);
//             for (int i = 0; i < bytes; i++) {
//                 ESP_LOGW(TAG, "Data[%d]: 0x%02X", i, data[i]);
//             }
//             lora_write_reg(0x12, 0xFF);
//             lora_write_reg(0x01, 0x01);
//             return false;
//         }
//         uint8_t my_mac[6];
//         esp_read_mac(my_mac, ESP_MAC_WIFI_STA);
//         if (memcmp(data, my_mac, 6) != 0) {
//             ESP_LOGW(TAG, "Packet not for this node");
//             lora_write_reg(0x12, 0xFF);
//             lora_write_reg(0x01, 0x01);
//             return false;
//         }
//         *len = bytes - 1;
//         *type = data[6];
//         *is_assigned = data[7];
//         ESP_LOGI(TAG, "Received packet: type=0x%02X, is_assigned=%d", *type, *is_assigned);
//         lora_write_reg(0x12, 0xFF);
//         lora_write_reg(0x01, 0x01);
//         return true;
//     } else {
//         ESP_LOGW(TAG, "Timeout waiting for packet (%lums)", timeout_ms);
//         lora_write_reg(0x01, 0x01);
//         return false;
//     }
// }

// static void handle_control_packet(uint8_t *data, uint8_t len) {
//     if (len < 13) {
//         ESP_LOGW(TAG, "CONTROL packet too short: %d", len);
//         return;
//     }
//     uint8_t cmd = data[7];
//     char command_id[7];
//     memcpy(command_id, &data[8], 6);
//     command_id[6] = '\0';
//     ESP_LOGI(TAG, "Received CONTROL cmd=%d commandId=%s", cmd, command_id);
//     if (cmd == 1) {
//         gpio_set_level(LED_PIN, 1);
//         ESP_LOGI(TAG, "LED ON");
//     } else if (cmd == 0) {
//         gpio_set_level(LED_PIN, 0);
//         ESP_LOGI(TAG, "LED OFF");
//     } else {
//         ESP_LOGW(TAG, "Unknown command: %d", cmd);
//         return;
//     }
//     uint8_t report[14];
//     uint8_t my_mac[6];
//     esp_read_mac(my_mac, ESP_MAC_WIFI_STA);
//     memcpy(report, my_mac, 6);
//     report[6] = TYPE_REPORT;
//     report[7] = cmd;
//     memcpy(&report[8], command_id, 6);
//     report[13] = calculate_checksum(report, 13);
//     lora_send_packet(report, 14);
//     ESP_LOGI(TAG, "Sent REPORT cmd=%d commandId=%s", cmd, command_id);
// }

// static void node_task(void *arg) {
//     uint8_t my_mac[6];
//     esp_read_mac(my_mac, ESP_MAC_WIFI_STA);
//     ESP_LOGI(TAG, "Node MAC: %02X:%02X:%02X:%02X:%02X:%02X",
//              my_mac[0], my_mac[1], my_mac[2], my_mac[3], my_mac[4], my_mac[5]);
//     while (1) {
//     if (!device_assigned) {
//         uint8_t pkt[8];
//         memcpy(pkt, my_mac, 6);
//         pkt[6] = TYPE_REGISTER;
//         pkt[7] = calculate_checksum(pkt, 7);
//         ESP_LOGI(TAG, "Send REGISTER");
//         lora_send_packet(pkt, 8);
//         uint8_t received_data[32];
//         uint8_t received_len = sizeof(received_data), received_type;
//         bool is_assigned = false;
//         if (lora_receive_packet(received_data, &received_len, &received_type, &is_assigned, 20000)) {  // Chờ 20s
//             if (received_type == TYPE_ACK_REG) {
//                 ESP_LOGI(TAG, "Got ACK, isAssigned=%d", is_assigned);
//                 if (is_assigned) {
//                     device_assigned = true;
//                     ESP_LOGI(TAG, "Device assigned, switching to control mode");
//                 } else {
//                     ESP_LOGI(TAG, "Not assigned yet, retrying in 20s");
//                     vTaskDelay(pdMS_TO_TICKS(20000));  // Delay 20s retry
//                 }
//             }
//         } else {
//             ESP_LOGW(TAG, "No ACK received, retrying in 20s");
//             vTaskDelay(pdMS_TO_TICKS(20000));  // Delay 20s retry
//         }
//     } else {
//         // Mode nhận command (giữ nguyên)
//         uint8_t pkt[32];
//         uint8_t len = sizeof(pkt), type;
//         bool is_assigned;  // Không dùng ở đây, nhưng giữ để compile
//         if (lora_receive_packet(pkt, &len, &type, &is_assigned, 1000)) {
//             if (type == TYPE_CONTROL) {
//                 handle_control_packet(pkt, len);
//             }
//         }
//         vTaskDelay(pdMS_TO_TICKS(100));
//     }
// }
// }
// void app_main(void) {
//     gpio_config_t io = {
//         .pin_bit_mask = (1ULL << LORA_CS) | (1ULL << LED_PIN),
//         .mode = GPIO_MODE_OUTPUT,
//         .pull_up_en = GPIO_PULLUP_DISABLE,
//         .pull_down_en = GPIO_PULLDOWN_DISABLE,
//         .intr_type = GPIO_INTR_DISABLE,
//     };
//     gpio_config(&io);
//     gpio_set_level(LED_PIN, 0);
//     spi_bus_config_t bus_cfg = {
//         .miso_io_num = 19,
//         .mosi_io_num = 23,
//         .sclk_io_num = 18,
//         .quadwp_io_num = -1,
//         .quadhd_io_num = -1,
//     };
//     spi_device_interface_config_t dev_cfg = {
//         .clock_speed_hz = 1000000,
//         .mode = 0,
//         .spics_io_num = LORA_CS,
//         .queue_size = 1,
//     };
//     ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH_AUTO));
//     ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &dev_cfg, &spi_handle));
//     dio0_semaphore = xSemaphoreCreateBinary();
//     if (!dio0_semaphore) {
//         ESP_LOGE(TAG, "Create semaphore failed");
//         while (1);
//     }
//     dio0_init();
//     lora_init();
//     xTaskCreate(node_task, "node_task", 4096, NULL, 5, NULL);
// }




/// code gps + MAC


// Node.c - ESP32 LoRa Node with integrated GPS (ESP-IDF)

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_mac.h"

// ---------------- HW PINS ----------------
#define LORA_CS    5
#define LORA_RST   14
#define LORA_DIO0  26
#define LED_PIN    2

// SPI (fixed in app_main)
#define SPI_MISO   19
#define SPI_MOSI   23
#define SPI_SCLK   18

// GPS UART2
#define GPS_UART_NUM UART_NUM_2
#define GPS_TXD_PIN  17
#define GPS_RXD_PIN  16
#define UART_BUF_SIZE 1024

// ---------------- PROTOCOL ----------------
#define TYPE_REGISTER 0x01
#define TYPE_ACK_REG  0xA1
#define TYPE_CONTROL  0x02
#define TYPE_REPORT   0x03
#define TYPE_UNREGISTER 0xA4  // Đồng bộ với gateway

static const char *TAG = "NODE";

// ---------------- LoRa State ----------------
static spi_device_handle_t spi_handle;
static SemaphoreHandle_t dio0_semaphore;

// ---------------- GPS State ----------------
typedef struct {
    double latitude;    // NMEA raw ddmm.mmmm
    char   lat_dir;     // 'N'/'S'
    double longitude;   // NMEA raw dddmm.mmmm
    char   lon_dir;     // 'E'/'W'
    float  altitude;    // meters
    int    satellites;  // count
    int    fix;         // 0/1
    int    year, month, day;   // Gregorian
    int    hour, minute, second;
    float  speed;       // km/h
} GPS_Data;

static GPS_Data g_gps = {0};
static SemaphoreHandle_t gps_mutex;

// ---------------- Utils ----------------
static uint8_t calculate_checksum(const uint8_t *data, uint8_t len) {
    uint8_t c = 0;
    for (int i = 0; i < len; i++) c ^= data[i];
    return c;
}

static int32_t deg_to_i32(double deg) {
    // pack degrees into int32 with 1e-6 scaling
    double scaled = round(deg * 1000000.0);
    if (scaled > 2147483647.0) scaled = 2147483647.0;
    if (scaled < -2147483648.0) scaled = -2147483648.0;
    return (int32_t)scaled;
}

static uint16_t kmh_to_u16x100(float kmh) {
    int32_t v = (int32_t)roundf(kmh * 100.0f);
    if (v < 0) v = 0;
    if (v > 65535) v = 65535;
    return (uint16_t)v;
}

static double nmea_to_decimal(double nmea, char dir) {
    if (nmea == 0) return 0;
    int deg = (int)(nmea / 100);
    double minutes = nmea - (deg * 100);
    double dec = deg + minutes / 60.0;
    if (dir == 'S' || dir == 'W') dec = -dec;
    return dec;
}

// ---------------- LoRa low-level ----------------
static esp_err_t lora_write_reg(uint8_t reg, uint8_t val) {
    uint8_t data[] = { (uint8_t)(reg | 0x80), val };
    spi_transaction_t t = { .length = 16, .tx_buffer = data };
    return spi_device_polling_transmit(spi_handle, &t);
}

static esp_err_t lora_read_reg(uint8_t reg, uint8_t *val) {
    uint8_t tx[] = { (uint8_t)(reg & 0x7F), 0x00 };
    uint8_t rx[2];
    spi_transaction_t t = { .length = 16, .tx_buffer = tx, .rx_buffer = rx };
    esp_err_t ret = spi_device_polling_transmit(spi_handle, &t);
    *val = rx[1];
    return ret;
}

static void dio0_isr(void *arg) {
    BaseType_t hp = pdFALSE;
    xSemaphoreGiveFromISR(dio0_semaphore, &hp);
    portYIELD_FROM_ISR(hp);
}

static void dio0_init(void) {
    gpio_config_t io = {
        .pin_bit_mask = 1ULL << LORA_DIO0,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_POSEDGE,
    };
    ESP_ERROR_CHECK(gpio_config(&io));
    ESP_ERROR_CHECK(gpio_install_isr_service(0));
    ESP_ERROR_CHECK(gpio_isr_handler_add(LORA_DIO0, dio0_isr, NULL));
}

static void lora_init(void) {
    // RST
    gpio_config_t io = {
        .pin_bit_mask = 1ULL << LORA_RST,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io);
    gpio_set_level(LORA_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(10));
    gpio_set_level(LORA_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(100));

    // LoRa sleep->standby->cfg
    lora_write_reg(0x01, 0x00);
    lora_write_reg(0x01, 0x80);

    // Freq 470MHz
    uint32_t frf = (uint32_t)(470000000UL / 61.03515625);
    lora_write_reg(0x06, (frf >> 16) & 0xFF);
    lora_write_reg(0x07, (frf >> 8) & 0xFF);
    lora_write_reg(0x08, frf & 0xFF);

    lora_write_reg(0x09, 0xFF);          // PA config
    lora_write_reg(0x1D, 0x70 | 0x02);   // BW125kHz, CR4/5
    lora_write_reg(0x1E, 0xA0 | 0x04);   // SF10
    lora_write_reg(0x26, 0x08 | 0x04);   // AGC+LNA
    lora_write_reg(0x0E, 0x80);          // FIFO TX base
    lora_write_reg(0x0F, 0x00);          // FIFO RX base
    lora_write_reg(0x40, 0x00);          // DIO0 = RxDone / TxDone
    lora_write_reg(0x01, 0x01);          // standby
    lora_write_reg(0x12, 0xFF);          // clear IRQs

    ESP_LOGI(TAG, "LoRa init OK");
}

static void lora_tx(const uint8_t *buf, uint8_t len) {
    lora_write_reg(0x0D, 0x80); // FIFO ptr
    uint8_t chk = calculate_checksum(buf, len);
    for (int i = 0; i < len; i++) lora_write_reg(0x00, buf[i]);
    lora_write_reg(0x00, chk);
    lora_write_reg(0x22, len + 1);
    lora_write_reg(0x01, 0x83); // TX
    uint8_t irq = 0; TickType_t t0 = xTaskGetTickCount();
    do {
        vTaskDelay(pdMS_TO_TICKS(5));
        lora_read_reg(0x12, &irq);
        if ((xTaskGetTickCount() - t0) > pdMS_TO_TICKS(1200)) break;
    } while (!(irq & 0x08));
    lora_write_reg(0x12, 0xFF);
    lora_write_reg(0x01, 0x01); // standby
}

static int lora_rx(uint8_t *out, int maxlen, uint32_t timeout_ms) {
    lora_write_reg(0x01, 0x85); // RX continuous
    lora_write_reg(0x12, 0xFF);
    if (xSemaphoreTake(dio0_semaphore, pdMS_TO_TICKS(timeout_ms)) != pdTRUE) {
        lora_write_reg(0x01, 0x01);
        return -1;
    }
    uint8_t irq; lora_read_reg(0x12, &irq);
    if (irq & 0x20) { // CRC error
        lora_write_reg(0x12, 0xFF);
        lora_write_reg(0x01, 0x01);
        return -1;
    }
    uint8_t bytes, cur;
    lora_read_reg(0x13, &bytes);
    lora_read_reg(0x10, &cur);
    lora_write_reg(0x0D, cur);
    if (bytes > maxlen) bytes = maxlen;
    for (int i = 0; i < bytes; i++) lora_read_reg(0x00, &out[i]);
    lora_write_reg(0x12, 0xFF);
    lora_write_reg(0x01, 0x01);

    if (bytes < 2) return -1;
    uint8_t chk = out[bytes - 1];
    uint8_t calc = calculate_checksum(out, bytes - 1);
    if (calc != chk) {
        ESP_LOGW(TAG, "RX checksum fail: calc=0x%02X, rcv=0x%02X, bytes=%d", calc, chk, bytes);
        return -1;
    }
    return bytes - 1; // exclude checksum
}

// ---------------- GPS parsing ----------------
static void parse_gpgga(const char *s, GPS_Data *d) {
    char *copy = strdup(s);
    char *tok = strtok(copy, ",");
    int f = 0;
    while (tok) {
        f++;
        switch (f) {
            case 2: if (strlen(tok) >= 6) {
                int t = atoi(tok); d->hour = t/10000; d->minute=(t/100)%100; d->second=t%100; } break;
            case 3: d->latitude = atof(tok); break;
            case 4: d->lat_dir = tok[0]; break;
            case 5: d->longitude = atof(tok); break;
            case 6: d->lon_dir = tok[0]; break;
            case 7: d->fix = atoi(tok); break;
            case 8: d->satellites = atoi(tok); break;
            case 10: d->altitude = atof(tok); break;
            default: break;
        }
        tok = strtok(NULL, ",");
    }
    free(copy);
}

static void parse_gprmc(const char *s, GPS_Data *d) {
    char *copy = strdup(s);
    char *tok = strtok(copy, ",");
    int f = 0;
    while (tok) {
        f++;
        switch (f) {
            case 2: if (strlen(tok) >= 6) {
                int t = atoi(tok); d->hour=t/10000; d->minute=(t/100)%100; d->second=t%100; } break;
            case 3: d->fix = (tok[0]=='A'); break;
            case 4: d->latitude = atof(tok); break;
            case 5: d->lat_dir = tok[0]; break;
            case 6: d->longitude = atof(tok); break;
            case 7: d->lon_dir = tok[0]; break;
            case 8: d->speed = atof(tok) * 1.852f; break; // knots->kmh
            case 10: if (strlen(tok) >= 6) {
                int t = atoi(tok); d->day=t/10000; d->month=(t/100)%100; d->year=2000+(t%100); } break;
            default: break;
        }
        tok = strtok(NULL, ",");
    }
    free(copy);
}

static void gps_task(void *arg) {
    uart_config_t cfg = {
        .baud_rate = 9600, .data_bits = UART_DATA_8_BITS, .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1, .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    ESP_ERROR_CHECK(uart_param_config(GPS_UART_NUM, &cfg));
    ESP_ERROR_CHECK(uart_set_pin(GPS_UART_NUM, GPS_TXD_PIN, GPS_RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(GPS_UART_NUM, UART_BUF_SIZE*2, 0, 0, NULL, 0));

    uint8_t *buf = (uint8_t*)malloc(UART_BUF_SIZE);
    GPS_Data loc = {0};

    while (1) {
        int len = uart_read_bytes(GPS_UART_NUM, buf, UART_BUF_SIZE-1, pdMS_TO_TICKS(200));
        if (len > 0) {
            buf[len] = 0;
            char *line = strtok((char*)buf, "\n\r");
            while (line) {
                if (strstr(line, "$GPGGA")) parse_gpgga(line, &loc);
                else if (strstr(line, "$GPRMC")) parse_gprmc(line, &loc);

                if (gps_mutex && xSemaphoreTake(gps_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
                    g_gps = loc;
                    xSemaphoreGive(gps_mutex);
                }
                line = strtok(NULL, "\n\r");
            }
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

// ---------------- CONTROL handler ----------------
static void handle_control_and_report(uint8_t cmd, const char commandId[6]) {
    // Actuate LED
    if (cmd == 1) gpio_set_level(LED_PIN, 1);
    else if (cmd == 0) gpio_set_level(LED_PIN, 0);

    // Snapshot GPS
    GPS_Data loc; memset(&loc, 0, sizeof(loc));
    if (gps_mutex && xSemaphoreTake(gps_mutex, pdMS_TO_TICKS(20)) == pdTRUE) {
        loc = g_gps;
        xSemaphoreGive(gps_mutex);
    }
    double lat = nmea_to_decimal(loc.latitude,  loc.lat_dir);
    double lon = nmea_to_decimal(loc.longitude, loc.lon_dir);

    // Build REPORT: MAC(6)+TYPE(1)+cmd(1)+commandId(6)+ GPS block(=Register block)
    uint8_t pkt[64];
    uint8_t mac[6]; esp_read_mac(mac, ESP_MAC_WIFI_STA);
    memcpy(pkt, mac, 6);
    pkt[6] = TYPE_REPORT;
    pkt[7] = cmd;
    memcpy(&pkt[8], commandId, 6);

    uint8_t *p = &pkt[14];
    *p++ = (uint8_t)(loc.fix ? 1 : 0);
    int32_t lat_i = deg_to_i32(lat);
    int32_t lon_i = deg_to_i32(lon);
    memcpy(p, &lat_i, 4); p += 4;
    memcpy(p, &lon_i, 4); p += 4;
    int16_t alt_i = (int16_t)lroundf(loc.altitude);
    memcpy(p, &alt_i, 2); p += 2;
    *p++ = (uint8_t)(loc.satellites & 0xFF);
    uint16_t spd = kmh_to_u16x100(loc.speed);
    memcpy(p, &spd, 2); p += 2;
    *p++ = (uint8_t)((loc.year>2000)?(loc.year-2000):0);
    *p++ = (uint8_t)loc.month;
    *p++ = (uint8_t)loc.day;
    *p++ = (uint8_t)loc.hour;
    *p++ = (uint8_t)loc.minute;
    *p++ = (uint8_t)loc.second;

    uint8_t len = (uint8_t)(p - pkt);
    lora_tx(pkt, len);
    ESP_LOGI(TAG, "REPORT sent (cmd=%u, fix=%d, lat=%.6f, lon=%.6f)", cmd, loc.fix, lat, lon);
}

// ---------------- Node task ----------------
static void node_task(void *arg) {
    // LED pin
    gpio_config_t io = {
        .pin_bit_mask = 1ULL << LED_PIN,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 0, .pull_down_en = 0, .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io);
    gpio_set_level(LED_PIN, 0);

    bool assigned = false;
    uint8_t mac[6]; esp_read_mac(mac, ESP_MAC_WIFI_STA);
    ESP_LOGI(TAG, "Node MAC: %02X:%02X:%02X:%02X:%02X:%02X", mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);

    // Watchdog: Thời gian không nhận packet nào (CONTROL hoặc ACK) thì reset assigned
    TickType_t last_activity = xTaskGetTickCount();
    const TickType_t WATCHDOG_TIMEOUT = pdMS_TO_TICKS(30000);  // 30s cho test

    while (1) {
        // Check watchdog trước
        if (assigned && (xTaskGetTickCount() - last_activity > WATCHDOG_TIMEOUT)) {
            ESP_LOGW(TAG, "Watchdog: No activity for long time, resetting assigned");
            assigned = false;
            last_activity = xTaskGetTickCount();
        }

        if (!assigned) {
            // Snapshot GPS
            GPS_Data loc; memset(&loc, 0, sizeof(loc));
            if (gps_mutex && xSemaphoreTake(gps_mutex, pdMS_TO_TICKS(20)) == pdTRUE) {
                loc = g_gps;
                xSemaphoreGive(gps_mutex);
            }
            double lat = nmea_to_decimal(loc.latitude, loc.lat_dir);
            double lon = nmea_to_decimal(loc.longitude, loc.lon_dir);

            // Build REGISTER packet with GPS
            uint8_t pkt[64];
            memcpy(pkt, mac, 6);
            pkt[6] = TYPE_REGISTER;

            uint8_t *p = &pkt[7];
            *p++ = (uint8_t)(loc.fix ? 1 : 0);
            int32_t lat_i = deg_to_i32(lat);
            int32_t lon_i = deg_to_i32(lon);
            memcpy(p, &lat_i, 4); p += 4;
            memcpy(p, &lon_i, 4); p += 4;
            int16_t alt_i = (int16_t)lroundf(loc.altitude);
            memcpy(p, &alt_i, 2); p += 2;
            *p++ = (uint8_t)(loc.satellites & 0xFF);
            uint16_t spd = kmh_to_u16x100(loc.speed);
            memcpy(p, &spd, 2); p += 2;
            *p++ = (uint8_t)((loc.year>2000)?(loc.year-2000):0);
            *p++ = (uint8_t)loc.month;
            *p++ = (uint8_t)loc.day;
            *p++ = (uint8_t)loc.hour;
            *p++ = (uint8_t)loc.minute;
            *p++ = (uint8_t)loc.second;

            uint8_t len = (uint8_t)(p - pkt);
            ESP_LOGI(TAG, "Send REGISTER (fix=%d, lat=%.6f lon=%.6f)", loc.fix, lat, lon);
            lora_tx(pkt, len);

            // Wait for ACK
            uint8_t rx[64];
            int n = lora_rx(rx, sizeof(rx), 20000);
            if (n >= 8 && memcmp(rx, mac, 6) == 0 && rx[6] == TYPE_ACK_REG) {
                assigned = (rx[7] != 0);
                last_activity = xTaskGetTickCount();
                ESP_LOGI(TAG, "ACK_REG received, isAssigned=%d", assigned);
            } else {
                ESP_LOGW(TAG, "No/invalid ACK, retry in 20s");
                vTaskDelay(pdMS_TO_TICKS(20000));
            }
        } else {
            // CONTROL loop
            uint8_t rx[64];
            int n = lora_rx(rx, sizeof(rx), 2000);
            if (n >= 8 && memcmp(rx, mac, 6) == 0) {
                uint8_t type = rx[6];
                ESP_LOGI(TAG, "RX packet: type=0x%02X, n=%d", type, n);  // Log RX type
                if (type == TYPE_CONTROL && n >= 14) {
                    uint8_t cmd = rx[7];
                    char commandId[6]; memcpy(commandId, &rx[8], 6);
                    ESP_LOGI(TAG, "CONTROL cmd=%u", cmd);
                    handle_control_and_report(cmd, commandId);
                    last_activity = xTaskGetTickCount();
                } else if (type == TYPE_UNREGISTER && n >= 8) {
                    uint8_t reason = rx[7];
                    ESP_LOGI(TAG, "UNREGISTER received, reason=%u, resetting assigned", reason);
                    assigned = false;
                    last_activity = xTaskGetTickCount();
                } else {
                    ESP_LOGW(TAG, "RX unknown/ignored type=0x%02X, n=%d (expected CONTROL=0x02 or UNREGISTER=0xA4)", type, n);
                }
            } else if (n == -1) {
                ESP_LOGD(TAG, "RX timeout or invalid (n=%d)", n);
            }
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}

// ---------------- app_main ----------------
void app_main(void) {
    // SPI
    spi_bus_config_t bus_cfg = {
        .miso_io_num = SPI_MISO, .mosi_io_num = SPI_MOSI, .sclk_io_num = SPI_SCLK,
        .quadwp_io_num = -1, .quadhd_io_num = -1
    };
    spi_device_interface_config_t dev_cfg = {
        .clock_speed_hz = 1000000, .mode = 0, .spics_io_num = LORA_CS, .queue_size = 1
    };
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH_AUTO));
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &dev_cfg, &spi_handle));

    dio0_semaphore = xSemaphoreCreateBinary();
    gps_mutex = xSemaphoreCreateMutex();
    if (!dio0_semaphore || !gps_mutex) { ESP_LOGE(TAG, "semaphore/mutex create failed"); while(1) vTaskDelay(1000); }

    dio0_init();
    lora_init();

    xTaskCreate(gps_task,  "gps_task",  4096, NULL, 6, NULL);
    xTaskCreate(node_task, "node_task", 4096, NULL, 5, NULL);
}