# Index
Added the following values available for SGO node names.

## xgs_scene_object_class 's value:
- [EDF Weapon (Weapon_*)](#EDFWeapon)
- [GiantAnt](#GiantAnt)
- [Monster501](#Monster501)

### EDFWeapon
| sgo node name | value type | function | value description |
|---|---|---|---|
|ReloadInit|float or ptr|initial load progress|now supports using star for it|
|ReloadPadType|ptr|midsection reload type|[parameters](#EDFWeapon-ReloadPadType), it is not necessary to have all parameters|

| xgs_scene_object_class's value | custom_parameter's ptr value |
|---|---|
|Weapon_Gatling|[parameters](#Weapon_Gatling-custom_parameter), min 9 nodes, max 11 nodes|

#### EDFWeapon ReloadPadType
| node index | value type | value description |
|---|---|---|
|0|int|> 0 will allow not to reload with full time|
|1|int|extra reload time when midsection reload|
| - | - |note: node1 for energy weapon is invalid, but should be 0|
| - | - |node0 = 2 requires node2|
|2|int|charge time, after a period of time 1 ammo will be added|
| - | - |but note that as long as ammo is not full, it will start|

#### Weapon_Gatling custom_parameter
| node index | value type | value description |
|---|---|---|
|4|ptr|this has 4 floats, but it uses 3?|
|7|int|preheat time, must > 0|
|9|ptr|some interval multiplier on preheat, all defaults are 1|
| - | - |node0's float is max ROF scale, node1 is min|
| - | - |node2's float is max accuracy scale, node3 is min|
|10|int|preheat type, 0 is original|
| - | - |1 is gradually increase ROF, will use node9's data|
| - | - |2 is gradually increase accuracy at the same time|

#### EDFWeapon ReloadPadType

##### [Return to Index](#index)

### GiantAnt
| sgo node name | value type | function | value description |
|---|---|---|---|
|ant_BulletAlive|int|change acid alive time|bulletType = 0, default 180; = 1, 240|
|ant_BulletColor|ptr|change ant acid color|[color parameters](#GiantAnt-BulletColor), all parameters must be present|
|ant_BulletExSet|ptr|change ant acid detail|[exset parameters](#GiantAnt-BulletExSet), all parameters must be present|
|ant_BulletFireHeight|float|change acid launch height||

#### GiantAnt BulletColor
| node index | value type | value description |
|---|---|---|
|0|float|red channel|
|1|float|green channel|
|2|float|blue channel|
|3|float|alpha channel|

#### GiantAnt BulletExSet
| node index | value type | value description |
|---|---|---|
|0|int|multiply this factor by original shot count, this must > 0|
|1|float|multiply this factor by original acid speed|
|2|float|multiply this factor by original acid damage|
|3|float|change acid explosion range|
|4|int|set acid penetration, only 0 and 1|
|5|float|change acid size|
|6|float|change acid hitbox size|
|7|float|change acid hit impulse|

##### [Return to Index](#index)

### Monster501
| sgo node name | value type | function | value description |
|---|---|---|---|
|BulletAlive|int|change ammo alive time|default 30|
|BulletColor|ptr|change ammo color|[color parameters](#Monster501-BulletColor), all parameters must be present|
|BulletExSet|ptr|change ammo detail|[exset parameters](#Monster501-BulletExSet), all parameters must be present|
|ChangeModelParam|ptr|change model color|same as ant_ChangeModelParam|
| - | - | - |here are functions that game exists but doesn't use|
|monster_blood_scale_limit|float||default 10|

#### Monster501 BulletColor
| node index | value type | value description |
|---|---|---|
|0|float|red channel|
|1|float|green channel|
|2|float|blue channel|
|3|float|alpha channel|

#### Monster501 BulletExSet
| node index | value type | original | value description |
|---|---|---|---|
|0|invalid|2|shot interval, this must > 0|
|1|float|8|ammo speed|
|2|float|100|ammo damage, invalid, overwrite when attack|
|3|float|0|ammo explosion range|
|4|int|0|set ammo penetration, only 0 and 1|
|5|float|4|change ammo size|
|6|float|0?|change ammo hitbox size|
|7|float|0?|change ammo hit impulse|

##### [Return to Index](#index)
