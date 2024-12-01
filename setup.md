# Setup instructions

This clock works best when connected to WiFi. Here you can find a detailed description of how to connect it to the home WiFi network.

Same instruction in Russian are in the buttom of this page.

## First setup

When turned on for the first time the clock displays "888888" and starts its own wifi access point called "NixieClock". This AP is visible for 1 minute, and you can connect to
it from computer or phone, and then access the clock setup with url http://192.168.4.1 (note, http:, not https:).
I recommend using a computer to connect to the clock, my Adndoid phone with Firefox refuses to work.

![WiFi Captive Portal Homepage](http://i.imgur.com/YPvW9eql.png)

You should see the WiFi Manager front page in the browser. Next click "Configure WiFi", you will see something similar to:

![clock text](https://github.com/alexander-krotov/apollo-clock/blob/main/wifiselect.png?raw=true)

There you can choose your home network (click on its name), or enter the network (ssid) name manually (in case it is not configured visible).
You also need to enter the network password.

Once you click "Save" the clock will try to connect to the WiFi network. When connected the clock will run its assigned IP address (something like "192 168 74 182") on the display,
and after that tries to get the time from the Internet, and finally starts to work as a clock (show the time). It might take a couple of seconds to get the time.

If 1 minute was not enough to complete the setup you can power off the clock and start from scratch.

Should you need more details about clock initial WiFi setup, you can refer it to WiFi Manager documentation: https://github.com/tzapu/WiFiManager

## Clock is configured

Next time you power on the clock it will try to connect to the known WiFi network automatically. If it fails to connect (network is not available) it starts the same
configuration AP NixieClock and will wait for the new network setup.

If the previously configured network is available, the clock will again run its assigned IP address on the display (something like "192 168 74 182") and then show the current time.

## Clock setting update

Clock has several configurable settings. You can access the clock config page using a url like http://192.168.74.182 (note http: not https:). The actual IP address might be different  
in your network, you should use the one the clock did show when powered on (you may want to write it down). Alternatively if your WiFi router shows the device names and their I
addresses, you can find it there: the clock device name should be something like "esp32c3-357010".

Clock setup page looks like:

![clock text](https://github.com/alexander-krotov/apollo-clock/blob/main/config.png?raw=true)

Here you can update
- the timezone: 2 for Helsinki, 0 for London, Winter time. You might want to change it for summer time shift
- display brightness: 16 is maximal brightness. On smaller brightness levels the tubes are nor gradually dimmed, they start to blink instead
- 12/24 mode - preferred time format. the clock does not have AM/PM indication
- leading 0: preferred time format 02:13:14/2:13:14
- bar mode: separators bars mode. Bar could be set alway on, always off, blinking synchronously or blinking asynchronously
- use NTP: clock will try to get the time from the Internet (using NTP protocol)
- use RTC: clock has onboard Real-Time clock, that keeps the time in case WiFi is not available (backup coin battery is needed to keep the time when it is not powered from USB)
- NTP server name to synchronize time from. fi.pool.ntp.org and uk.pool.ntp.org are ok, or you can set any other NTP server name if you know what it is.

Click "Update" to update the settings. Settings are stored to clock memory and survive over powering off.

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

