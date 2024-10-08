# Index
The following values are available for Ammo_CustomParameter when AmmoClass is X.

## AmmoClass 's value:
- [SolidBullet01](#SolidBullet01)
- [SolidExpBullet01](#SolidExpBullet01)
- [SolidPelletBullet01](#SolidPelletBullet01)
- [LaserBullet01](#LaserBullet01)
- [LaserBullet02](#LaserBullet02)
- [LightningBullet01](#LightningBullet01)
- [MissileBullet01](#MissileBullet01)
- [RocketBullet01](#RocketBullet01)
- [Volcanicrock01](#Volcanicrock01)

### SolidBullet01
Its Ammo_CustomParameter has a maximum of 3 parameters.

| node index | value type | function | value description |
|---|---|---|---|
|0|int|ammo type|0 is normal, 1 is bounce|
|1|float|hit FX adjust|adjusting FX when bullet hit, default 1.0|
|2|float|hit FX time adjust|adjusting FX time when bullet hit, default 1.0|
|new| - | - | - |
|3|int or ptr| - | - |
| - |when it is int|building destruction type|0 is none, 1 is has|
| - |when it is ptr|random damage|node0 float is minFactor, node1 float is maxFactor|
|4|int or ptr|tail smoke length|when it is ptr, use [all parameters must be present](#SolidBullet01-Node4)|

#### SolidBullet01 Node4
| node index | value type | value description |
|---|---|---|
|0|int|tail smoke length|
|1|string|tail smoke texture name|
|2|float|tail smoke shake amplitude|

#### [Return to Index](#index)

### SolidExpBullet01
Its Ammo_CustomParameter has a maximum of 3 parameters, minimum 1 parameter required.

| node index | value type | function | value description |
|---|---|---|---|
|0|int or float or ptr|explosion delay (frame)|when it is ptr, there are 2 float or int parameters|
|1|int or float|explosion brightness|default 100.0|
|2|ptr|hit damage (new)|[minimum 2 parameters](#SolidExpBullet01-Node2)|

#### SolidExpBullet01 Node2
| node index | value type | value description |
|---|---|---|
|0|float|hit damage factor, default 0.2|
|1|float|explosion damage factor, default 0.8|
|2 (optional)|float|ammo hit brightness, defaults as explosion brightness's value|

#### [Return to Index](#index)

### SolidPelletBullet01
Its Ammo_CustomParameter has a maximum of 4 parameters, minimum 1 parameter required.

| node index | value type | function | value description |
|---|---|---|---|
|0|int|penetration time|time that bullet is able to penetrate, should be greater than 1|
|1|float|hit FX adjust|adjusting FX when bullet hit, default 1.0|
|2|float|hit FX time adjust|adjusting FX time when bullet hit, default 1.0|
|3|int or ptr|no PT ammo color (new)|when it is int, only R/B is exchanged|
|-|-|-|when it is ptr, setting RGBA of bullet when there is no penetration|

#### [Return to Index](#index)

### LaserBullet01
Its Ammo_CustomParameter has a maximum of 13 parameters, minimum 11 parameters required.

| node index | value type | function | value description |
|---|---|---|---|
|0|ptr|color|adjust hit FX color|
|1|ptr|color|adjust color of hit light source|
|2|float|hit scale|adjust size of light point on hit|
|3|float|light scale|adjust luminous intensity of entire laser|
|4|int|hit point time|set time of light point display when hit|
|5|int|hit laser FX number|set number of laser FX scattered after hit|
|-|-|(new)|when it is -1, change hit light texture|
|6|float|spread (radian)|set angle at which laser FX spreads out on hit|
|7|float|spread (radian)|ditto|
|8|float|FX length|set length of laser fx on hit|
|9|float|FX bend|set bend of laser fx on hit|
|10|int|FX segment|set number of segments per laser FX, more is more round.|
|11|float|unknown|default 1.0|
|12|int|ammo type (new)|1 means no explosive knockdown|

#### [Return to Index](#index)

### LaserBullet02
Its Ammo_CustomParameter has a maximum of 2 parameter, minimum 1 parameter required.

| node index | value type | function | value description |
|---|---|---|---|
|0|int|ammo type|0 is normal laser, 1 is laser bullet|
|-|-|-|2 is delayed explosion laser|
|-|-|-|3 is no explosion effect|
|-|-|-|(new)4 is delayed normal explosion|
|1|int|knock out type (new)|0 is knock out, 1 is no knock out|

#### [Return to Index](#index)

### LightningBullet01
Its Ammo_CustomParameter has a maximum of 8 parameter, minimum 5 parameter required.

| node index | value type | function | value description |
|---|---|---|---|
|0|float|||
|1|float|||
|2|float|||
|3|float|bounce strength|0 will move with ground|
|4|int|factor|greater will make it faster|
|5|float|lightning speed|extra speed? greater will make it straighter and faster. default is 0|
|6|float||default is 0.5|
|7|float|lightning size|adjusts the extra size of lightning. default is 0.005|

#### [Return to Index](#index)

### MissileBullet01
Its Ammo_CustomParameter has a maximum of 12 parameter, minimum 11 parameter required.

| node index | value type | function | value description |
|---|---|---|---|
|0|int|ammo type|0,1,2 is almost no difference|
|-|-|-|(new)3 is genocide explosion effect|
|-|-|-|(new)4 is genocide horizontal explosion|
|1|int|unknown|(new)10 is no explosive knockout|
|2|int|tail smoke length|it will be multiplied by 0.9 and 1.1 respectively|
|4|float|acceleration||
|5|float|guide performance||
|6|float|max speed||

#### [Return to Index](#index)

### RocketBullet01
Its Ammo_CustomParameter has a maximum of 4 parameter, minimum 3 parameter (or 0) required.

| node index | value type | function | value description |
|---|---|---|---|
|0|int|ammo type|0,1,2|
|-|-|-|2 is genocide explosion effect with no gravity|
|1|int|tail flame length||
|2|int|tail smoke length||
|3|float|acceleration|can only be used as acceleration, not as deceleration|
|4|int|hit type (new)|0 is default|
|-|-|-|1 is no explosive knockout|
|-|-|-|2 is no knockout and building destruction|

#### [Return to Index](#index)

### Volcanicrock01
Its Ammo_CustomParameter has a maximum of 5 parameter, minimum 5 parameters required.

| node index | value type | function | value description |
|---|---|---|---|
|0|int|ammo type|1 is delayed explosion and bounce|
|1|float|bounce strength|< 0.001 is no bounce|
|2|float|ammo rotation speed|initial velocity of forward rotation|
|3|float|ammo rotation damping|should be 0.98|
|4|ptr|ammo material update|[all parameters must be present](#Volcanicrock01-Node4)|

#### Volcanicrock01 Node4
| node index | value type | value description |
|---|---|---|
|0|str|material name in mdb|
|1|str|shader parameter name in mdb|
|2|ptr|initial color, [all parameters must be present](#Volcanicrock01-Node4-parameter)|
|3|ptr|transitional color, [all parameters must be present](#Volcanicrock01-Node4-parameter)|

##### Volcanicrock01 Node4 parameter
| node index | value type | value description |
|---|---|---|
|0|int|required time|
|1|ptr|color|

#### [Return to Index](#index)
