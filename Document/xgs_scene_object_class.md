# Index
Added the following values available for SGO node names.

## xgs_scene_object_class 's value:
- [EDF Weapon (Weapon_*)](#EDFWeapon)
- [GiantAnt](#GiantAnt)
- [GiantSpider](#GiantSpider)
- [GiantBee](#GiantBee)
- [DragonSmall](#DragonSmall)
- [Monster501](#Monster501)
- [Humanoid_Basic](#Humanoid_Basic)

### EDFWeapon
| sgo node name | value type | function | value description |
|---|---|---|---|
|ReloadInit|float or ptr|initial load progress|now supports using star for it|
|ReloadPadType|ptr|midsection reload type|[parameters](#EDFWeapon-ReloadPadType), all parameters must be present|
|use_extraShotType|int||now used only as a marker for multi-weapon fire, only 1|

| xgs_scene_object_class's value | custom_parameter's ptr value |
|---|---|
|Weapon_HeavyShoot|[parameters](#Weapon_HeavyShoot-custom_parameter), min 9 nodes, max 11 nodes|
|Weapon_Gatling|[parameters](#Weapon_Gatling-custom_parameter), min 10 nodes, max 11 nodes|
|Weapon_Accessory|[new available values](#Weapon_Accessory-custom_parameter)|

#### EDFWeapon ReloadPadType
| node index | value type | value description |
|---|---|---|
|0|int|> 0 will allow not to reload with full time|
| - | - |2 means it can be charged, 3 does not allow manual reload|
|1|int|extra reload time when midsection reload|
| - | - |note: node1 for energy weapon is invalid, but should be 0|
|2|int|charge time, after a period of time 1 ammo will be added|
| - | - |but note that as long as ammo is not full, it will start|

##### [Return to EDFWeapon](###EDFWeapon)

#### Weapon_HeavyShoot custom_parameter
| node index | value type | value description |
|---|---|---|
|0|string|animation name|
|1|int||
|2|int|animation application options, it is recommended to refer to the original version|
|3|float|animation speed|
|4|int||
|5|ptr|equipment weight effect|
| - | - |float0 is move speed adjust|
| - | - |float1 is rotation speed adjust|
| - | - |float2 is sight speed adjust|
|6|ptr|this has 3 floats?|
|7|float|speed adjust in firing?|
|8|ptr|sight recoil|
| - | - |float0 is down(+) or up(-)|
| - | - |float1 is left(+) or right(-)|
| - | - |float2 is no effect?|
|9|float|sight recoil adjust?|
|new| - | - |
|10|int|extra sight display type, 0 is original|
| - | - |1 is laser sight, 2 is shot indicator circle|
| - | - |if it is float, set fixed indicator circle radius|

##### [Return to EDFWeapon](###EDFWeapon)

#### Weapon_Gatling custom_parameter
| node index | value type | value description |
|---|---|---|
|4|ptr|this has 4 floats, but it uses 3?|
|7|int|preheat time, must > 0|
|new| - | - |
|9|ptr|some interval multiplier on preheat, all defaults are 1|
| - | - |node0's int is shot delay, is min fire time|
| - | - |node1's float is max ROF scale, node2 is min|
| - | - |node3's float is max accuracy scale, node4 is min|
|10|int|preheat type, 0 is original|
| - | - |1 is gradually increase ROF, will use node9's data|
| - | - |2 is gradually increase accuracy at the same time|

##### [Return to EDFWeapon](###EDFWeapon)

#### Weapon_Accessory custom_parameter
| type int | value type | value description |
|---|---|---|
|909|float|> 0, multiply this factor by recoil|
|912|float|> 0, multiply this factor by LockonAngle's node0|
|913|float|> 0, multiply this factor by LockonAngle's node1|
|1001|float|> 0, multiply this factor by weapon's ammo amount|
|1002|float|> 0, multiply this factor by weapon's reload time|
|1003|float|> 0, multiply this factor by weapon's rate of fire|
|1012|int|0 or 1, 1 is set LockonTargetType to 0|
|1050|float|> 0, multiply this factor by weapon's damage|
|1051|float|> 0, multiply this factor by weapon's bullet speed|
|1052|float|> 0, multiply this factor by weapon's explosion radius|
|1060|float|> 0, multiply this factor by weapon's bullet size|
|1150|float|> 0, multiply this factor by weapon's -damage|
|fencer only| - | - |
|1300|int|0 or 1, 1 is switch dash to boost|
|1301|int|0 or 1, 1 is switch boost to dash|
|1310|int|1 is switch zoom to boost, 2 is to dash|
|1311|int|1 is switch shield reflect to boost, 2 is to dash|

##### [Return to EDFWeapon](###EDFWeapon)

### GiantAnt
| sgo node name | value type | function | value description |
|---|---|---|---|
|ant_BulletAlive|int|change acid alive time|ant_BulletType = 0, default 180; = 1, 240|
|ant_BulletClass|ptr|change ant ammo|[ammo parameters](#GiantAnt-BulletClass), all parameters must be present|
|ant_BulletColor|ptr|change ant acid color|[color parameters](#GiantAnt-BulletColor), all parameters must be present|
|ant_BulletExSet|ptr|change ant acid detail|[exset parameters](#GiantAnt-BulletExSet), all parameters must be present|
|ant_BulletFireHeight|float|change acid launch height||

#### GiantAnt BulletClass
| node index | value type | value description |
|---|---|---|
|0|int|ammo type, only when ant_BulletType = 0, 0 is default|
| - | - |1 is AcidBullet01, 2 is FlameBullet01|
| - | - |3 is PlasmaBullet01, 4 is SolidBullet01|
| - | - |5 is PulseBullet01, 6 is SolidExpBullet01|
| - | - |7 is HomingLaserBullet01, 8 is LaserBullet02|
| - | - |9 is FlameBullet02, 10 is NeedleBullet01|
| - | - |11 is LightningBullet01, 12 is LaserBullet01|
| - | - |13 is RocketBullet01, 14 is BeamBullet01|
|1|float|normal shot accuracy, default 0.1|
|2|int|set continuous firing count, default 90|
| - | - |able to use animation event "BurstFire" to start new continuous fire|
| - | - |animation event "BurstFire_Stop" to stop new continuous fire|
|3|int|overriding original singly shot count in new continuous firings when > 0|
|4|int|new continuous fire interval|

#### GiantAnt BulletColor
| node index | value type | value description |
|---|---|---|
|0|float|red channel. ant_BulletType = 0, default 4.5; = 1, 3|
|1|float|green channel. ant_BulletType = 0, default 2.25; = 1, 1.5|
|2|float|blue channel. ant_BulletType = 0, default 1.5; = 1, 1.05|
|3|float|alpha channel. ant_BulletType = 0, default 1; = 1, 1|

#### GiantAnt BulletExSet
| node index | value type | value description |
|---|---|---|
|0|int|multiply this factor by original shot count. If is 0, shot count is 1.|
|1|float|multiply this factor by original acid speed|
|2|float|multiply this factor by original acid damage|
|3|float|change acid explosion range|
|4|int|set acid penetration, only 0 and 1|
|5|float|change acid size|
|6|float|change acid hitbox size|
|7|float|change acid hit impulse|

##### [Return to Index](#index)

### GiantSpider
| sgo node name | value type | function | value description |
|---|---|---|---|
|spd_AmmoAlive|int|change ammo alive time|default 300|
|spd_AmmoClass|ptr|change ant ammo|[ammo parameters](#GiantSpider-BulletClass), all parameters must be present|
|spd_AmmoColor|ptr|change ammo color|[color parameters](#GiantSpider-BulletColor), all parameters must be present|
|spd_AmmoExSet|ptr|change ammo detail|[exset parameters](#GiantSpider-BulletExSet), all parameters must be present|
|spd_ChangeModelParam|ptr|change model color|same as ant_ChangeModelParam|
| - | - |original function|
|spd_AiSetting|ptr|ai range|[ai parameters](#GiantSpider-spdAiSetting), all parameters must be present|
|spd_Ammo|ptr|change ammo|[ammo parameters](#GiantSpider-spdammo), all parameters must be present|

#### GiantSpider spdAiSetting
| node index | value type | value description |
|---|---|---|
|0|int|extra value|
|1|int|base value|
|2|float|extra search range?|
|3|float|base search range?|

#### GiantSpider spdammo
| node index | value type | value description |
|---|---|---|
|0|float|multiply this factor by original bullet size|
|1|float|multiply this factor by original bullet speed|
|2|float|multiply this factor by original bullet damage|

#### GiantSpider BulletClass
| node index | value type | value description |
|---|---|---|
|0|int|ammo type, 0 is default|
| - | - |1 is AcidBullet01, 2 is FlameBullet01|
| - | - |3 is PlasmaBullet01, 4 is SolidBullet01|
| - | - |5 is PulseBullet01, 6 is SolidExpBullet01|
| - | - |7 is HomingLaserBullet01, 8 is LaserBullet02|
| - | - |9 is FlameBullet02, 10 is NeedleBullet01|
| - | - |11 is LightningBullet01, 12 is LaserBullet01|
| - | - |13 is RocketBullet01, 14 is BeamBullet01|
|1|float|normal shot accuracy, default 0.1|
|2|int|set continuous firing count, default 0|
| - | - |able to use animation event "BurstFire" to start continuous fire|
| - | - |animation event "BurstFire_Stop" to stop continuous fire|
|3|int|overriding original singly shot count in continuous firings when > 0|
|4|int|continuous fire interval|

#### GiantSpider BulletColor
| node index | value type | value description |
|---|---|---|
|0|float|red channel. default 0.5|
|1|float|green channel. default 0.5|
|2|float|blue channel. default 0.5|
|3|float|alpha channel. default 1|

#### GiantSpider BulletExSet
| node index | value type | value description |
|---|---|---|
|0|int|multiply this factor by original shot count. If is 0, shot count is 1.|
|1|float|change ammo explosion range|
|2|float|change ammo hitbox size|
|3|float|change ammo hit impulse|

##### [Return to Index](#index)

### GiantBee
| sgo node name | value type | function | value description |
|---|---|---|---|
|bee_BulletAccuracy|float|change shot accuracy|default 0.7|
|bee_BulletAlive|int|change ammo alive time|insectbase_Type = 0, default 180; = 1, 300|
|bee_BulletExSet|ptr|change ammo detail|[exset parameters](#GiantBee-BulletExSet), all parameters must be present|
|bee_BulletSet|ptr|change ammo type and color|[set parameters](#GiantBee-BulletSet), all parameters must be present|
|bee_BurstShot|ptr|continuous shot parameters|[burst parameters](#GiantBee-BurstShot), all parameters must be present|

#### GiantBee BulletExSet
| node index | value type | value description |
|---|---|---|
|0|int|multiply this factor by original shot count. If is 0, shot count is 1.|
|1|float|multiply this factor by original ammo speed|
|2|float|multiply this factor by original ammo damage|
|3|float|change ammo explosion range. insectbase_Type = 1, default 3|
|4|int|set ammo penetration, only 0 and 1|
|5|float|change ammo size. insectbase_Type = 0, default 0.2; = 1, 1|
|6|float|change ammo hitbox size|
|7|float|change ammo hit impulse|

#### GiantBee BulletSet
| node index | value type | value description |
|---|---|---|
|0|int|ammo type, 0 is default|
| - | - |1 is AcidBullet01, 2 is FlameBullet01|
| - | - |3 is PlasmaBullet01, 4 is SolidBullet01|
| - | - |5 is PulseBullet01, 6 is SolidExpBullet01|
| - | - |7 is HomingLaserBullet01, 8 is LaserBullet02|
| - | - |9 is FlameBullet02, 10 is NeedleBullet01|
| - | - |11 is LightningBullet01, 12 is LaserBullet01|
| - | - |13 is RocketBullet01, 14 is BeamBullet01|
|1|float|color red channel|
|2|float|color green channel|
|3|float|color blue channel|
|4|float|color alpha channel|

#### GiantBee BurstShot
| node index | value type | value description |
|---|---|---|
|0|int|set continuous firing count, default 0|
| - | - |able to use animation event "BurstFire" to start continuous fire|
| - | - |animation event "BurstFire_Stop" to stop continuous fire|
|1|int|overriding original singly shot count in continuous firings when > 0|
|2|int|continuous fire interval|

##### [Return to Index](#index)

### DragonSmall
| sgo node name | value type | function | value description |
|---|---|---|---|
|dragon_AttackBulletAlive|int|change ammo alive time|default 360|
|dragon_AttackBulletColor|ptr|change ammo color|[color parameters](#DragonSmall-BulletColor), all parameters must be present|
|dragon_AttackBulletExSet|ptr|change ant acid detail|[exset parameters](#DragonSmall-BulletExSet), all parameters must be present|
|dragon_AttackBulletType|int|change ammo type|0 is default|
| - | - |1 is AcidBullet01|2 is LightningBullet01|
| - | - |3 is PlasmaBullet01|4 is SolidBullet01|
| - | - |5 is PulseBullet01|6 is SolidExpBullet01|
| - | - |7 is HomingLaserBullet01|8 is LaserBullet02|
|dragon_ChangeModelParam|ptr|change model color|same as ant_ChangeModelParam|
|dragon_CustomBulletParam|ptr||like Ammo_CustomParameter|
| - | - |original function|
|dragon_AttackBulletSetting|ptr|change ammo damage|[parameters](#DragonSmall-AttackBulletSetting), all parameters must be present|

#### DragonSmall BulletColor
| node index | value type | value description |
|---|---|---|
|0|float|red channel, default 1.0|
|1|float|green channel, default 4.0|
|2|float|blue channel, default 4.0|
|3|float|alpha channel, default 1.0|

#### DragonSmall BulletExSet
| node index | value type | value description |
|---|---|---|
|0|int|multiply this factor by original shot count. If is 0, shot count is 1.|
|1|float|change acid explosion range|
|2|int|set acid penetration, only 0 and 1|
|3|float|change acid hitbox size|
|4|float|change acid hit impulse|

#### DragonSmall AttackBulletSetting
| node index | value type | value description |
|---|---|---|
|0|float|change ammo size|
|1|float|increase this by original ammo speed|
|2|float|divide this factor by original ammo speed|
|3|float|shot accuracy, default 0.1|
|4|float|multiply this factor by original ammo damage|

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
|0|int|2|shot interval, this must > 0|
|1|float|8|ammo speed|
|2|float|100|ammo damage, invalid, overwrite when attack|
|3|float|0|ammo explosion range|
|4|int|0|set ammo penetration, only 0 and 1|
|5|float|4|change ammo size|
|6|float|0?|change ammo hitbox size|
|7|float|0?|change ammo hit impulse|

##### [Return to Index](#index)

### Humanoid_Basic
| sgo node name | value type | function | value description |
|---|---|---|---|
| - | - | - |here are functions that game exists but doesn't use|
|humanoid_climb_setting|ptr||[exset parameters](#Humanoid_Basic-ClimbSet), all parameters must be present|

#### Humanoid_Basic ClimbSet
| node index | value type | value description |
|---|---|---|
|0|float|default 6.0|
|1|float|default 20.5|

##### [Return to Index](#index)
