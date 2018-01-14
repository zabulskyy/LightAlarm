# Smart alarm on STM32 with RTC and...

# RTC

## info:
* https://datasheets.maximintegrated.com/en/ds/DS3231.pdf __page 11__ -> pin adresses


## pins:
* SCL -> PB6
* SDA -> PB7

## use data:

``` read_day    ```  weekday in range 1-7

``` read_dhrs, read_hrs  ```  actual hour = dhrs*10 + hrs

``` read_dmin, read_min  ```  actual minutes = dmin*10 + min

``` read_dsec, read_sec  ```  actual seconds = dsec*10 + sec

If you are really interested, these lines contain actual addresses in RTC:
```
read_sec       ==	xBuffer[0]&0xF
read_dsec      ==	(xBuffer[0]&(0xF<<4))>>4
read_min       ==	xBuffer[1]&0xF
read_dmin      == 	(xBuffer[1]&(0xF<<4))>>4
read_hrs       == 	xBuffer[2]&0xF
read_dhrs      == 	(xBuffer[2]&(0xF<<4))>>4
read_day       ==    xBuffer[3]&0xF
```

## set alarm
```
set_alarm(int dhr, int hr, int dmn, int mn)
/*
 * dhr: hours * 10
 * hr: hours
 * dmn: minutes * 10
 * mn: minutes
 */
```

Documentation is all it says. In this instance alarm will sound at Monday, 7:30
# LightAlarm
