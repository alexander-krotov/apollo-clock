# Setup Instructions

This clock works best when connected to Wi-Fi. Below you’ll find step-by-step instructions for connecting it to your home network.

*([Instructions in Russian](https://github.com/alexander-krotov/apollo-clock/blob/main/setup.md#%D0%B8%D0%BD%D1%81%D1%82%D1%80%D1%83%D0%BA%D1%86%D0%B8%D0%B8-%D0%BF%D0%BE-%D0%BD%D0%B0%D1%81%D1%82%D1%80%D0%BE%D0%B9%D0%BA%D0%B5) are available at the bottom of this page.)*

---

## First Setup

When powered on for the first time, the clock displays `888888` and creates its own Wi-Fi access point (AP) named **NixieClock**.

* The AP is visible for **1 minute**.
* Connect to it from a computer or phone, then open [http://192.168.4.1](http://192.168.4.1) in your browser (**note:** `http`, not `https`).
* Using a computer is recommended, as some Android devices (e.g., Firefox on Android) may not work reliably.

---

## Wi-Fi Manager

![WiFi Captive Portal Homepage](http://i.imgur.com/YPvW9eql.png)

You should now see the **WiFiManager** homepage.

1. Click **“Configure WiFi”**.
2. Select your home network from the list, or enter the SSID manually if it is hidden.
3. Enter your network password.
4. Click **“Save”**.

![clock text](https://github.com/alexander-krotov/apollo-clock/blob/main/wifiselect.png?raw=true)

The clock will then:

* Connect to your Wi-Fi network.
* Display its assigned IP address (e.g., `192 168 74 182`).
* Fetch the current time from the internet. *(This may take a few seconds.)*
* Start running as a regular clock. 

If the one-minute setup window expires, simply power off the clock and try again.

For more details on Wi-Fi setup, refer to the [WiFiManager documentation](https://github.com/tzapu/WiFiManager).

---

## After Setup

On the next startup, the clock will automatically reconnect to the saved Wi-Fi network.

* If the network is unavailable, it will start the **NixieClock** AP again and wait for a new setup.
* If the connection succeeds, the clock will briefly show its IP address, then display the current time.

---

## Updating Clock Settings

The clock has several configurable options accessible through its configuration page.

![clock text](https://github.com/alexander-krotov/apollo-clock/blob/main/config.png?raw=true)

1. Open your browser and navigate to the clock’s IP address, e.g., http://192.168.74.182

   * The IP address is shown on the display at startup.
   * Alternatively, check your router’s device list: the clock should appear as something like `esp32c3-357010`.

2. The setup page allows you to adjust:

   * **Timezone** (e.g., `2` for Helsinki, `0` for London, winter time); update manually for daylight savings.
   * **Display brightness** (max: `16`; lower values cause blinking instead of smooth dimming).
   * **12/24-hour format** (no AM/PM indicator).
   * **Leading zero** (choose between `02:13:14` and `2:13:14`).
   * **Separator bar mode** (always on, always off, synchronous blink, or asynchronous blink).
   * **Use NTP** (synchronize time via the Internet).
   * **Use RTC** (keep time with the onboard Real-Time Clock; requires a backup coin battery).
   * **NTP server** (e.g., `fi.pool.ntp.org`, `uk.pool.ntp.org`, or a custom server).

3. Click **“Update”** to save your settings.

   * All changes are stored in memory and will persist after power-off.

On the same page you can set the time manually. Note, if "use NTP" is on the clock will reset the time to the time it has read from NTP soon.

---

# Инструкции по настройке

Часы работают лучше всего при подключении к Wi‑Fi. Ниже приведены пошаговые инструкции по подключению к вашей домашней сети.

---

## Первая настройка

При первом включении часы отображают `888888` и создают собственную точку доступа Wi‑Fi (AP) с именем **NixieClock**.

* Точка доступа доступна в течение **1 минуты**.
* Подключитесь к ней с компьютера или телефона, затем откройте в браузере http://192.168.4.1 (**обратите внимание:** `http`, а не `https`).
* Рекомендуется использовать компьютер, так как некоторые устройства Android (например, Firefox на Android) могут работать некорректно.

---

## Настройка Wi‑Fi

Вы должны увидеть стартовую страницу **WiFiManager**.

![WiFi Captive Portal Homepage](http://i.imgur.com/YPvW9eql.png)


1. Нажмите **«Configure WiFi»**.
2. Выберите свою домашнюю сеть из списка или введите SSID вручную, если сеть скрыта.
3. Введите пароль от сети.
4. Нажмите **«Save»**.

![clock text](https://github.com/alexander-krotov/apollo-clock/blob/main/wifiselect.png?raw=true)

После этого часы:

* Подключатся к вашей Wi‑Fi сети.
* Отобразят назначенный IP‑адрес (например, `192 168 74 182`).
* Получат текущее время из интернета. *(Это может занять несколько секунд.)*
* Начнут работать как обычные часы. 

Если минуты оказалось недостаточно, просто выключите часы и попробуйте снова.

Для более подробной информации о настройке Wi‑Fi см. [документацию WiFiManager](https://github.com/tzapu/WiFiManager).

---

## После настройки

При следующем включении часы автоматически попытаются подключиться к сохранённой Wi‑Fi сети.

* Если сеть недоступна, часы снова запустят точку доступа **NixieClock** и будут ожидать новой настройки.
* Если подключение прошло успешно, часы кратковременно покажут свой IP‑адрес, затем начнут отображать текущее время.

---

## Обновление настроек часов

Часы имеют несколько параметров, доступных через страницу конфигурации.

1. Откройте браузер и введите IP‑адрес часов, например [http://192.168.74.182](http://192.168.74.182).

   * IP‑адрес показывается на дисплее при запуске.
   * Также вы можете найти устройство в списке клиентов вашего роутера: часы должны отображаться как `esp32c3-357010` или похожее.

![clock text](https://github.com/alexander-krotov/apollo-clock/blob/main/config.png?raw=true)

2. На странице настроек можно изменить:

   * **Часовой пояс** (например, `2` для Хельсинки, `0` для Лондона, зимнее время; для перехода на летнее время обновляется вручную).
   * **Яркость дисплея** (максимум: `16`; при меньших значениях лампы начинают мигать, а не плавно тускнеть).
   * **Формат 12/24 часа** (AM/PM не поддерживается).
   * **Ведущий ноль** (отображение `02:13:14` или `2:13:14`).
   * **Режим разделительных полос** (всегда включены, всегда выключены, мигают синхронно или асинхронно).
   * **Использовать NTP** (синхронизация времени через интернет).
   * **Использовать RTC** (поддержка времени встроенными часами реального времени; требуется резервная батарейка).
   * **Сервер NTP** (например, `fi.pool.ntp.org`, `uk.pool.ntp.org` или другой сервер по вашему выбору).

3. Нажмите **«Update»**, чтобы сохранить настройки.

   * Все изменения сохраняются в памяти и остаются после выключения питания.

На той же странице вы можете вручную установить время. Обратите внимание, если включено «использовать NTP», часы вскоре установят время на время, полученное с NTP.
