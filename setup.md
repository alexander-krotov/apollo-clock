# Setup Instructions

This clock works best when connected to Wi-Fi. Below you’ll find step-by-step instructions for connecting it to your home network.
*(Instructions in Russian are available at the bottom of this page.)*

---

## First Setup

When powered on for the first time, the clock displays `888888` and creates its own Wi-Fi access point (AP) named **NixieClock**.

* The AP is visible for **1 minute**.
* Connect to it from a computer or phone, then open [http://192.168.4.1](http://192.168.4.1) in your browser (**note:** `http`, not `https`).
* Using a computer is recommended, as some Android devices (e.g., Firefox on Android) may not work reliably.

---

## Wi-Fi Manadger

![WiFi Captive Portal Homepage](http://i.imgur.com/YPvW9eql.png)

You should now see the **WiFiManager** homepage.

1. Click **“Configure WiFi”**.
2. Select your home network from the list, or enter the SSID manually if it is hidden.
3. Enter your network password.
4. Click **“Save”**.

![clock text](https://github.com/alexander-krotov/apollo-clock/blob/main/wifiselect.png?raw=true)

The clock will then:

* Connect to your Wi-Fi network.
* Display its assigned IP address (e.g., `192.168.74.182`).
* Fetch the current time from the internet.
* Start running as a regular clock. *(This may take a few seconds.)*

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

## Первоначальная настройка

При первом включении часы показывают на дисплее «888888» и запускают собственную точку доступа Wi-Fi под названием «NixieClock». Эта точка доступа видна в течение 1 минуты, и вы можете подключиться к ней с компьютера или телефона, а затем получить доступ к настройке часов с помощью URL http://192.168.4.1 (обратите внимание, http:, а не https:).
Лучше использовать компьютер для подключения к часам, мой телефон Adndoid не смог подключиться.

![WiFi Captive Portal Homepage](http://i.imgur.com/YPvW9eql.png)

Вы должны увидеть главную страницу WiFi Manager в браузере. Затем нажмите «Configure WiFi», вы увидите что-то похожее на:

![clock text](https://github.com/alexander-krotov/apollo-clock/blob/main/wifiselect.png?raw=true)

Теперь вы можете выбрать свою домашнюю сеть (нажмите на ее имя) или ввести имя сети (ssid) вручную (если она не настроена видимой).
Вам также нужно ввести пароль сети.

После нажатия «Save» часы попытаются подключиться к сети WiFi. При успешном подключении часы отобразят назначенный им IP-адрес (что-то вроде «192 168 74 182») на дисплее,
а затем попытаются получить точное время из Интернета и, наконец, начнут работать как часы (показывать время). Получение времени из Интернета может занять несколько секунд.

Если для завершения настройки вам не хватило 1 минуты, вы можете выключить часы и начать с нуля.

Если вам нужны более подробные сведения о первоначальной настройке WiFi, вы можете обратиться к документации WiFi Manager: https://github.com/tzapu/WiFiManager

## Часы уже настроены

В следующий раз, когда вы включите часы, они автоматически попытаются подключиться к известной сети WiFi. Если подключение не удастся (сеть недоступна), они запустят ту же AP NixieClock и будут ждать новой настройки новой сети.

Если ранее настроенная сеть доступна, часы снова отобразят назначенный им IP-адрес на дисплее (что-то вроде «192 168 74 182»), а затем покажут текущее время.

## Настройка часов

Часы можно настраивать. Вы можете получить доступ к странице конфигурации часов, используя URL, похожий на http://192.168.74.182 (обратите внимание, http:, а не https:). Фактический IP-адрес может быть другим в вашей сети, вам следует использовать тот, который часы показывали при включении (вы можете записать его).
Если ваш WiFi роутер умеет показывать имена устройств и их IP-адреса, вы можете найти их там: имя устройства часов должно быть похоже на "esp32c3-357010".

Страница настройки часов выглядит так:

![clock text](https://github.com/alexander-krotov/apollo-clock/blob/main/config.png?raw=true)

Здесь вы можете настроить
- часовой пояс: 2 для Хельсинки, 0 для Лондона (зимнее время). Вы можете изменить его для перехода на летнее время
- яркость дисплея: 16 — максимальная яркость. На меньших уровнях яркости трубки не затемняются, а начинают мигать
- режим 12/24 — предпочтительный формат времени. Часы не имеют индикации AM/PM
- ведущий 0: предпочтительный формат времени 02:13:14/2:13:14
- режим работы разделительных точек. Точки могут быть всегда включены, всегда выключены, мигать синхронно или мигать асинхронно
- использование NTP: часы будут пытаться получить время из Интернета (используя протокол NTP)
- использование RTC: часы имеют встроенный чип реального времени, который сохраняет время в случае, если WiFi недоступен (необходима батарейка для сохранения времени, когда часы не питаются от USB)
- имя сервера NTP для синхронизации времени. fi.pool.ntp.org и uk.pool.ntp.org подойдут, или вы можете задать любое другое имя сервера NTP, если знаете его.

Нажмите «Update», чтобы обновить настройки. Настройки сохраняются в памяти часов и переживают выключения питания.

На той же странице вы можете вручную установить время. Обратите внимание, если включено «использовать NTP», часы вскоре установят время на время, полученное с NTP.

