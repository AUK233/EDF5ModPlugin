# Index
The following values are available for Ammo_CustomParameter when AmmoClass is X.

## AmmoClass 's value:
- [SolidBullet01](#SolidBullet01)
- [SolidExpBullet01](#SolidExpBullet01)

### SolidBullet01
Its Ammo_CustomParameter has a maximum of 3 parameters.

| node index | value type | function | value description |
|---|---|---|---|
|0|int|ammo type|0 is normal, 1 is bounce|
|1|float|hit FX adjust|adjusting FX when bullet hit, default 1.0|
|2|float|hit FX time adjust|adjusting FX time when bullet hit, default 1.0|

#### [Return to Index](#index)

### SolidExpBullet01
Its Ammo_CustomParameter has a maximum of 3 parameters.

| node index | value type | function | value description |
|---|---|---|---|
|0|int or float or ptr|explosion delay (frame)|when it is ptr, there are 2 float or int parameters|
|1|int or float|explosion brightness|default 100.0|
|2|ptr|hit damage (new)|[all parameters must be present](#SolidExpBullet01-Node2)|

#### SolidExpBullet01 Node2
| node index | value type | value description |
|---|---|---|
|0|float|hit damage factor, default 0.2|
|1|float|explosion damage factor, default 0.8|

#### [Return to Index](#index)
